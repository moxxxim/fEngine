//#pragma once
//
//#include "RenderProperties.h"
//#include "FrameBuffersPool.h"
//
//namespace fengine
//{
//
//class Renderer;
//class Camera;
//class ScenesManager;
//class RenderPostProcessing;
//
//class RenderSystem
//{
//
//	friend ScenesManager;
//	friend RenderProperties;
//
//public:
//
//	void SetCamera(Camera *camera);
//	void SetSkybox(Renderer *skyboxRenderer);
//	void AddRenderer(Renderer *renderer);
//	void AddLight(Light *light);
//	
//	void RemoveCamera(Camera *camera);
//	void RemoveSkybox(Renderer *skyboxRenderer);
//	void RemoveRenderer(Renderer *renderer);
//	void RemoveLight(Light *light);
//
//	void Reset();
//	void Draw(ESContext *esContext);
//
//private:
//
//	RenderSystem();
//	~RenderSystem();
//
//	void SetupDraw();
//	void DrawRenderers(ESContext *esContext);
//	void ApplyPostProcessing();
//	void BindFrameBuffer();
//
//	LinkedList<Renderer> m_renderers;
//	LinkedList<Light> m_lights;
//	Camera *m_camera = nullptr;
//	Renderer *m_skybox = nullptr;
//	FrameBuffersPool m_buffersPool;
//	RenderPostProcessing *m_postProcessing = nullptr;
//	RenderProperties m_renderProperties;
//	FrameBuffer m_frameBuffer;
//};
//
//}
