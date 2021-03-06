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
uniform sampler2D uTexture0;    // Diffuse.
uniform sampler2D uTexture1;    // Specular mask.
uniform float uSpecularity;
uniform float uShininess;

uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform PointLight uPointLight;
uniform DirectLight uDirectLight;
uniform SpotLight uSpotLight;

layout (std140) uniform ubCamera
{
                            // base alignment (size occupied)       // aligned offset (multiple of a base alignment)
    mat4 uViewProjMatrix;   // 4 * 4 = 16                               0   (col #1)
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

in vec3 varFragPos;
in vec3 varNorm;
in vec2 varUv0;

out vec4 FragColor;

vec3 CalculateDirLight(DirectLight light, vec3 norm, vec3 viewDir, vec4 specularityFilter);
vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir, vec4 specularityFilter);
vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec4 specularityFilter);

void main()
{
    vec4 outColor = texture(uTexture0, varUv0);
    vec4 specularityFilter = texture(uTexture1, varUv0);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.w * uAmbientColor.rgb;

    vec3 norm = normalize(varNorm);
    vec3 viewDir = normalize(uCamPos - varFragPos);

    vec3 lightColor = CalculateDirLight(uDirectLight, norm, viewDir, specularityFilter);
    lightColor += CalculatePointLight(uPointLight, norm, viewDir, specularityFilter);    
    lightColor += CalculateSpotLight(uSpotLight, norm, viewDir, specularityFilter);

    outColor.rgb *= (ambientColor + lightColor);

    FragColor = outColor;
}

vec3 CalculateDirLight(DirectLight light, vec3 norm, vec3 viewDir, vec4 specularityFilter)
{
    vec3 lightDir = normalize(-light.Dir);
    vec3 lightColor = light.Color.w * light.Color.rgb;

    // Calculate diffuse component. 
    float diffuseImpact = max(dot(norm, -lightDir), 0.f);
    vec3 diffuseColor = diffuseImpact * lightColor;

    // Calculate specular component.

    vec3 reflectDir = reflect(lightDir, norm);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    vec3 specularColor = ((light.Color.w * specularImpact) * uSpecularity) * light.Color.rgb * specularityFilter.rgb;

    return diffuseColor + specularColor;
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir, vec4 specularityFilter)
{
    vec3 lightDir = varFragPos - light.PositionAndRange.xyz;
    vec3 lightDirNorm = normalize(lightDir);
    float lightDistance = length(lightDir);
    float distanceAttenuation = pow(clamp(1.f - lightDistance / light.PositionAndRange.w, 0.f, 1.f), 2.f);

    vec3 lightColor = (distanceAttenuation * light.Color.w) * light.Color.rgb;

    // Calculate diffuse component. 
    float diffuseImpact = max(dot(norm, -lightDirNorm), 0.f);
    vec3 diffuseColor = diffuseImpact * lightColor;

    // Calculate specular component.

    vec3 reflectDir = reflect(lightDirNorm, norm);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    vec3 specularColor = (specularImpact * uSpecularity) * lightColor * specularityFilter.rgb;

    return diffuseColor + specularColor;
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec4 specularityFilter)
{
    vec3 rayDir = varFragPos - light.PositionAndRange.xyz;
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
    vec3 specularColor = (specularImpact * uSpecularity) * lightColor * specularityFilter.rgb;

    return diffuseColor + specularColor;
}