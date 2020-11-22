precision mediump float;

uniform sampler2D u_screenColorOriginal;
uniform sampler2D u_screenColorProcessed;

varying vec2 v_uv;

void main()
{
	vec4 color = texture2D(u_screenColorOriginal, v_uv);
	vec4 blur = texture2D(u_screenColorProcessed, v_uv);
	
	gl_FragColor = color + 1.2 * blur;
}