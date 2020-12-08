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
//const char *feng::ResourcesSerializer::k_resources = "../Resources/Resources.txt";
//const char *feng::ResourcesSerializer::k_texturesTag = "Textures ";
//const char *feng::ResourcesSerializer::k_meshesTag = "Meshes ";
//const char *feng::ResourcesSerializer::k_shadersTag = "Shaders ";
//const char *feng::ResourcesSerializer::k_materialsTag = "Materials ";
//const char *feng::ResourcesSerializer::k_postEffectsTag = "PostEffects ";
//const char *feng::ResourcesSerializer::k_countTag = "Count ";
//const char *feng::ResourcesSerializer::k_idTag = "Id ";
//const char *feng::ResourcesSerializer::k_locationTag = "Location ";
//const char *feng::ResourcesSerializer::k_textureTypeTag = "TextureType ";
//const char *feng::ResourcesSerializer::k_textureWrappingTag = "Wrapping ";
//const char *feng::ResourcesSerializer::k_textureMinFilterTag = "MinFilter ";
//const char *feng::ResourcesSerializer::k_textureMagFilterTag = "MagFilter ";
//const char *feng::ResourcesSerializer::k_textureMipMapsTag = "MipMaps ";
//const char *feng::ResourcesSerializer::k_shaderVsTag = "VsLocation ";
//const char *feng::ResourcesSerializer::k_shaderFsTag = "FsLocation ";
//const char *feng::ResourcesSerializer::k_shaderStatesTag = "ShaderStates ";
//const char *feng::ResourcesSerializer::k_shaderAttributesTag = "Attributes";
//const char *feng::ResourcesSerializer::k_shaderUniformsTag = "Uniforms";
//const char *feng::ResourcesSerializer::k_materialShaderTag = "Shader";
//const char *feng::ResourcesSerializer::k_materialTexturesTag = "Textures";
//const char *feng::ResourcesSerializer::k_materialFloatsTag = "Floats";
//const char *feng::ResourcesSerializer::k_materialVec4Tag = "Vec4";
//const char *feng::ResourcesSerializer::k_postEffectTypeTag = "Type";
//const char *feng::ResourcesSerializer::k_postEffectMaterialTag = "Material";
//const char *feng::ResourcesSerializer::k_postEffectPassBehaviourTag = "PassBehaviour";
//const char *feng::ResourcesSerializer::k_postEffectSequenceTag = "Sequence";
//
//feng::ResourcesSerializer::ResourcesSerializer()
//{
//}
//
//void feng::ResourcesSerializer::Serialize(const ResourcesManager& resourcesManager)
//{
//}
//
//int feng::ResourcesSerializer::Deserialize(ResourcesManager& resourcesManager)
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
//feng::ResourceManagerTextures * feng::ResourcesSerializer::ReadTextures(FILE *file)
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
//feng::ResourceManagerMeshes * feng::ResourcesSerializer::ReadMeshes(FILE *file)
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
//feng::ResourceManagerShaders * feng::ResourcesSerializer::ReadShaders(FILE *file)
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
//feng::ResourceManagerMaterials * feng::ResourcesSerializer::ReadMaterials(
//	FILE *file,
//	feng::ResourceManagerTextures *texturesManager,
//	feng::ResourceManagerShaders *shadersManager)
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
//feng::ResourceManagerPostEffects * feng::ResourcesSerializer::ReadPostEffects(
//	FILE *file,
//	feng::ResourceManagerMaterials *materialsManager)
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
//void feng::ResourcesSerializer::ReadTexture(FILE *file, CachedTexture *texture)
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
//void feng::ResourcesSerializer::ReadMesh(FILE *file, CachedMesh *mesh)
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
//void feng::ResourcesSerializer::ReadShader(FILE *file, CachedShader *shader)
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
//void feng::ResourcesSerializer::ReadShaderStates(FILE *file, CachedShader *shader)
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
//void feng::ResourcesSerializer::ReadMaterial(FILE *file, CachedMaterial *material)
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
//void feng::ResourcesSerializer::ReadPostEffect(FILE *file, CachedPostEffectDefinition *postEffect)
//{
//	fscanf_s(file, k_idTag);
//	fscanf_s(file, "%lu ", &postEffect->Id);
//
//	int type;
//	fscanf_s(file, k_postEffectTypeTag);
//	fscanf_s(file, "%lu ", &type);
//	postEffect->Type = (feng::PostEffectType)type;
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
//void feng::ResourcesSerializer::ReadMaterialTextures(FILE *file, CachedMaterial *material)
//{
//	unsigned int texturesCount;
//	fscanf_s(file, k_materialTexturesTag);
//	fscanf_s(file, "%u ", &texturesCount);
//
//	feng::StringMap<obj_id> textures(texturesCount);
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
//void feng::ResourcesSerializer::ReadMaterialFloats(FILE *file, CachedMaterial *material)
//{
//	unsigned int floatsCount;
//	fscanf_s(file, k_materialFloatsTag);
//	fscanf_s(file, "%u ", &floatsCount);
//
//	feng::StringMap<float> floats(floatsCount);
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
//void feng::ResourcesSerializer::ReadMaterialVec4(FILE *file, CachedMaterial *material)
//{
//	unsigned int vectors4Count;
//	fscanf_s(file, k_materialVec4Tag);
//	fscanf_s(file, "%u ", &vectors4Count);
//
//	feng::StringMap<Vector4> vec4Values(vectors4Count);
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
//void feng::ResourcesSerializer::TrimPathQuotes(char *path)
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
//void feng::ResourcesSerializer::ReadVector4(FILE *file, Vector4& vec)
//{
//	const char *const format = "[%f, %f, %f, %f] ";
//	fscanf_s(file, format, &vec.x, &vec.y, &vec.z, &vec.w);
//}
