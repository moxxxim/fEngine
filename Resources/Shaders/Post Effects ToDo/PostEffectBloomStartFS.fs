precision mediump float;

uniform sampler2D u_screenColorOriginal;
uniform float u_limit;

varying vec2 v_uv;

void main()
{
	vec3 color = texture2D(u_screenColorOriginal, v_uv).rgb;
	float grayscale = 0.3 * color.x * 0.59 * color.y + 0.11 * color.z;
	float val = step(u_limit, grayscale);
	gl_FragColor = vec4(color * val, 1.0);
}	