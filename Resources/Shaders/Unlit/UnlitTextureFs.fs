#version 330 core

uniform sampler2D uTexture0;

in vec2 varUv0;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, varUv0);
}