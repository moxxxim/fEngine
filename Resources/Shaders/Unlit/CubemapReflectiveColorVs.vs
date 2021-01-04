#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 uModelMatrix;
uniform mat4 uViewProjMatrix;

out vec3 varFragPos;
out vec3 varNorm;

void main()
{
    vec4 worldPos = uModelMatrix * vec4(aPos, 1.f);

    gl_Position = uViewProjMatrix * worldPos;
    varFragPos = worldPos.xyz;
    varNorm = aNorm;
}