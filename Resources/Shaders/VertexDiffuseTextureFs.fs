#version 330 core

// Phong shading diffuse material.

uniform sampler2D uTexture0;
uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform vec4 uPointLightColor; // xyz - color, w - intencity.
uniform vec4 uPointLightPositionAndRange; // xyz - world position, w - range.

in vec3 varFragPos;
in vec3 varNorm;
in vec2 varUv0;

out vec4 FragColor;

void main()
{
    vec4 outColor = texture(uTexture0, varUv0);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.xyz * uAmbientColor.w;

    // Calculate diffuse component. 
    vec3 norm = normalize(varNorm);
    vec3 lightDir = normalize(uPointLightPositionAndRange.xyz - varFragPos);
    float diffuseImpact = max(dot(norm, lightDir), 0.f);
    vec3 diffuseColor = uPointLightColor.xyz * (uPointLightColor.w * diffuseImpact);

    outColor.xyz *= (diffuseColor + ambientColor);

    FragColor = outColor;
}