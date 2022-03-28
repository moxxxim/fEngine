#include <Feng/ScenesManager/RenderSystem.h>

#include <Feng/App/Globals.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Light.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    namespace SRenderSystem
    {
        float GetDistanceToCamSqr(const MeshRenderer& renderer, const Vector3& camPos)
        {
            const Entity *entity = renderer.GetEntity();
            const Transform *transform = entity->GetComponent<Transform>();
            const Vector3 &rendererPosition = transform->GetPosition();
            
            return Vector3::DistanceSqr(camPos, rendererPosition);
        }

        void SortSceneByDistance(const Camera& cam, std::vector<MeshRenderer*> &renderers)
        {
            Transform *camTransform = cam.GetEntity()->GetComponent<Transform>();
            const Vector3 &camPosition = camTransform->GetPosition();
            auto compare = [&camPosition] (const MeshRenderer* r1, const MeshRenderer* r2)
            {
                float distanceSqr1 = GetDistanceToCamSqr(*r1, camPosition);
                float distanceSqr2 = GetDistanceToCamSqr(*r2, camPosition);

                return distanceSqr2 < distanceSqr1;
            };

            std::sort(renderers.begin(), renderers.end(), compare);
        }
    }

    RenderSystem::RenderSystem()
    {
        CreateCamUniformBuffer();
    }

    RenderSystem::~RenderSystem()
    {
        fboPool.DeleteBuffer(frameBuffer);
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
        const Material* material = renderer->GetMaterial();
        if(material->IsTransparent())
        {
            renderersTransparent.push_back(renderer);
        }
        else
        {
            renderersOpaque.push_back(renderer);
        }
    }

    void RenderSystem::RemoveRenderer(MeshRenderer *renderer)
    {
        const Material* material = renderer->GetMaterial();
        std::vector<MeshRenderer*> renderers = material->IsTransparent()
                                                ? renderersTransparent
                                                : renderersOpaque;
        
        auto it = std::find(renderers.begin(), renderers.end(), renderer);
        if(it != renderers.end())
        {
            std::swap(*it, renderers.back());
            renderers.pop_back();
        }
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
            if(frameBuffer.Frame == FrameBuffer::Default)
            {
                frameBuffer = CreateFrameBuffer();
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.Frame);
            Print_Errors_OpengGL();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::Default);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        BindCamUniformBuffer();
        DrawOpaque();

        // Draw skybox after opaque objects for good use of a depth buffer.
        // Draw skybox before transparent object for correct visual effect.
        DrawSkybox();
        DrawTransparent();

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
        glBindBufferBase(GL_UNIFORM_BUFFER, RenderProperties::CamBufferBinding, camUbo);
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
    
    void RenderSystem::DrawOpaque()
    {
        for(MeshRenderer *renderer : renderersOpaque)
        {
            renderer->Draw(renderProperties);
        }
        
        Print_Errors_OpengGL();
    }
    
    void RenderSystem::DrawTransparent()
    {
        SRenderSystem::SortSceneByDistance(*renderProperties.cam, renderersTransparent);
        for(MeshRenderer *renderer : renderersTransparent)
        {
            renderer->Draw(renderProperties);
        }
        
        Print_Errors_OpengGL();
    }

    void RenderSystem::DrawSkybox()
    {
        if(skybox != nullptr)
        {
            // We need less-or-equal function for skybox to pass depth testing,
            // as its depth will be 1 (initial depth value of a fragment in depth buffer).
            glDepthFunc(GL_LEQUAL);
            skybox->Draw(renderProperties);
            glDepthFunc(GL_LESS);
        }

        Print_Errors_OpengGL();
    }
    
    FrameBuffer RenderSystem::CreateFrameBuffer()
    {
        #ifdef __APPLE__
            // m.alekseev Hack-fix for GLFW problev with hight dpi screens.
            return fboPool.CreateBuffer(2 * Screen::ScreenWidth, 2 * Screen::ScreenHeight, true, false);
        #else
            return fboPool.CreateBuffer(Screen::ScreenWidth, Screen::ScreenHeight, true, false);
        #endif
    }
}
