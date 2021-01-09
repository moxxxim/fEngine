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

    class RenderSystem final
    {
    public:
        RenderSystem();
        ~RenderSystem();

        void SetCamera(Camera *aCamera);
        void SetSkybox(MeshRenderer *aSkybox);

        void AddRenderer(MeshRenderer *renderer);
        void RemoveRenderer(MeshRenderer *renderer);

        void AddLight(Light *light);

        void Reset();
        void Draw();

    private:
        void SetupDraw();
        void DrawRenderers();
        void ApplyPostProcessing();
        void BindFrameBuffer();

        std::vector<MeshRenderer*> renderers;
        std::vector<Light*> lights;
        FrameBuffersPool buffersPool;
        RenderPostProcessing postProcessing;
        RenderProperties renderProperties;
        FrameBuffer frameBuffer;
        Camera *camera = nullptr;
        MeshRenderer *skybox = nullptr;
    };

    inline void RenderSystem::SetCamera(Camera *aCamera)
    {
        camera = aCamera;
    }

    inline void RenderSystem::SetSkybox(MeshRenderer *aSkybox)
    {
        skybox = aSkybox;
    }
}
