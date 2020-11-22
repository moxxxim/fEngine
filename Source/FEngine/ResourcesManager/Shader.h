#pragma once

#include "../../../Utilities/utilities.h"

#include "../Utilities/utilities.h"
#include "../App/Object.h"
#include "../Utils/StringMap.h"
#include "CachedShader.h"

namespace fengine
{

enum class ShaderStates : int;

class Shader final : public Object
{

public:

	Shader(const CachedShader& cachedShader);
	~Shader();

	void StartUse() const;
	void StopUse() const;
	void Destroy() override;

	const StringMap<int> * GetAttributes() const;
	const StringMap<int> * GetUniforms() const;
	bool TryGetAttribute(const char *name, int& location) const;
	bool HasUniform(const char *name) const;
	bool SetUniformInt(const char *name, int value);
	bool SetUniformFloat(const char *name, float value);
	bool SetUniformFloatArray(const char *name, const float *value, int size);
	bool SetUniformVector3(const char *name, const Vector3& value);
	bool SetUniformVector3Array(const char *name, const Vector3 *value, int size);
	bool SetUniformVector4(const char *name, const Vector4& value);
	bool SetUniformVector4Array(const char *name, const Vector4 *value, int size);
	bool SetUniformMatrix4(const char *name, const Matrix& value);

private:

	void EnableState() const;
	void DisableStates() const;
	void FetchActiveAttributes();
	void FetchActiveUniforms();

	unsigned int m_program = 0;
	unsigned int m_vertexShader = 0;
	unsigned int m_fragmentShader = 0;
	StringMap<int> m_attributes;
	StringMap<int> m_uniforms;
	CachedShader m_cachedInfo;

	static const int k_undefinedShaderParamLocation = -1;

};

}