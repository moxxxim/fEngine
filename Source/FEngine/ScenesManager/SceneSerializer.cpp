//#include "SceneSerializer.h"
//#include "Scene.h"
//#include "GameObject.h"
//#include "MeshRenderer.h"
//#include "Camera.h"
//#include "../Utils/LinkedList.h"
//#include "../ResourcesManager/ResourceManager.h"
//#include "../App\GuidGenerator.h"
//#include "../Serialization/SerializationNode.h"
//#include "../Serialization/SerializableObjectsFactory.h"
//#include "../Utils/Debug.h"
//#include "../Utils/SerializableObjectParser.h"
//
//const char *fengine::SceneSerializer::k_sceneSettingsTag = "Settings ";
//const char *fengine::SceneSerializer::k_sceneFogTag = "Fog ";
//const char *fengine::SceneSerializer::k_fogStartTag = "FogStart ";
//const char *fengine::SceneSerializer::k_fogRangeTag = "FogRange ";
//const char *fengine::SceneSerializer::k_fogDensityTag = "FogDensity ";
//const char *fengine::SceneSerializer::k_fogColorTag = "FogColor ";
//const char *fengine::SceneSerializer::k_sceneSkyboxTag = "Skybox ";
//const char *fengine::SceneSerializer::k_sceneSkyboxTextureTag = "Texture ";
//const char *fengine::SceneSerializer::k_sceneAmbientLightTag = "AmbientLight ";
//const char *fengine::SceneSerializer::k_sceneAmbientLightColorTag = "Color ";
//const char *fengine::SceneSerializer::k_gameObjectsCountTag = "GameObjectsCount ";
//const char *fengine::SceneSerializer::k_gameObjectComponentsCountTag = "ComponentsCount ";
//const char *fengine::SceneSerializer::k_objectIdTag = "Id ";
//const char *fengine::SceneSerializer::k_gameObjectPositionTag = "Position ";
//const char *fengine::SceneSerializer::k_gameObjectRotationTag = "Rotation ";
//const char *fengine::SceneSerializer::k_gameObjectScaleTag = "Scale ";
//const char *fengine::SceneSerializer::k_gameObjectComponentTag = "Component ";
//
//const char *fengine::SceneSerializer::k_serializedObjectInfoStart = "[";
//const char *fengine::SceneSerializer::k_serializedObjectInfoEnd = "]";
//const char *fengine::SceneSerializer::k_longValuePrefix = "l_";
//const char *fengine::SceneSerializer::k_floatValuePrefix = "f_";
//
//void fengine::SceneSerializer::Serialize(Scene *scene, const char *sceneFile)
//{
//}
//
//int fengine::SceneSerializer::Deserialize(Scene *scene, const char * sceneFile)
//{
//	FILE *file = fopen(sceneFile, "r");
//
//	if (file)
//	{
//		ReadSceneSettings(file, scene);
//
//		unsigned int goCount = ReadGameObjectsCount(file);
//		for (unsigned i = 0; i < goCount; ++i)
//		{
//			ReadGameObject(file, scene);
//		}
//
//		DeserializeComponents();
//		SetupSkybox(scene);
//		CleanUp();
//
//		fclose(file);
//		return 0;
//	}
//
//	return 1;
//}
//
//void fengine::SceneSerializer::ReadSceneSettings(FILE *file, Scene *scene)
//{
//	fscanf_s(file, k_sceneSettingsTag);
//
//	SceneSettings *settings = scene->GetSettings();
//	ReadSceneFog(file, settings->GetFog());
//	ReadSceneSkybox(file, scene);
//	ReadSceneAmbientLight(file, settings);
//}
//
//void fengine::SceneSerializer::ReadSceneFog(FILE *file, Fog& fog) const
//{
//	fscanf_s(file, k_sceneFogTag);
//
//	float fogStart;
//	fscanf_s(file, k_fogStartTag);
//	fscanf_s(file, "%f ", &fogStart);
//	fog.SetStart(fogStart);
//
//	float fogRange;
//	fscanf_s(file, k_fogRangeTag);
//	fscanf_s(file, "%f ", &fogRange);
//	fog.SetRange(fogRange);
//
//	float fogDensity;
//	fscanf_s(file, k_fogDensityTag);
//	fscanf_s(file, "%f ", &fogDensity);
//	fog.SetDensity(fogDensity);
//
//	Vector3 fogColor;
//	fscanf_s(file, k_fogColorTag);
//	ReadVector3(file, fogColor);
//	fog.SetColor(fogColor);
//}
//
//void fengine::SceneSerializer::ReadSceneSkybox(FILE *file, Scene *scene)
//{
//	fscanf_s(file, k_sceneSkyboxTag);
//
//	obj_id skyboxId;
//	fscanf_s(file, k_objectIdTag);
//	fscanf_s(file, "%lu ", &skyboxId);
//	if (skyboxId > 0)
//	{
//		const char *skyboxComponentName = "MeshRenderer";
//		GuidGenerator::SetNextSceneObjectId(skyboxId);
//		SerializableObject *skyboxComponent = SerializableObjectsFactory::GetSerializableObject(skyboxComponentName);
//		m_skybox = dynamic_cast<fengine::Renderer*>(skyboxComponent);
//		unsigned int componentsCount = ReadGameObjectComponentsCount(file);
//		SerializationNode *node = new SerializationNode();
//		node->SetName(skyboxComponentName);
//		ReadComponentInfo(file, node);
//		scene->SetSkybox(m_skybox);
//		m_components.Add(m_skybox);
//		m_serializationData.Add(node);
//	}
//}
//
//void fengine::SceneSerializer::ReadSceneAmbientLight(FILE *file, SceneSettings *sceneSettings)
//{
//	fscanf_s(file, k_sceneAmbientLightTag);
//	fscanf_s(file, k_sceneAmbientLightColorTag);
//	ReadVector4(file, sceneSettings->GetAmbientLightColor());
//}
//
//unsigned int fengine::SceneSerializer::ReadGameObjectsCount(FILE *file) const
//{
//	unsigned int count;
//	fscanf_s(file, k_gameObjectsCountTag);
//	fscanf_s(file, "%u ", &count);
//
//	return count;
//}
//
//void fengine::SceneSerializer::ReadGameObject(FILE *file, Scene *scene)
//{
//	fengine::obj_id id;
//	fscanf_s(file, k_objectIdTag);
//	fscanf_s(file, "%lu ", &id);
//	GuidGenerator::SetNextSceneObjectId(id);
//	GameObject *go = scene->CreateGameObject();
//	ReadGameObjectTransform(file, go->GetTransform());
//	unsigned int componentsCount = ReadGameObjectComponentsCount(file);
//	for (unsigned i = 0; i < componentsCount; ++i)
//	{
//		ReadGameObjectComponent(file, go);
//	}
//}
//
//unsigned int fengine::SceneSerializer::ReadGameObjectComponentsCount(FILE *file) const
//{
//	unsigned int count;
//	fscanf_s(file, k_gameObjectComponentsCountTag);
//	fscanf_s(file, "%u ", &count);
//
//	return count;
//}
//
//void fengine::SceneSerializer::ReadGameObjectTransform(FILE *file, Transform *transform) const
//{
//	Vector3 vec;
//	fscanf_s(file, k_gameObjectPositionTag);
//	ReadVector3(file, vec);
//	transform->SetPosition(vec);
//
//	fscanf_s(file, k_gameObjectRotationTag);
//	ReadVector3(file, vec);
//	transform->SetRotation(vec);
//
//	fscanf_s(file, k_gameObjectScaleTag);
//	ReadVector3(file, vec);
//	transform->SetScale(vec);
//}
//
//void fengine::SceneSerializer::ReadGameObjectComponent(FILE *file, GameObject *go)
//{
//	fscanf(file, k_gameObjectComponentTag);
//
//	SerializableObject *serializableObject = nullptr;
//	SerializationNode *node = nullptr;
//	fengine::SerializableObjectParser::ReadSceneObject(file, serializableObject, node);
//	if (serializableObject)
//	{
//		GameObjectComponent *component = dynamic_cast<fengine::GameObjectComponent*>(serializableObject);
//		go->AddComponent(component);
//		m_components.Add(component);
//		m_serializationData.Add(node);
//	}
//}
//
//void fengine::SceneSerializer::ReadComponentInfo(FILE *file, fengine::SerializationNode *node) const
//{
//	char key[128];
//	fscanf(file, k_serializedObjectInfoStart);
//	fscanf(file, " %s ", key);
//
//	while (std::strcmp(key, k_serializedObjectInfoEnd))
//	{
//		if (IsLongValue(key))
//		{
//			unsigned long value;
//			fscanf(file, "%li ", &value);
//			node->SetLong(key + k_prefixLength, value);
//		}
//		else
//		{
//			float value;
//			fscanf(file, "%f ", &value);
//			node->SetFloat(key + k_prefixLength, value);
//		}
//
//		fscanf(file, " %s ", key);
//	}
//}
//
//void fengine::SceneSerializer::DeserializeComponents()
//{
//	unsigned int componentsCount = m_components.GetSize();
//	for (unsigned int i = 0; i < componentsCount; ++i)
//	{
//		GameObjectComponent *component = m_components[i];
//		SerializationNode *node = m_serializationData[i];
//		component->Deserialize(*node);
//	}
//}
//
//void fengine::SceneSerializer::SetupSkybox(fengine::Scene *scene)
//{
//	if (m_skybox)
//	{
//		auto gameObjectsIterator = scene->GetGameObjectsIterator();
//		while (gameObjectsIterator.HasCurrent())
//		{
//			fengine::GameObject *go = gameObjectsIterator.GetCurrent();
//			fengine::Camera * camera = go->GetComponent<fengine::Camera>();
//			if (camera)
//			{
//				go->AddComponent(m_skybox);
//				break;
//			}
//
//			gameObjectsIterator.MoveNext();
//		}
//	}
//}
//
//void fengine::SceneSerializer::CleanUp()
//{
//	unsigned int componentsCount = m_components.GetSize();
//	for (unsigned int i = 0; i < componentsCount; ++i)
//	{
//		delete m_serializationData[i];
//	}
//
//	m_components.Clear();
//	m_serializationData.Clear();
//}
//
//bool fengine::SceneSerializer::IsLongValue(const char *key) const
//{
//	return !strncmp(k_longValuePrefix, key, k_prefixLength);
//}
//
//void fengine::SceneSerializer::ReadVector3(FILE *file, Vector3& vec) const
//{
//	const char *const format = "[%f, %f, %f] ";
//	fscanf_s(file, format, &vec.x, &vec.y, &vec.z);
//}
//
//void fengine::SceneSerializer::ReadVector4(FILE *file, Vector4& vec) const
//{
//	const char *const format = "[%f, %f, %f, %f] ";
//	fscanf_s(file, format, &vec.x, &vec.y, &vec.z, &vec.w);
//}
