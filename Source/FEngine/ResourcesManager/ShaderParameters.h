#pragma once

namespace fengine
{

class ShaderParameters final
{

public:

	enum class ReservedAttribute
	{
		VertexPositionL = 0,
		VertexUV = 1,
		VertexNorm = 2,
		VertexBinorm = 3,
		VertexTangent = 4
	};

	enum class ReservedUniform
	{
		ViewProjMatrix = 0,
		Time = 1,
		FogStart = 2,
		FogRange = 3,
		FogColor = 4,
		FogDensity = 5,
		CamPos = 6,
		ModelGlobalMatrix = 7,
		DirectLightCount = 8,
		DirectLightDirection = 9,
		DirectLightColor = 10,
		DirectLightIntensity = 11,
		PointLightCount = 12,
		PointLightPosition = 13,
		PointLightRange = 14,
		PointLightColor = 15,
		PointLightIntensity = 16,
		AmbientLightColor = 17,
		ScreenColorOriginal = 18,
		ScreenDepthOriginal = 19,
		ScreenColorProcessed = 20
	};

	static bool IsParameterReserved(const char *attribute);
	static ReservedAttribute GetReservedAttribute(const char *attributeName);
	static ReservedUniform GetReservedUniform(const char *uniformName);
	static const char * GetReservedAttributeName(ReservedAttribute attribute);
	static const char * GetReservedUniformName(ReservedUniform uniform);
	static const char ** GetReservedAttributes();
	static const char ** GetReservedUniforms();

	static const unsigned int ReservedAttributesCount = 5;
	static const unsigned int ReservedUniformsCount = 21;

private:

	static char const **s_attibutes;
	static char const **s_uniforms;

	static const char *k_uniformViewProjMatrix;
	static const char *k_uniformModelGlobalMatrix;
	static const char *k_uniformTime;
	static const char *k_uniformCamPos;
	static const char *k_uniformFogStart;
	static const char *k_uniformFogRange;
	static const char *k_uniformFogDensity;
	static const char *k_uniformFogColor;
	static const char *k_uniformScreenColorOriginal;
	static const char *k_uniformScreenDepthOriginal;
	static const char *k_uniformScreenColorProcessed;

	static const char *k_uniformDirectLightCount;
	static const char *k_uniformDirectLightDirections;
	static const char *k_uniformDirectLightColors;
	static const char *k_uniformDirectLightIntensities;
	static const char *k_uniformPointLightCount;
	static const char *k_uniformPointLightPositions;
	static const char *k_uniformPointLightRanges;
	static const char *k_uniformPointLightColors;
	static const char *k_uniformPointLightIntensities;
	static const char *k_uniformAmbientLightColor;

	static const char *k_attributeVertexPositionL;
	static const char *k_attributeVertexUv;
	static const char *k_attributeVertexNorm;
	static const char *k_attributeVertexBinorm;
	static const char *k_attributeVertexTangent;

};

}