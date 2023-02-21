#version 330 core

in vec2 varUv0;

uniform sampler2D uShadowMap;

out vec4 FragColor;

void main()
{
    float depth = texture(uShadowMap, varUv0).r;
    FragColor = vec4(vec3(depth), 1.0);
}