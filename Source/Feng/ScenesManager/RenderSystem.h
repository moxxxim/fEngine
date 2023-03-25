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

        void SetAmbientLight(Vector4 color, float intensity);

        void SetCamera(Camera *camera);
        void SetSkybox(MeshRenderer *aSkybox);
        void SetShadowMaterials(Material *directLightShadowMaterial,
                               Material *pointLightShadowMaterial,
                               Material *directLightShadowDebugMaterial);
        void SetCascadesCount(int32_t aCascadesCount);
        void SetDirectionalShadowLight(Entity *light);
        void SetPointShadowLight(Entity *light);

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
            int32_t cascadesCount = 1;
        };
        
        bool IsShadowsEnabled();
        void CreateCamUniformBuffer();
        void BindCamUniformBuffer();
        void DrawShadowMap();
        void DrawDirectShadowMap();
        void DrawPointShadowMap();
        void DrawShadowCastersInShadowMap(Material *shadowMaterial);
        void DrawDebugDirectShadowMap();
        void DrawDebugOmniShadowMap();
        void DrawGeneric();
        void DrawOpaque();
        void DrawTransparent();
        void DrawSkybox();
        void ApplyPostEffects(const FrameBuffer& renderBuffer);
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
        uint32_t camUbo = 0;
    };

    inline void RenderSystem::SetSkybox(MeshRenderer *aSkybox)
    {
        skybox = aSkybox;
    }
}
