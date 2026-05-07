#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gFragPosViewSpace_and_shiness;
layout (location = 3) out vec4 gDiffuse_and_specular;

in vec2 texCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 FragPosViewSpace;

out vec4 FragColor;

struct material_maps
{
sampler2D texture_diffuse;
sampler2D texture_specular;
//sampler2D back_Texture;

//bool blendTextureDiffuse; /////ESTE VALOR SE DEJA AL AIRE
bool use_texture_diffuse;
bool use_texture_specular;

};

uniform material_maps Mat_tex;

struct material_standard
{
//vec3 ambient;
vec3 difusse;
float specular;
float shiness;

};

uniform material_standard Mat_SS;


void main()
{
  gPosition = FragPos;
  gNormal = normalize(Normal);
  gFragPosViewSpace_and_shiness = vec4(FragPosViewSpace.xyz, Mat_SS.shiness);

 // vec3 diffuse = texture(Mat_tex.texture_diffuse, texCoords).rgb;
  //float specular = texture(Mat_tex.texture_specular, texCoords).r;

  if(Mat_tex.use_texture_diffuse == true)
  {
     //vec3 diffuse = texture(Mat_tex.texture_diffuse, texCoords).rgb;
      vec3 diffuse = texture(Mat_tex.texture_diffuse, texCoords).rgb;
      gDiffuse_and_specular.rgb = texture(Mat_tex.texture_diffuse, texCoords).rgb;  /////////////////////////
  }

  else
  {
     gDiffuse_and_specular.rgb = Mat_SS.difusse;
  }

  if(Mat_tex.use_texture_specular == true)
  {
     float specular = texture(Mat_tex.texture_specular, texCoords).r;
     gDiffuse_and_specular.a = specular; /////////////////////////
  }

  else
  {
     gDiffuse_and_specular.a = Mat_SS.specular;
  }


//FragColor = vec4(diffuse, 1.0);

}