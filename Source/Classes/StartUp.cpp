//#include <Classes/StartUp.h>
//
//#include <Classes/Globals.h>
//
//feng::ScenesManager *scenesManager;
//
//int Init(ESContext *esContext)
//{
//	scenesManager = feng::ScenesManager::GetInstance();
//
//	return 0;
//}
//
//void Draw(ESContext *esContext)
//{
//	scenesManager->Draw(esContext);
//}
//
//void Update(ESContext *esContext, float deltaTime)
//{
//	scenesManager->Update(deltaTime);
//}
//
//void Key(ESContext *esContext, unsigned char key, bool bIsPressed)
//{
//	feng::Input *inputManager = feng::Input::GetInstance();
//	inputManager->PressKey(key, bIsPressed);
//}
//
//void CleanUp()
//{
//
//}
//
////////////////////////////////////////////// WINDOWS main function ////////////////////////////////////////////////////
//#ifdef _WIN32
//
//#include <conio.h>
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	ESContext esContext;
//
//	esInitContext(&esContext);
//
//	esCreateWindow(&esContext, "Some app", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
//
//	if (Init(&esContext) != 0)
//		return 0;
//
//	esRegisterDrawFunc(&esContext, Draw);
//	esRegisterUpdateFunc(&esContext, Update);
//	esRegisterKeyFunc(&esContext, Key);
//
//	esMainLoop(&esContext);
//
//	// releasing OpenGL resources
//	CleanUp();
//
//	printf("Press any key...\n");
//	_getch();
//
//	return 0;
//}
//
//#endif
////////////////////////////////////////////// WINDOWS main function ////////////////////////////////////////////////////
