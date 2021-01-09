#version 330 core

// Phong shading specular material.

struct DirectLight
{
    vec4 Color; // xyz - color, w - intencity.
    vec3 Dir;
};

struct PointLight
{
    vec4 Color; // xyz - color, w - intencity.
    vec4 PositionAndRange; // xyz - world position, w - range.
};

struct SpotLight
{
    vec4 Color; // xyz - color, w - intencity.
    vec4 PositionAndRange; // xyz - world position, w - range.
    vec4 DirAndAngle; // xyz - direction, w - cone angle.
};

// Material.
uniform sampler2D uTexture0;
uniform float uSpecularity;
uniform float uShininess;

uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform PointLight uPointLight;
uniform DirectLight uDirectLight;
uniform SpotLight uSpotLight;

uniform vec3 uCamPos;

in VsOut
{
    vec3 FragPos;
    vec3 Norm;
    vec2 Uv0;
} vsOut;

out vec4 FragColor;

vec3 CalculateDirLight(DirectLight light, vec3 norm, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir);

void main()
{
    vec4 outColor = texture(uTexture0, vsOut.Uv0);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.w * uAmbientColor.rgb;

    vec3 norm = normalize(vsOut.Norm);
    vec3 viewDir = normalize(uCamPos - vsOut.FragPos);

    vec3 lightColor = CalculateDirLight(uDirectLight, norm, viewDir);
    lightColor += CalculatePointLight(uPointLight, norm, viewDir);    
    lightColor += CalculateSpotLight(uSpotLight, norm, viewDir);

    outColor.rgb *= (ambientColor + lightColor);

    FragColor = outColor;
}

vec3 CalculateDirLight(DirectLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Dir);
    vec3 lightColor = light.Color.w * light.Color.rgb;

    // Calculate diffuse component. 
    float diffuseImpact = max(dot(norm, -lightDir), 0.f);
    vec3 diffuseColor = diffuseImpact * lightColor;

    // Calculate specular component.

    vec3 reflectDir = reflect(lightDir, norm);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    vec3 specularColor = ((light.Color.w * specularImpact) * uSpecularity) * light.Color.xyz;

    return diffuseColor + specularColor;
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = vsOut.FragPos - light.PositionAndRange.xyz;
    vec3 lightDirNorm = normalize(lightDir);
    float lightDistance = length(lightDir);
    float distanceAttenuation = pow(clamp(1.f - lightDistance / light.PositionAndRange.w, 0.f, 1.f), 2.f);

    vec3 lightColor = (distanceAttenuation * light.Color.w) * light.Color.rgb;

    // Calculate diffuse component. 
    float diffuseImpact = max(dot(norm, -lightDirNorm), 0.f);
    vec3 diffuseColor = lightColor * diffuseImpact;

    // Calculate specular component.

    vec3 reflectDir = reflect(lightDirNorm, norm);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    vec3 specularColor = (specularImpact * uSpecularity) * lightColor;

    return diffuseColor + specularColor;
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir)
{
    vec3 rayDir = vsOut.FragPos - light.PositionAndRange.xyz;
    vec3 rayDirNorm = normalize(rayDir);
    float lightDistance = length(rayDir);

    float angleCosine = cos(light.DirAndAngle.w);
    float rayOffset = abs(dot(rayDirNorm, normalize(light.DirAndAngle.xyz)));
    float angleAttenuation = clamp((rayOffset - angleCosine) / (1.f - angleCosine), 0.f, 1.f);
    float distanceAttenuation = pow(clamp(1.f - lightDistance / light.PositionAndRange.w, 0.f, 1.f), 2.f);

    vec3 lightColor = (angleAttenuation * distanceAttenuation * light.Color.w) * light.Color.rgb;

    // Calculate diffuse component. 

    float diffuseImpact = max(dot(norm, -rayDirNorm), 0.f);
    vec3 diffuseColor = lightColor * diffuseImpact;

    // Calculate specular component.

    vec3 reflectDir = reflect(rayDirNorm, norm);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    vec3 specularColor = (specularImpact * uSpecularity) * lightColor;

    return diffuseColor + specularColor;
}