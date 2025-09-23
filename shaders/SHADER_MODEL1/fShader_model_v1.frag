#version 330 core

out vec4 FragColor;

in vec3 colorOut;
in vec2 coordTexOut;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec2 texTransform1;
uniform vec2 texTransform2;

void main()
{ 
 vec2 coordTex1 = coordTexOut;
 vec2 coordTex2 = coordTexOut;

 coordTex1 = coordTexOut * texTransform1;
 coordTex2 = coordTexOut * texTransform2;

//ragColor = mix(texture(texture1, coordTexOut), texture(texture2, coordTexOut), 0.2);
FragColor = texture(texture1, coordTexOut);
}