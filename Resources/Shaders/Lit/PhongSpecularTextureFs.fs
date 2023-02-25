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
uniform sampler2D uShadowMap;
uniform float uSpecularity;
uniform float uShininess;

uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform PointLight uPointLight;
uniform DirectLight uDirectLight;
uniform SpotLight uSpotLight;
uniform bool uBlinn = true;

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

in VsOut
{
    vec4 FragPosLightSpace;
    vec3 FragPos;
    vec3 Norm;
    vec2 Uv0;
} fsIn;

out vec4 FragColor;

// 0.f - fragment is in shadow, 1.f - fragment not in shadow.
float CalculateShadowMultiplier(vec4 fragPosLightSpace);
vec3 CalculateDirLight(DirectLight light, vec3 norm, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir);

void main()
{
    vec4 outColor = texture(uTexture0, fsIn.Uv0);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.w * uAmbientColor.rgb;

    vec3 norm = normalize(fsIn.Norm);
    vec3 viewDir = normalize(uCamPos - fsIn.FragPos);

    float shadowValue = CalculateShadowMultiplier(fsIn.FragPosLightSpace);
    vec3 lightColor = shadowValue * CalculateDirLight(uDirectLight, norm, viewDir);
    lightColor += CalculatePointLight(uPointLight, norm, viewDir);    
    lightColor += CalculateSpotLight(uSpotLight, norm, viewDir);

    outColor.rgb *= (ambientColor + lightColor);

    FragColor = outColor;
}

float CalculateShadowMultiplier(vec4 fragPosLightSpace)
{
    const float bias = 0.00f;
    vec3 ndcFragPos = fragPosLightSpace.xyz / fragPosLightSpace.w;
    vec3 fragPos01 = ndcFragPos * 0.5 + 0.5;
    float shadowDepth = texture(uShadowMap, fragPos01.xy).r;
    float fragmentDepth = fragPos01.z;
    float shadowMultiplier = step(fragmentDepth - bias, shadowDepth); // Gives 0.f if outside far clipping plane
    float afterClippingPlaneMultiplier = step(1.f, fragmentDepth); // 1.f if outside far clipping plane (as deisred)
    float resultMultiplier = clamp(shadowMultiplier + afterClippingPlaneMultiplier, 0.f, 1.f);

    return resultMultiplier;
}

vec3 CalculateDirLight(DirectLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Dir);
    vec3 lightColor = light.Color.w * light.Color.rgb;

    // Calculate diffuse component. 
    float diffuseImpact = max(dot(norm, lightDir), 0.f);
    vec3 diffuseColor = diffuseImpact * lightColor;

    // Calculate specular component.

    float specularImpact = 0.f;
    // Blinn-phong.
    if(uBlinn)
    {
        vec3 halfway = normalize(lightDir + viewDir);
        specularImpact = pow(max(dot(halfway, norm), 0.f), uShininess);
    }
    // Phong.
    else
    {
        vec3 reflectDir = reflect(lightDir, norm);
        specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);
    }

    vec3 specularColor = (specularImpact * (light.Color.w * uSpecularity)) * light.Color.xyz;
    return diffuseColor + specularColor;
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = fsIn.FragPos - light.PositionAndRange.xyz;
    vec3 lightDirNorm = normalize(lightDir);
    float lightDistance = length(lightDir);
    float distanceAttenuation = pow(clamp(1.f - lightDistance / light.PositionAndRange.w, 0.f, 1.f), 2.f);

    vec3 lightColor = (distanceAttenuation * light.Color.w) * light.Color.rgb;

    // Calculate diffuse component. 
    float diffuseImpact = max(dot(norm, -lightDirNorm), 0.f);
    vec3 diffuseColor = lightColor * diffuseImpact;

    // Calculate specular component.

    float specularImpact = 0.f;
    // Blinn-phong.
    if(uBlinn)
    {
        vec3 halfway = normalize(viewDir - lightDirNorm);
        specularImpact = pow(max(dot(halfway, norm), 0.f), uShininess);    
    }
    // Phong.
    else
    {
        vec3 reflectDir = reflect(lightDirNorm, norm);
        specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);    
    }

    vec3 specularColor = (specularImpact * uSpecularity) * lightColor;
    return diffuseColor + specularColor;
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir)
{
    vec3 rayDir = fsIn.FragPos - light.PositionAndRange.xyz;
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

    float specularImpact = 0.f;
    // Blinn-phong.
    if(uBlinn)
    {
        vec3 halfway = normalize(viewDir - rayDirNorm);
        specularImpact = pow(max(dot(halfway, norm), 0.f), uShininess);
    }
    // Phong.
    else
    {
        vec3 reflectDir = reflect(rayDirNorm, norm);
        specularImpact = pow(max(dot(viewDir, reflectDir), 0.f), uShininess);   
    }

    vec3 specularColor = (specularImpact * uSpecularity) * lightColor;
    return diffuseColor + specularColor;
}