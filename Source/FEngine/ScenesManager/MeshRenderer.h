//#pragma once
//
//#include "Renderer.h"
//#include "../Utils/StringMap.h"
//#include "../ResourcesManager/Material.h"
//
//namespace fengine
//{
//
//class Mesh;
//class ResourcesManager;
//
//class MeshRenderer : public fengine::Renderer
//{
//
//public:
//
//	MeshRenderer();
//	~MeshRenderer();
//
//	void SetMaterial(fengine::Material *material);
//	void SetMesh(fengine::Mesh *mesh);
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
//	void SetupMeshBuffers(const fengine::Mesh *mesh);
//	void SetupTexturesBuffers();
//	void SetupTextureBuffer(const fengine::Texture *texture, const char *name);
//	void GenerateTexture2D(const fengine::Texture *texture);
//	void GenerateTextureCube(const fengine::Texture *texture);
//	void ClearTexturesBuffers();
//	void ActivateTexture(const char *name, int unit);
//
//	unsigned int m_vbo = 0;
//	unsigned int m_ibo = 0;
//	StringMap<unsigned int> m_textureBuffers = 0;
//
//	fengine::Material *m_material = nullptr;
//	fengine::Mesh *m_mesh = nullptr;
//	fengine::ResourcesManager *m_resources = nullptr;
//	fengine::Shader *m_shader = nullptr;
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
