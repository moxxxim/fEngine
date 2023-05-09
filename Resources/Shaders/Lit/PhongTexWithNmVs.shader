#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUv0;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (std140) uniform ubCamera
{
                            // base alignment (size occupied)       // aligned offset (multiple of a base alignment)
    mat4 uViewMatrix;       // 4 * 4 = 16                               0   (col #1)
                            // 4 * 4 = 16                               16  (col #2)
                            // 4 * 4 = 16                               32  (col #3)
                            // 4 * 4 = 16                               48  (col #4)

    mat4 uProjMatrix;       // 4 * 4 = 16                               64  (col #1)
                            // 4 * 4 = 16                               80  (col #2)
                            // 4 * 4 = 16                               96  (col #3)
                            // 4 * 4 = 16                               112 (col #4)

    mat3 uCamRotation;      // 4 * 4 = 16                               128 (col #1)
                            // 4 * 4 = 16                               144 (col #2)
                            // 4 * 4 = 16                               160 (col #3)
    vec3 uCamPos;           // 4 * 4 = 16                               176
    vec3 uCamDir;           // 4 * 4 = 16                               192
                            // Total            208
};

uniform mat4 uModelMatrix;

out VsOut
{
    mat3 TBN;
    vec3 FragPos;
    vec2 Uv0;
} vsOut;

void main()
{
    vec4 worlPos = uModelMatrix * vec4(aPos, 1.0);
    gl_Position = uProjMatrix * uViewMatrix * worlPos;
    vsOut.FragPos = worlPos.xyz;
    vsOut.Uv0 = aUv0;

    vec3 T = normalize(vec3(uModelMatrix * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(uModelMatrix * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(uModelMatrix * vec4(aNorm, 0.0)));
    vsOut.TBN = mat3(T, B, N);
}