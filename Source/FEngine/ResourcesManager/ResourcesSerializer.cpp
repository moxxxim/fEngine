//#include <cstdio>
//#include <cstring>
//#include "ResourcesSerializer.h"
//#include "ResourcesManager.h"
//#include "ResourceManagerTextures.h"
//#include "ResourceManagerMeshes.h"
//#include "ResourceManagerShaders.h"
//#include "ResourceManagerMaterials.h"
//#include "ResourceManagerPostEffects.h"
//
//const char *fengine::ResourcesSerializer::k_resources = "../Resources/Resources.txt";
//const char *fengine::ResourcesSerializer::k_texturesTag = "Textures ";
//const char *fengine::ResourcesSerializer::k_meshesTag = "Meshes ";
//const char *fengine::ResourcesSerializer::k_shadersTag = "Shaders ";
//const char *fengine::ResourcesSerializer::k_materialsTag = "Materials ";
//const char *fengine::ResourcesSerializer::k_postEffectsTag = "PostEffects ";
//const char *fengine::ResourcesSerializer::k_countTag = "Count ";
//const char *fengine::ResourcesSerializer::k_idTag = "Id ";
//const char *fengine::ResourcesSerializer::k_locationTag = "Location ";
//const char *fengine::ResourcesSerializer::k_textureTypeTag = "TextureType ";
//const char *fengine::ResourcesSerializer::k_textureWrappingTag = "Wrapping ";
//const char *fengine::ResourcesSerializer::k_textureMinFilterTag = "MinFilter ";
//const char *fengine::ResourcesSerializer::k_textureMagFilterTag = "MagFilter ";
//const char *fengine::ResourcesSerializer::k_textureMipMapsTag = "MipMaps ";
//const char *fengine::ResourcesSerializer::k_shaderVsTag = "VsLocation ";
//const char *fengine::ResourcesSerializer::k_shaderFsTag = "FsLocation ";
//const char *fengine::ResourcesSerializer::k_shaderStatesTag = "ShaderStates ";
//const char *fengine::ResourcesSerializer::k_shaderAttributesTag = "Attributes";
//const char *fengine::ResourcesSerializer::k_shaderUniformsTag = "Uniforms";
//const char *fengine::ResourcesSerializer::k_materialShaderTag = "Shader";
//const char *fengine::ResourcesSerializer::k_materialTexturesTag = "Textures";
//const char *fengine::ResourcesSerializer::k_materialFloatsTag = "Floats";
//const char *fengine::ResourcesSerializer::k_materialVec4Tag = "Vec4";
//const char *fengine::ResourcesSerializer::k_postEffectTypeTag = "Type";
//const char *fengine::ResourcesSerializer::k_postEffectMaterialTag = "Material";
//const char *fengine::ResourcesSerializer::k_postEffectPassBehaviourTag = "PassBehaviour";
//const char *fengine::ResourcesSerializer::k_postEffectSequenceTag = "Sequence";
//
//fengine::ResourcesSerializer::ResourcesSerializer()
//{
//}
//
//void fengine::ResourcesSerializer::Serialize(const ResourcesManager& resourcesManager)
//{
//}
//
//int fengine::ResourcesSerializer::Deserialize(ResourcesManager& resourcesManager)
//{
//	FILE *file = fopen(k_resources, "r");
//
//	if (file)
//	{
//		std::fscanf(file, k_texturesTag);
//		resourcesManager.m_texturesManager = ReadTextures(file);
//
//		std::fscanf(file, k_meshesTag);
//		resourcesManager.m_meshesManager = ReadMeshes(file);
//
//		std::fscanf(file, k_shadersTag);
//		resourcesManager.m_shadersManager = ReadShaders(file);
//
//		std::fscanf(file, k_materialsTag);
//		resourcesManager.m_materialsManager = ReadMaterials(file, resourcesManager.m_texturesManager, resourcesManager.m_shadersManager);
//
//		std::fscanf(file, k_postEffectsTag);
//		resourcesManager.m_postEffectsManager = ReadPostEffects(file, resourcesManager.m_materialsManager);
//
//		fclose(file);
//
//		return 0;
//	}
//
//	return 1;
//}
//
//fengine::ResourceManagerTextures * fengine::ResourcesSerializer::ReadTextures(FILE *file)
//{
//	unsigned int resourcesCount;
//	fscanf_s(file, k_countTag);
//	fscanf_s(file, "%u ", &resourcesCount);
//
//	CachedTexture *resources = nullptr;
//	if (resourcesCount > 0)
//	{
//		resources = new CachedTexture[resourcesCount];
//		for (unsigned i = 0; i < resourcesCount; ++i)
//		{
//			ReadTexture(file, resources + i);
//		}
//	}
//
//	return new ResourceManagerTextures(resources, resourcesCount);
//}
//
//fengine::ResourceManagerMeshes * fengine::ResourcesSerializer::ReadMeshes(FILE *file)
//{
//	unsigned int resourcesCount;
//	fscanf_s(file, k_countTag);
//	fscanf_s(file, "%u ", &resourcesCount);
//
//	CachedMesh *resources = nullptr;
//	if (resourcesCount > 0)
//	{
//		resources = new CachedMesh[resourcesCount];
//		for (unsigned i = 0; i < resourcesCount; ++i)
//		{
//			ReadMesh(file, resources + i);
//		}
//	}
//
//	return new ResourceManagerMeshes(resources, resourcesCount);
//}
//
//fengine::ResourceManagerShaders * fengine::ResourcesSerializer::ReadShaders(FILE *file)
//{
//	unsigned int resourcesCount;
//	fscanf_s(file, k_countTag);
//	fscanf_s(file, "%u ", &resourcesCount);
//
//	CachedShader *resources = nullptr;
//	if (resourcesCount > 0)
//	{
//		resources = new CachedShader[resourcesCount];
//		for (unsigned i = 0; i < resourcesCount; ++i)
//		{
//			ReadShader(file, resources + i);
//		}
//	}
//
//	return new ResourceManagerShaders(resources, resourcesCount);
//}
//
//fengine::ResourceManagerMaterials * fengine::ResourcesSerializer::ReadMaterials(
//	FILE *file,
//	fengine::ResourceManagerTextures *texturesManager,
//	fengine::ResourceManagerShaders *shadersManager)
//{
//	unsigned int resourcesCount;
//	fscanf_s(file, k_countTag);
//	fscanf_s(file, "%u ", &resourcesCount);
//	CachedMaterial *resources = nullptr;
//	if (resourcesCount > 0)
//	{
//		resources = new CachedMaterial[resourcesCount];
//		for (unsigned i = 0; i < resourcesCount; ++i)
//		{
//			ReadMaterial(file, resources + i);
//		}
//	}
//
//	return new ResourceManagerMaterials(resources, resourcesCount, texturesManager, shadersManager);
//}
//
//fengine::ResourceManagerPostEffects * fengine::ResourcesSerializer::ReadPostEffects(
//	FILE *file,
//	fengine::ResourceManagerMaterials *materialsManager)
//{
//	unsigned int resourcesCount;
//	fscanf_s(file, k_countTag);
//	fscanf_s(file, "%u ", &resourcesCount);
//	CachedPostEffectDefinition *resources = nullptr;
//	if (resourcesCount > 0)
//	{
//		resources = new CachedPostEffectDefinition[resourcesCount];
//		for (unsigned i = 0; i < resourcesCount; ++i)
//		{
//			ReadPostEffect(file, resources + i);
//		}
//	}
//
//	return new ResourceManagerPostEffects(resources, resourcesCount, materialsManager);
//}
//
//void fengine::ResourcesSerializer::ReadTexture(FILE *file, CachedTexture *texture)
//{
//	// Read Id.
//
//	fscanf_s(file, k_idTag);
//	fscanf_s(file, "%lu ", &texture->Id);
//
//	// Read location.
//
//	char location[256];
//	fscanf_s(file, k_locationTag);
//	fscanf(file, "%s ", location);
//	TrimPathQuotes(location);
//	texture->Location = new char[strlen(location) + 1];
//	strcpy(texture->Location, location);
//
//	// Read wrap mode.
//
//	int wrapMode;
//	fscanf_s(file, k_textureWrappingTag);
//	fscanf_s(file, "%i ", &wrapMode);
//	texture->WrapMode = (TextureWarapMode)wrapMode;
//
//	// Read texture type.
//
//	int textureType;
//	fscanf_s(file, k_textureTypeTag);
//	fscanf_s(file, "%i ", &textureType);
//	texture->Type = (TextureType)textureType;
//
//	// Read minification filter.
//
//	int minFilter;
//	fscanf_s(file, k_textureMinFilterTag);
//	fscanf_s(file, "%i ", &minFilter);
//	texture->MinFilter = (TextureMinFilter)minFilter;
//
//	// Read magnification filter.
//
//	int magFilter;
//	fscanf_s(file, k_textureMagFilterTag);
//	fscanf_s(file, "%i ", &magFilter);
//	texture->MagFilter = (TextureMagFilter)magFilter;
//
//	// Read mip maps.
//
//	int generateMipMaps;
//	fscanf_s(file, k_textureMipMapsTag);
//	fscanf_s(file, "%i ", &generateMipMaps);
//	texture->GenerateMipMaps = generateMipMaps;
//}
//
//void fengine::ResourcesSerializer::ReadMesh(FILE *file, CachedMesh *mesh)
//{
//	fscanf_s(file, k_idTag);
//	fscanf_s(file, "%lu ", &mesh->Id);
//
//	char location[256];
//	fscanf_s(file, k_locationTag);
//	fscanf(file, "%s ", location);
//	TrimPathQuotes(location);
//	mesh->Location = new char[strlen(location) + 1];
//	strcpy(mesh->Location, location);
//}
//
//void fengine::ResourcesSerializer::ReadShader(FILE *file, CachedShader *shader)
//{
//	fscanf_s(file, k_idTag);
//	fscanf_s(file, "%lu ", &shader->Id);
//
//	char location[256];
//	fscanf_s(file, k_shaderVsTag);
//	fscanf(file, "%s ", location);
//	TrimPathQuotes(location);
//	shader->Vs = new char[strlen(location) + 1];
//	strcpy(shader->Vs, location);
//
//	fscanf_s(file, k_shaderFsTag);
//	fscanf(file, "%s ", location);
//	TrimPathQuotes(location);
//	shader->Fs = new char[strlen(location) + 1];
//	strcpy(shader->Fs, location);
//
//	ReadShaderStates(file, shader);
//}
//
//void fengine::ResourcesSerializer::ReadShaderStates(FILE *file, CachedShader *shader)
//{
//	int statesCount;
//	fscanf_s(file, k_shaderStatesTag);
//	fscanf_s(file, "%u ", &statesCount);
//
//	for (int i = 0; i < statesCount; ++i)
//	{
//		int state;
//		fscanf_s(file, "%i ", &state);
//		shader->States = (ShaderStates)((int)(shader->States) | state);
//	}
//}
//
//void fengine::ResourcesSerializer::ReadMaterial(FILE *file, CachedMaterial *material)
//{
//	fscanf_s(file, k_idTag);
//	fscanf_s(file, "%lu ", &material->Id);
//
//	fscanf_s(file, k_materialShaderTag);
//	fscanf_s(file, "%lu ", &material->ShaderId);
//
//	ReadMaterialTextures(file, material);
//	ReadMaterialFloats(file, material);
//	ReadMaterialVec4(file, material);
//}
//
//void fengine::ResourcesSerializer::ReadPostEffect(FILE *file, CachedPostEffectDefinition *postEffect)
//{
//	fscanf_s(file, k_idTag);
//	fscanf_s(file, "%lu ", &postEffect->Id);
//
//	int type;
//	fscanf_s(file, k_postEffectTypeTag);
//	fscanf_s(file, "%lu ", &type);
//	postEffect->Type = (fengine::PostEffectType)type;
//
//	fscanf_s(file, k_postEffectMaterialTag);
//	fscanf_s(file, "%lu ", &postEffect->MaterialId);
//
//	fscanf_s(file, k_postEffectPassBehaviourTag);
//	fscanf_s(file, "%lu ", &postEffect->PassBehaviourId);
//
//	int sequenceLength;
//	fscanf_s(file, k_postEffectSequenceTag);
//	fscanf_s(file, "%lu ", &sequenceLength);
//
//	Vector<obj_id> sequence(sequenceLength);
//
//	fscanf_s(file, "[ ");
//	for (int i = 0; i < sequenceLength; ++i)
//	{
//		obj_id id;
//		fscanf(file, "%lu ", &id);
//		sequence.Add(id);
//	}
//
//	postEffect->Sequence = std::move(sequence);
//	fscanf_s(file, "] ");
//}
//
//void fengine::ResourcesSerializer::ReadMaterialTextures(FILE *file, CachedMaterial *material)
//{
//	unsigned int texturesCount;
//	fscanf_s(file, k_materialTexturesTag);
//	fscanf_s(file, "%u ", &texturesCount);
//
//	fengine::StringMap<obj_id> textures(texturesCount);
//	fscanf_s(file, "[ ");
//	for (int i = 0; i < texturesCount; ++i)
//	{
//		char name[128];
//		fscanf(file, "%s ", name);
//		obj_id id;
//		fscanf_s(file, "%lu ", &id);
//		textures.Add(name, id);
//	}
//
//	material->Textures = std::move(textures);
//	fscanf_s(file, "] ");
//}
//
//void fengine::ResourcesSerializer::ReadMaterialFloats(FILE *file, CachedMaterial *material)
//{
//	unsigned int floatsCount;
//	fscanf_s(file, k_materialFloatsTag);
//	fscanf_s(file, "%u ", &floatsCount);
//
//	fengine::StringMap<float> floats(floatsCount);
//	fscanf_s(file, "[ ");
//	for (int i = 0; i < floatsCount; ++i)
//	{
//		char name[128];
//		fscanf(file, "%s ", name);
//		float value;
//		fscanf_s(file, "%f ", &value);
//		floats.Add(name, value);
//	}
//
//	material->Floats = std::move(floats);
//	fscanf_s(file, "] ");
//}
//
//void fengine::ResourcesSerializer::ReadMaterialVec4(FILE *file, CachedMaterial *material)
//{
//	unsigned int vectors4Count;
//	fscanf_s(file, k_materialVec4Tag);
//	fscanf_s(file, "%u ", &vectors4Count);
//
//	fengine::StringMap<Vector4> vec4Values(vectors4Count);
//	fscanf_s(file, "[ ");
//	for (int i = 0; i < vectors4Count; ++i)
//	{
//		char name[128];
//		fscanf(file, "%s ", name);
//		Vector4 vec4;
//		ReadVector4(file, vec4);
//		vec4Values.Add(name, vec4);
//	}
//
//	material->Vec4Values= std::move(vec4Values);
//	fscanf_s(file, "] ");
//}
//
//void fengine::ResourcesSerializer::TrimPathQuotes(char *path)
//{
//	int strLength = strlen(path);
//	int i = 1;
//	for (; i < strLength - 1; ++i)
//	{
//		path[i - 1] = path[i];
//	}
//
//	path[i - 1] = '\0';
//}
//
//void fengine::ResourcesSerializer::ReadVector4(FILE *file, Vector4& vec)
//{
//	const char *const format = "[%f, %f, %f, %f] ";
//	fscanf_s(file, format, &vec.x, &vec.y, &vec.z, &vec.w);
//}
