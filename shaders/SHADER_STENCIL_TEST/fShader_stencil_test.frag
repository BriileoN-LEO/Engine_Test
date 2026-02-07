#version 460 core

uniform vec3 color_stencil;
out vec4 FragColor;

void main()
{
  FragColor = vec4(color_stencil, 1.0);

}