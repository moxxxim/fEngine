#version 410 core
    
// 'invocations' equals cascades count
layout(triangles, invocations = 3) in;
layout(triangle_strip, max_vertices = 3) out;
    
uniform mat4 uDirShadowLightViewProj[16];

void main()
{          
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = uDirShadowLightViewProj[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
} 