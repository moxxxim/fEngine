//#pragma once
//
//#include "../ResourcesManager/ShaderParameters.h"
//#include "../Utils/StringMap.h"
//
//namespace fengine
//{
//
//class Material;
//class Shader;
//class Texture;
//
//class PostEffectRenderer final
//{
//
//public:
//
//	PostEffectRenderer(Material *material, const StringMap<unsigned int> *textureBuffers);
//
//	void Draw();
//
//private:
//
//	void SetupVertexAttribute(ShaderParameters::ReservedAttribute attribute);
//	void SetupBufferedTextures();
//	void SetCustomDrawUniforms();
//	void ActivateTexture(const char *name, int unit);
//	void SetupTexturesBuffers();
//	void SetupTextureBuffer(const Texture *texture, const char *name);
//	void GenerateTexture2D(const Texture *texture);
//	void GenerateTextureCube(const Texture *texture);
//	void ClearTexturesBuffers();
//
//	unsigned m_colorBuffer = 0;
//	unsigned m_depthBuffer = 0;
//	Material *m_material = nullptr;
//	Shader *m_shader = nullptr;
//	const StringMap<unsigned int> *m_textureBuffers = nullptr;
//	StringMap<unsigned int> m_materialTextures;
//};
//
//}
