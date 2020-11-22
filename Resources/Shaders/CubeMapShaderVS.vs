attribute vec3 a_posL;

uniform mat4 u_matT;

varying vec3 v_posL;

void main()
{
	gl_Position = u_matT * vec4(a_posL, 1.0);
	v_posL = a_posL;
}