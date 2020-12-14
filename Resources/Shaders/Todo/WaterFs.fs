precision mediump float;

uniform vec3 u_camPos;
uniform samplerCube u_reflectionMap;

// Water parameters.

uniform sampler2D u_waterBottom;
uniform sampler2D u_normalMap;
uniform sampler2D u_displacementMap;
uniform vec4 u_waterColor;
uniform float u_flowSpeed;
uniform float u_time;
uniform float u_depthAdjustDisplacement;
uniform float u_transparencyFactor;
uniform float u_depthAdjust;
uniform float u_surfaceMaxDisplacement;

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

vec4 applyWaterBottomLight(vec4 texColor)
{
	vec3 normalW = normalize(v_normW);
	vec3 toEye = normalize(u_camPos - v_posW);
	vec4 colorDiff = vec4(0.0, 0.0, 0.0, 1.0);

	// Apply directional lights.
	for(int i = 0; i < u_directLightCount; i++)
	{
		vec4 color = u_directLightColor[i];
		vec3 direction = u_directLightDirection[i];
		float intensity = u_directLightIntensity[i];
		vec3 lightReflection = reflect(direction, normalW);
		vec4 pureColor = color * intensity;
		colorDiff += pureColor * max(dot(normalW, -direction), 0.0);
	}

	// Apply point lights.
	for(int i = 0; i < int(u_pointLightCount); i++)
	{
		vec4 color = u_pointLightColor[i];
		vec3 direction = v_posW - u_pointLightPos[i];
		float lightDistance = length(direction);
		float intensity = u_pointLightIntensity[i];
		float lightRange = u_pointLightRange[i];
		float rangeIntensity = pow(clamp(1.0 - lightDistance/lightRange, 0.0, 1.0), 2.0);
		vec3 reflection = reflect(normalize(direction), normalW);
		vec4 pureColor = color * intensity * rangeIntensity;
		colorDiff += pureColor * max(dot(normalW, -normalize(direction)), 0.0);
	}

	vec4 finalColor = (w * u_ambientLightColor + (1.0 - w) * colorDiff) * texColor;
	finalColor.a = texColor.a;

	return finalColor;
}

vec2 getUV(float maxDisplacement)
{
	vec2 displacementUv = vec2(v_uv.x, v_uv.y + u_time * u_flowSpeed);
	vec2 normDisp = texture2D(u_displacementMap, displacementUv).xy;
	vec2 disp = (2.0 * normDisp - 1.0) * maxDisplacement;
	return v_uv + disp;
}

vec4 getRefractionColor()
{
	vec2 uv = getUV(v_uv.x * u_depthAdjustDisplacement);
	vec4 waterBottom = texture2D(u_waterBottom, uv);
	float depthFactor = clamp(u_depthAdjust * v_uv.x, 0.0, 1.0);
	vec4 waterColor = (1.0 - depthFactor) * waterBottom + depthFactor * u_waterColor;
	
	return applyWaterBottomLight(waterColor);
}

vec3 getNormalW()
{
	vec2 uv = getUV(u_surfaceMaxDisplacement);
	vec3 normal = texture2D(u_normalMap, uv).xyz;
	mat3 tbn = mat3(normalize(v_tanW), normalize(v_binormW), normalize(v_normW));

	return normalize(tbn * (2.0 * normal - 1.0));
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
	vec3 fromEye = normalize(v_posW - u_camPos);
	vec3 normalW = getNormalW();
	vec3 reflectDir = reflect(fromEye, normalW);
	vec4 reflectionColor = textureCube(u_reflectionMap, reflectDir);
	vec4 refractionColor = getRefractionColor();
	float fresnelTerm = pow(1.0 - abs(dot(normalW, -fromEye)), u_transparencyFactor);
	vec4 finalColor = mix(refractionColor, reflectionColor, fresnelTerm);


	gl_FragColor = applyFog(finalColor);
}