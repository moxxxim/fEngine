#include <cstring>
#include "ShaderParameters.h"
#include "../Utils/Debug.h"

char const **fengine::ShaderParameters::s_attibutes = nullptr;
char const **fengine::ShaderParameters::s_uniforms = nullptr;

// Common engine uniforms.

const char *fengine::ShaderParameters::k_uniformViewProjMatrix = "u_matT";
const char *fengine::ShaderParameters::k_uniformModelGlobalMatrix = "u_matGlobal";
const char *fengine::ShaderParameters::k_uniformTime = "u_time";
const char *fengine::ShaderParameters::k_uniformCamPos = "u_camPos";
const char *fengine::ShaderParameters::k_uniformScreenColorOriginal = "u_screenColorOriginal";
const char *fengine::ShaderParameters::k_uniformScreenDepthOriginal = "u_screenDepthOriginal";
const char *fengine::ShaderParameters::k_uniformScreenColorProcessed = "u_screenColorProcessed";

// Fog uniforms.

const char *fengine::ShaderParameters::k_uniformFogStart = "u_fogStart";
const char *fengine::ShaderParameters::k_uniformFogRange = "u_fogRange";
const char *fengine::ShaderParameters::k_uniformFogDensity = "u_fogDensity";
const char *fengine::ShaderParameters::k_uniformFogColor = "u_fogColor";

// Light uniforms.

const char *fengine::ShaderParameters::k_uniformDirectLightCount = "u_directLightCount";
const char *fengine::ShaderParameters::k_uniformDirectLightDirections = "u_directLightDirection[0]";
const char *fengine::ShaderParameters::k_uniformDirectLightColors = "u_directLightColor[0]";
const char *fengine::ShaderParameters::k_uniformDirectLightIntensities = "u_directLightIntensity[0]";
const char *fengine::ShaderParameters::k_uniformPointLightCount = "u_pointLightCount";
const char *fengine::ShaderParameters::k_uniformPointLightPositions = "u_pointLightPos[0]";
const char *fengine::ShaderParameters::k_uniformPointLightRanges = "u_pointLightRange[0]";
const char *fengine::ShaderParameters::k_uniformPointLightColors = "u_pointLightColor[0]";
const char *fengine::ShaderParameters::k_uniformPointLightIntensities = "u_pointLightIntensity[0]";
const char *fengine::ShaderParameters::k_uniformAmbientLightColor= "u_ambientLightColor";

// Vertex attributes.

const char *fengine::ShaderParameters::k_attributeVertexPositionL = "a_posL";
const char *fengine::ShaderParameters::k_attributeVertexUv = "a_uv";
const char *fengine::ShaderParameters::k_attributeVertexNorm = "a_norm";
const char *fengine::ShaderParameters::k_attributeVertexBinorm = "a_binorm";
const char *fengine::ShaderParameters::k_attributeVertexTangent = "a_tan";

bool fengine::ShaderParameters::IsParameterReserved(const char *parameter)
{
	const char **reservedAttributes = GetReservedAttributes();
	for (unsigned int i = 0; i < ReservedAttributesCount; ++i)
	{
		const char *reservedAttribute = reservedAttributes[i];
		if (!strcmp(parameter, reservedAttribute))
		{
			return true;
		}
	}

	const char **reservedUniforms = GetReservedUniforms();
	for (unsigned int i = 0; i < ReservedUniformsCount; ++i)
	{
		const char *reservedUniform = reservedUniforms[i];
		if (!strcmp(parameter, reservedUniform))
		{
			return true;
		}
	}

	return false;
}

fengine::ShaderParameters::ReservedAttribute fengine::ShaderParameters::GetReservedAttribute(const char *attributeName)
{
	if (!strcmp(attributeName, k_attributeVertexPositionL))
	{
		return fengine::ShaderParameters::ReservedAttribute::VertexPositionL;
	}
	else if (!strcmp(attributeName, k_attributeVertexUv))
	{
		return fengine::ShaderParameters::ReservedAttribute::VertexUV;
	}
	else if (!strcmp(attributeName, k_attributeVertexNorm))
	{
		return fengine::ShaderParameters::ReservedAttribute::VertexNorm;
	}
	else if (!strcmp(attributeName, k_attributeVertexBinorm))
	{
		return fengine::ShaderParameters::ReservedAttribute::VertexBinorm;
	}
	else if (!strcmp(attributeName, k_attributeVertexTangent))
	{
		return fengine::ShaderParameters::ReservedAttribute::VertexTangent;
	}

	fengine::Debug::LogWarning("Wrong attribute name specified.");
	return (fengine::ShaderParameters::ReservedAttribute) (-1);
}

