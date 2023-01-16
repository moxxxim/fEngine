#pragma once

#include <Feng/Render/FrameBuffersPool.h>
#include <Feng/Render/FrameBuffer.h>
#include <Feng/Render/PostEffects/Core/RenderPostProcessing.h>
#include <Feng/ScenesManager/RenderProperties.h>

#include <cstdint>
#include <vector>

namespace Feng
{
    class Camera;
    class Entity;
    class Light;
    class MeshRenderer;
    class PostEffectDefinition;

    class RenderSystem final
    {
    public:
        struct ShadowSetup final
        {
            Entity *light = nullptr;
            int32_t width = 0;
            int32_t height = 0;
        };
        
        RenderSystem();
        ~RenderSystem();

        void SetShadowSetup(const ShadowSetup& aShadowSetup);
        void SetAmbientLight(Vector4 color, float intensity);

        void SetCamera(Camera *camera);
        void SetSkybox(MeshRenderer *aSkybox);

        void AddRenderer(MeshRenderer *renderer);
        void RemoveRenderer(MeshRenderer *renderer);

        void AddLight(Light *light);

        void SetPostEffect(PostEffectDefinition *postEffect);
        void RemovePostEffect();
        void Draw();

    private:
        void CreateCamUniformBuffer();
        void BindCamUniformBuffer();
        void DrawShadowMap();
        void DrawOpaque();
        void DrawTransparent();
        void DrawSkybox();
        void ApplyPostEffects(const FrameBuffer& renderBuffer);
        FrameBuffer GetFrameBuffer(bool multisample);

        static constexpr int32_t DefaultShadowMapSize = 1024;
        
        std::vector<MeshRenderer*> renderersTransparent;
        std::vector<MeshRenderer*> renderersOpaque;
        std::vector<Light*> lights;
        ShadowSetup shadowSetup;
        FrameBuffersPool fboPool;
        RenderPostProcessing postProcessing;
        RenderProperties renderProperties;
        FrameBuffer shadowMap;
        MeshRenderer *skybox = nullptr;
        uint32_t camUbo = 0;
    };

    inline void RenderSystem::SetSkybox(MeshRenderer *aSkybox)
    {
        skybox = aSkybox;
    }
}
