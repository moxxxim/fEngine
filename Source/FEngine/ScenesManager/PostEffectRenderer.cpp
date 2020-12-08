//#include "PostEffectRenderer.h"
//#include "RenderUtils.h"
//#include "../Utils/VertexAttributeInfo.h"
//#include "../ResourcesManager/Material.h"
//#include "../ResourcesManager/Texture.h"
//#include "../ResourcesManager/Shader.h"
//#include "../ResourcesManager/Vertex.h"
//
//feng::PostEffectRenderer::PostEffectRenderer(Material *material, const StringMap<unsigned int> *textureBuffers) :
//	m_material(material),
//	m_shader(material->GetShader()),
//	m_textureBuffers(textureBuffers)
//{
//}
//
//void feng::PostEffectRenderer::Draw()
//{
//	m_shader->StartUse();
//
//	SetupVertexAttribute(ShaderParameters::ReservedAttribute::VertexPositionL);
//	SetupVertexAttribute(ShaderParameters::ReservedAttribute::VertexUV);
//	SetupBufferedTextures();
//	SetCustomDrawUniforms();
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//	m_shader->StopUse();
//}
//
//void feng::PostEffectRenderer::SetupVertexAttribute(feng::ShaderParameters::ReservedAttribute attribute)
//{
//	const char *positionAttribute = feng::ShaderParameters::GetReservedAttributeName(attribute);
//	int attributeName = 0;
//	if (m_shader->TryGetAttribute(positionAttribute, attributeName))
//	{
//		feng::VertexAttributeInfo info(positionAttribute);
//
//		glEnableVertexAttribArray(attributeName);
//		glVertexAttribPointer(attributeName, info.Size, info.Type, info.IsNormalized, sizeof(Vertex), info.Pointer);
//	}
//}
//
//void feng::PostEffectRenderer::SetupBufferedTextures()
//{
//	auto iterator = m_textureBuffers->GetIterator();
//	unsigned int textureUnit = 0;
//	while (iterator.HasCurrent())
//	{
//		KeyValuePair<const char*, unsigned> namedTexture = iterator.GetCurrent();
//		if (m_shader->HasUniform(namedTexture.Key))
//		{
//			glActiveTexture(GL_TEXTURE0 + textureUnit);
//			glBindTexture(GL_TEXTURE_2D, namedTexture.Value);
//			m_shader->SetUniformInt(namedTexture.Key, textureUnit);
//			textureUnit++;
//		}
//
//		iterator.MoveNext();
//	}
//}
//
//void feng::PostEffectRenderer::SetCustomDrawUniforms()
//{
//	const StringMap<int> *uniforms = m_shader->GetUniforms();
//	auto iterator = uniforms->GetIterator();
//	unsigned int textureUnit = m_textureBuffers->GetSize();
//
//	while (iterator.HasCurrent())
//	{
//		KeyValuePair<const char*, int> namedUniform = iterator.GetCurrent();
//
//		if (m_material->HasFloat(namedUniform.Key))
//		{
//			float value = m_material->GetFloat(namedUniform.Key);
//			m_shader->SetUniformFloat(namedUniform.Key, value);
//		}
//
//		if (m_material->HasTexture(namedUniform.Key))
//		{
//			ActivateTexture(namedUniform.Key, textureUnit);
//			textureUnit++;
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
//void feng::PostEffectRenderer::ActivateTexture(const char *name, int unit)
//{
//	const Texture *texture = m_material->GetTexture(name);
//	GLuint target = texture->GetType() == feng::TextureType::Texture2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
//	glActiveTexture(GL_TEXTURE0 + unit);
//	glBindTexture(target, m_materialTextures[name]);
//	m_shader->SetUniformInt(name, unit);
//}
//
//void feng::PostEffectRenderer::SetupTexturesBuffers()
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
//void feng::PostEffectRenderer::SetupTextureBuffer(const feng::Texture *texture, const char *name)
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
//		m_materialTextures.Add(name, tbo);
//		glBindTexture(target, 0);
//	}
//	else
//	{
//		Debug::LogError("Couldn't create texture buffer. Texture is null.\n");
//	}
//}
//
//void feng::PostEffectRenderer::GenerateTexture2D(const feng::Texture *texture)
//{
//	const GLint format = feng::GetTextureFormat(texture);
//	int width = texture->GetWidth();
//	int height = texture->GetHeight();
//	const char *data = texture->GetData();
//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//}
//
//void feng::PostEffectRenderer::GenerateTextureCube(const feng::Texture *texture)
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
//void feng::PostEffectRenderer::ClearTexturesBuffers()
//{
//	auto iterator = m_materialTextures.GetIterator();
//	while (iterator.HasCurrent())
//	{
//		feng::KeyValuePair<const char*, unsigned>& buffer = iterator.GetCurrent();
//		glDeleteTextures(1, &buffer.Value);
//		iterator.MoveNext();
//	}
//
//	m_materialTextures.Clear();
//}
