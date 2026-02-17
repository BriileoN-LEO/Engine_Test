#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

//layout (std140) uniform matrices_cam
//{
// mat4 view_c;
/// mat4 projection_c;
//};

in VS_OUT {
    vec3 Normal_;
    mat4 view;
    mat4 projection;
} gs_in[];

uniform bool active_exploded;
uniform float dist_exploded;

const float magnitud = 0.4;

vec3 getNormal()
{
    vec3 pos_01 = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    vec3 pos_02 = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
    return normalize(cross(pos_01, pos_02));
}

vec4 explodePos(vec4 posVert, vec3 normals)
{
    vec3 explode_pos = posVert.xyz - (normals * dist_exploded);
    return vec4(explode_pos, posVert.w);
}

void generateNormal(int index, vec4 currentPos)
{
 gl_Position = gs_in[index].projection * gs_in[index].view * currentPos;
 EmitVertex();

 gl_Position =  gs_in[index].projection * (( gs_in[index].view * currentPos) + (vec4(gs_in[index].Normal_, 0.0) * magnitud));
 EmitVertex();

 EndPrimitive();
}

void main() {

    vec3 normal_View = getNormal();
    for(int i = 0; i < gl_in.length(); i++) {

        vec4 currentPos;
        if(active_exploded == false)
        {
            currentPos = gl_in[i].gl_Position;
        }

        else if(active_exploded == true)
        {
            currentPos = explodePos(gl_in[i].gl_Position, normal_View);
        }

         generateNormal(i, currentPos);
    }
}