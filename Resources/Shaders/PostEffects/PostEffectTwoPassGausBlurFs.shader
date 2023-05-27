
#version 330 core

in vec2 varUv0;

uniform sampler2D uScreenColorProcessed;
  
uniform bool uHorizontalGausBlurPass;
uniform float uBlurWeight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

out vec4 FragColor;

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(uScreenColorProcessed, 0); // gets size of single texel
    vec3 result = texture(uScreenColorProcessed, TexCoords).rgb * weight[0]; // current fragment's contribution
    if(uHorizontalGausBlurPass)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(uScreenColorProcessed, varUv0 + vec2(tex_offset.x * i, 0.0)).rgb * uBlurWeight[i];
            result += texture(uScreenColorProcessed, varUv0 - vec2(tex_offset.x * i, 0.0)).rgb * uBlurWeight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(uScreenColorProcessed, varUv0 + vec2(0.0, tex_offset.y * i)).rgb * uBlurWeight[i];
            result += texture(uScreenColorProcessed, varUv0 - vec2(0.0, tex_offset.y * i)).rgb * uBlurWeight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}