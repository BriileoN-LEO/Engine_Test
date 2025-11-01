#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoordTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 centroidTriangle;
uniform int selectionStencil;

void main()
{
  vec3 newPos;

  if(selectionStencil == 0)
  {
   vec3 prePos = -aNormal;
   newPos = aPos - (prePos * 0.01);
  }

  else if(selectionStencil == 1)
  {
   vec3 prePos = centroidTriangle - aPos;
   newPos = aPos - (prePos * 0.1);

  }


  gl_Position = projection * view * model * vec4(newPos, 1.0);
//  gl_Position = projection * view * vec4(newPos, 1.0);

}