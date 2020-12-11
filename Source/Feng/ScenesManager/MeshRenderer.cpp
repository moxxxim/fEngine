//#include "Transform.h"
//#include "../App/Time.h"
//#include "../Feng.h"
//#include "../ResourcesManager/ResourcesManager.h"
//#include "../ResourcesManager/ShaderParameters.h"
//#include "RenderUtils.h"
//#include "RenderProperties.h"
//#include "Fog.h" 
//#include "../Utils/VertexAttributeInfo.h"
//#include "ScenesManager.h"
//
//feng::MeshRenderer::MeshRenderer()
//{
//	m_resources = ResourcesManager::GetInstance();
//}
//
//feng::MeshRenderer::~MeshRenderer()
//{
//	glDeleteBuffers(1, &m_vbo);
//	glDeleteBuffers(1, &m_ibo);
//	ClearTexturesBuffers();
//}
//
//void feng::MeshRenderer::SetMaterial(Material *material)
//{
//	if (material)
//	{
//		ClearTexturesBuffers();
//
//		m_material = material;
//		m_shader = m_material->GetShader();
//		SetupTexturesBuffers();
//	}
//}
//
//void feng::MeshRenderer::SetMesh(Mesh *mesh)
//{
//	if (mesh)
//	{
//		if (m_vbo)
//		{
//			glDeleteBuffers(1, &m_vbo);
//		}
//
//		if (m_ibo)
//		{
//			glDeleteBuffers(1, &m_ibo);
//		}
//
//		m_mesh = mesh;
//		m_indicesCount = m_mesh->GetIndicesCount();
//		SetupMeshBuffers(mesh);
//	}
//}
//
//void feng::MeshRenderer::Serialize(feng::SerializationNode& node) const
//{
//	node.SetName("MeshRenderer");
//	node.SetFloat("MaterialId", m_material ? m_material->GetId() : -1);
//	node.SetFloat("MeshId", m_mesh ? m_mesh->GetId() : -1);
//}
//
//void feng::MeshRenderer::Deserialize(const feng::SerializationNode& node)
//{
//	DeserializeMesh(node);
//	DeserializeMaterial(node);
//}
//
//void feng::MeshRenderer::Draw(const RenderProperties *renderProperties)
//{
//	if (IsValidState())
//	{
//		StartDraw();
//		SetupDrawVertices();
//		SetupFog(renderProperties);
//		SetupLight(renderProperties);
//		SetViewProjectionMatrix(renderProperties->GetCamera());
//		SetupReservedUniforms(renderProperties->GetCamera());
//		SetCustomDrawUniforms();
//		DrawElements();
//		FinishDraw();
//	}
//}
//
//void feng::MeshRenderer::StartDraw()
//{
//	m_shader->StartUse();
//}
//
//void feng::MeshRenderer::SetupDrawVertices()
//{
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
//
//	const StringMap<int> *attributes = m_shader->GetAttributes();
//	unsigned int attributesCount = attributes->GetSize();
//	auto iterator = attributes->GetIterator();
//
//	while (iterator.HasCurrent())
//	{
//		KeyValuePair<const char*, int> namedAttribute = iterator.GetCurrent();
//		GLint attributeLocation = 0;
//		if (m_shader->TryGetAttribute(namedAttribute.Key, attributeLocation))
//		{
//			feng::VertexAttributeInfo info(namedAttribute.Key);
//
//			glEnableVertexAttribArray(attributeLocation);
//			glVertexAttribPointer(attributeLocation, info.Size, info.Type, info.IsNormalized, sizeof(Vertex), info.Pointer);
//		}
//
//		iterator.MoveNext();
//	}
//}
//
//void feng::MeshRenderer::SetupLight(const RenderProperties *renderProperties)
//{
//	const char *ambientLightColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::AmbientLightColor);
//	m_shader->SetUniformVector4(ambientLightColorUniform, renderProperties->GetAmbientLightColor());
//
//	int directLightsCount = renderProperties->GetDirectLightsCount();
//	if (directLightsCount > 0)
//	{
//		const char *lightsCountUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightCount);
//		m_shader->SetUniformInt(lightsCountUniform, directLightsCount);
//		const char *lightDirectionsUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightDirection);
//		const Vector3 *directions = renderProperties->GetDirectLightDirections();
//		m_shader->SetUniformVector3Array(lightDirectionsUniform, directions, directLightsCount);
//		const char *lightColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightColor);
//		const Vector4 *colors = renderProperties->GetDirectLightColors();
//		m_shader->SetUniformVector4Array(lightColorUniform, colors, directLightsCount);
//		const char *lightsIntensityUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightIntensity);
//		const float *intensities = renderProperties->GetDirectLightIntensities();
//		m_shader->SetUniformFloatArray(lightsIntensityUniform, intensities, directLightsCount);
//	}
//
//	int pointLightsCount = renderProperties->GetPointLightsCount();
//	if (pointLightsCount > 0)
//	{
//		const char *lightsCountUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightCount);
//		m_shader->SetUniformInt(lightsCountUniform, pointLightsCount);
//		const char *lightPositionsUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightPosition);
//		m_shader->SetUniformVector3Array(lightPositionsUniform, renderProperties->GetPointLightPositions(), pointLightsCount);
//		const char *lightsRangeUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightRange);
//		m_shader->SetUniformFloatArray(lightsRangeUniform, renderProperties->GetPointLightRanges(), pointLightsCount);
//		const char *lightColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightColor);
//		m_shader->SetUniformVector4Array(lightColorUniform, renderProperties->GetPointLightColors(), pointLightsCount);
//		const char *lightsIntensityUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightIntensity);
//		m_shader->SetUniformFloatArray(lightsIntensityUniform, renderProperties->GetPointLightIntensities(), pointLightsCount);
//	}
//}
//
//void feng::MeshRenderer::SetupFog(const RenderProperties *renderProperties)
//{
//	const feng::Camera *cam = renderProperties->GetCamera();
//	const feng::Fog& fog = renderProperties->GetFog();
//
//	const char *fogStartUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogStart);
//	m_shader->SetUniformFloat(fogStartUniform, fog.GetStart());
//	const char *fogRangeUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogRange);
//	m_shader->SetUniformFloat(fogRangeUniform, fog.GetRange());
//	const char *fogDensityUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogDensity);
//	m_shader->SetUniformFloat(fogDensityUniform, fog.GetDensity());
//	const char *fogColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogColor);
//	m_shader->SetUniformVector3(fogColorUniform, fog.GetColor());
//}
//
//void feng::MeshRenderer::SetViewProjectionMatrix(const Camera *cam)
//{
//	const char *viewProjectionMatrixUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::ViewProjMatrix);
//	Transform *transform = GetTransform();
//
//	if (cam->GetGameObject() == GetGameObject())
//	{
//		Matrix camProjectionMatrix = cam->GetProjectionMatrix();
//		Matrix camRotationMatrix = transform->GetRotationMatrix();
//		float nearClipPlane = cam->GetNearClipPlane();
//		Matrix scale;
//		scale.SetScale(nearClipPlane + 1);
//
//		m_shader->SetUniformMatrix4(viewProjectionMatrixUniform, scale * camRotationMatrix * camProjectionMatrix);
//	}
//	else
//	{
//		Matrix transformMatrix = transform->GetTransformMatrix();
//		Matrix viewProjectionMatrix = cam->GetViewProjectionMatrix();
//		m_shader->SetUniformMatrix4(viewProjectionMatrixUniform, transformMatrix * viewProjectionMatrix);
//	}
//}
//
//void feng::MeshRenderer::SetupReservedUniforms(const Camera *cam)
//{
//	Time *time = Time::GetInstance();
//	Transform *camTransform = cam->GetTransform();
//	Transform *myTransform = GetTransform();
//
//	const char *modelTransformMatrix = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::ModelGlobalMatrix);
//	m_shader->SetUniformMatrix4(modelTransformMatrix, myTransform->GetTransformMatrix());
//	const char *camPosUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::CamPos);
//	m_shader->SetUniformVector3(camPosUniform, camTransform->GetPosition());
//	const char *timeUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::Time);
//	m_shader->SetUniformFloat(timeUniform, time->GetSceneTime());
//}
//
//void feng::MeshRenderer::SetCustomDrawUniforms()
//{
//	const StringMap<int> *uniforms = m_shader->GetUniforms();
//	auto iterator = uniforms->GetIterator();
//	unsigned int textureUnit = 0;
//
//	while (iterator.HasCurrent())
//	{
//		KeyValuePair<const char*, int> namedUniform = iterator.GetCurrent();
//
//		if (m_material->HasTexture(namedUniform.Key))
//		{
//			ActivateTexture(namedUniform.Key, textureUnit);
//			textureUnit++;
//		}
//
//		if (m_material->HasFloat(namedUniform.Key))
//		{
//			float value = m_material->GetFloat(namedUniform.Key);
//			m_shader->SetUniformFloat(namedUniform.Key, value);
//		}
//
//		if (m_material->HasVector4(namedUniform.Key))
//		{
//			Vector4 value = m_material->GetVector4(namedUniform.Key);
//			m_shader->SetUniformVector4(namedUniform.Key, value);
//		}
//
//		iterator.MoveNext();
//	}
//}
//
//void feng::MeshRenderer::DrawElements()
//{
//	glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
//}
//
//void feng::MeshRenderer::FinishDraw()
//{
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	m_shader->StopUse();
//}
//
//void feng::MeshRenderer::ActivateTexture(const char *name, int unit)
//{
//	const Texture *texture = m_material->GetTexture(name);
//	GLuint target = texture->GetType() == feng::TextureType::Texture2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
//	glActiveTexture(GL_TEXTURE0 + unit);
//	glBindTexture(target, m_textureBuffers[name]);
//	m_shader->SetUniformInt(name, unit);
//}
//
//void feng::MeshRenderer::DeserializeMesh(const feng::SerializationNode& node)
//{
//	long meshId;
//	node.TryGetLong("MeshId", meshId);
//
//	feng::Mesh *mesh = m_resources->GetMesh(meshId);
//	if (mesh)
//	{
//		SetMesh(mesh);
//	}
//	else
//	{
//		feng::Debug::LogError("Couldn't find mesh.");
//	}
//}
//
//void feng::MeshRenderer::DeserializeMaterial(const feng::SerializationNode& node)
//{
//	long materialId;
//	node.TryGetLong("MaterialId", materialId);
//
//	feng::Material *material = m_resources->GetMaterial(materialId);
//	if (material)
//	{
//		SetMaterial(material);
//	}
//	else
//	{
//		feng::Debug::LogError("Couldn't find material.\n");
//	}
//}
//
//void feng::MeshRenderer::SetupMeshBuffers(const feng::Mesh *mesh)
//{
//	if (mesh)
//	{
//		const unsigned verticesCount = mesh->GetVerticesCount();
//		const unsigned indicesCount = mesh->GetIndicesCount();
//		const Vertex *vertices = mesh->GetVertices();
//		const unsigned *indices = mesh->GetIndices();
//
//		// Create vertices buffer.
//
//		glGenBuffers(1, &m_vbo);
//		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verticesCount, vertices, GL_STATIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//		// Create indices buffer.
//
//		glGenBuffers(1, &m_ibo);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indicesCount, indices, GL_STATIC_DRAW);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//	else
//	{
//		Debug::LogError("Couldn't create vertices buffer. Mesh is null.\n");
//	}
//}
//
//void feng::MeshRenderer::SetupTexturesBuffers()
//{
//	const StringMap<int> *uniforms = m_shader->GetUniforms();
//	auto iterator = uniforms->GetIterator();
//
//	while (iterator.HasCurrent())
//	{
//		KeyValuePair<const char*, int> namedUniform = iterator.GetCurrent();
//		if (m_material->HasTexture(namedUniform.Key))
//		{
//			const feng::Texture *texture = m_material->GetTexture(namedUniform.Key);
//			SetupTextureBuffer(texture, namedUniform.Key);
//		}
//
//		iterator.MoveNext();
//	}
//}
//
//void feng::MeshRenderer::SetupTextureBuffer(const feng::Texture *texture, const char *name)
//{
//	if (texture)
//	{
//		feng::TextureType textureType = texture->GetType();
//		GLuint target = textureType == feng::TextureType::Texture2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
//
//		unsigned int tbo;
//		glGenTextures(1, &tbo);
//		glBindTexture(target, tbo);
//
//		GLint wrapMode = feng::GetTextureWrapMode(texture);
//		GLint minFilter = feng::GetTextureMinFilter(texture);
//		GLint magFilter = feng::GetTextureMagFilter(texture);
//		glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
//		glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);
//		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
//		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
//
//		if (textureType == feng::TextureType::Texture2D)
//		{
//			GenerateTexture2D(texture);
//		}
//		else
//		{
//			GenerateTextureCube(texture);
//		}
//
//		if (texture->HasMipMaps())
//		{
//			glGenerateMipmap(target);
//		}
//
//		m_textureBuffers.Add(name, tbo);
//		glBindTexture(target, 0);
//	}
//	else
//	{
//		Debug::LogError("Couldn't create texture buffer. Texture is null.\n");
//	}
//}
//
//void feng::MeshRenderer::GenerateTexture2D(const feng::Texture *texture)
//{
//	const GLint format = feng::GetTextureFormat(texture);
//	int width = texture->GetWidth();
//	int height = texture->GetHeight();
//	const char *data = texture->GetData();
//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//}
//
//void feng::MeshRenderer::GenerateTextureCube(const feng::Texture *texture)
//{
//	const GLint format = feng::GetTextureFormat(texture);
//	int faceWidth = texture->GetWidth() / 4;
//	int faceHeight = texture->GetHeight() / 3;
//	for (int i = 0; i < 6; ++i)
//	{
//		const char *data = texture->GetCubeMapFace((feng::CubeMapFace)i);
//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, data);
//	}
//}
//
//void feng::MeshRenderer::ClearTexturesBuffers()
//{
//	auto iterator = m_textureBuffers.GetIterator();
//	while (iterator.HasCurrent())
//	{
//		feng::KeyValuePair<const char*, unsigned>& buffer = iterator.GetCurrent();
//		glDeleteTextures(1, &buffer.Value);
//		iterator.MoveNext();
//	}
//
//	m_textureBuffers.Clear();
//}
