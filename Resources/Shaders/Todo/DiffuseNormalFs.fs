precision mediump float;

uniform sampler2D u_textureMain;
uniform sampler2D u_normalMap;
uniform vec3 u_camPos;
uniform vec4 u_colorMain;

// Fog parameters.

uniform vec3 u_fogColor;
uniform float u_fogStart;
uniform float u_fogRange;
uniform float u_fogDensity;

// Light parameters

uniform vec4 u_ambientLightColor;
uniform int u_directLightCount;
uniform vec3 u_directLightDirection[10];
uniform vec4 u_directLightColor[10];
uniform float u_directLightIntensity[10];
uniform int u_pointLightCount;
uniform vec3 u_pointLightPos[10];
uniform float u_pointLightRange[10];
uniform vec4 u_pointLightColor[10];
uniform float u_pointLightIntensity[10];

// Varying parameters.

varying vec2 v_uv;
varying vec3 v_posW;
varying vec3 v_normW;
varying vec3 v_binormW;
varying vec3 v_tanW;

const float specPower = 20.0;
const float w = 0.3;

vec4 applyLight(vec4 texColor)
{
	vec3 normal = texture2D(u_normalMap, v_uv).xyz;
	mat3 tbn = mat3(normalize(v_tanW), normalize(v_binormW), normalize(v_normW));
	vec3 normalW = normalize(tbn * (2.0 * normal - 1.0));
	vec4 colorDiff = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 colorSpec = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 fromEye = normalize(v_posW - u_camPos);

	// Apply directional lights.
	for(int i = 0; i < u_directLightCount; i++)
	{
		vec4 color = u_directLightColor[i];
		vec3 direction = u_directLightDirection[i];
		float intensity = u_directLightIntensity[i];
		vec3 lightReflection = reflect(direction, normalW);
		vec4 colorComponent = color * intensity;
		colorDiff += colorComponent * max(dot(normalW, -direction), 0.0);
		colorSpec += colorComponent * pow(max(dot(lightReflection, -fromEye), 0.0), specPower);
	}

	// Apply point lights.
	for(int i = 0; i < int(u_pointLightCount); i++)
	{
		vec4 color = u_pointLightColor[i];
		vec3 direction = u_pointLightPos[i] - v_posW;
		vec3 normDirection = normalize(direction);
		float lightDistance = length(direction);
		float intensity = u_pointLightIntensity[i];
		float lightRange = u_pointLightRange[i];
		float rangeIntensity = pow(clamp(1.0 - lightDistance/lightRange, 0.0, 1.0), 2.0);
		vec3 reflection = reflect(normDirection, normalW);
		vec4 colorComponent = color * intensity * rangeIntensity;
		colorDiff += colorComponent * max(dot(normalW, -normDirection), 0.0);
		colorSpec += colorComponent * pow(max(dot(reflection, -fromEye), 0.0), specPower);
	}

	vec4 finalColor = (w * u_ambientLightColor + (1.0 - w) * colorDiff) * texColor + colorSpec;
	finalColor.a = texColor.a;

	return finalColor;
}

vec4 applyFog(vec4 texColor)
{
	float distanceToCam = length(v_posW - u_camPos);
	float fogLerp = clamp(u_fogDensity * (distanceToCam - u_fogStart)/u_fogRange, 0.0, 1.0);
	vec4 finalColor = (1.0 - fogLerp) * texColor + fogLerp * vec4(u_fogColor, 1.0);
	finalColor.a = texColor.a;

	return finalColor;
}

void main()
{
	vec4 texColor = texture2D(u_textureMain, v_uv);
	gl_FragColor = applyFog(applyLight(texColor));
}