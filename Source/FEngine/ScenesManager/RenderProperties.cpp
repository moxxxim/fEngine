//#include "RenderProperties.h"
//#include "RenderSystem.h"
//#include "ScenesManager.h"
//#include "Scene.h"
//#include "Light.h"
//#include "GameObject.h"
//
//fengine::RenderProperties::RenderProperties(fengine::RenderSystem *renderSystem) :
//	m_renderSystem(renderSystem)
//{
//}
//
//const fengine::Fog& fengine::RenderProperties::GetFog() const
//{
//	fengine::ScenesManager *scenesManager = fengine::ScenesManager::GetInstance();
//	fengine::Scene *scene = scenesManager->GetCurrentScene();
//	fengine::SceneSettings *sceneSettings = scene->GetSettings();
//
//	return sceneSettings->GetFog();
//}
//
//const fengine::Camera * fengine::RenderProperties::GetCamera() const
//{
//	return m_renderSystem->m_camera;
//}
//
//Vector4 fengine::RenderProperties::GetAmbientLightColor() const
//{
//	fengine::ScenesManager *scenesManager = fengine::ScenesManager::GetInstance();
//	fengine::Scene *scene = scenesManager->GetCurrentScene();
//	fengine::SceneSettings *sceneSettings = scene->GetSettings();
//
//	return sceneSettings->GetAmbientLightColor();
//}
//
//void fengine::RenderProperties::Update()
//{
//	m_directLightDirections.Clear();
//	m_directLightColors.Clear();
//	m_directLightIntensities.Clear();
//	m_pointLightPositions.Clear();
//	m_pointLightRanges.Clear();
//	m_pointLightColors.Clear();
//	m_pointLightIntensities.Clear();
//
//	auto lights = m_renderSystem->m_lights.GetIterator();
//	while (lights.HasCurrent())
//	{
//		Light *light = lights.GetCurrent();
//		Light::Type lightType = light->GetType();
//		if (lightType == Light::Type::Directional)
//		{
//			CacheDirectLight(light);
//		}
//		else
//		{
//			CachePointLight(light);
//		}
//
//		lights.MoveNext();
//	}
//}
//
//void fengine::RenderProperties::CacheDirectLight(Light *light)
//{
//	Transform *lightTransform = light->GetTransform();
//
//	m_directLightDirections.Add(lightTransform->GetForward());
//	m_directLightColors.Add(light->GetColor());
//	m_directLightIntensities.Add(light->GetIntecity());
//}
//
//void fengine::RenderProperties::CachePointLight(Light *light)
//{
//	Transform *lightTransform = light->GetTransform();
//
//	m_pointLightPositions.Add(lightTransform->GetPosition());
//	m_pointLightRanges.Add(light->GetRange());
//	m_pointLightColors.Add(light->GetColor());
//	m_pointLightIntensities.Add(light->GetIntecity());
//}
