#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aCoordTex;
layout(location = 2) in float aLayer;

out vec2 coordTex;
out float layer;

uniform mat4 projection_UI;

void main()
{
gl_Position = projection_UI * vec4(aPos, 0.0, 1.0);
coordTex = aCoordTex;
layer = aLayer;

}