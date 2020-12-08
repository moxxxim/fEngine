//#include "Scene.h"
//#include "GameObject.h"
//#include "Renderer.h"
//#include "RenderSystem.h"
//#include "MeshRenderer.h"
//#include "../ResourcesManager/ResourcesManager.h"
//
//feng::Scene::Scene(RenderSystem *renderSystem) :
//	m_renderSystem(renderSystem),
//	m_settings(new SceneSettings(this))
//{
//}
//
//feng::Scene::~Scene()
//{
//	if (m_skybox)
//	{
//		m_renderSystem->RemoveSkybox(m_skybox);
//	}
//
//	if (m_gameObjects.GetSize() > 0)
//	{
//		auto iterator = m_gameObjects.GetIterator();
//		do
//		{
//			GameObject *go = iterator.GetCurrent();
//			if (go)
//			{
//				delete go;
//			}
//
//		} while (iterator.MoveNext());
//
//		m_gameObjects.Clear();
//	}
//
//	delete[] m_settings;
//}
//
//feng::GameObject * feng::Scene::CreateGameObject()
//{
//	feng::GameObject *go = new GameObject(this, m_renderSystem);
//	AddGameObject(go);
//
//	return go;
//}
//
//void feng::Scene::DestroyGameObject(GameObject *go)
//{
//	RemoveGameObject(go);
//	delete go;
//}
//
//feng::GameObject * feng::Scene::GetGameObject(obj_id id) const
//{
//	if (m_gameObjects.GetSize() > 0)
//	{
//		auto gameObjectsIterator = m_gameObjects.GetIterator();
//		do
//		{
//			GameObject *go = gameObjectsIterator.GetCurrent();
//			if (go->GetId() == id)
//			{
//				return go;
//			}
//
//		} while (gameObjectsIterator.MoveNext());
//	}
//
//	return nullptr;
//}
//
//void feng::Scene::Update(float deltaTime)
//{
//	if (m_gameObjects.GetSize() > 0)
//	{
//		auto gameObjectsIterator = m_gameObjects.GetIterator();
//		do
//		{
//			GameObject *go = gameObjectsIterator.GetCurrent();
//			if (go->IsEnabled())
//			{
//				go->Update(deltaTime);
//			}
//
//		} while (gameObjectsIterator.MoveNext());
//	}
//}
//
//void feng::Scene::AddGameObject(feng::GameObject *go)
//{
//	Renderer *renderer = go->GetRenderer();
//	if (renderer)
//	{
//		m_renderSystem->AddRenderer(renderer);
//	}
//
//	m_gameObjects.Add(go);
//}
//
//void feng::Scene::RemoveGameObject(feng::GameObject *go)
//{
//	Renderer *renderer = go->GetRenderer();
//	if (renderer)
//	{
//		m_renderSystem->RemoveRenderer(renderer);
//	}
//
//	m_gameObjects.Remove(go);
//}
//
//void feng::Scene::SetSkybox(Renderer *skybox)
//{
//	if (m_skybox != skybox)
//	{
//		m_skybox = skybox;
//		m_renderSystem->SetSkybox(m_skybox);
//	}
//}
//
//void feng::Scene::RemoveSkybox()
//{
//	if (m_skybox)
//	{
//		m_renderSystem->RemoveSkybox(m_skybox);
//
//		delete m_skybox;
//		m_skybox = nullptr;
//	}
//}
