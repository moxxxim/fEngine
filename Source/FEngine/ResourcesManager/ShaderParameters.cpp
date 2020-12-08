#include <cstring>
#include "ShaderParameters.h"
#include "../Utils/Debug.h"

char const **feng::ShaderParameters::s_attibutes = nullptr;
char const **feng::ShaderParameters::s_uniforms = nullptr;

// Common engine uniforms.

const char *feng::ShaderParameters::k_uniformViewProjMatrix = "u_matT";
const char *feng::ShaderParameters::k_uniformModelGlobalMatrix = "u_matGlobal";
const char *feng::ShaderParameters::k_uniformTime = "u_time";
const char *feng::ShaderParameters::k_uniformCamPos = "u_camPos";
const char *feng::ShaderParameters::k_uniformScreenColorOriginal = "u_screenColorOriginal";
const char *feng::ShaderParameters::k_uniformScreenDepthOriginal = "u_screenDepthOriginal";
const char *feng::ShaderParameters::k_uniformScreenColorProcessed = "u_screenColorProcessed";

// Fog uniforms.

const char *feng::ShaderParameters::k_uniformFogStart = "u_fogStart";
const char *feng::ShaderParameters::k_uniformFogRange = "u_fogRange";
const char *feng::ShaderParameters::k_uniformFogDensity = "u_fogDensity";
const char *feng::ShaderParameters::k_uniformFogColor = "u_fogColor";

// Light uniforms.

const char *feng::ShaderParameters::k_uniformDirectLightCount = "u_directLightCount";
const char *feng::ShaderParameters::k_uniformDirectLightDirections = "u_directLightDirection[0]";
const char *feng::ShaderParameters::k_uniformDirectLightColors = "u_directLightColor[0]";
const char *feng::ShaderParameters::k_uniformDirectLightIntensities = "u_directLightIntensity[0]";
const char *feng::ShaderParameters::k_uniformPointLightCount = "u_pointLightCount";
const char *feng::ShaderParameters::k_uniformPointLightPositions = "u_pointLightPos[0]";
const char *feng::ShaderParameters::k_uniformPointLightRanges = "u_pointLightRange[0]";
const char *feng::ShaderParameters::k_uniformPointLightColors = "u_pointLightColor[0]";
const char *feng::ShaderParameters::k_uniformPointLightIntensities = "u_pointLightIntensity[0]";
const char *feng::ShaderParameters::k_uniformAmbientLightColor= "u_ambientLightColor";

// Vertex attributes.

const char *feng::ShaderParameters::k_attributeVertexPositionL = "a_posL";
const char *feng::ShaderParameters::k_attributeVertexUv = "a_uv";
const char *feng::ShaderParameters::k_attributeVertexNorm = "a_norm";
const char *feng::ShaderParameters::k_attributeVertexBinorm = "a_binorm";
const char *feng::ShaderParameters::k_attributeVertexTangent = "a_tan";

bool feng::ShaderParameters::IsParameterReserved(const char *parameter)
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

feng::ShaderParameters::ReservedAttribute feng::ShaderParameters::GetReservedAttribute(const char *attributeName)
{
	if (!strcmp(attributeName, k_attributeVertexPositionL))
	{
		return feng::ShaderParameters::ReservedAttribute::VertexPositionL;
	}
	else if (!strcmp(attributeName, k_attributeVertexUv))
	{
		return feng::ShaderParameters::ReservedAttribute::VertexUV;
	}
	else if (!strcmp(attributeName, k_attributeVertexNorm))
	{
		return feng::ShaderParameters::ReservedAttribute::VertexNorm;
	}
	else if (!strcmp(attributeName, k_attributeVertexBinorm))
	{
		return feng::ShaderParameters::ReservedAttribute::VertexBinorm;
	}
	else if (!strcmp(attributeName, k_attributeVertexTangent))
	{
		return feng::ShaderParameters::ReservedAttribute::VertexTangent;
	}

	feng::Debug::LogWarning("Wrong attribute name specified.");
	return (feng::ShaderParameters::ReservedAttribute) (-1);
}

