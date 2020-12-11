//#include "ResourcesManager.h"
//#include "ResourceManagerTextures.h"
//#include "ResourceManagerMeshes.h"
//#include "ResourceManagerShaders.h"
//#include "ResourceManagerMaterials.h"
//#include "ResourceManagerPostEffects.h"
//#include "ResourcesSerializer.h"
//#include "../Utils/Debug.h"
//
//feng::ResourcesManager * feng::ResourcesManager::GetInstance()
//{
//	static ResourcesManager resources;
//
//	return &resources;
//}
//
//feng::ResourcesManager::ResourcesManager()
//{
//	ResourcesSerializer serializer;
//	if (serializer.Deserialize(*this))
//	{
//		Debug::LogError("Couldn't deserialize resources' file.");
//	}
//}
//
//feng::ResourcesManager::~ResourcesManager()
//{
//	delete m_texturesManager;
//	delete m_meshesManager;
//	delete m_shadersManager;
//	delete m_materialsManager;
//	delete m_postEffectsManager;
//}
//
//unsigned int feng::ResourcesManager::GetTextureInstancesCount() const
//{
//	return m_texturesManager->GetInstancesCount();
//}
//
//unsigned int feng::ResourcesManager::GetMeshInstancesCount() const
//{
//	return m_meshesManager->GetInstancesCount();
//}
//
//unsigned int feng::ResourcesManager::GetShaderInstancesCount() const
//{
//	return m_shadersManager->GetInstancesCount();
//}
//
//feng::Texture * feng::ResourcesManager::GetTexture(feng::obj_id id)
//{
//	return m_texturesManager->GetInstance(id);
//}
//
//feng::Mesh * feng::ResourcesManager::GetMesh(feng::obj_id id)
//{
//	return m_meshesManager->GetInstance(id);
//}
//
//feng::Shader * feng::ResourcesManager::GetShader(feng::obj_id id)
//{
//	return m_shadersManager->GetInstance(id);
//}
//
//feng::Material * feng::ResourcesManager::GetMaterial(feng::obj_id id)
//{
//	return m_materialsManager->GetInstance(id);
//}
//
//feng::PostEffectDefinition * feng::ResourcesManager::GetPostEffect(feng::obj_id id)
//{
//	return m_postEffectsManager->GetInstance(id);
//}
//
//void feng::ResourcesManager::Purge()
//{
//	m_texturesManager->Purge();
//	m_meshesManager->Purge();
//	m_shadersManager->Purge();
//	m_materialsManager->Purge();
//	m_postEffectsManager->Purge();
//}
//
//bool feng::ResourcesManager::DestroyResource(unsigned long id)
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
