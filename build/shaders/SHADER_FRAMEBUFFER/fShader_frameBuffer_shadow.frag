#version 460 core

in vec2 texCoords;
uniform sampler2DArray screenTexture;
uniform int layerTexture;
uniform int intensity;

out vec4 FragColor;

void depthBufferTest()
{
 vec4 depthValue = texture(screenTexture, vec3(texCoords, layerTexture));
 float dV = depthValue.r;
 FragColor = vec4(vec3(dV - (pow(dV, intensity) * 0.5)), 1.0);
 //FragColor = vec4(vec3(dV), 1.0);

}

void main()
{
 depthBufferTest();

}