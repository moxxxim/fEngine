//#include "../../Globals.h"
//#include "RenderSystem.h"
//#include "Camera.h"
//#include "Renderer.h"
//#include "RenderPostProcessing.h"
//
//fengine::RenderSystem::RenderSystem() :
//	m_renderProperties(this),
//	m_postProcessing(fengine::RenderPostProcessing::GetInstance())
//{
//	m_frameBuffer = m_buffersPool.GetBuffer(true);
//}
//
//fengine::RenderSystem::~RenderSystem()
//{
//	m_buffersPool.PutBuffer(m_frameBuffer);
//	m_frameBuffer = FrameBuffer(0, 0, 0);
//}
//
//void fengine::RenderSystem::SetCamera(fengine::Camera *camera)
//{
//	m_camera = camera;
//}
//
//void fengine::RenderSystem::SetSkybox(fengine::Renderer *skyboxRenderer)
//{
//	m_skybox = skyboxRenderer;
//}
//
//void fengine::RenderSystem::AddRenderer(fengine::Renderer *renderer)
//{
//	if (!m_renderers.Contains(renderer))
//	{
//		m_renderers.Add(renderer);
//	}
//}
//
//void fengine::RenderSystem::AddLight(fengine::Light *light)
//{
//	if (!m_lights.Contains(light))
//	{
//		m_lights.Add(light);
//	}
//}
//
//void fengine::RenderSystem::RemoveSkybox(fengine::Renderer *skyboxRenderer)
//{
//	if (m_skybox == skyboxRenderer)
//	{
//		m_skybox = nullptr;
//	}
//}
//
//void fengine::RenderSystem::RemoveCamera(fengine::Camera *camera)
//{
//	if (m_camera == camera)
//	{
//		m_camera = nullptr;
//	}
//}
//
//void fengine::RenderSystem::RemoveRenderer(fengine::Renderer *renderer)
//{
//	m_renderers.Remove(renderer);
//}
//
//void fengine::RenderSystem::RemoveLight(fengine::Light *light)
//{
//	m_lights.Remove(light);
//}
//
//void fengine::RenderSystem::Reset()
//{
//	m_renderers.Clear();
//	m_lights.Clear();
//	m_postProcessing->RemoveEffects();
//	m_camera = nullptr;
//}
//
//void fengine::RenderSystem::Draw(ESContext *esContext)
//{
//	SetupDraw();
//	DrawRenderers(esContext);
//	ApplyPostProcessing();
//
//	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
//}
//
//void fengine::RenderSystem::SetupDraw()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	m_renderProperties.Update();
//
//	if (m_postProcessing->HasPostEffects())
//	{
//		BindFrameBuffer();
//	}
//}
//
//void fengine::RenderSystem::DrawRenderers(ESContext *esContext)
//{
//	if (m_skybox)
//	{
//		m_skybox->Draw(&m_renderProperties);
//	}
//
//	if (m_camera && m_renderers.GetSize() > 0)
//	{
//		auto renderersIterator = m_renderers.GetIterator();
//		do
//		{
//			Renderer *renderer = renderersIterator.GetCurrent();
//			bool isSkybox = renderer == m_skybox;
//			if (!isSkybox && renderer->IsGameObjectEnabled())
//			{
//				renderer->Draw(&m_renderProperties);
//			}
//
//		} while (renderersIterator.MoveNext());
//	}
//}
//
//void fengine::RenderSystem::ApplyPostProcessing()
//{
//	if (m_frameBuffer.Frame)
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		m_postProcessing->ApplyPostEffects(m_frameBuffer);
//	}
//}
//
//void fengine::RenderSystem::BindFrameBuffer()
//{
//	if (!m_frameBuffer.Frame)
//	{
//		m_frameBuffer = m_buffersPool.GetBuffer(true);
//	}
//	else
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer.Frame);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//}
