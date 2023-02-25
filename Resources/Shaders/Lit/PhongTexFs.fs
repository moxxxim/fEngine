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

uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform PointLight uPointLight;
uniform DirectLight uDirectLight;
uniform SpotLight uSpotLight;

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
vec3 CalculateDirLight(DirectLight light, vec3 norm);
vec3 CalculatePointLight(PointLight light, vec3 norm);
vec3 CalculateSpotLight(SpotLight light, vec3 norm);
vec4 ToLinearColor(vec4 srgbColor);
vec4 ToSrgbColor(vec4 linearColor);

const float gamma = 2.2f;

void main()
{
    vec4 srgbColor = texture(uTexture0, fsIn.Uv0);
    vec4 linearColor = ToLinearColor(srgbColor);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.w * uAmbientColor.rgb;

    vec3 norm = normalize(fsIn.Norm);

    float shadowValue = CalculateShadowMultiplier(fsIn.FragPosLightSpace);
    vec3 lightColor = shadowValue * CalculateDirLight(uDirectLight, norm);
    lightColor += CalculatePointLight(uPointLight, norm);    
    lightColor += CalculateSpotLight(uSpotLight, norm);

    linearColor.rgb *= (ambientColor + lightColor);

    FragColor = ToSrgbColor(linearColor);
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

vec3 CalculateDirLight(DirectLight light, vec3 norm)
{
    vec3 lightDir = normalize(-light.Dir);

    float diffuseImpact = max(dot(norm, -lightDir), 0.f);
    return (diffuseImpact * light.Color.w) * light.Color.rgb;;
}

vec3 CalculatePointLight(PointLight light, vec3 norm)
{
    vec3 lightDir = fsIn.FragPos - light.PositionAndRange.xyz;
    vec3 lightDirNorm = normalize(lightDir);
    float lightDistance = length(lightDir);
    float distanceAttenuation = pow(clamp(1.f - lightDistance / light.PositionAndRange.w, 0.f, 1.f), 2.f);

    float diffuseImpact = max(dot(norm, -lightDirNorm), 0.f);
    return (diffuseImpact * distanceAttenuation * light.Color.w) * light.Color.rgb;
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm)
{
    vec3 rayDir = fsIn.FragPos - light.PositionAndRange.xyz;
    vec3 rayDirNorm = normalize(rayDir);
    float lightDistance = length(rayDir);

    float angleCosine = cos(light.DirAndAngle.w);
    float rayOffset = abs(dot(rayDirNorm, normalize(light.DirAndAngle.xyz)));
    float angleAttenuation = clamp((rayOffset - angleCosine) / (1.f - angleCosine), 0.f, 1.f);
    float distanceAttenuation = pow(clamp(1.f - lightDistance / light.PositionAndRange.w, 0.f, 1.f), 2.f);

    vec3 lightColor = (angleAttenuation * distanceAttenuation * light.Color.w) * light.Color.rgb;
    float diffuseImpact = max(dot(norm, -rayDirNorm), 0.f);
    
    return lightColor * diffuseImpact;
}

vec4 ToLinearColor(vec4 srgbColor)
{
    return vec4(pow(srgbColor.rgb, vec3(gamma)), srgbColor.a);
}

vec4 ToSrgbColor(vec4 linearColor)
{
    return vec4(pow(linearColor.rgb, vec3(1.f / gamma)), linearColor.a);
}