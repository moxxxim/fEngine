#pragma once

#include <Feng/Render/FrameBuffersPool.h>
#include <Feng/Render/FrameBuffer.h>
#include <Feng/Render/PostEffects/RenderPostProcessing.h>
#include <Feng/ScenesManager/RenderProperties.h>

#include <vector>

namespace feng
{
    class Camera;
    class Entity;
    class Light;
    class MeshRenderer;
    class PostEffectDefinition;

    class RenderSystem final
    {
    public:
        RenderSystem();
        ~RenderSystem();

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
        std::vector<MeshRenderer*> renderers;
        std::vector<Light*> lights;
        FrameBuffersPool buffersPool;
        RenderPostProcessing postProcessing;
        RenderProperties renderProperties;
        FrameBuffer frameBuffer;
        MeshRenderer *skybox = nullptr;
    };

    inline void RenderSystem::SetSkybox(MeshRenderer *aSkybox)
    {
        skybox = aSkybox;
    }
}
