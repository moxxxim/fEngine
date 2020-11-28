#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uProjMat;

out vec2 uv;
out vec3 vColor;

void main()
{
    gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPosition, 1.0);
    uv = aUv;
    vColor = aColor;
}