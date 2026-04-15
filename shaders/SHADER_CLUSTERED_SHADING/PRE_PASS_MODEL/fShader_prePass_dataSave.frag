#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gFragPosViewSpace_and_shiness;
layout (location = 3) out vec4 gDiffuse_and_specular;

in vec2 texCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 FragPosViewSpace;

struct material_maps
{
sampler2D texture_diffuse;
sampler2D texture_specular;
//sampler2D back_Texture;

bool blendTextureDiffuse; /////ESTE VALOR SE DEJA AL AIRE
bool use_texture_diffuse;
bool use_texture_specular;

};

uniform material_maps Mat_tex;

struct material_standard
{
vec3 ambient;
vec3 difusse;
float specular;
float shiness;

};

uniform material_standard Mat_SS;


void main()
{
  gPosition = FragPos;
  gNormal = normalize(Normal);
  gFragPosViewSpace = vec4(FragPosViewSpace.xyz, Mat_SS.shiness);

  if(Mat_tex.use_texture_diffuse == true)
  {
      gDiffuse_and_specular.rgb = texture(Mat_tex.texture_diffuse, texCoords).rgb;
  }

  else if(Mat_tex.use_texture_diffuse == false)
  {
      gDiffuse_and_specular.rgb = Mat_SS.difusse;
  }

  if(Mat_tex.use_texture_specular == true)
  {
      gDiffuse_and_specular.a = texture(Mat_tex.texture_specular, texCoords).r;
  }

  else if(Mat_tex.use_texture_specular == false)
  {
    gDiffuse_and_specular.a = Mat_SS.specular;
  }


}