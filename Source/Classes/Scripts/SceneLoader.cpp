//#include "SceneLoader.h"
//#include <iostream>
//
//bool SceneLoader::m_wasKeyPressed = false;
//int SceneLoader::s_sceneIndex = 0;
//
//void SceneLoader::Start()
//{
////	m_input = fengine::Input::GetInstance();
////	m_scenesManager = fengine::ScenesManager::GetInstance();
//}
//
//void SceneLoader::Update(float deltaTime)
//{
////	if (m_input)
////	{
////		bool isKeyPressed = m_input->IsKeyPressed();
////		if (!m_wasKeyPressed && isKeyPressed)
////		{
////			unsigned char pressedKey = m_input->GetPressedKey();
////			if (k_switchSceneKey == pressedKey)
////			{
////				int scenesCount = fengine::AppSettings::GetScenesCount();
////				int nextScene = (s_sceneIndex + 1) % scenesCount;
////				LoadScene(nextScene);
////			}
////		}
////
////		m_wasKeyPressed = isKeyPressed;
////	}
//}
//
//void SceneLoader::LoadScene(int index)
//{
////	s_sceneIndex = index;
////	const char *scene = GetSceneName(s_sceneIndex);
////	m_scenesManager->Load(scene);
//}
//
//const char * SceneLoader::GetSceneName(int index)
//{
////	const char ** scenes = fengine::AppSettings::GetScenes();
////	return scenes[index];
//    return "";
//}
