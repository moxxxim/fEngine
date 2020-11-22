precision mediump float;

uniform sampler2D u_textureMain;
uniform sampler2D u_displacementMap;
uniform sampler2D u_fireMask;
uniform float u_time;
uniform float u_maxDisplacement;

varying vec2 v_uv;

vec2 getUV()
{
	vec2 normDisp = texture2D(u_displacementMap, vec2(v_uv.x + u_time, v_uv.y + u_time)).xy;
	vec2 disp = (2.0 * normDisp - 1.0) * u_maxDisplacement;
	return v_uv + disp;
}

void main()
{
	vec2 texCoord = getUV();
	vec4 fireColor = texture2D(u_textureMain, texCoord);
	vec4 fireAlpha = texture2D(u_fireMask, v_uv);
	gl_FragColor = fireColor * vec4(1.0, 1.0, 1.0, fireAlpha.r);
}
