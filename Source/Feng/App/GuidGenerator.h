#pragma once

#include "Globals.h"

namespace feng
{

class Object;
class SceneSerializer;
class ScenesManager;
class ResourceManagerTextures;
class ResourceManagerMeshes;
class ResourceManagerShaders;
class ResourceManagerMaterials;
class ResourceManagerPostEffects;
class SerializableObjectParser;

class GuidGenerator final
{

	friend Object;
	friend SceneSerializer;
	friend ScenesManager;
	friend ResourceManagerTextures;
	friend ResourceManagerMeshes;
	friend ResourceManagerShaders;
	friend ResourceManagerMaterials;
	friend ResourceManagerPostEffects;
	friend SerializableObjectParser;

private:

	static void ResetNextSceneObjectId();
	static void SetNextSceneObjectId(obj_id id);
	static void SetNextResourceId(obj_id id);
	static obj_id GenerateNextSceneObjectId();
	static obj_id GenerateNextResourceId();
	static void UpdateNextSceneObjectId(obj_id current);
	static void UpdateNextResourceId(obj_id current);

	static obj_id s_nextSceneObjectId;
	static obj_id s_nextResourceId;
	static obj_id s_currentMaxSceneObjectId;
	static obj_id s_currentMaxResourceId;

};

}
