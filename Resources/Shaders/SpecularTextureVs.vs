#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUv0;

uniform mat4 uModelMatrix;
uniform mat4 uViewProjMatrix;

out VsOut
{
    vec3 FragPos;
    vec3 Norm;
    vec2 Uv0;
} vsOut;

void main()
{
    vec4 worlPos = uModelMatrix * vec4(aPos, 1.0);
    gl_Position = uViewProjMatrix * worlPos;
    vsOut.FragPos = worlPos.xyz;
    // Inverse is a costly operation, it should not be used in shader.
    vsOut.Norm = transpose(inverse(mat3(uModelMatrix))) * aNorm;
    vsOut.Uv0 = aUv0;
}