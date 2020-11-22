precision mediump float;

uniform sampler2D u_texGrass;
uniform sampler2D u_texDirt;
uniform sampler2D u_texRock;
uniform sampler2D u_texBlend;
uniform float u_tiling;

uniform vec3 u_fogColor;
uniform float u_fogStart;
uniform float u_fogRange;
uniform float u_fogDensity;
uniform vec3 u_camPos;

varying vec2 v_uv;
varying vec4 v_fragPos;

void main()
{
	vec4 colorGrass = texture2D(u_texGrass, v_uv * u_tiling);
	vec4 colorDirt = texture2D(u_texDirt, v_uv * u_tiling);
	vec4 colorRock = texture2D(u_texRock, v_uv * u_tiling);
	vec4 blend = texture2D(u_texBlend, v_uv);
	vec4 texColor = (blend.r * colorGrass + blend.g * colorDirt + blend.b * colorRock)/(blend.r + blend.g + blend.b);

	float distanceToCam = length(v_fragPos.xyz - u_camPos);
	float fogLerp = clamp(u_fogDensity * (distanceToCam - u_fogStart)/u_fogRange, 0.0, 1.0);
	gl_FragColor = (1.0 - fogLerp) * texColor + fogLerp * vec4(u_fogColor, 1.0);
}
