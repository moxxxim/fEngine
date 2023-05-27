#version 330 core

uniform sampler2D uScreenColorProcessed;

in vec2 varUv0;

out vec4 FragColor;

void main()
{
    //const float gamma = 2.2;
    vec3 hdrColor = texture(uScreenColorProcessed, varUv0).rgb;
  
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // gamma correction 
    //mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}