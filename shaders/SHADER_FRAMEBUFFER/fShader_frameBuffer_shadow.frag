#version 460 core

in vec2 texCoords;
uniform sampler2D screenTexture;

out vec4 FragColor;

void depthBufferTest()
{
 vec4 depthValue = texture(screenTexture, texCoords);
 float dV = depthValue.r;
 FragColor = vec4(vec3(dV), 1.0);

}

void main()
{
 depthBufferTest();

}