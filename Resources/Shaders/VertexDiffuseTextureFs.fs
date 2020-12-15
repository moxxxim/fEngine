#version 330 core

// Gouraud shading diffuse material.

uniform sampler2D uTexture0;

in vec3 varLight;
in vec2 varUv0;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(uTexture0, varUv0);

    FragColor = texColor * vec4(varLight, 1.0f);
}