fengine::ShaderParameters::ReservedUniform fengine::ShaderParameters::GetReservedUniform(const char * uniformName)
{
	if (!strcmp(uniformName, k_uniformViewProjMatrix))
	{
		return fengine::ShaderParameters::ReservedUniform::ViewProjMatrix;
	}
	else if (!strcmp(uniformName, k_uniformModelGlobalMatrix))
	{
		return fengine::ShaderParameters::ReservedUniform::ModelGlobalMatrix;
	}
	else if (!strcmp(uniformName, k_uniformViewProjMatrix))
	{
		return fengine::ShaderParameters::ReservedUniform::Time;
	}
	else if (!strcmp(uniformName, k_uniformCamPos))
	{
		return fengine::ShaderParameters::ReservedUniform::CamPos;
	}
	else if (!strcmp(uniformName, k_uniformFogColor))
	{
		return fengine::ShaderParameters::ReservedUniform::FogColor;
	}
	else if (!strcmp(uniformName, k_uniformFogStart))
	{
		return fengine::ShaderParameters::ReservedUniform::FogStart;
	}
	else if (!strcmp(uniformName, k_uniformFogRange))
	{
		return fengine::ShaderParameters::ReservedUniform::FogRange;
	}
	else if (!strcmp(uniformName, k_uniformFogDensity))
	{
		return fengine::ShaderParameters::ReservedUniform::FogDensity;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightCount))
	{
		return fengine::ShaderParameters::ReservedUniform::DirectLightCount;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightDirections))
	{
		return fengine::ShaderParameters::ReservedUniform::DirectLightDirection;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightColors))
	{
		return fengine::ShaderParameters::ReservedUniform::DirectLightColor;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightIntensities))
	{
		return fengine::ShaderParameters::ReservedUniform::DirectLightIntensity;
	}
	else if (!strcmp(uniformName, k_uniformPointLightCount))
	{
		return fengine::ShaderParameters::ReservedUniform::PointLightCount;
	}
	else if (!strcmp(uniformName, k_uniformPointLightPositions))
	{
		return fengine::ShaderParameters::ReservedUniform::PointLightPosition;
	}
	else if (!strcmp(uniformName, k_uniformPointLightRanges))
	{
		return fengine::ShaderParameters::ReservedUniform::PointLightRange;
	}
	else if (!strcmp(uniformName, k_uniformPointLightColors))
	{
		return fengine::ShaderParameters::ReservedUniform::PointLightColor;
	}
	else if (!strcmp(uniformName, k_uniformPointLightIntensities))
	{
		return fengine::ShaderParameters::ReservedUniform::PointLightIntensity;
	}
	else if (!strcmp(uniformName, k_uniformAmbientLightColor))
	{
		return fengine::ShaderParameters::ReservedUniform::AmbientLightColor;
	}
	else if (!strcmp(uniformName, k_uniformScreenColorOriginal))
	{
		return fengine::ShaderParameters::ReservedUniform::ScreenColorOriginal;
	}
	else if (!strcmp(uniformName, k_uniformScreenDepthOriginal))
	{
		return fengine::ShaderParameters::ReservedUniform::ScreenDepthOriginal;
	}
	else if (!strcmp(uniformName, k_uniformScreenColorProcessed))
	{
		return fengine::ShaderParameters::ReservedUniform::ScreenColorProcessed;
	}

	fengine::Debug::LogWarning("Wrong attribute name specified.");
	return (fengine::ShaderParameters::ReservedUniform) (-1);
}

const char * fengine::ShaderParameters::GetReservedAttributeName(fengine::ShaderParameters::ReservedAttribute attribute)
{
	switch (attribute)
	{
		case fengine::ShaderParameters::ReservedAttribute::VertexPositionL:
			return k_attributeVertexPositionL;

		case fengine::ShaderParameters::ReservedAttribute::VertexUV:
			return k_attributeVertexUv;

		case fengine::ShaderParameters::ReservedAttribute::VertexNorm:
			return k_attributeVertexNorm;

		case fengine::ShaderParameters::ReservedAttribute::VertexBinorm:
			return k_attributeVertexBinorm;

		case fengine::ShaderParameters::ReservedAttribute::VertexTangent:
			return k_attributeVertexTangent;

		default:
			return "";
	}
}

