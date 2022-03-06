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

uniform vec4 uAmbientColor; // xyz - color, w - intencity.
uniform PointLight uPointLight;
uniform DirectLight uDirectLight;
uniform SpotLight uSpotLight;

in VsOut
{   
    vec3 FragPos;
    vec3 Norm;
    vec2 Uv0;
} vsOut;

out vec4 FragColor;

vec3 CalculateDirLight(DirectLight light, vec3 norm);
vec3 CalculatePointLight(PointLight light, vec3 norm);
vec3 CalculateSpotLight(SpotLight light, vec3 norm);

void main()
{
    const float gamma = 2.2f;
    vec4 srgbColor = texture(uTexture0, vsOut.Uv0);
    vec4 linearColor = vec4(pow(srgbColor.rgb, vec3(gamma)), srgbColor.a);

    // Calculate ambient component.
    vec3 ambientColor = uAmbientColor.w * uAmbientColor.rgb;

    vec3 norm = normalize(vsOut.Norm);

    vec3 lightColor = CalculateDirLight(uDirectLight, norm);
    lightColor += CalculatePointLight(uPointLight, norm);    
    lightColor += CalculateSpotLight(uSpotLight, norm);

    linearColor.rgb *= (ambientColor + lightColor);

    FragColor = vec4(pow(linearColor.rgb, vec3(1.f / gamma)), linearColor.a);
}

vec3 CalculateDirLight(DirectLight light, vec3 norm)
{
    vec3 lightDir = normalize(-light.Dir);

    float diffuseImpact = max(dot(norm, -lightDir), 0.f);
    return (diffuseImpact * light.Color.w) * light.Color.rgb;;
}

vec3 CalculatePointLight(PointLight light, vec3 norm)
{
    vec3 lightDir = vsOut.FragPos - light.PositionAndRange.xyz;
    vec3 lightDirNorm = normalize(lightDir);
    float lightDistance = length(lightDir);
    float distanceAttenuation = pow(clamp(1.f - lightDistance / light.PositionAndRange.w, 0.f, 1.f), 2.f);

    float diffuseImpact = max(dot(norm, -lightDirNorm), 0.f);
    return (diffuseImpact * distanceAttenuation * light.Color.w) * light.Color.rgb;
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm)
{
    vec3 rayDir = vsOut.FragPos - light.PositionAndRange.xyz;
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