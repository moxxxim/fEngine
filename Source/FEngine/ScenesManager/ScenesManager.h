//#pragma once
//
//namespace fengine
//{
//
//class GameObject;
//class Scene;
//class RenderSystem;
//class Time;
//
//class ScenesManager final
//{
//
//public:
//
//	inline Scene * GetCurrentScene() const;
//	void Load(const char *sceneName);
//	void Update(float deltaTime);
//	void Draw(ESContext *esContext);
//
//	static ScenesManager * GetInstance();
//
//private:
//
//	void CreateCurrentScene(const char *sceneFile);
//	void DeserializeScene(Scene *scene, const char *sceneFile) const;
//	void UpdateTime(float deltaTime);
//	void DestroyCurrentScene();
//	void PurgeResources();
//	void LoadCachedScene();
//
//	ScenesManager();
//	~ScenesManager();
//	ScenesManager(const ScenesManager& manager) = delete;
//	ScenesManager& operator=(const ScenesManager& manager) = delete;
//
//	bool m_isLoadingScene = false;
//	char *m_cachedScene = nullptr;
//	RenderSystem *m_renderSystem = nullptr;
//	Scene *m_currentScene = nullptr;
//	Time *m_time = nullptr;
//	static ScenesManager *m_scenesManager;
//};
//
//inline Scene * ScenesManager::GetCurrentScene() const
//{
//	return m_currentScene;
//}
//
//}
