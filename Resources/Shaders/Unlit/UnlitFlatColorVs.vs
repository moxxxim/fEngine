#version 330 core

layout (location = 0) vec3 uPos;

uniform mat4 uModelMatrix;
uniform mat4 uViewProjMatrix;

void main()
{
    gl_Position = uViewProjMatrix * uModelMatrix * vec4(vec3, 1.f);
}