#version 330 core

uniform samplerCube uTexture0;

in vec3 varPos;

out vec4 FragColor;

void main()
{
	FragColor = texture(uTexture0, varPos);
}
