precision mediump float;

uniform sampler2D u_colorProcessed;
uniform vec4 u_step;

varying vec2 v_uv;

void main()
{
	vec3 color1 = texture2D(u_colorProcessed, v_uv).rgb;
	vec3 color2 = texture2D(u_colorProcessed, vec2(v_uv.x + u_step.x, v_uv.y)).rgb;
	vec3 color3 = texture2D(u_colorProcessed, vec2(v_uv.x - u_step.x, v_uv.y)).rgb;
	vec3 color4 = texture2D(u_colorProcessed, vec2(v_uv.x, v_uv.y + u_step.y)).rgb; 
	vec3 color5 = texture2D(u_colorProcessed, vec2(v_uv.x, v_uv.y - u_step.y)).rgb; 
	vec3 color6 = texture2D(u_colorProcessed, vec2(v_uv.x + u_step.z, v_uv.y + u_step.w)).rgb;
	vec3 color7 = texture2D(u_colorProcessed, vec2(v_uv.x - u_step.z, v_uv.y + u_step.w)).rgb;
	vec3 color8 = texture2D(u_colorProcessed, vec2(v_uv.x + u_step.z, v_uv.y - u_step.w)).rgb;
	vec3 color9 = texture2D(u_colorProcessed, vec2(v_uv.x - u_step.z, v_uv.y - u_step.w)).rgb;
	vec3 colorRGB = (color1 * 2.0 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9) * 0.1;
	gl_FragColor = vec4(colorRGB, 1.0);
}