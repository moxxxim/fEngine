#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat3 uCamRotation;
uniform mat4 uProjMatrix;

out vec3 varPos;

void main()
{
	gl_Position = uProjMatrix * (vec4(aPos, 1.0) * mat4(uCamRotation));
	varPos = aPos;
}