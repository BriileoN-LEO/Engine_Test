#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 Normal_;
    vec3 FragPos_;
    vec2 coordTexOut_;

} gs_in[];

out vec3 Normal;
out vec3 FragPos;
out vec2 coordTexOut;

vec3 getNormal()
{
    vec3 pos_01 = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    vec3 pos_02 = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
    return normalize(cross(pos_01, pos_02));
}

vec4 explodePos(vec4 posVert, vec3 normals)
{
    vec3 explode_pos = posVert.xyz - (normals * 0.01);
    return vec4(explode_pos, posVert.w);
}

void main() {

    vec3 normal_View = getNormal();
    for(int i = 0; i < gl_in.length(); i++) {
      // gl_Position = gl_in[i].gl_Position;
         gl_Position = explodePos(gl_in[i].gl_Position, normal_View);

        Normal = gs_in[i].Normal_;
        FragPos = gs_in[i].FragPos_;
        coordTexOut = gs_in[i].coordTexOut_;
        EmitVertex();
    }
    EndPrimitive();
}