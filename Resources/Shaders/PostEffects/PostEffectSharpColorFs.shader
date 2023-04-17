#version 330 core

uniform sampler2D uScreenColorProcessed;

in vec2 varUv0;

out vec4 FragColor;

void main()
{
    float kernel[9] = float[](
        -1.f, -1.f, -1.f,
        -1.f,  9.f, -1.f,
        -1.f, -1.f, -1.f
    );

    vec3 samples[9];
    samples[0] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(-1, 1)));
    samples[1] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(0, 1)));
    samples[2] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(1, 1)));
    samples[3] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(-1, 0)));
    samples[4] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(0, 0)));
    samples[5] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(1, 0)));
    samples[6] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(-1, -1)));
    samples[7] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(0, -1)));
    samples[8] = vec3(textureOffset(uScreenColorProcessed, varUv0, ivec2(1, -1)));

    vec3 color = vec3(0.f);
    for(int i = 0; i < 9; ++i)
    {
        color += samples[i] * kernel[i];
    }

    FragColor = vec4(color, 1.0);
}