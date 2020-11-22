precision mediump float;

uniform sampler2D u_textureMain;

// Reflection params.

uniform samplerCube u_reflectionMap;
uniform float u_specularity;

// Fog params.

uniform vec3 u_fogColor;
uniform float u_fogStart;
uniform float u_fogRange;
uniform float u_fogDensity;
uniform vec3 u_camPos;

varying vec2 v_uv;
varying vec3 v_posW;
varying vec3 v_normW;

vec4 applyFog(vec4 texColor)
{
	float distanceToCam = length(v_posW - u_camPos);
	float fogLerp = clamp(u_fogDensity * (distanceToCam - u_fogStart)/u_fogRange, 0.0, 1.0);
	vec4 finalColor = mix(texColor, vec4(u_fogColor, 1.0), fogLerp);
	finalColor.a = texColor.a;

	return finalColor;
}

vec4 getReflection()
{
	vec3 fromEye = v_posW - u_camPos;
	vec3 reflectDir = reflect(normalize(fromEye), normalize(v_normW));
	return textureCube(u_reflectionMap, reflectDir);
}

void main()
{
	vec4 texColor = texture2D(u_textureMain, v_uv);
	vec4 reflection = getReflection();

	gl_FragColor = (1.0 - u_specularity) * applyFog(texColor) + u_specularity * reflection;
}
