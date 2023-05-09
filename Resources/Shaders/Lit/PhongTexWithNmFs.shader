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
uniform sampler2D uNormalMap0;

// Shadows.
uniform sampler2DArray uDirectShadowMap;
uniform mat4 uDirShadowLightViewProj[16];
uniform float uCascadeDistances[16];
uniform int uCascadesCount = 3;
uniform samplerCube uPointShadowMap;
uniform float uFarClipPlane;

uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform PointLight uPointLight;
uniform DirectLight uDirectLight;
uniform SpotLight uSpotLight;

layout (std140) uniform ubCamera
{
                            // base alignment (size occupied)       // aligned offset (multiple of a base alignment)
    mat4 uViewMatrix;       // 4 * 4 = 16                               0   (col #1)
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
    vec3 FragPos;
    //vec3 Norm;
    vec2 Uv0;
} fsIn;

out vec4 FragColor;

const float gamma = 2.2f;

float CalculateDirectShadowBias(int layer)
{
    return 0.001f;
}

float IsInShadow(float fragmentDepth, float shadowDepth, float bias)
{
    return step(fragmentDepth - bias, shadowDepth); // Gives 0.f if outside far clipping plane
}

int CalculateShadowMapLayer(vec3 fragPosWorld)
{
    int layer = -1;

    vec4 fragPosViewSpace = uViewMatrix * vec4(fragPosWorld, 1.0);
    float depthValue = abs(fragPosViewSpace.z);
    
    for (int i = 0; i < uCascadesCount; ++i)
    {
        if (depthValue < uCascadeDistances[i])
        {
            layer = i;
            break;
        }
    }

    if (layer == -1)
    {
        layer = uCascadesCount - 1;
    }

    return layer;
}

vec3 CalculateNdcFragPos01(vec4 fragPosLightSpace)
{
    vec3 ndcFragPos = fragPosLightSpace.xyz / fragPosLightSpace.w;
    return ndcFragPos * 0.5 + 0.5;
}

float CalculateDirectShadowMultiplierPcfPart(vec4 fragPosLightSpace, vec2 uvOffset, int layer)
{
    vec3 fragPos01 = CalculateNdcFragPos01(fragPosLightSpace);
    float shadowDepth = texture(uDirectShadowMap, vec3(fragPos01.xy + uvOffset, layer)).r;
    float fragmentDepth = fragPos01.z;

    float bias = CalculateDirectShadowBias(layer);
    float shadowMultiplier = IsInShadow(fragmentDepth, shadowDepth, bias);
    float afterClippingPlaneMultiplier = step(1.f, fragmentDepth); // 1.f if outside far clipping plane (as deisred)

    return clamp(shadowMultiplier + afterClippingPlaneMultiplier, 0.f, 1.f);
}

// 0.f - fragment is in shadow, 1.f - fragment not in shadow.
float CalculateDirectShadowMultiplier()
{
    float shadowMultiplier = 0.f;

    int shadowMapLayer = CalculateShadowMapLayer(fsIn.FragPos);
    vec4 fragPosLightSpace = uDirShadowLightViewProj[shadowMapLayer] * vec4(fsIn.FragPos, 1.0);
    vec2 texelSize = 1.0f / textureSize(uDirectShadowMap, 0).xy;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 uvOffset = vec2(x, y) * texelSize;
            shadowMultiplier += CalculateDirectShadowMultiplierPcfPart(fragPosLightSpace, uvOffset, shadowMapLayer);
        }    
    }

    return shadowMultiplier / 9.0;
}

// 0.f - fragment is in shadow, 1.f - fragment not in shadow.
float CalculatePointShadowMultiplier(PointLight light, vec3 fragPos)
{
    vec3 fragToLight = fragPos - light.PositionAndRange.xyz;
    float shadowDepth = texture(uPointShadowMap, fragToLight).r;

    // It is currently in linear range between [0,1]. Re-transform back to original value
    shadowDepth *= uFarClipPlane;

    float fragmentDepth = length(fragToLight);
    float bias = 0.05;
    return IsInShadow(fragmentDepth, shadowDepth, bias);
}

vec3 CalculateDirLight(DirectLight light, vec3 norm)
{
    vec3 lightDir = normalize(light.Dir);

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

vec3 DeduceNorm()
{
    //return normalize(fsIn.Norm);

    vec4 norm01 = texture(uNormalMap0, fsIn.Uv0);
    return normalize(norm01.xyz * 2 - 1);
}

void main()
{
    vec4 srgbColor = texture(uTexture0, fsIn.Uv0);
    vec4 linearColor = ToLinearColor(srgbColor);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.w * uAmbientColor.rgb;

    vec3 norm = DeduceNorm();

    float directShadowValue = CalculateDirectShadowMultiplier();
    float pointShadowValue = CalculatePointShadowMultiplier(uPointLight, fsIn.FragPos);

    vec3 lightColor = directShadowValue * CalculateDirLight(uDirectLight, norm);
    lightColor += pointShadowValue * CalculatePointLight(uPointLight, norm);    
    lightColor += CalculateSpotLight(uSpotLight, norm);

    linearColor.rgb *= (ambientColor + lightColor);

    FragColor = ToSrgbColor(linearColor);
}