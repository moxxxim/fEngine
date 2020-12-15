#version 330 core

// Gouraud shading diffuse material.

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUv0;

uniform mat4 uModelMatrix;
uniform mat4 uViewProjMatrix;

// Light.
uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform vec4 uPointLightColor; // xyz - color, w - intencity.
uniform vec4 uPointLightPositionAndRange; // xyz - world position, w - range.
uniform vec3 uCamPos;
uniform float uSpecularity;
uniform float uShininess;

out vec3 varLight;
out vec2 varUv0;

void main()
{
    vec4 worlPos = uModelMatrix * vec4(aPos, 1.0);
    gl_Position = uViewProjMatrix * worlPos;

    // Inverse is a costly operation, it should not be used in shader.
    vec3 normWorld = normalize(transpose(inverse(mat3(uModelMatrix))) * aNorm);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.xyz * uAmbientColor.w;

    // Calculate diffuse component. 
    vec3 lightDir = normalize(uPointLightPositionAndRange.xyz - worlPos.xyz);
    float diffuseImpact = max(dot(normWorld, lightDir), 0.f);
    vec3 diffuseColor = uPointLightColor.xyz * (uPointLightColor.w * diffuseImpact);

    // Calculate specular component.

    vec3 viewDir = normalize(uCamPos - worlPos.xyz);
    vec3 reflectDir = reflect(-lightDir, normWorld);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    vec3 specularColor = ((uPointLightColor.w * specularImpact) * uSpecularity) * uPointLightColor.xyz;

    varLight = diffuseColor + ambientColor + specularColor;
    varUv0 = aUv0;
}