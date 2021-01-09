#include <Feng/ScenesManager/RenderSystem.h>

#include <Feng/App/Globals.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Light.h>

namespace feng
{
    RenderSystem::RenderSystem()
    {
#ifdef __APPLE__
        frameBuffer = buffersPool.CreateBuffer(2 * screen::ScreenWidth, 2 * screen::ScreenHeight, true);
#else
        frameBuffer = buffersPool.CreateBuffer(screen::ScreenWidth, screen::ScreenHeight, true);
#endif
    }

    RenderSystem::~RenderSystem()
    {
        buffersPool.DeleteBuffer(frameBuffer);
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

    void RenderSystem::Reset()
    {
//        m_renderers.Clear();
//        m_lights.Clear();
//        m_postProcessing->RemoveEffects();
//        m_camera = nullptr;
    }

    void RenderSystem::Draw()
    {
//        SetupDraw();
//        DrawRenderers(esContext);
//        ApplyPostProcessing();
//
//        eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
    }

    void RenderSystem::SetupDraw()
    {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        m_renderProperties.Update();
//
//        if (m_postProcessing->HasPostEffects())
//        {
//            BindFrameBuffer();
//        }
    }

    void RenderSystem::DrawRenderers()
    {
//        if (m_skybox)
//        {
//            m_skybox->Draw(&m_renderProperties);
//        }
//
//        if (m_camera && m_renderers.GetSize() > 0)
//        {
//            auto renderersIterator = m_renderers.GetIterator();
//            do
//            {
//                Renderer *renderer = renderersIterator.GetCurrent();
//                bool isSkybox = renderer == m_skybox;
//                if (!isSkybox && renderer->IsGameObjectEnabled())
//                {
//                    renderer->Draw(&m_renderProperties);
//                }
//
//            } while (renderersIterator.MoveNext());
//        }
    }

    void RenderSystem::ApplyPostProcessing()
    {
//        if (m_frameBuffer.Frame)
//        {
//            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//            m_postProcessing->ApplyPostEffects(m_frameBuffer);
//        }
    }

    void RenderSystem::BindFrameBuffer()
    {
//        if (!m_frameBuffer.Frame)
//        {
//            m_frameBuffer = m_buffersPool.GetBuffer(true);
//        }
//        else
//        {
//            glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer.Frame);
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        }
    }
}
