//#include "Shader.h"
//#include "ResourcesManager.h"
//#include "CachedShader.h"
//#include "ShaderParameters.h"
//
//fengine::Shader::Shader(const CachedShader& cachedShader) :
//	Object(ObjectType::Resource),
//	m_program(k_undefinedShaderParamLocation),
//	m_cachedInfo(cachedShader)
//{
//	// Load vertex shader.
//
//	m_vertexShader = esLoadShader(GL_VERTEX_SHADER, cachedShader.Vs);
//	if (m_vertexShader == 0)
//	{
//		return;
//	}
//
//	// Load fragment shader.
//
//	m_fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, cachedShader.Fs);
//	if (m_fragmentShader == 0)
//	{
//		glDeleteShader(m_vertexShader);
//		return;
//	}
//
//	m_program = esLoadProgram(m_vertexShader, m_fragmentShader);
//
//	if (m_program)
//	{
//		FetchActiveAttributes();
//		FetchActiveUniforms();
//	}
//}
//
//fengine::Shader::~Shader()
//{
//	glDeleteProgram(m_program);
//	glDeleteShader(m_vertexShader);
//	glDeleteShader(m_fragmentShader);
//}
//
//void fengine::Shader::StartUse() const
//{
//	EnableState();
//	glUseProgram(m_program);
//}
//
//void fengine::Shader::StopUse() const
//{
//	DisableStates();
//}
//
//const fengine::StringMap<int> * fengine::Shader::GetAttributes() const
//{
//	return &m_attributes;
//}
//
//const fengine::StringMap<int> * fengine::Shader::GetUniforms() const
//{
//	return &m_uniforms;
//}
//
//bool fengine::Shader::TryGetAttribute(const char *name, int& location) const
//{
//	return m_attributes.TryGetValue(name, location);
//}
//
//bool fengine::Shader::HasUniform(const char *name) const
//{
//	return m_uniforms.HasKey(name);
//}
//
//bool fengine::Shader::SetUniformInt(const char *name, int value)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniform1i(location, value);
//		return true;
//	}
//
//	return false;
//}
//
//bool fengine::Shader::SetUniformFloat(const char *name, float value)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniform1f(location, value);
//		return true;
//	}
//
//	return false;
//}
//
//bool fengine::Shader::SetUniformFloatArray(const char *name, const float *value, int size)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniform1fv(location, size, value);
//		return true;
//	}
//
//	return false;
//}
//
//bool fengine::Shader::SetUniformVector3(const char *name, const Vector3& value)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniform3f(location, value.x, value.y, value.z);
//		return true;
//	}
//
//	return false;
//}
//
//bool fengine::Shader::SetUniformVector3Array(const char *name, const Vector3 *value, int size)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniform3fv(location, size, &(value->x));
//		return true;
//	}
//
//	return false;
//}
//
//bool fengine::Shader::SetUniformVector4(const char *name, const Vector4& value)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniform4f(location, value.x, value.y, value.z, value.w);
//		return true;
//	}
//
//	return false;
//}
//
//bool fengine::Shader::SetUniformVector4Array(const char *name, const Vector4 *value, int size)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniform4fv(location, size, &(value->x));
//		return true;
//	}
//
//	return false;
//}
//
//bool fengine::Shader::SetUniformMatrix4(const char *name, const Matrix& matrix)
//{
//	int location;
//	if (m_uniforms.TryGetValue(name, location))
//	{
//		glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&matrix.m);
//		return true;
//	}
//
//	return false;
//}
//
//void fengine::Shader::Destroy()
//{
//	ResourcesManager *manager = ResourcesManager::GetInstance();
//	manager->DestroyResource(GetId());
//}
//
//void fengine::Shader::EnableState() const
//{
//	if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::Culling)
//	{
//		glEnable(GL_CULL_FACE);
//	}
//
//	if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::AlphaTest)
//	{
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	}
//
//	if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::DepthTest)
//	{
//		glEnable(GL_DEPTH_TEST);
//	}
//}
//
//void fengine::Shader::DisableStates() const
//{
//	if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::Culling)
//	{
//		glDisable(GL_CULL_FACE);
//	}
//
//	if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::AlphaTest)
//	{
//		glDisable(GL_BLEND);
//	}
//
//	if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::DepthTest)
//	{
//		glDisable(GL_DEPTH_TEST);
//	}
//}
//
//void fengine::Shader::FetchActiveAttributes()
//{
//	int attributesCount;
//	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &attributesCount);
//
//	for (int i = 0; i < attributesCount; ++i)
//	{
//		char name[80];
//		GLint length = 0;
//		GLint size = 0;
//		GLenum type = 0;
//		glGetActiveAttrib(m_program, i, 80, &length, &size, &type, name);
//		int attributeLocation = glGetAttribLocation(m_program, name);
//		if (attributeLocation != k_undefinedShaderParamLocation)
//		{
//			m_attributes.Add(name, attributeLocation);
//		}
//	}
//}
//
//void fengine::Shader::FetchActiveUniforms()
//{
//	int uniformsCount;
//	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformsCount);
//
//	for (int i = 0; i < uniformsCount; ++i)
//	{
//		char name[80];
//		GLint length = 0;
//		GLint size = 0;
//		GLenum type = 0;
//		glGetActiveUniform(m_program, i, 80, &length, &size, &type, name);
//		int uniformLocation = glGetUniformLocation(m_program, name);
//		if (uniformLocation != k_undefinedShaderParamLocation)
//		{
//			m_uniforms.Add(name, uniformLocation);
//		}
//	}
//}
