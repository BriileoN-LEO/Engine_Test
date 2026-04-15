#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoordTex;

out vec2 texCoords;
out vec3 FragPos;
out vec3 Normal;
out vec3 FragPosViewSpace;

layout (std140, binding = 0) uniform matrices_cam
{
 mat4 view_c;
 mat4 projection_c;
};

uniform mat4 model;


void main()
{
  gl_Position = projection_c * view_c * model * vec4(aPos.xyz, 1.0);

  FragPos = vec3(model * vec4(aPos, 1.0));

  FragPosViewSpace = vec3(view_c * model * vec4(aPos.xyz, 1.0));

  mat3 normalMatrix = mat3(transpose(inverse(model)));
  Normal = normalize(normalMatrix * aNormal);

  texCoords = aCoordTex;
}