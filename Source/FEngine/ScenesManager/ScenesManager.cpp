//#include "../App/AppSettings.h"
//#include "../App/Time.h"
//#include "../App/GuidGenerator.h"
//#include "../Utils/Debug.h"
//#include "../Serialization/GameObjectComponentsRegistration.h"
//#include "../ResourcesManager/ResourcesManager.h"
//#include "ScenesManager.h"
//#include "Scene.h"
//#include "RenderSystem.h"
//#include "SceneSerializer.h"
//
//fengine::ScenesManager *fengine::ScenesManager::m_scenesManager = nullptr;
//
//fengine::ScenesManager * fengine::ScenesManager::GetInstance()
//{
//	if (!m_scenesManager)
//	{
//		fengine::RegisterSerializableObjects();
//		m_scenesManager = new ScenesManager();
//		const char *initialScene = AppSettings::GetInitialSceneLocation();
//		m_scenesManager->CreateCurrentScene(initialScene);
//	}
//
//	return m_scenesManager;
//}
//
//fengine::ScenesManager::ScenesManager() :
//	m_renderSystem(new RenderSystem()),
//	m_time(Time::GetInstance())
//{
//}
//
//fengine::ScenesManager::~ScenesManager()
//{
//	delete m_currentScene;
//	delete m_renderSystem;
//}
//
//void fengine::ScenesManager::Load(const char *sceneName)
//{
//	m_isLoadingScene = true;
//	m_cachedScene = new char[strlen(sceneName) + 1];
//	strcpy(m_cachedScene, sceneName);
//}
//
//void fengine::ScenesManager::Update(float deltaTime)
//{
//	UpdateTime(deltaTime);
//
//	if (m_isLoadingScene)
//	{
//		LoadCachedScene();
//	}
//	else
//	{
//		m_currentScene->Update(deltaTime);
//	}
//}
//
//void fengine::ScenesManager::Draw(ESContext *esContext)
//{
//	m_renderSystem->Draw(esContext);
//}
//
//void fengine::ScenesManager::CreateCurrentScene(const char *sceneFile)
//{
//	m_currentScene = new Scene(m_renderSystem);
//	DeserializeScene(m_currentScene, sceneFile);
//}
//
//void fengine::ScenesManager::DeserializeScene(Scene *scene, const char *sceneFile) const
//{
//	SceneSerializer serializer;
//	if (serializer.Deserialize(scene, sceneFile))
//	{
//		const char *message[] = {
//			"Couldn't deserialize scene: ",
//			sceneFile
//		};
//
//		fengine::Debug::LogError(message, 2);
//	}
//}
//
//void fengine::ScenesManager::UpdateTime(float deltaTime)
//{
//	m_time->m_time += deltaTime;
//	m_time->m_sceneTime += deltaTime;
//	m_time->m_deltaTime = deltaTime;
//}
//
//void fengine::ScenesManager::DestroyCurrentScene()
//{
//	delete m_currentScene;
//	m_currentScene = nullptr;
//	m_time->m_sceneTime = 0;
//	m_renderSystem->Reset();
//	
//	GuidGenerator::ResetNextSceneObjectId();
//}
//
//void fengine::ScenesManager::PurgeResources()
//{
//	ResourcesManager *manager = ResourcesManager::GetInstance();
//	manager->Purge();
//}
//
//void fengine::ScenesManager::LoadCachedScene()
//{
//	DestroyCurrentScene();
//	PurgeResources();
//
//	const char *sceneFile = AppSettings::GetSceneLocation(m_cachedScene);
//	CreateCurrentScene(sceneFile);
//	m_isLoadingScene = false;
//	delete[] m_cachedScene;
//}
