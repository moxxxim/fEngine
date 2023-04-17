#version 330 core

out vec4 FragColor;

uniform float uNearClipPlane;
uniform float uFarClipPlane;

float LinearizeDepth(float depth, float near, float far) 
{
    float ndc = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - ndc * (far - near));	
}

void main()
{
    float linearDepth = LinearizeDepth(gl_FragCoord.z, uNearClipPlane, uFarClipPlane);
    FragColor = vec4(vec3(linearDepth / uFarClipPlane), 1.f);
}