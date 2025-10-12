#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoordTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec3 prePos = -aNormal;
  vec3 newPos = aPos - (prePos * 0.1);
  
  gl_Position = projection * view * model * vec4(newPos, 1.0);

}