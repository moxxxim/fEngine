//#include <FEngine/App/AppSettings.h>
//
//#include <FEngine/Utils/Debug.h>
//
//static const int k_scenesCount = 5;
//
//static const char *k_sceneNames[] =
//{
//	"MainScene",
//	"TerrainScene",
//	"EmptyScene",
//	"Misc",
//	"Light"
//};
//
//static const char *k_sceneLocations[] =
//{
//	"../Resources/Scenes/MainScene.txt",
//	"../Resources/Scenes/TerrainScene.txt",
//	"../Resources/Scenes/EmptyScene.txt",
//	"../Resources/Scenes/Misc.txt",
//	"../Resources/Scenes/Light.txt"
//};
//
//int fengine::AppSettings::GetScenesCount()
//{
//	return k_scenesCount;
//}
//
//const char ** fengine::AppSettings::GetScenes()
//{
//	return k_sceneNames;
//}
//
//int fengine::AppSettings::GetSceneIndex(const char *sceneName)
//{
////	for (int i = 0; i < k_scenesCount; ++i)
////	{
////		const char *sceneName2 = k_sceneNames[i];
////		if (!strcmp(sceneName, sceneName2))
////		{
////			return i;
////		}
////	}
//
//	return -1;
//}
//
//const char * fengine::AppSettings::GetInitialSceneLocation()
//{
//	return k_sceneLocations[4];
//}
//
//const char * fengine::AppSettings::GetSceneLocation(const char *sceneName)
//{
//	int sceneIndex = GetSceneIndex(sceneName);
//
//	if (sceneIndex >= 0)
//	{
//		return k_sceneLocations[sceneIndex];
//	}
//
////	const char *message[] =
////	{
////		"AppSettings.cpp: ",
////		"There is no scene with name ",
////		sceneName
////	};
////
////	Debug::LogMessage(message, 3);
//
//	return nullptr;
//}
