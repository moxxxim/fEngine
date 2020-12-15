precision mediump float;

uniform sampler2D u_screenColorOriginal;
uniform float u_brightness;

varying vec2 v_uv;

void main()
{
	vec4 color = texture2D(u_screenColorOriginal, v_uv);
	float grayscale = 0.3 * color.x * 0.59 * color.y + 0.11 * color.z;
	gl_FragColor = vec4(vec3(grayscale), 1.0) * u_brightness;
}