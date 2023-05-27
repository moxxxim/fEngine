#pragma once

#include <Feng/Math/Size.h>
#include <Feng/Render/FrameBuffersPool.h>
#include <Feng/Render/FrameBuffer.h>
#include <Feng/Render/PostEffects/Core/RenderPostProcessing.h>
#include <Feng/Render/VertexBuffer.h>
#include <Feng/ScenesManager/RenderProperties.h>

#include <cstdint>
#include <vector>

namespace Feng
{
    class Camera;
    class Entity;
    class Light;
    class Material;
    class MeshRenderer;
    class PostEffectDefinition;

    class RenderSystem final
    {
    public:
        static constexpr int32_t DefaultShadowMapSize = 1024;
        
        RenderSystem();
        ~RenderSystem();

        const ShaderBindings& GlobalBindings() const;
        ShaderBindings& GlobalBindings();
        void SetAmbientLight(Vector4 color, float intensity);

        void SetCamera(Camera *camera);
        void SetSkybox(MeshRenderer *aSkybox);
        void SetShadowMaterials(Material *directLightShadowMaterial,
                               Material *pointLightShadowMaterial,
                               Material *directLightShadowDebugMaterial);
        void SetToneMappingMaterial(Material* toneMapping);
        void SetCascadeBorders(std::vector<float> aCascadeBorders);
        void SetDirectionalShadowLight(Entity *light);
        void SetPointShadowLight(Entity *light);
        bool IsHdr() const;
        void SetHdr(bool value);

        void AddRenderer(MeshRenderer *renderer);
        void RemoveRenderer(MeshRenderer *renderer);

        void AddLight(Light *light);

        void SetPostEffect(PostEffectDefinition *postEffect);
        void RemovePostEffect();
        void Draw();

    private:
        struct ShadowSetup final
        {
            Material *directLightShadowMaterial = nullptr;
            Material *pointLightShadowMaterial = nullptr;
            Material *directLightShadowDebugMaterial = nullptr;
            Size2ui size { DefaultShadowMapSize, DefaultShadowMapSize };
            FrameBuffer directShadowMap;
            FrameBuffer pointShadowMap;
        };
        
        bool IsShadowsEnabled();
        void CreateCamUniformBuffer();
        void UpdateShadowMap();
        void UpdateDirectShadowMap();
        void UpdatePointShadowMap();
        void DrawShadowCastersInShadowMap(Material *shadowMaterial);
        void DrawDebugDirectShadowMap();
        void DrawDebugOmniShadowMap();
        void DrawGeneric();
        void DrawOpaque();
        void DrawTransparent();
        void DrawSkybox();
        void ApplyPostEffects(const FrameBuffer& renderBuffer, const FrameBuffer& outBuffer);
        void UpdateGlobalBindings();
        void UpdateCameraUniforms();
        void UpdateLightUniforms();
        void UpdateShadowLightUniform();
        void BindCamUniformBuffer();
        bool IsOffscreenRender() const;
        void RenderOnScreenQuad(const FrameBuffer& screen);
        FrameBuffer BlitFrameBuffer(const FrameBuffer& input);
        FrameBuffer GetHdrBuffer(bool multisample);
        FrameBuffer GetFrameBuffer(bool multisample);
        FrameBuffer GetDirectShadowMapBuffer();
        FrameBuffer GetPointShadowMapBuffer();
        
        std::vector<MeshRenderer*> renderersTransparent;
        std::vector<MeshRenderer*> renderersOpaque;
        std::vector<Light*> lights;
        ShadowSetup shadowSetup;
        VertexBuffer quadBuffer;
        FrameBuffersPool fboPool;
        RenderPostProcessing postProcessing;
        RenderProperties renderProperties;
        MeshRenderer *skybox = nullptr;
        Material *toneMappingMaterial = nullptr;
        uint32_t camUbo = 0;
        bool hdr = false;
    };

    inline const ShaderBindings& RenderSystem::GlobalBindings() const
    {
        return renderProperties.globalBindings;
    }
    
    inline ShaderBindings& RenderSystem::GlobalBindings()
    {
        return const_cast<ShaderBindings&>(static_cast<const RenderSystem*>(this)->GlobalBindings());
    }
    
    inline void RenderSystem::SetSkybox(MeshRenderer *aSkybox)
    {
        skybox = aSkybox;
    }
}
