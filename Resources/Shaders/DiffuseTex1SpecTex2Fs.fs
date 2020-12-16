#version 330 core

// Phong shading specular material.

uniform sampler2D uTexture0;    // Diffuse.
uniform sampler2D uTexture1;    // Specular mask.
uniform float uSpecularity;
uniform float uShininess;

uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform vec4 uPointLightColor; // xyz - color, w - intencity.
uniform vec4 uPointLightPositionAndRange; // xyz - world position, w - range.

uniform vec3 uCamPos;

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
    vec3 diffuseColor = (uPointLightColor.w * diffuseImpact) * uPointLightColor.xyz;

    // Calculate specular component.

    vec4 specularityFilter = texture(uTexture1, varUv0);
    vec3 viewDir = normalize(uCamPos - varFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    vec3 specularColor = ((uPointLightColor.w * specularImpact) * uSpecularity) * uPointLightColor.xyz * specularityFilter.xyz;

    outColor.xyz = outColor.xyz * (diffuseColor + ambientColor + specularColor);

    FragColor = outColor;
}