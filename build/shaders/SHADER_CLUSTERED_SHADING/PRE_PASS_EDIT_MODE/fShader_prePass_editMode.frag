#version 460 core

layout (location = 0) out vec3 gColorPoint;

uniform vec3 PointColor;

void main()
{
 gColorPoint = PointColor;
}