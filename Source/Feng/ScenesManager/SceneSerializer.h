//#pragma once
//
//namespace feng
//{
//
//typedef unsigned long int obj_id;
//class Scene;
//class SceneSettings;
//class Fog;
//class ScenesManager;
//class GameObject;
//class GameObjectComponent;
//class Transform;
//class SerializationNode;
//class Renderer;
//
//class SceneSerializer final
//{
//
//public:
//
//	void Serialize(Scene * scene, const char *sceneFile);
//	int Deserialize(Scene * scene, const char *sceneFile);
//
//private:
//
//	void ReadSceneSettings(FILE *file, Scene *scene);
//	void ReadSceneFog(FILE *file, Fog& fog) const;
//	void ReadSceneSkybox(FILE *file, Scene *scene);
//	void ReadSceneAmbientLight(FILE *file, SceneSettings *scene);
//	unsigned int ReadGameObjectsCount(FILE *file) const;
//	void ReadGameObject(FILE *file, Scene *scene);
//	unsigned int ReadGameObjectComponentsCount(FILE *file) const;
//	void ReadGameObjectTransform(FILE *file, Transform *transform) const;
//	void ReadGameObjectComponent(FILE *file, GameObject *go);
//	void ReadComponentInfo(FILE *file, SerializationNode *node) const;
//	bool IsLongValue(const char *key) const;
//	void DeserializeComponents();
//	void SetupSkybox(Scene *scene);
//	void CleanUp();
//	void ReadVector3(FILE *file, Vector3& vec) const;
//	void ReadVector4(FILE *file, Vector4& vec) const;
//
//	Renderer *m_skybox = nullptr;
//	Vector<GameObjectComponent*> m_components;
//	Vector<SerializationNode*> m_serializationData;
//
//	static const char *k_sceneSettingsTag;
//	static const char *k_sceneFogTag;
//	static const char *k_fogStartTag;
//	static const char *k_fogRangeTag;
//	static const char *k_fogDensityTag;
//	static const char *k_fogColorTag;
//	static const char *k_sceneSkyboxTag;
//	static const char *k_sceneSkyboxTextureTag;
//	static const char *k_sceneAmbientLightTag;
//	static const char *k_sceneAmbientLightColorTag;
//	static const char *k_gameObjectsCountTag;
//	static const char *k_gameObjectComponentsCountTag;
//	static const char *k_objectIdTag;
//	static const char *k_gameObjectPositionTag;
//	static const char *k_gameObjectRotationTag;
//	static const char *k_gameObjectScaleTag;
//	static const char *k_gameObjectComponentTag;
//
//
//	static const char *k_serializedObjectInfoStart;
//	static const char *k_serializedObjectInfoEnd;
//	static const char *k_longValuePrefix;
//	static const char *k_floatValuePrefix;
//	static const int k_prefixLength = 2;
//
//};
//
//}
