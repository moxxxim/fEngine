#include <Feng/ScenesManager/RenderSystem.h>

#include <Feng/App/Globals.h>
#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Light.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/Utils/Debug.h>
#include <Feng/Utils/Render/RenderUtils.h>
#include <Feng/Utils/Render/ShaderParams.h>

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
        
        void ClampShadowMapToBorder(FrameBuffer buffer)
        {
            GLenum target = static_cast<GLenum>(FrameBuffersPool::GetBindTarget(buffer.settings.depth));
            
            glBindTexture(target, buffer.depth);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            if(buffer.settings.depth == FrameBuffer::eAttachement::Cubemap)
            {
                glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
            }
            
            glBindTexture(target, 0);
        }
        
        bool ValidateCascadeBorders(const std::vector<float> cascadeBorders)
        {
            static constexpr uint32_t cascadesCount = 3;
            
            if(!std::is_sorted(cascadeBorders.cbegin(), cascadeBorders.cend()))
            {
                return false;
            }
            
            if(cascadeBorders.size() != 2)
            {
                Debug::LogError("There only execty 3 shadow cascades supported, i.e. 2 cascade borders.");
                return false;
            }
            
            return true;
        }
        
        namespace Shadow
        {
            Matrix4 GetCamViewProjInverse(const RenderProperties &renderProperties, const std::pair<float, float>& nearFar)
            {
                const float camFov = renderProperties.cam->GetFovY();
                const float camAspect = renderProperties.cam->GetAspectRatio();
                const Matrix4 camTransform = renderProperties.cam->GetEntity()->GetComponent<Transform>()->GetGlobalMatrix();
                const Matrix4 projection = Mat4::MakePerspectiveProjection(camFov, camAspect, nearFar.first, nearFar.second);
                Matrix4 projectionInverted;
                std::ignore = projection.TryInvert(projectionInverted);
                
                return projectionInverted * camTransform;
            }
            
            Matrix4 GetShadowCastLightProjectionMatrix(
                                                       const RenderProperties &renderProperties,
                                                       const Matrix4& lightViewMatrix,
                                                       const std::pair<float, float>& nearFar)
            {
                Matrix4 camViewProjInverse = GetCamViewProjInverse(renderProperties, nearFar);
                std::array<Vector4, 8> camFrustum = Render::GetFrustumXyzMinMax(camViewProjInverse);

                float minX = std::numeric_limits<float>::max();
                float maxX = std::numeric_limits<float>::lowest();
                float minY = std::numeric_limits<float>::max();
                float maxY = std::numeric_limits<float>::lowest();
                float minZ = std::numeric_limits<float>::max();
                float maxZ = std::numeric_limits<float>::lowest();
                for(const Vector4& corner : camFrustum)
                {
                    const Vector4 cornerInLightSpace = corner * lightViewMatrix;
                    minX = std::min(minX, cornerInLightSpace.x);
                    maxX = std::max(maxX, cornerInLightSpace.x);
                    minY = std::min(minY, cornerInLightSpace.y);
                    maxY = std::max(maxY, cornerInLightSpace.y);
                    minZ = std::min(minZ, cornerInLightSpace.z);
                    maxZ = std::max(maxZ, cornerInLightSpace.z);
                }

                minZ -= 10.f;
                maxZ += 20.f;

                return Mat4::MakeOrthogonalProjection(minX, maxX, minY, maxY, minZ, maxZ);
            }
            
            Matrix4 GetShadowCastLightViewMatrix(const Entity* light)
            {
                const Transform *lightTransform = light->GetComponent<Transform>();
                return Mat4::MakeTransformation(Vector3::One, Vector3::Zero, lightTransform->GetRotation().Inverse());
            }

            std::vector<float> CalculateCascadeFarDistances(const RenderProperties &renderProperties)
            {
                std::vector<float> distances;
                
                float minNear = renderProperties.cam->GetNearClipPlane();
                float maxFar = renderProperties.cam->GetFarClipPlane();
                for(int i = 0; i < renderProperties.cascadeBorders.size(); ++i)
                {
                    float cascadeBorder = renderProperties.cascadeBorders[i];
                    float far = minNear + (maxFar - minNear) * cascadeBorder;
                    distances.push_back(far);
                }
                
                distances.push_back(maxFar);

                return distances;
            }
            
            std::vector<Matrix4> GetDirectShadowMatrices(const RenderProperties &renderProperties,
                                                         std::vector<float> farDistances)
            {
                std::vector<Matrix4> cascadeMatrices;

                Matrix4 lightViewMatrix = GetShadowCastLightViewMatrix(renderProperties.directShadowLight);
                float near = renderProperties.cam->GetNearClipPlane();
                for(int i = 0; i < farDistances.size(); ++i)
                {
                    std::pair<float, float> nearFar = std::make_pair(near, farDistances[i]);
                    Matrix4 lightProjection = GetShadowCastLightProjectionMatrix(renderProperties, lightViewMatrix, nearFar);
                    cascadeMatrices.push_back(lightViewMatrix * lightProjection);
                    near = nearFar.second;
                }
                
                return cascadeMatrices;
            }
            
            std::vector<Matrix4> GetOmnidirectionalShadowLightMatrices(const Entity* lightEntity)
            {
                std::vector<Matrix4> matrices;
                
                const Light* light = lightEntity->GetComponent<Light>();
                const Matrix4 lightProjection = Mat4::MakePerspectiveProjection(90.f, 1.f, 1.f, light->GetRange());
                const Vector3& lightPos = lightEntity->GetComponent<Transform>()->GetPosition();
                
                Matrix4 transformPosX;
                Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f} * Quaternion{Vector3::OneY, -90.f}).TryInvert(transformPosX);
                Matrix4 transformNegX;
                Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f} * Quaternion{Vector3::OneY, 90.f}).TryInvert(transformNegX);
                matrices.push_back(transformPosX * lightProjection);
                matrices.push_back(transformNegX * lightProjection);
                
                Matrix4 transformPosY;
                Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneX, 90.f}).TryInvert(transformPosY);
                Matrix4 transformNegY;
                Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneX, -90.f}).TryInvert(transformNegY);
                matrices.push_back(transformPosY * lightProjection);
                matrices.push_back(transformNegY * lightProjection);
                
                Matrix4 transformPosZ;
                Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f} * Quaternion{Vector3::OneY, 180.f}).TryInvert(transformPosZ);
                Matrix4 transformNegZ;
                Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f}).TryInvert(transformNegZ);
                matrices.push_back(transformPosZ * lightProjection);
                matrices.push_back(transformNegZ * lightProjection);

                return matrices;
            }
        }
    }

    RenderSystem::RenderSystem()
    {
        CreateCamUniformBuffer();
    }

    RenderSystem::~RenderSystem()
    {
        glDeleteBuffers(1, &camUbo);
        glDeleteBuffers(1, &quadBuffer.vao);
        glDeleteBuffers(1, &quadBuffer.vbo);
    }
    
    void RenderSystem::SetAmbientLight(Vector4 color, float intensity)
    {
        renderProperties.ambientColorAndIntencity = Vector4{color.GetXyz(), intensity };
    }

    void RenderSystem::SetCamera(Camera *camera)
    {
        renderProperties.cam = camera;
    }
    
    void RenderSystem::SetShadowMaterials(
                                         Material *directLightShadowMaterial,
                                         Material *pointLightShadowMaterial,
                                         Material *directLightShadowDebugMaterial)
    {
        shadowSetup.directLightShadowMaterial = directLightShadowMaterial;
        shadowSetup.pointLightShadowMaterial = pointLightShadowMaterial;
        shadowSetup.directLightShadowDebugMaterial = directLightShadowDebugMaterial;
    }
    
    void RenderSystem::SetToneMappingMaterial(Material* toneMapping)
    {
        toneMappingMaterial = toneMapping;
    }
    
    void RenderSystem::SetCascadeBorders(std::vector<float> aCascadeBorders)
    {
        bool validCascades = SRenderSystem::ValidateCascadeBorders(aCascadeBorders);
        renderProperties.cascadeBorders = validCascades ? aCascadeBorders : std::vector<float>{ 0.33f, 0.66f };
    }
    
    void RenderSystem::SetDirectionalShadowLight(Entity *light)
    {
        renderProperties.directShadowLight = light;
    }
    
    void RenderSystem::SetPointShadowLight(Entity *light)
    {
        renderProperties.pointShadowLight = light;
    }

    bool RenderSystem::IsHdr() const
    {
        return hdr;
    }
    
    void RenderSystem::SetHdr(bool value)
    {
        hdr = value;
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
        UpdateGlobalBindings();
        UpdateShadowMap();

        if(Engine::IsDirectShowDebugShadowMap())
        {
            DrawDebugDirectShadowMap();
        }
        else if (Engine::IsOmniShowDebugShadowMap())
        {
            DrawDebugOmniShadowMap();
        }
        else
        {
            DrawGeneric();
        }
        
        if(IsShadowsEnabled())
        {
            fboPool.Push(shadowSetup.directShadowMap);
            fboPool.Push(shadowSetup.pointShadowMap);
        }
        
        Print_Errors_OpengGL();
    }
    
    bool RenderSystem::IsShadowsEnabled()
    {
        return Engine::IsShadowsEnabled() && renderProperties.directShadowLight && shadowSetup.directLightShadowMaterial;
    }

    void RenderSystem::CreateCamUniformBuffer()
    {
        constexpr uint32_t camUniformsSize = 208;
        glGenBuffers(1, &camUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, camUbo);
        glBufferData(GL_UNIFORM_BUFFER, camUniformsSize, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    void RenderSystem::UpdateShadowMap()
    {
        if (IsShadowsEnabled())
        {
            UpdateDirectShadowMap();
            UpdatePointShadowMap();
        }
    }
    
    void RenderSystem::UpdateDirectShadowMap()
    {
        shadowSetup.directShadowMap = GetDirectShadowMapBuffer();
        SRenderSystem::ClampShadowMapToBorder(shadowSetup.directShadowMap);

        glViewport(0, 0, shadowSetup.directShadowMap.settings.size.width, shadowSetup.directShadowMap.settings.size.height);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowSetup.directShadowMap.frame);
        glClear(GL_DEPTH_BUFFER_BIT);
        Print_Errors_OpengGL();

        DrawShadowCastersInShadowMap(shadowSetup.directLightShadowMaterial);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void RenderSystem::UpdatePointShadowMap()
    {
        shadowSetup.pointShadowMap = GetPointShadowMapBuffer();
        SRenderSystem::ClampShadowMapToBorder(shadowSetup.pointShadowMap);

        glViewport(0, 0, shadowSetup.pointShadowMap.settings.size.width, shadowSetup.pointShadowMap.settings.size.height);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowSetup.pointShadowMap.frame);
        glClear(GL_DEPTH_BUFFER_BIT);
        Print_Errors_OpengGL();

        DrawShadowCastersInShadowMap(shadowSetup.pointLightShadowMaterial);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void RenderSystem::DrawShadowCastersInShadowMap(Material *shadowMaterial)
    {
        auto firstNotCaster = std::partition(
                                             renderersOpaque.begin(),
                                             renderersOpaque.end(),
                                             std::mem_fn(&MeshRenderer::IsShadowCaster));
        std::for_each(renderersOpaque.begin(), firstNotCaster, [this, shadowMaterial](MeshRenderer *renderer)
        {
            renderer->Draw(renderProperties, true, shadowMaterial);
        });

        Print_Errors_OpengGL();
    }
    
    void RenderSystem::DrawDebugDirectShadowMap()
    {
        if (quadBuffer.vao == 0)
        {
            quadBuffer = Render::CreateQuadBuffer();
        }
        
        glViewport(0, 0, Screen::ScreenSize.width, Screen::ScreenSize.height);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::Default);
        Print_Errors_OpengGL();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glBindVertexArray(quadBuffer.vao);

        Shader *shader = shadowSetup.directLightShadowDebugMaterial->GetShader();
        shader->StartUse();
        Render::ResolveBindings(*shader, renderProperties.globalBindings, { });
        Render::SetDrawFace(eDrawFace::Cw);
        Print_Errors_OpengGL();

        glActiveTexture(GL_TEXTURE0);
        Print_Errors_OpengGL();
        glBindTexture(GL_TEXTURE_2D_ARRAY, shadowSetup.directShadowMap.depth);
        Print_Errors_OpengGL();
        shader->SetUniformInt(ShaderParams::DirectShadowMap.data(), 0);
        Print_Errors_OpengGL();
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        shader->StopUse();
        Print_Errors_OpengGL();

        glBindVertexArray(Render::UndefinedBuffer);
    }
    
    void RenderSystem::DrawDebugOmniShadowMap()
    {
        skybox->AddExternalTexture(ShaderParams::Texture0, eTextureType::Cubemap, shadowSetup.pointShadowMap.depth);
        DrawGeneric();
        skybox->RemoveExternalTexture(ShaderParams::Texture0);
    }
    
    void RenderSystem::DrawGeneric()
    {
        FrameBuffer outBuffer = IsOffscreenRender()
            ? GetHdrBuffer(Engine::IsMultisampleEnabled())
            : FrameBuffer{};
        
        FrameBuffer renderBuffer = postProcessing.HasPostEffects()
                    ? GetFrameBuffer(Engine::IsMultisampleEnabled())
                    : outBuffer;

        glViewport(0, 0, Screen::ScreenSize.width, Screen::ScreenSize.height);
        glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer.frame);
        Print_Errors_OpengGL();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        DrawOpaque();

        // Draw skybox after opaque objects for good use of a depth buffer.
        // Draw skybox before transparent object for correct visual effect.
        DrawSkybox();
        DrawTransparent();
        ApplyPostEffects(renderBuffer, outBuffer);
        
        if(IsOffscreenRender())
        {
            RenderOnScreenQuad(outBuffer);
        }
        
        fboPool.Push(outBuffer);
        fboPool.Push(renderBuffer);
    }
    
    void RenderSystem::DrawOpaque()
    {
        for(MeshRenderer *renderer : renderersOpaque)
        {
            renderer->AddExternalTexture(ShaderParams::DirectShadowMap, eTextureType::Array2d, shadowSetup.directShadowMap.depth);
            renderer->AddExternalTexture(ShaderParams::PointShadowMap, eTextureType::Cubemap, shadowSetup.pointShadowMap.depth);
            renderer->Draw(renderProperties, false);
        }
        
        Print_Errors_OpengGL();
    }
    
    void RenderSystem::DrawTransparent()
    {
        SRenderSystem::SortSceneByDistance(*renderProperties.cam, renderersTransparent);
        for(MeshRenderer *renderer : renderersTransparent)
        {
            renderer->Draw(renderProperties, false);
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
            skybox->Draw(renderProperties, false);
            glDepthFunc(GL_LESS);
        }

        Print_Errors_OpengGL();
    }
    
    void RenderSystem::ApplyPostEffects(const FrameBuffer& renderBuffer, const FrameBuffer& outBuffer)
    {
        if(postProcessing.HasPostEffects())
        {
            FrameBuffer postEffectInput = BlitFrameBuffer(renderBuffer);
            postProcessing.ApplyPostEffects(postEffectInput, outBuffer);
            fboPool.Push(postEffectInput);
        }
    }
    
    void RenderSystem::UpdateGlobalBindings()
    {
        UpdateCameraUniforms();
        UpdateLightUniforms();
        UpdateShadowLightUniform();
    }
    
    void RenderSystem::UpdateCameraUniforms()
    {
        BindCamUniformBuffer();
        renderProperties.globalBindings.SetFloat(ShaderParams::NearClipPlane.data(), renderProperties.cam->GetNearClipPlane());
        renderProperties.globalBindings.SetFloat(ShaderParams::FarClipPlane.data(), renderProperties.cam->GetFarClipPlane());
        renderProperties.globalBindings.SetBuffer(ShaderParams::CamUniformBlock.data(), RenderProperties::CamBufferBinding);
    }

    void RenderSystem::UpdateLightUniforms()
    {
        renderProperties.globalBindings.SetVector4(ShaderParams::AmbientColor.data(), renderProperties.ambientColorAndIntencity);

        const Transform *pointLightTransform = renderProperties.pointLight->GetEntity()->GetComponent<Transform>();
        Vector4 pointLightPositionAndRange = pointLightTransform->GetPosition();
        pointLightPositionAndRange.w = renderProperties.pointLight->GetRange();
        Vector4 pointLightColor = renderProperties.pointLight->GetColor();
        pointLightColor.w = renderProperties.pointLight->GetIntesity();

        renderProperties.globalBindings.SetVector4(ShaderParams::PointLightColor.data(), pointLightColor);
        renderProperties.globalBindings.SetVector4(ShaderParams::PointLightPositionAndRange.data(), pointLightPositionAndRange);

        const Transform *directLightTransform = renderProperties.directLight->GetEntity()->GetComponent<Transform>();
        Vector3 directLightDirection = directLightTransform->GetForward();
        Vector4 directLightColor = renderProperties.directLight->GetColor();
        directLightColor.w = renderProperties.directLight->GetIntesity();

        renderProperties.globalBindings.SetVector4(ShaderParams::DirectLightColor.data(), directLightColor);
        renderProperties.globalBindings.SetVector3(ShaderParams::DirectLightDir.data(), directLightDirection);

        const Transform *spotLightTransform = renderProperties.spotLight->GetEntity()->GetComponent<Transform>();
        Vector4 spotLightDirAndAngle = spotLightTransform->GetForward();
        spotLightDirAndAngle.w = renderProperties.spotLight->GetSpotAngle();
        Vector4 spotLightColor = renderProperties.spotLight->GetColor();
        spotLightColor.w = renderProperties.spotLight->GetIntesity();
        Vector4 spotLightPosAndRange = spotLightTransform->GetPosition();
        spotLightPosAndRange.w = renderProperties.spotLight->GetRange();

        renderProperties.globalBindings.SetVector4(ShaderParams::SpotLightColor.data(), spotLightColor);
        renderProperties.globalBindings.SetVector4(ShaderParams::SpotLightPositionAndRange.data(), spotLightPosAndRange);
        renderProperties.globalBindings.SetVector4(ShaderParams::SpotLightDirAndAngle.data(), spotLightDirAndAngle);
    }
    
    void RenderSystem::UpdateShadowLightUniform()
    {
        using namespace SRenderSystem::Shadow;
        
        if(renderProperties.directShadowLight)
        {
            std::vector<float> farDistances = CalculateCascadeFarDistances(renderProperties);
            std::vector<Matrix4> lightViewProjectionMatrices = GetDirectShadowMatrices(renderProperties, farDistances);
            renderProperties.globalBindings.SetArrayMatrices4(ShaderParams::DirectShadowLightViewProj.data(), lightViewProjectionMatrices);
            renderProperties.globalBindings.SetArrayFloats(ShaderParams::CascadeDistances.data(), farDistances);
            renderProperties.globalBindings.SetInt(
                                  ShaderParams::CascadesCount.data(),
                                  static_cast<int32_t>(renderProperties.cascadeBorders.size()) + 1);
        }

        if(renderProperties.pointShadowLight)
        {
            std::vector<Matrix4> matrices = GetOmnidirectionalShadowLightMatrices(renderProperties.pointShadowLight);
            const Light* light = renderProperties.pointShadowLight->GetComponent<Light>();
            renderProperties.globalBindings.SetArrayMatrices4(ShaderParams::OmniShadowLightViewProj.data(), matrices);
            renderProperties.globalBindings.SetFloat(ShaderParams::FarClipPlane.data(), light->GetRange());
        }
    }
    
    void RenderSystem::BindCamUniformBuffer()
    {
        const Entity *camEntity = renderProperties.cam->GetEntity();
        const Transform *camTransform = camEntity->GetComponent<Transform>();

        const Matrix4 viewMatrix = renderProperties.cam->GetViewMatrix();
        const Matrix4 projMatrix = renderProperties.cam->GetProjectionMatrix();
        const Quaternion& camRotation = camTransform->GetRotation();
        const Matrix3 camRotationMat = camRotation.ToMatrix3();
        const Vector3& camPos = camTransform->GetPosition();
        const Vector3 camDir = camTransform->GetForward();

        glBindBuffer(GL_UNIFORM_BUFFER, camUbo);
        glBindBufferBase(GL_UNIFORM_BUFFER, RenderProperties::CamBufferBinding, camUbo);
        // Offsets are specified in shader's uniform block layout.
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), viewMatrix.data);
        glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(Matrix4), projMatrix.data);
        glBufferSubData(GL_UNIFORM_BUFFER, 128, sizeof(Vector3), camRotationMat.rows[0].data);
        glBufferSubData(GL_UNIFORM_BUFFER, 144, sizeof(Vector3), camRotationMat.rows[1].data);
        glBufferSubData(GL_UNIFORM_BUFFER, 160, sizeof(Vector3), camRotationMat.rows[2].data);
        glBufferSubData(GL_UNIFORM_BUFFER, 176, sizeof(Vector3), camPos.data);
        glBufferSubData(GL_UNIFORM_BUFFER, 192, sizeof(Vector3), camDir.data);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    bool RenderSystem::IsOffscreenRender() const
    {
        return hdr && toneMappingMaterial;
    }
    
    void RenderSystem::RenderOnScreenQuad(const FrameBuffer& screen)
    {
        if (quadBuffer.vao == 0)
        {
            quadBuffer = Render::CreateQuadBuffer();
        }
        
        glViewport(0, 0, Screen::ScreenSize.width, Screen::ScreenSize.height);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::Default);
        Print_Errors_OpengGL();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        glBindVertexArray(quadBuffer.vao);
        
        Shader *shader = toneMappingMaterial->GetShader();
        shader->StartUse();
        Render::ResolveBindings(*shader, renderProperties.globalBindings, { });
        Render::SetDrawFace(eDrawFace::Cw);
        Print_Errors_OpengGL();

        FrameBuffer screenProcessed = BlitFrameBuffer(screen);
        glActiveTexture(GL_TEXTURE0);
        Print_Errors_OpengGL();
        glBindTexture(GL_TEXTURE_2D, screenProcessed.color.front());
        Print_Errors_OpengGL();
        shader->SetUniformInt(ShaderParams::ScreenColorProcessed.data(), 0);
        Print_Errors_OpengGL();
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        shader->StopUse();
        Print_Errors_OpengGL();
        glBindVertexArray(Render::UndefinedBuffer);
        fboPool.Push(screenProcessed);
    }
    
    FrameBuffer RenderSystem::BlitFrameBuffer(const FrameBuffer& input)
    {
        FrameBuffer output = input;
        if (input.settings.multisample)
        {
            output = GetFrameBuffer(false);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, input.frame);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output.frame);
            glBlitFramebuffer(0, 0, input.settings.size.width, input.settings.size.height,
                              0, 0, output.settings.size.width, output.settings.size.height,
                              GL_COLOR_BUFFER_BIT, GL_NEAREST);
            fboPool.Push(input);
        }
        
        return output;
    }
    
    FrameBuffer RenderSystem::GetHdrBuffer(bool multisample)
    {
        FrameBuffer::Settings bufferSettings;
        bufferSettings.size = Screen::ScreenSize;
        bufferSettings.color = FrameBuffer::eAttachement::Texture2d;
        bufferSettings.depth = FrameBuffer::eAttachement::Buffer;
        bufferSettings.stencil = FrameBuffer::eAttachement::Buffer;
        bufferSettings.multisample = multisample;
        bufferSettings.combinedDepthStencil = true;
        bufferSettings.hdr = true;
        
        return fboPool.Pop(bufferSettings);
    }
    
    FrameBuffer RenderSystem::GetFrameBuffer(bool multisample)
    {
        FrameBuffer::Settings bufferSettings;
        bufferSettings.size = Screen::ScreenSize;
        bufferSettings.color = FrameBuffer::eAttachement::Texture2d;
        bufferSettings.depth = FrameBuffer::eAttachement::Buffer;
        bufferSettings.stencil = FrameBuffer::eAttachement::Buffer;
        bufferSettings.multisample = multisample;
        bufferSettings.combinedDepthStencil = true;
        bufferSettings.hdr = hdr;
        
        return fboPool.Pop(bufferSettings);
    }
    
    FrameBuffer RenderSystem::GetDirectShadowMapBuffer()
    {
        FrameBuffer::Settings bufferSettings;
        bufferSettings.size = shadowSetup.size;
        bufferSettings.color = FrameBuffer::eAttachement::None;
        bufferSettings.depth = FrameBuffer::eAttachement::Texture2dArray;
        bufferSettings.stencil = FrameBuffer::eAttachement::None;
        bufferSettings.multisample = false;
        bufferSettings.combinedDepthStencil = false;
        bufferSettings.depth2dArraySize = renderProperties.cascadeBorders.size() + 1;

        return fboPool.Pop(bufferSettings);
    }
    
    FrameBuffer RenderSystem::GetPointShadowMapBuffer()
    {
        FrameBuffer::Settings bufferSettings;
        bufferSettings.size = shadowSetup.size;
        bufferSettings.color = FrameBuffer::eAttachement::None;
        bufferSettings.depth = FrameBuffer::eAttachement::Cubemap;
        bufferSettings.stencil = FrameBuffer::eAttachement::None;
        bufferSettings.multisample = false;
        bufferSettings.combinedDepthStencil = false;

        return fboPool.Pop(bufferSettings);
    }
}
