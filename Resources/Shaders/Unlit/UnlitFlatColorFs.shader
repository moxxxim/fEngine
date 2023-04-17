#version 330 core

uniform vec3 uMainColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(uMainColor, 1.f);
}