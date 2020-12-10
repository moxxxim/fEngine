#version 330 core

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float uMixValue;

in vec2 varUv0;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(uTexture0, varUv0), texture(uTexture1, varUv0), uMixValue);
}