const char * fengine::ShaderParameters::GetReservedUniformName(fengine::ShaderParameters::ReservedUniform uniform)
{
	switch (uniform)
	{
		case fengine::ShaderParameters::ReservedUniform::ViewProjMatrix:
			return k_uniformViewProjMatrix;

		case fengine::ShaderParameters::ReservedUniform::ModelGlobalMatrix:
			return k_uniformModelGlobalMatrix;

		case fengine::ShaderParameters::ReservedUniform::Time:
			return k_uniformTime;

		case fengine::ShaderParameters::ReservedUniform::CamPos:
			return k_uniformCamPos;

		case fengine::ShaderParameters::ReservedUniform::FogColor:
			return k_uniformFogColor;

		case fengine::ShaderParameters::ReservedUniform::FogStart:
			return k_uniformFogStart;

		case fengine::ShaderParameters::ReservedUniform::FogRange:
			return k_uniformFogRange;

		case fengine::ShaderParameters::ReservedUniform::FogDensity:
			return k_uniformFogDensity;

		case fengine::ShaderParameters::ReservedUniform::DirectLightCount:
			return k_uniformDirectLightCount;

		case fengine::ShaderParameters::ReservedUniform::DirectLightDirection:
			return k_uniformDirectLightDirections;

		case fengine::ShaderParameters::ReservedUniform::DirectLightColor:
			return k_uniformDirectLightColors;

		case fengine::ShaderParameters::ReservedUniform::DirectLightIntensity:
			return k_uniformDirectLightIntensities;

		case fengine::ShaderParameters::ReservedUniform::PointLightCount:
			return k_uniformPointLightCount;

		case fengine::ShaderParameters::ReservedUniform::PointLightPosition:
			return k_uniformPointLightPositions;

		case fengine::ShaderParameters::ReservedUniform::PointLightRange:
			return k_uniformPointLightRanges;

		case fengine::ShaderParameters::ReservedUniform::PointLightColor:
			return k_uniformPointLightColors;

		case fengine::ShaderParameters::ReservedUniform::PointLightIntensity:
			return k_uniformPointLightIntensities;

		case fengine::ShaderParameters::ReservedUniform::AmbientLightColor:
			return k_uniformAmbientLightColor;

		case fengine::ShaderParameters::ReservedUniform::ScreenColorOriginal:
			return k_uniformScreenColorOriginal;

		case fengine::ShaderParameters::ReservedUniform::ScreenDepthOriginal:
			return k_uniformScreenDepthOriginal;

		case fengine::ShaderParameters::ReservedUniform::ScreenColorProcessed:
			return k_uniformScreenColorProcessed;

		default:
			return "";
	}
}

const char ** fengine::ShaderParameters::GetReservedAttributes()
{
	if (!s_attibutes)
	{
		fengine::ShaderParameters::s_attibutes = new const char*[ReservedAttributesCount];
		s_attibutes[0] = k_attributeVertexPositionL;
		s_attibutes[1] = k_attributeVertexUv;
		s_attibutes[2] = k_attributeVertexNorm;
		s_attibutes[3] = k_attributeVertexBinorm;
		s_attibutes[4] = k_attributeVertexTangent;
	}

	return s_attibutes;
}

const char ** fengine::ShaderParameters::GetReservedUniforms()
{
	if (!s_uniforms)
	{
		fengine::ShaderParameters::s_uniforms = new const char*[ReservedUniformsCount];
		s_uniforms[0] = k_uniformViewProjMatrix;
		s_uniforms[1] = k_uniformTime;
		s_uniforms[2] = k_uniformCamPos;
		s_uniforms[3] = k_uniformFogColor;
		s_uniforms[4] = k_uniformFogStart;
		s_uniforms[5] = k_uniformFogRange;
		s_uniforms[6] = k_uniformFogDensity;
		s_uniforms[7] = k_uniformModelGlobalMatrix;
		s_uniforms[8] = k_uniformDirectLightCount;
		s_uniforms[9] = k_uniformDirectLightDirections;
		s_uniforms[10] = k_uniformDirectLightColors;
		s_uniforms[11] = k_uniformDirectLightIntensities;
		s_uniforms[12] = k_uniformPointLightCount;
		s_uniforms[13] = k_uniformPointLightPositions;
		s_uniforms[14] = k_uniformPointLightRanges;
		s_uniforms[15] = k_uniformPointLightColors;
		s_uniforms[16] = k_uniformPointLightIntensities;
		s_uniforms[17] = k_uniformAmbientLightColor;
		s_uniforms[18] = k_uniformScreenColorOriginal;
		s_uniforms[19] = k_uniformScreenDepthOriginal;
		s_uniforms[20] = k_uniformScreenColorProcessed;
	}

	return s_uniforms;
}
