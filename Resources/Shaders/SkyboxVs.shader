#version 330 core

layout (location = 0) in vec3 aPos;

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

out vec3 varPos;

void main()
{
    varPos = aPos;
	vec4 pos = uProjMatrix * (vec4(aPos, 1.0) * mat4(uCamRotation));

    // By setting z-coordinate equal to 'w' we ensure,
    // that after perspecitve division ( (x, y, z) / w) the depth of resulting fragment will be 1 (maximal value).
    // Thus we ensure, that the skybox will not be rendered on top of the whole scene 
    // and will not affect depth testing for objects renderd afterwards.
    gl_Position = pos.xyww;
}