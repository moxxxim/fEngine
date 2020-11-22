//#include "ResourcesManager.h"
//#include "ResourceManagerTextures.h"
//#include "ResourceManagerMeshes.h"
//#include "ResourceManagerShaders.h"
//#include "ResourceManagerMaterials.h"
//#include "ResourceManagerPostEffects.h"
//#include "ResourcesSerializer.h"
//#include "../Utils/Debug.h"
//
//fengine::ResourcesManager * fengine::ResourcesManager::GetInstance()
//{
//	static ResourcesManager resources;
//
//	return &resources;
//}
//
//fengine::ResourcesManager::ResourcesManager()
//{
//	ResourcesSerializer serializer;
//	if (serializer.Deserialize(*this))
//	{
//		Debug::LogError("Couldn't deserialize resources' file.");
//	}
//}
//
//fengine::ResourcesManager::~ResourcesManager()
//{
//	delete m_texturesManager;
//	delete m_meshesManager;
//	delete m_shadersManager;
//	delete m_materialsManager;
//	delete m_postEffectsManager;
//}
//
//unsigned int fengine::ResourcesManager::GetTextureInstancesCount() const
//{
//	return m_texturesManager->GetInstancesCount();
//}
//
//unsigned int fengine::ResourcesManager::GetMeshInstancesCount() const
//{
//	return m_meshesManager->GetInstancesCount();
//}
//
//unsigned int fengine::ResourcesManager::GetShaderInstancesCount() const
//{
//	return m_shadersManager->GetInstancesCount();
//}
//
//fengine::Texture * fengine::ResourcesManager::GetTexture(fengine::obj_id id)
//{
//	return m_texturesManager->GetInstance(id);
//}
//
//fengine::Mesh * fengine::ResourcesManager::GetMesh(fengine::obj_id id)
//{
//	return m_meshesManager->GetInstance(id);
//}
//
//fengine::Shader * fengine::ResourcesManager::GetShader(fengine::obj_id id)
//{
//	return m_shadersManager->GetInstance(id);
//}
//
//fengine::Material * fengine::ResourcesManager::GetMaterial(fengine::obj_id id)
//{
//	return m_materialsManager->GetInstance(id);
//}
//
//fengine::PostEffectDefinition * fengine::ResourcesManager::GetPostEffect(fengine::obj_id id)
//{
//	return m_postEffectsManager->GetInstance(id);
//}
//
//void fengine::ResourcesManager::Purge()
//{
//	m_texturesManager->Purge();
//	m_meshesManager->Purge();
//	m_shadersManager->Purge();
//	m_materialsManager->Purge();
//	m_postEffectsManager->Purge();
//}
//
//bool fengine::ResourcesManager::DestroyResource(unsigned long id)
//{
//	bool isDestoyed = m_texturesManager->DestroyResource(id);
//
//	if (!isDestoyed)
//	{
//		isDestoyed = m_meshesManager->DestroyResource(id);
//	}
//	
//	if (!isDestoyed)
//	{
//		isDestoyed = m_shadersManager->DestroyResource(id);
//	}
//	
//	if (!isDestoyed)
//	{
//		isDestoyed = m_materialsManager->DestroyResource(id);
//	}
//
//	if (!isDestoyed)
//	{
//		isDestoyed = m_postEffectsManager->DestroyResource(id);
//	}
//
//	return isDestoyed;
//}
