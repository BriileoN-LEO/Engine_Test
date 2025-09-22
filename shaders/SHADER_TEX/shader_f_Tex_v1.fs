#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float rotTexture;

uniform vec2 scaleTexFaces;
uniform vec2 scaleTexContainer;

void main()
{
 //ragColor = vec4(ourColor.xyz, 1.0);
//agColor = texture(texture1, TexCoord);;


vec2 texCoord1 = TexCoord;
vec2 texCoord2 = TexCoord;

texCoord2.x = texCoord2.x * cos(rotTexture) - texCoord2.y * sin(rotTexture);
texCoord2.y = texCoord2.x * sin(rotTexture) + texCoord2.y * cos(rotTexture);

//para escalar la textura

vec2 texCoord_scale1 = TexCoord;

texCoord_scale1.x = texCoord_scale1.x * scaleTexContainer.x;
texCoord_scale1.y = texCoord_scale1.y * scaleTexContainer.y;

vec2 texCoord_scale2 = TexCoord;

texCoord_scale2.x = texCoord_scale2.x * scaleTexFaces.x;
texCoord_scale2.y = texCoord_scale2.y * scaleTexFaces.y;

FragColor = mix(texture(texture1, texCoord_scale1), texture(texture2, texCoord_scale2), 0.2) * vec4(ourColor.xyz, 1.0);
//agColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3);


}


