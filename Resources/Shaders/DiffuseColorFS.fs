precision mediump float;

uniform sampler2D u_textureMain;
uniform vec3 u_camPos;

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

const float specPower = 20.0;
const float w = 0.3;

vec4 applyLight(vec4 texColor)
{
	vec4 colorDiff = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 colorSpec = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 toEye = normalize(u_camPos - v_posW);
	vec3 normW = normalize(v_normW);

	// Apply directional lights.
	for(int i = 0; i < u_directLightCount; i++)
	{
		vec4 color = u_directLightColor[i];
		vec3 direction = u_directLightDirection[i];
		float intensity = u_directLightIntensity[i];
		vec3 lightReflection = reflect(direction, normW);
		vec4 colorComponent = color * intensity;
		colorDiff += colorComponent * max(dot(normW, -direction), 0.0);
		colorSpec += colorComponent * pow(max(dot(lightReflection, toEye), 0.0), specPower);
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
		vec3 reflection = reflect(normDirection, normW);
		vec4 colorComponent = color * intensity * rangeIntensity;
		colorDiff += colorComponent * max(dot(normW, -normDirection), 0.0);
		colorSpec += colorComponent * pow(max(dot(reflection, toEye), 0.0), specPower);
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