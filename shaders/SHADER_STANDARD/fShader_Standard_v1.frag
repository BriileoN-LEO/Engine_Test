#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 coordTexOut;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

struct directional_Light
{
 vec3 lightDir;
 vec3 lightColor;
};

uniform directional_Light directionalLight_1;

struct point_Light
{
  vec3 lightPos;
  vec3 lightColor;

};

uniform point_Light pointLight_1;

struct material_standard
{
vec3 ambient;
vec3 difusse;
vec3 specular;
float shiness;
};

uniform material_standard Mat;

/*
struct material_maps
{
sampler2D texture_diffuse;
sampler2D texture_specular;
float shiness;
};
*/
//uniform material_maps Mat_1;

void main()
{

 //AMBIENT LIGHT 
 vec3 ambientLight = (vec3(0.8) * Mat.ambient) * lightColor;
 
 //DIFUSSE LIGHT
 vec3 normal_Face = normalize(Normal);
 vec3 lightDir = normalize(lightPos - FragPos);
 
 float diff = max(dot(lightDir, normal_Face), 0.0);
 vec3 difusseLight = (diff * Mat.difusse) * lightColor;

///SPECULAR LIGHT 

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, normal_Face);

float specular = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
vec3 specularLight = (Mat.specular * specular) * lightColor;

vec3 result = (ambientLight + difusseLight + specularLight) * objectColor;

FragColor = vec4(result, 1.0);

}

