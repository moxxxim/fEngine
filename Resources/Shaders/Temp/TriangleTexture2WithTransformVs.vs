#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

uniform mat4 uViewProjMatrix;

out vec2 uv;

void main()
{
    gl_Position = uViewProjMatrix * vec4(aPosition, 1.0);
    uv = aUv;
}