#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUv0;

uniform mat4 uModelMatrix;
uniform mat4 uViewProjMatrix;

out vec3 varFragPos;
out vec3 varNorm;
out vec2 varUv0;

void main()
{
    vec4 worlPos = uModelMatrix * vec4(aPos, 1.0);
    gl_Position = uViewProjMatrix * worlPos;
    varFragPos = worlPos.xyz;
    // Inverse is a costly operation, it should not be used in shader.
    varNorm = transpose(inverse(mat3(uModelMatrix))) * aNorm;
    varUv0 = aUv0;
}