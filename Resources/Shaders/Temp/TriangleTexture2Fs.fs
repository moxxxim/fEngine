#version 330 core

in vec2 uv;

out vec4 FragColor;

uniform sampler2D uTex1;
uniform sampler2D uTex2;
uniform float mixValue;

void main()
{
    FragColor = mix(texture(uTex1, uv), texture(uTex2, uv), mixValue);
}