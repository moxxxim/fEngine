#version 330 core

uniform sampler2D uScreenColorProcessed;

in vec2 varUv0;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(1.0f - texture(uScreenColorProcessed, varUv0)), 1.0);
}