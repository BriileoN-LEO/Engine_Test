#version 460 core

layout (triangles, invocations = 4) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 lightSpaceMatrices[16];
uniform int size_LSM;

void main()
{
    if(gl_InvocationID < size_LSM)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
            gl_Layer = gl_InvocationID;
            EmitVertex();
        }

        EndPrimitive();
    }

    else
    {
       return;
    }
}