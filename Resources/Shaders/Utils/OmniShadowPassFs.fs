#version 330 core

struct PointLight
{
    vec4 Color; // xyz - color, w - intencity.
    vec4 PositionAndRange; // xyz - world position, w - range.
};

in vec4 FragPos;

uniform PointLight uPointLight;
uniform float uFarClipPlane;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - uPointLight.PositionAndRange.xyz);
    
    // map to [0;1] range by dividing by far clip plane (near equals 1); 
    gl_FragDepth = lightDistance / uFarClipPlane;
}  