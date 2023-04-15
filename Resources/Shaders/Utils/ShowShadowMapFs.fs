#version 330 core

in vec2 varUv0;

uniform sampler2DArray uDirectShadowMap;

out vec4 FragColor;

void main()
{
    float depth = texture(uDirectShadowMap, vec3(varUv0, 0)).r;
    FragColor = vec4(vec3(depth), 1.0);
}