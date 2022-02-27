#include <Feng/ScenesManager/RenderSystem.h>

#include <Feng/App/Globals.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Light.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace feng
{
    namespace SRenderSystem
    {
        void SortSceneByDistance(const Camera& cam, std::vector<MeshRenderer*> &renderers)
        {
            Transform *camTransform = cam.GetEntity()->GetComponent<Transform>();
            const Vector3 &camPosition = camTransform->GetPosition();
            auto compare = [&camPosition] (const MeshRenderer* r1, const MeshRenderer* r2)
            {
                const Entity *e1 = r1->GetEntity();
                const Entity *e2 = r2->GetEntity();
                const Transform *transform1 = e1->GetComponent<Transform>();
                const Vector3 &position1 = transform1->GetPosition();
                float distanceSqr1 = Vector3::DistanceSqr(camPosition, position1);

                const Transform *transform2 = e2->GetComponent<Transform>();
                const Vector3 &position2 = transform2->GetPosition();
                float distanceSqr2 = Vector3::DistanceSqr(camPosition, position2);

                return distanceSqr2 < distanceSqr1;
            };

            std::sort(renderers.begin(), renderers.end(), compare);
        }
    }

    RenderSystem::RenderSystem()
    {
#ifdef __APPLE__
        frameBuffer = buffersPool.CreateBuffer(2 * screen::ScreenWidth, 2 * screen::ScreenHeight, true);
#else
        frameBuffer = buffersPool.CreateBuffer(screen::ScreenWidth, screen::ScreenHeight, true);
#endif
        CreateCamUniformBuffer();
        renderProperties.camBufferIndex = camBufferIndex;
    }

    RenderSystem::~RenderSystem()
    {
        buffersPool.DeleteBuffer(frameBuffer);
        glDeleteBuffers(1, &camUbo);
    }

    void RenderSystem::SetAmbientLight(Vector4 color, float intensity)
    {
        renderProperties.ambientColorAndIntencity = Vector4{color.GetXyz(), intensity };
    }

    void RenderSystem::SetCamera(Camera *camera)
    {
        renderProperties.cam = camera;
    }

    void RenderSystem::AddRenderer(MeshRenderer *renderer)
    {
        renderers.push_back(renderer);
    }

    void RenderSystem::RemoveRenderer(MeshRenderer *renderer)
    {
        renderers.erase(std::remove(renderers.begin(), renderers.end(), renderer), renderers.end());
    }

    void RenderSystem::AddLight(Light *light)
    {
        switch (light->GetType())
        {
            case Light::eType::Directional:
            renderProperties.directLight = light;
            break;

            case Light::eType::Point:
            renderProperties.pointLight = light;
            break;

            case Light::eType::Spot:
            renderProperties.spotLight = light;
            break;

        }
    }

    void RenderSystem::SetPostEffect(PostEffectDefinition *postEffect)
    {
        postProcessing.SetPostEffect(*postEffect);
        Print_Errors_OpengGL();
    }

    void RenderSystem::RemovePostEffect()
    {
        postProcessing.RemoveEffects();
    }

    void RenderSystem::Draw()
    {
        if(postProcessing.HasPostEffects())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.Frame);
            Print_Errors_OpengGL();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        BindCamUniformBuffer();
        DrawRenderers();
        DrawSkybox();

        if(postProcessing.HasPostEffects())
        {
            postProcessing.ApplyPostEffects(frameBuffer);
        }
        Print_Errors_OpengGL();
    }

    void RenderSystem::CreateCamUniformBuffer()
    {
        constexpr uint32_t camUniformsSize = 208;
        glGenBuffers(1, &camUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, camUbo);
        glBufferData(GL_UNIFORM_BUFFER, camUniformsSize, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void RenderSystem::BindCamUniformBuffer()
    {
        const Entity *camEntity = renderProperties.cam->GetEntity();
        const Transform *camTransform = camEntity->GetComponent<Transform>();

        const Matrix4 viewProjMatrix = renderProperties.cam->GetViewProjectionMatrix();
        const Matrix4 projMatrix = renderProperties.cam->GetProjectionMatrix();
        const Quaternion& camRotation = camTransform->GetRotation();
        const Matrix3 camRotationMat = camRotation.ToMatrix3();
        const Vector3& camPos = camTransform->GetPosition();
        const Vector3 camDir = camTransform->GetForward();

        glBindBuffer(GL_UNIFORM_BUFFER, camUbo);
        glBindBufferBase(GL_UNIFORM_BUFFER, renderProperties.camBufferIndex, camUbo);
        // Offsets are specified in shader's uniform block layout.
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), viewProjMatrix.data);
        glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(Matrix4), projMatrix.data);
        glBufferSubData(GL_UNIFORM_BUFFER, 128, sizeof(Vector3), camRotationMat.rows[0].data);
        glBufferSubData(GL_UNIFORM_BUFFER, 144, sizeof(Vector3), camRotationMat.rows[1].data);
        glBufferSubData(GL_UNIFORM_BUFFER, 160, sizeof(Vector3), camRotationMat.rows[2].data);
        glBufferSubData(GL_UNIFORM_BUFFER, 176, sizeof(Vector3), camPos.data);
        glBufferSubData(GL_UNIFORM_BUFFER, 192, sizeof(Vector3), camDir.data);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void RenderSystem::DrawRenderers()
    {
        SRenderSystem::SortSceneByDistance(*renderProperties.cam, renderers);

        for(MeshRenderer *renderer : renderers)
        {
            renderer->Draw(renderProperties);
        }
        Print_Errors_OpengGL();
    }

    void RenderSystem::DrawSkybox()
    {
        if(skybox != nullptr)
        {
            glDepthFunc(GL_LEQUAL);
            skybox->Draw(renderProperties);
            glDepthFunc(GL_LESS); // Default.
        }

        Print_Errors_OpengGL();
    }
}
