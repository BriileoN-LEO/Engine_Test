#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoordTex;

layout (std140, binding = 0) uniform matrices_cam
{
 mat4 view_c;
 mat4 projection_c;
};

out VS_OUT
{
vec3 Normal_;
mat4 view;
mat4 projection;

} vs_out;


uniform mat4 model;

void main()
{
 // gl_Position =  projection * view * model * vec4(aPos.xyz, 1.0);
  gl_Position =  model * vec4(aPos.xyz, 1.0);
  mat3 normalMatrix = mat3(transpose(inverse(view_c * model)));
  vs_out.Normal_ = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
  vs_out.view = view_c;
  vs_out.projection = projection_c;

}
