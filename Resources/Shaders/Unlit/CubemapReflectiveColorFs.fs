#version 330 core

uniform samplerCube uTexture0;
uniform vec3 uCamPos;

in vec3 varFragPos;
in vec3 varNorm;

out vec4 FragColor;

void main()
{
    vec3 viewDir = normalize(varFragPos - uCamPos);
    vec3 norm = normalize(varNorm);
    vec3 sampleDir = reflect(viewDir, norm);

    FragColor = texture(uTexture0, sampleDir);
}