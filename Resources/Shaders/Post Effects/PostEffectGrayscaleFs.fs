#version 330 core

uniform sampler2D uScreenColorProcessed;

in vec2 varUv0;

out vec4 FragColor;

void main()
{
	vec4 color = texture(uScreenColorProcessed, varUv0);
	float grayscale = 0.3 * color.x + 0.59 * color.y + 0.11 * color.z;
	FragColor = vec4(vec3(grayscale), 1.0);
}