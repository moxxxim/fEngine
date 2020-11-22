attribute vec3 a_posL;
attribute vec3 a_norm;
attribute vec3 a_binorm;
attribute vec3 a_tan;
attribute vec2 a_uv;

uniform mat4 u_matT;
uniform mat4 u_matGlobal;

varying vec2 v_uv;
varying vec3 v_posW;
varying vec3 v_normW;
varying vec3 v_binormW;
varying vec3 v_tanW;

void main()
{
	gl_Position = u_matT * vec4(a_posL, 1.0);
	v_uv = a_uv;
	v_posW = (u_matGlobal * vec4(a_posL, 1.0)).xyz;
	v_normW = (u_matGlobal * vec4(a_norm, 0.0)).xyz;
	v_binormW = (u_matGlobal * vec4(a_binorm, 0.0)).xyz;
	v_tanW = (u_matGlobal * vec4(a_tan, 0.0)).xyz;
}