#version 330 core
#define NUM_POINT_LIGHTS 9
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_SPOT_LIGHTS 1

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 coordTexOut;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

float near = 0.001;
float far = 500.0;  

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


struct spot_Light
{
  vec3 lightPos; 
  vec3 lightDir;
  float cutOff;
  float outerCutOff;
     
  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  bool lightState;
  
};

uniform spot_Light spotLights_Array[NUM_SPOT_LIGHTS];


struct material_maps
{
sampler2D texture_diffuse;
sampler2D texture_specular;
float shiness;
};

uniform material_maps Mat_1;

struct material_standard
{
vec3 ambient;
vec3 difusse;
vec3 specular;
float shiness;

};

uniform material_standard Mat;
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

vec3 CalcDirLight(directional_Light dirLight_Cal, vec3 normal_Face, vec3 viewDir, bool diffuseExist, bool specExist)
{
   vec3 lightDir = normalize(dirLight_Cal.lightDir);
   
   float diff = max(dot(lightDir, normal_Face), 0.0);
   
   vec3 reflectDir = reflect(-lightDir, normal_Face);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  if(diffuseExist == true)
  {
  ambient = dirLight_Cal.ambient * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
  diffuse = dirLight_Cal.diffuse * diff * vec3(texture(Mat_1.texture_diffuse, coordTexOut));

  }

  else if(diffuseExist == false)
  {
  ambient = dirLight_Cal.ambient * Mat.ambient;
  diffuse = dirLight_Cal.diffuse * diff * Mat.difusse;

  }

  if(specExist == true)
  {
  specular = dirLight_Cal.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut));
  }
  else if(specExist == false)
  {
  specular = dirLight_Cal.specular * spec * Mat.specular;
  }

  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(point_Light pointLight, vec3 normal_Face, vec3 viewDir, bool diffuseExist, bool specExist)
{
  vec3 lightDir = normalize(pointLight.lightPos - FragPos);
  
  float diff = max(dot(lightDir, normal_Face), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal_Face);
  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
  
  float distance_attenuation = length(pointLight.lightPos - FragPos);
  float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance_attenuation + pointLight.quadratic * (distance_attenuation * distance_attenuation));

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  if(diffuseExist == true)
  {
  ambient = pointLight.ambient * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
  diffuse = pointLight.diffuse * diff * vec3(texture(Mat_1.texture_diffuse, coordTexOut));

  }

  else if(diffuseExist == false)
  {
  ambient = pointLight.ambient * Mat.ambient;
  diffuse = pointLight.diffuse * diff * Mat.difusse;

  }

  if(specExist == true)
  {
  specular = pointLight.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut));
  }
  else if(specExist == false)
  {
  specular = pointLight.specular * spec * Mat.specular;
  }

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(spot_Light spotLight, vec3 normal_Face, vec3 viewDir, bool diffuseExist, bool specExist)
{
  vec3 lightDir = normalize(spotLight.lightPos - FragPos);
  
  float theta = dot(lightDir, -spotLight.lightDir);
  
  vec3 FinalResult; 


  float diff = max(dot(lightDir, normal_Face), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal_Face);
  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
  
  float distance_attenuation = length(spotLight.lightPos - FragPos);
  float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance_attenuation + spotLight.quadratic * (distance_attenuation * distance_attenuation));

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

   ///Calculate smooth Edge
  float epsilon = spotLight.cutOff - spotLight.outerCutOff;
  float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
 

  if(diffuseExist == true)
  {
  ambient = spotLight.ambient * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
  diffuse = spotLight.diffuse * diff * vec3(texture(Mat_1.texture_diffuse, coordTexOut));

  }

  else if(diffuseExist == false)
  {
  ambient = spotLight.ambient * Mat.ambient;
  diffuse = spotLight.diffuse * diff * Mat.difusse;

  }

  if(specExist == true)
  {
  specular = spotLight.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut));
  }
  else if(specExist == false)
  {
  specular = spotLight.specular * spec * Mat.specular;
  }

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  diffuse *= intensity;
  specular *= intensity;

  FinalResult = (ambient + diffuse + specular);
  

  return FinalResult;
}

float linearDepth(float depth)
{
  float z = depth * 2.0 - 1.0;

  return (2.0 * near * far) / (far + near - z * (far - near));
}


//////////////
void main()
{

vec4 texDiff = texture(Mat_1.texture_diffuse, coordTexOut);


vec4 texSpec = texture(Mat_1.texture_specular, coordTexOut);


vec3 normal_Face = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);

bool diffExist = true;
bool specExist = true;


float depth = linearDepth(gl_FragCoord.z) / far;

if(texDiff.x == 0.0 && texDiff.y == 0.0 && texDiff.z == 0.0)
{
 diffExist = false;
}

if(texSpec.x == 0.0 && texSpec.y == 0.0 && texSpec.z == 0.0)
{
 specExist = false;
}

////PARA saber si existe una textura en texDiff y no descartarla por completo
if(diffExist == true && texDiff.a < 0.1)
{
 discard;

}


vec3 DL_1 = CalcDirLight(directionalLight_1, normal_Face, viewDir, diffExist, specExist);
vec3 result = vec3(0.0);

///Add point lights
for(int i = 0; i < NUM_POINT_LIGHTS; i++)
{

result += CalcPointLight(pointLights_Array[i], normal_Face, viewDir, diffExist, specExist);

}

for(int i = 0; i < NUM_SPOT_LIGHTS; i++)
{ 
 if(spotLights_Array[i].lightState == true)
 {
  result += CalcSpotLight(spotLights_Array[i], normal_Face, viewDir, diffExist, specExist);
 }

}
result += DL_1;

vec4 resultVec4 = vec4(result, 1.0) * (vec4(vec3(1.0), 1.0) - vec4(vec3(depth), 1.0)) - vec4(vec3(depth), 1.0);

if(diffExist == true)
{
 resultVec4.w = texDiff.w;

}

FragColor = resultVec4;

}







































































































































































































































































































































































































































































































































































