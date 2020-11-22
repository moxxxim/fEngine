precision mediump float;

uniform sampler2D u_textureMain;

uniform bool u_fogEnabled;
uniform vec4 u_fogColor;
uniform float u_fogStart;
uniform float u_fogRange;

varying vec2 v_uv;
varying vec3 v_camPos;

void main()
{
	gl_FragColor = texture2D(u_textureMain, v_uv1);
}
