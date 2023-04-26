#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUv0;
layout (location = 3) in mat4 aModelMatrix;

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

out VsOut
{
    vec3 FragPos;
    vec3 Norm;
    vec2 Uv0;
} vsOut;

void main()
{
    vec4 worlPos = aModelMatrix * vec4(aPos, 1.0);
    gl_Position = uProjMatrix * uViewMatrix * worlPos;
    vsOut.FragPos = worlPos.xyz;
    // Inverse is a costly operation, it should not be used in shader.
    vsOut.Norm = transpose(inverse(mat3(aModelMatrix))) * aNorm;
    vsOut.Uv0 = aUv0;
}