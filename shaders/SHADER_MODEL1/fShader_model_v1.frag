#version 330 core
#define NUM_POINT_LIGHTS 9
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_SPOT_LIGHTS 1

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 coordTexOut;
//in vec3 resultBasicLight;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec2 texTransform1;
uniform vec2 texTransform2;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float constant;
uniform float linear;
uniform float quadratic;

float near = 0.0001;
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


struct material_standard
{
vec3 ambient;
vec3 difusse;
vec3 specular;
float shiness;
};

uniform material_standard Mat;


vec3 CalcDirLight(directional_Light dirLight_Cal, vec3 normal_Face, vec3 viewDir)
{
   vec3 lightDir = normalize(dirLight_Cal.lightDir);
   
   float diff = max(dot(lightDir, normal_Face), 0.0);
   
   vec3 reflectDir = reflect(-lightDir, normal_Face);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);

   vec3 ambient = dirLight_Cal.ambient * vec3(texture(texture1, coordTexOut));
   vec3 difusse = dirLight_Cal.diffuse * diff * vec3(texture(texture1, coordTexOut)) ;
   vec3 specular = dirLight_Cal.specular * spec ;  

  return (ambient + difusse + specular);
}


vec3 CalcPointLight(point_Light pointLight, vec3 normal_Face, vec3 viewDir)
{
  vec3 lightDir = normalize(pointLight.lightPos - FragPos);
  
  float diff = max(dot(lightDir, normal_Face), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal_Face);
  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
  
  float distance_attenuation = length(pointLight.lightPos - FragPos);
  float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance_attenuation + pointLight.quadratic * (distance_attenuation * distance_attenuation));

  vec3 ambient = pointLight.ambient * vec3(texture(texture1, coordTexOut));
  vec3 difusse = pointLight.diffuse * diff * vec3(texture(texture1, coordTexOut));
  vec3 specular = pointLight.specular * spec;
 

  ambient *= attenuation;
  difusse *= attenuation;
  specular *= attenuation;

  return (ambient + difusse + specular);
}

vec3 CalcSpotLight(spot_Light spotLight, vec3 normal_Face, vec3 viewDir)
{
  vec3 lightDir = normalize(spotLight.lightPos - FragPos);
  
  float theta = dot(lightDir, -spotLight.lightDir);

  vec3 FinalResult;


  float diff = max(dot(lightDir, normal_Face), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal_Face);
  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
  
  float distance_attenuation = length(spotLight.lightPos - FragPos);
  float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance_attenuation + spotLight.quadratic * (distance_attenuation * distance_attenuation));

  vec3 ambient = spotLight.ambient * vec3(texture(texture1, coordTexOut));
  vec3 difusse = spotLight.diffuse * diff * vec3(texture(texture1, coordTexOut));
  vec3 specular = spotLight.specular * spec;

  ambient *= attenuation;
  difusse *= attenuation;
  specular *= attenuation;

     ///Calculate smooth Edge
  float epsilon = spotLight.cutOff - spotLight.outerCutOff;
  float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

  difusse *= intensity;
  specular *= intensity;

  FinalResult = (ambient + difusse + specular);


  return FinalResult;
}

float linearDepth(float depth)
{
  float z = depth * 2.0 - 1.0;

  return (2.0 * near * far) / (far + near - z * (far - near));
}





void main()
{ 

 vec3 normal_Face = normalize(Normal);
 vec3 viewDir = normalize(viewPos - FragPos);
 
 vec3 result = vec3(0.0);
 vec3 DL = CalcDirLight(directionalLight_1, normal_Face, viewDir);
 
 float depth = linearDepth(gl_FragCoord.z) / far;

 for(int i = 0; i < NUM_POINT_LIGHTS; i++)
 {
   result += CalcPointLight(pointLights_Array[i], normal_Face, viewDir);
    
 } 

 for(int i = 0; i < NUM_SPOT_LIGHTS; i++)
 { 
   if(spotLights_Array[i].lightState == true)
   {
     result +=  CalcSpotLight(spotLights_Array[i], normal_Face, viewDir);
   }

 }
 
 //result *= 0.1; 
   result += DL;
vec4 resultVec4 = vec4(result, 1.0) * (vec4(vec3(1.0), 1.0) - vec4(vec3(depth), 1.0)) - vec4(vec3(depth), 1.0);

FragColor =  resultVec4;
//FragColor = vec4(resultBasicLight, 1.0);

}







































































































































































































































































































































































































































































































































































































































































































































































































































































































































