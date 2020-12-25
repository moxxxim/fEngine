//#include "RenderSystem.h"
//
//#include "Camera.h"
//#include "Renderer.h"
//#include "RenderPostProcessing.h"
//
//feng::RenderSystem::RenderSystem() :
//	m_renderProperties(this),
//	m_postProcessing(feng::RenderPostProcessing::GetInstance())
//{
//	m_frameBuffer = m_buffersPool.GetBuffer(true);
//}
//
//feng::RenderSystem::~RenderSystem()
//{
//	m_buffersPool.PutBuffer(m_frameBuffer);
//	m_frameBuffer = FrameBuffer(0, 0, 0);
//}
//
//void feng::RenderSystem::SetCamera(feng::Camera *camera)
//{
//	m_camera = camera;
//}
//
//void feng::RenderSystem::SetSkybox(feng::Renderer *skyboxRenderer)
//{
//	m_skybox = skyboxRenderer;
//}
//
//void feng::RenderSystem::AddRenderer(feng::Renderer *renderer)
//{
//	if (!m_renderers.Contains(renderer))
//	{
//		m_renderers.Add(renderer);
//	}
//}
//
//void feng::RenderSystem::AddLight(feng::Light *light)
//{
//	if (!m_lights.Contains(light))
//	{
//		m_lights.Add(light);
//	}
//}
//
//void feng::RenderSystem::RemoveSkybox(feng::Renderer *skyboxRenderer)
//{
//	if (m_skybox == skyboxRenderer)
//	{
//		m_skybox = nullptr;
//	}
//}
//
//void feng::RenderSystem::RemoveCamera(feng::Camera *camera)
//{
//	if (m_camera == camera)
//	{
//		m_camera = nullptr;
//	}
//}
//
//void feng::RenderSystem::RemoveRenderer(feng::Renderer *renderer)
//{
//	m_renderers.Remove(renderer);
//}
//
//void feng::RenderSystem::RemoveLight(feng::Light *light)
//{
//	m_lights.Remove(light);
//}
//
//void feng::RenderSystem::Reset()
//{
//	m_renderers.Clear();
//	m_lights.Clear();
//	m_postProcessing->RemoveEffects();
//	m_camera = nullptr;
//}
//
//void feng::RenderSystem::Draw(ESContext *esContext)
//{
//	SetupDraw();
//	DrawRenderers(esContext);
//	ApplyPostProcessing();
//
//	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
//}
//
//void feng::RenderSystem::SetupDraw()
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
//void feng::RenderSystem::DrawRenderers(ESContext *esContext)
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
//void feng::RenderSystem::ApplyPostProcessing()
//{
//	if (m_frameBuffer.Frame)
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		m_postProcessing->ApplyPostEffects(m_frameBuffer);
//	}
//}
//
//void feng::RenderSystem::BindFrameBuffer()
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
