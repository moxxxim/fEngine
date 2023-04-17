#version 330 core

in vec2 varUv0;

uniform sampler2DArray uDirectShadowMap;
uniform int uDebugCascadeNum = 0;

out vec4 FragColor;

void main()
{
    float depth = texture(uDirectShadowMap, vec3(varUv0, uDebugCascadeNum)).r;
    FragColor = vec4(vec3(depth), 1.0);
}