#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aCoordTex;


out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transformSR;
uniform mat4 tranformTranslate;
uniform mat4 tranformScale;

void main()
{

 if(transformSR == 0 && tranformTranslate == 0)
 {
gl_Position = vec4(aPos.xyz, 1.0);
 }

 else
 {
  vec4 newPos = vec4(aPos.xyz, 1.0f);
  
  if(tranformScale != 0)
  {
   newPos = tranformScale * newPos;

  }

  if(transformSR != 0)
  {
   newPos = transformSR * newPos;

  }

  if(tranformTranslate != 0)
  {
   newPos = tranformTranslate * newPos;

  }
 
    gl_Position = newPos;
 }

 ourColor = aColor;
 TexCoord = aCoordTex;

}

