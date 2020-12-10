precision mediump float;

uniform samplerCube u_cubeMap;

varying vec3 v_posL;

void main()
{
	gl_FragColor = textureCube(u_cubeMap, v_posL);
}
