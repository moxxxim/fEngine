#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv0;

out vec2 varUv0;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	varUv0 = aUv0;
}