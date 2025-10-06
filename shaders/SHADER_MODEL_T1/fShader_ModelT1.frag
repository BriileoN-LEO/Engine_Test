#version 330 core
#define NUM_POINT_LIGHTS 9
#define NUM_DIRECTIONAL_LIGHTS 1

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

 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 bool lightState;

};

uniform directional_Light directionalLight_1;

struct point_Light
{
  vec3 lightPos;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  bool lightState;
};

uniform point_Light pointLight_1;
uniform point_Light pointLights_Array[NUM_POINT_LIGHTS];


struct material_maps
{
sampler2D texture_diffuse;
sampler2D texture_specular;
float shiness;
};

uniform material_maps Mat_1;
/*
vec3 calculateDirLight(directional_Light dirLight_Cal)
{
  //AMBIENT LIGHT 
 vec3 ambientLight = (vec3(0.8) * Mat.ambient) * dirLight_Cal.lightColor;
 
 //DIFUSSE LIGHT
 vec3 normal_Face = normalize(Normal);
 vec3 lightDir = normalize(dirLight_Cal.lightDir);
 
 float diff = max(dot(lightDir, normal_Face), 0.0);
 vec3 difusseLight = (diff * Mat.difusse) * dirLight_Cal.lightColor;

///SPECULAR LIGHT 

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, normal_Face);

float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
vec3 specularLight = (Mat.specular * spec) * dirLight_Cal.lightColor;

vec3 ambient = ambientLight * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
vec3 difusse = difusseLight * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
vec3 specular = specularLight * vec3(texture(Mat_1.texture_specular, coordTexOut));

vec3 result = (ambient + difusse + specular) * objectColor;

return result;

}
*/

vec3 CalcDirLight(directional_Light dirLight_Cal, vec3 normal_Face, vec3 viewDir)
{
   vec3 lightDir = normalize(dirLight_Cal.lightDir);
   
   float diff = max(dot(lightDir, normal_Face), 0.0);
   
   vec3 reflectDir = reflect(-lightDir, normal_Face);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat_1.shiness);

   vec3 ambient = dirLight_Cal.ambient * vec3(texture(Mat_1.texture_diffuse, coordTexOut)) ;
   vec3 difusse = dirLight_Cal.diffuse * diff * vec3(texture(Mat_1.texture_diffuse, coordTexOut)) ;
   vec3 specular = dirLight_Cal.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut)) ;  

  return (ambient + difusse + specular);
}

vec3 CalcPointLight(point_Light pointLight, vec3 normal_Face, vec3 viewDir)
{
  vec3 lightDir = normalize(pointLight.lightPos - FragPos);
  
  float diff = max(dot(lightDir, normal_Face), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal_Face);
  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat_1.shiness);
  
  float distance_attenuation = length(pointLight.lightPos - FragPos);
  float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance_attenuation + pointLight.quadratic * (distance_attenuation * distance_attenuation));

  vec3 ambient = pointLight.ambient * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
  vec3 difusse = pointLight.diffuse * diff * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
  vec3 specular = pointLight.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut));
 
  ambient *= attenuation;
  difusse *= attenuation;
  specular *= attenuation;

  return (ambient + difusse + specular);
}




void main()
{

vec3 normal_Face = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);

vec3 DL_1 = CalcDirLight(directionalLight_1, normal_Face, viewDir);
vec3 result = vec3(0.0);

///Add point lights
for(int i = 0; i < NUM_POINT_LIGHTS; i++)
{

result += CalcPointLight(pointLights_Array[i], normal_Face, viewDir);

}
result += DL_1;

//result *= 0.1; 

FragColor = vec4(result, 1.0);

}

























