feng::ShaderParameters::ReservedUniform feng::ShaderParameters::GetReservedUniform(const char * uniformName)
{
	if (!strcmp(uniformName, k_uniformViewProjMatrix))
	{
		return feng::ShaderParameters::ReservedUniform::ViewProjMatrix;
	}
	else if (!strcmp(uniformName, k_uniformModelGlobalMatrix))
	{
		return feng::ShaderParameters::ReservedUniform::ModelGlobalMatrix;
	}
	else if (!strcmp(uniformName, k_uniformViewProjMatrix))
	{
		return feng::ShaderParameters::ReservedUniform::Time;
	}
	else if (!strcmp(uniformName, k_uniformCamPos))
	{
		return feng::ShaderParameters::ReservedUniform::CamPos;
	}
	else if (!strcmp(uniformName, k_uniformFogColor))
	{
		return feng::ShaderParameters::ReservedUniform::FogColor;
	}
	else if (!strcmp(uniformName, k_uniformFogStart))
	{
		return feng::ShaderParameters::ReservedUniform::FogStart;
	}
	else if (!strcmp(uniformName, k_uniformFogRange))
	{
		return feng::ShaderParameters::ReservedUniform::FogRange;
	}
	else if (!strcmp(uniformName, k_uniformFogDensity))
	{
		return feng::ShaderParameters::ReservedUniform::FogDensity;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightCount))
	{
		return feng::ShaderParameters::ReservedUniform::DirectLightCount;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightDirections))
	{
		return feng::ShaderParameters::ReservedUniform::DirectLightDirection;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightColors))
	{
		return feng::ShaderParameters::ReservedUniform::DirectLightColor;
	}
	else if (!strcmp(uniformName, k_uniformDirectLightIntensities))
	{
		return feng::ShaderParameters::ReservedUniform::DirectLightIntensity;
	}
	else if (!strcmp(uniformName, k_uniformPointLightCount))
	{
		return feng::ShaderParameters::ReservedUniform::PointLightCount;
	}
	else if (!strcmp(uniformName, k_uniformPointLightPositions))
	{
		return feng::ShaderParameters::ReservedUniform::PointLightPosition;
	}
	else if (!strcmp(uniformName, k_uniformPointLightRanges))
	{
		return feng::ShaderParameters::ReservedUniform::PointLightRange;
	}
	else if (!strcmp(uniformName, k_uniformPointLightColors))
	{
		return feng::ShaderParameters::ReservedUniform::PointLightColor;
	}
	else if (!strcmp(uniformName, k_uniformPointLightIntensities))
	{
		return feng::ShaderParameters::ReservedUniform::PointLightIntensity;
	}
	else if (!strcmp(uniformName, k_uniformAmbientLightColor))
	{
		return feng::ShaderParameters::ReservedUniform::AmbientLightColor;
	}
	else if (!strcmp(uniformName, k_uniformScreenColorOriginal))
	{
		return feng::ShaderParameters::ReservedUniform::ScreenColorOriginal;
	}
	else if (!strcmp(uniformName, k_uniformScreenDepthOriginal))
	{
		return feng::ShaderParameters::ReservedUniform::ScreenDepthOriginal;
	}
	else if (!strcmp(uniformName, k_uniformScreenColorProcessed))
	{
		return feng::ShaderParameters::ReservedUniform::ScreenColorProcessed;
	}

	feng::Debug::LogWarning("Wrong attribute name specified.");
	return (feng::ShaderParameters::ReservedUniform) (-1);
}

const char * feng::ShaderParameters::GetReservedAttributeName(feng::ShaderParameters::ReservedAttribute attribute)
{
	switch (attribute)
	{
		case feng::ShaderParameters::ReservedAttribute::VertexPositionL:
			return k_attributeVertexPositionL;

		case feng::ShaderParameters::ReservedAttribute::VertexUV:
			return k_attributeVertexUv;

		case feng::ShaderParameters::ReservedAttribute::VertexNorm:
			return k_attributeVertexNorm;

		case feng::ShaderParameters::ReservedAttribute::VertexBinorm:
			return k_attributeVertexBinorm;

		case feng::ShaderParameters::ReservedAttribute::VertexTangent:
			return k_attributeVertexTangent;

		default:
			return "";
	}
}

const char * feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform uniform)
{
	switch (uniform)
	{
		case feng::ShaderParameters::ReservedUniform::ViewProjMatrix:
			return k_uniformViewProjMatrix;

		case feng::ShaderParameters::ReservedUniform::ModelGlobalMatrix:
			return k_uniformModelGlobalMatrix;

		case feng::ShaderParameters::ReservedUniform::Time:
			return k_uniformTime;

		case feng::ShaderParameters::ReservedUniform::CamPos:
			return k_uniformCamPos;

		case feng::ShaderParameters::ReservedUniform::FogColor:
			return k_uniformFogColor;

		case feng::ShaderParameters::ReservedUniform::FogStart:
			return k_uniformFogStart;

		case feng::ShaderParameters::ReservedUniform::FogRange:
			return k_uniformFogRange;

		case feng::ShaderParameters::ReservedUniform::FogDensity:
			return k_uniformFogDensity;

		case feng::ShaderParameters::ReservedUniform::DirectLightCount:
			return k_uniformDirectLightCount;

		case feng::ShaderParameters::ReservedUniform::DirectLightDirection:
			return k_uniformDirectLightDirections;

		case feng::ShaderParameters::ReservedUniform::DirectLightColor:
			return k_uniformDirectLightColors;

		case feng::ShaderParameters::ReservedUniform::DirectLightIntensity:
			return k_uniformDirectLightIntensities;

		case feng::ShaderParameters::ReservedUniform::PointLightCount:
			return k_uniformPointLightCount;

		case feng::ShaderParameters::ReservedUniform::PointLightPosition:
			return k_uniformPointLightPositions;

		case feng::ShaderParameters::ReservedUniform::PointLightRange:
			return k_uniformPointLightRanges;

		case feng::ShaderParameters::ReservedUniform::PointLightColor:
			return k_uniformPointLightColors;

		case feng::ShaderParameters::ReservedUniform::PointLightIntensity:
			return k_uniformPointLightIntensities;

		case feng::ShaderParameters::ReservedUniform::AmbientLightColor:
			return k_uniformAmbientLightColor;

		case feng::ShaderParameters::ReservedUniform::ScreenColorOriginal:
			return k_uniformScreenColorOriginal;

		case feng::ShaderParameters::ReservedUniform::ScreenDepthOriginal:
			return k_uniformScreenDepthOriginal;

		case feng::ShaderParameters::ReservedUniform::ScreenColorProcessed:
			return k_uniformScreenColorProcessed;

		default:
			return "";
	}
}

const char ** feng::ShaderParameters::GetReservedAttributes()
{
	if (!s_attibutes)
	{
		feng::ShaderParameters::s_attibutes = new const char*[ReservedAttributesCount];
		s_attibutes[0] = k_attributeVertexPositionL;
		s_attibutes[1] = k_attributeVertexUv;
		s_attibutes[2] = k_attributeVertexNorm;
		s_attibutes[3] = k_attributeVertexBinorm;
		s_attibutes[4] = k_attributeVertexTangent;
	}

	return s_attibutes;
}

const char ** feng::ShaderParameters::GetReservedUniforms()
{
	if (!s_uniforms)
	{
		feng::ShaderParameters::s_uniforms = new const char*[ReservedUniformsCount];
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
