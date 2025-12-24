#version 330 core

out vec4 FragColor;

in vec3 colorOut;
in vec2 coordTexOut;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform mat4 texTransform1;
uniform mat4 texTransform2;

void main()
{ 
 vec2 coordTex1 = coordTexOut;
 vec2 coordTex2 = coordTexOut;

 coordTex1 = texTransform1 * coordTexOut;
 coordTex2 = texTransform2 * coordTexOut;

 FragColor = mix(texture(texture1, coordTex1), texture(texture2, coordTex2), 0.2);
}