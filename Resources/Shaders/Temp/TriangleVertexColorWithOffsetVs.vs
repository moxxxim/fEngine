#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;
uniform float uOffset;

void main()
{
    gl_Position = vec4(aPosition.x + uOffset, aPosition.y, aPosition.z, 1.0);
    vertexColor = aColor;
}