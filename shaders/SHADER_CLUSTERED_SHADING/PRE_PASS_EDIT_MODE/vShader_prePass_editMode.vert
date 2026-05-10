#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

layout (std140, binding = 0) uniform matrices_cam
{
 mat4 view_c;
 mat4 projection_c;
};

uniform float sizePointer;

void main()
{
gl_Position = projection_c * view_c * model * vec4(aPos, 1.0);
//gl_Position = projection * view * vec4(aPos, 1.0);
gl_PointSize = sizePointer;

}