//#pragma once
//
//#include "Renderer.h"
//#include "../Utils/StringMap.h"
//#include "../ResourcesManager/Material.h"
//
//namespace feng
//{
//
//class Mesh;
//class ResourcesManager;
//
//class MeshRenderer : public feng::Renderer
//{
//
//public:
//
//	MeshRenderer();
//	~MeshRenderer();
//
//	void SetMaterial(feng::Material *material);
//	void SetMesh(feng::Mesh *mesh);
//
//	void Serialize(SerializationNode& node) const override;
//	void Deserialize(const SerializationNode& node) override;
//
//protected:
//
//	void Draw(const RenderProperties *renderProperties) override;
//
//private:
//
//	void StartDraw();
//	void SetupDrawVertices();
//	void SetupLight(const RenderProperties *renderProperties);
//	void SetupFog(const RenderProperties *renderProperties);
//	void SetViewProjectionMatrix(const Camera* cam);
//	void SetupReservedUniforms(const Camera *cam);
//	void SetCustomDrawUniforms();
//	void DrawElements();
//	void FinishDraw();
//	inline bool IsValidState() const;
//	void DeserializeMesh(const SerializationNode& node);
//	void DeserializeMaterial(const SerializationNode& node);
//	void SetupMeshBuffers(const feng::Mesh *mesh);
//	void SetupTexturesBuffers();
//	void SetupTextureBuffer(const feng::Texture *texture, const char *name);
//	void GenerateTexture2D(const feng::Texture *texture);
//	void GenerateTextureCube(const feng::Texture *texture);
//	void ClearTexturesBuffers();
//	void ActivateTexture(const char *name, int unit);
//
//	unsigned int m_vbo = 0;
//	unsigned int m_ibo = 0;
//	StringMap<unsigned int> m_textureBuffers = 0;
//
//	feng::Material *m_material = nullptr;
//	feng::Mesh *m_mesh = nullptr;
//	feng::ResourcesManager *m_resources = nullptr;
//	feng::Shader *m_shader = nullptr;
//
//	unsigned int m_indicesCount = 0;
//};
//
//inline bool MeshRenderer::IsValidState() const
//{
//	if (m_material && m_mesh)
//	{
//		const Shader *shader = m_material->GetShader();
//		if (shader)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
//
//}
