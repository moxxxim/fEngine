//#pragma once
//
//#include "ScenesManager.h"
//#include "SceneSettings.h"
//#include "../App/Globals.h"
//
//namespace feng
//{
//
//class GameObject;
//class Renderer;
//class RenderSystem;
//class Texture;
//class Mesh;
//class SceneSerializer;
//
//class Scene final
//{
//
//	friend SceneSerializer;
//	friend ScenesManager;
//
//public:
//
//	inline unsigned int GetGameObjectsCount() const;
//	GameObject * CreateGameObject();
//	void DestroyGameObject(GameObject *go);
//	inline SceneSettings * GetSettings();
//	GameObject * GetGameObject(obj_id id) const;
//	inline LinkedListIterator<GameObject> GetGameObjectsIterator() const;
//
//private:
//
//	Scene(RenderSystem *renderSystem);
//	~Scene();
//
//	void Update(float deltaTime);
//	void AddGameObject(GameObject *go);
//	void RemoveGameObject(GameObject *go);
//	void SetSkybox(Renderer *renderer);
//	void RemoveSkybox();
//
//	RenderSystem *m_renderSystem = nullptr;
//	SceneSettings *m_settings = nullptr;
//	LinkedList<GameObject> m_gameObjects;
//	Renderer *m_skybox = nullptr;
//};
//
//inline unsigned int Scene::GetGameObjectsCount() const
//{
//	return m_gameObjects.GetSize();
//}
//
//inline SceneSettings * Scene::GetSettings()
//{
//	return m_settings;
//}
//
//inline inline LinkedListIterator<GameObject> feng::Scene::GetGameObjectsIterator() const
//{
//	return m_gameObjects.GetIterator();
//}
//}
