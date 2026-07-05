#version 460 core

out vec4 FragColor;

uniform vec3 PointColor;

void main()
{
 FragColor = vec4(PointColor, 1.0);

}