attribute vec3 a_posL;
attribute vec2 a_uv;

uniform mat4 u_matT;
uniform mat4 u_matGlobal;

varying vec2 v_uv;
varying vec4 v_fragPos;

void main()
{
	gl_Position = u_matT * vec4(a_posL, 1.0);
	v_uv = a_uv;
	v_fragPos = u_matGlobal * vec4(a_posL, 1.0);
}