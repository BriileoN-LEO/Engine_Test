#version 330 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;



void grayScale_Filter()
{
  FragColor = texture(screenTexture, texCoords);

  float grayscaleIntensity = (FragColor.x * 0.216)+ (FragColor.y * 0.751) + (FragColor.z * 0.710); 

  FragColor = vec4(grayscaleIntensity, grayscaleIntensity, grayscaleIntensity, 1.0);
}

void firstKernelShader()
{
 const float offset = 1.0 / 300.0;

  vec2 offsets[9] = vec2[](
  vec2(-offset, offset),
  vec2( 0.0, offset),
  vec2(offset, offset),
  vec2(-offset, 0.0),
  vec2(0.0, 0.0),
  vec2(offset, 0.0),
  vec2(-offset, -offset),
  vec2(0.0, -offset),
  vec2(offset, -offset)
  );

  ////KERNEL DRUGS
  float kernel_DRUGS[9] = float[](
  -1, -1, -1,
  -1, 9, -1,
  -1, -1, -1
  );

  float kernel_BLUR[9] = float[](
  1.0 / 16, 2.0 / 16, 1.0 / 16,
  2.0 / 16, 4.0 / 16, 2.0 / 16,
  1.0 / 16, 2.0 / 16, 1.0 / 16
  );

  vec3 sampleTex[9];

  for(int i = 0; i < 9; i++)
  {
   sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
  }
  
  vec3 col = vec3(0.0);

  for(int i = 0; i < 9; i++)
  {
   col += sampleTex[i] * kernel_BLUR[i];

  }

  FragColor = vec4(col, 1.0);

}

void normalFilter()
{
 vec3 normalFilt = texture(screenTexture, texCoords).rgb;
 FragColor = vec4(normalFilt, 1.0);
}


void main()
{


 // FragColor = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);
 normalFilter();

}