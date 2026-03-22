#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

void main()
{

//gl_Position = projection_c * view_c * vec4(aPos, 1.0);
 gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
 texCoords = aTexCoords;


}