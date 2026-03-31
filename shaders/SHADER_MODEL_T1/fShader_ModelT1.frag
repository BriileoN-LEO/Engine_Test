#version 460 core
#define NUM_POINT_LIGHTS 9
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_SPOT_LIGHTS 1

layout(depth_any) out float gl_FragDepth;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 coordTexOut;
in vec3 FragPosViewSpace;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform samplerCube skybox;
uniform bool activeSkybox;
uniform mat3 transformation_SkyBox;

uniform float refractiveIndex;

uniform bool editMode;

float near = 0.001;
float far = 500.0;

bool blinn_phong_S = true;

bool automatic_LIGHT_shininess = true;  ////relative brightness
const float pi = 3.14159265;
const float kShininess = 16.0;

///////////SHADOW MAP///////////////
uniform sampler2DArray shadowMap;
uniform float cascadeDistances[5];
uniform mat4 lightSpaceMatrices[4];
uniform mat4 lightSpace_Mat;
int layer_shadowMap = 3;
vec4 FragPos_LSM = vec4(1.0);


void calc_layerShadowMap()
{
    ///THE LAYER OF THE SHADOW MAP IS -1

   float FragDepth = abs(FragPosViewSpace.z);

  for(int i = 0; i < 4; ++i)
  {
          if (FragDepth <= cascadeDistances[i])
          {
              layer_shadowMap = i;
              break;
          }

  }


    FragPos_LSM = lightSpaceMatrices[layer_shadowMap] * vec4(FragPos, 1.0);
   // FragPos_LSM = lightSpace_Mat * vec4(FragPos, 1.0);
}

float Shadow_calculation_dirLight(vec4 fragPosLightSpace, float bias)
{
    if(fragPosLightSpace.w == 0.0)
    {
        return 0.0;
    }

  vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoord = projCoord * 0.5 + 0.5;

  if(projCoord.z > 1.0 || projCoord.z < 0.0)
  {
     return 0.0;
  }

  //float closestDepth = texture(shadowMap, projCoord.xy).r;
  float depthFragment = projCoord.z;

    ///BIAS CALCULATION DEPENDS OF DISTANCE
  float cascadeDist =  cascadeDistances[layer_shadowMap];

  if(cascadeDist <= 0.0)
  {
      cascadeDist = 1.0;
  }

  bias *= (1.0 / cascadeDist) * 0.5f;
 //bias *= cascadeDist * 0.01f;
 vec2 texSize = vec2(textureSize(shadowMap, 0).xy);

 if(texSize.x == 0.0 || texSize.y == 0.0)
  {
      return 0.0;
  }

  float shadowCalc = 0.0;

       vec2 texelSize = vec2(1.0) / texSize;
      for (int x = -1; x <= 1; ++x)
      {
          for (int y = -1; y <= 1; ++y)
          {
              vec2 calculateShadowPos = projCoord.xy + vec2(x, y) * texelSize;

              float closest_pcfDepth = texture(shadowMap, vec3(calculateShadowPos, layer_shadowMap)).r;
              shadowCalc += (depthFragment - bias > closest_pcfDepth) ? 1.0 : 0.0;
          }
      }


   shadowCalc /= 9.0;

/*
  float closestDepth = texture(shadowMap, vec3(projCoord.xy, 0)).r;
  // check whether current frag pos is in shadow
  float shadow = depthFragment > closestDepth  ? 1.0 : 0.0;
*/
  return shadowCalc;
}


struct directional_Light
{
 vec3 lightDir;// 0-12
 float pad_01;//12-16

 vec3 ambient; // 16-28
 float pad_02; // 28-32

 vec3 diffuse; // 32-44
 float pad_03; // 44-48

 vec3 specular; // 48-60
 int lightState; // 64-68

};

//uniform directional_Light directionalLight_1;  ////TEST TO LOAD IN UNIFORM BUFFER
//uniform directional_Light directionalLight_1[NUM_DIRECTIONAL_LIGHTS];

struct point_Light
{
  vec3 lightPos; // 0-12
  float pad_01; // 12-16

  vec3 ambient; // 16-28
  float constant; // 16-32

  vec3 diffuse; // 32-44
  float linear; // 44-48

  vec3 specular; // 48-60
  float quadratic; // 60-64

  vec3 pad_02; // 64-76
  int lightState; // 76-80
};

uniform point_Light pointLight_1;
//uniform point_Light pointLights_Array[NUM_POINT_LIGHTS];  ////TEST TO LOAD IN UNIFORM BUFFER


struct spot_Light
{
  vec3 lightPos; 
  float cutOff;

  vec3 lightDir;
  float outerCutOff;

  vec3 ambient;
  float constant;

  vec3 diffuse;
  float linear;

  vec3 specular;
  float quadratic;

  vec3 pad_01;
  int lightState;
  
};

//uniform spot_Light spotLights_Array[NUM_SPOT_LIGHTS];  ////TEST TO LOAD IN UNIFORM BUFFER

layout(std430, binding = 1) buffer lights
{ 
directional_Light directionalLight_1[NUM_DIRECTIONAL_LIGHTS];
point_Light pointLights_Array[NUM_POINT_LIGHTS];
spot_Light spotLights_Array[NUM_SPOT_LIGHTS];
};


  layout(std140, binding = 2) uniform shading_settings
 {
  vec3 pad_001;
  bool blinn_phong_active;
 };

struct material_maps
{
sampler2D texture_diffuse;
sampler2D texture_specular;
sampler2D back_Texture;
float shiness;

bool blendTextureDiffuse; /////ESTE VALOR SE DEJA AL AIRE
bool use_texture_diffuse;
bool use_texture_specular;

};

uniform material_maps Mat_1;
uniform bool blendTexture;
uniform bool NotTexture;

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

   vec3 reflectDir;
   float spec;

   if(automatic_LIGHT_shininess == true && blinn_phong_active == true)
  {
   const float energy_Conservation = (8.0 + kShininess) / (8.0 + pi);
   reflectDir = normalize(lightDir + viewDir);
   spec = energy_Conservation * pow(max(dot(normal_Face, reflectDir), 0.0), kShininess);
  }

  else if(automatic_LIGHT_shininess == true && blinn_phong_active == false)
  {
   const float energy_Conservation = (8.0 + kShininess) / (8.0 + pi);
   reflectDir = reflect(-lightDir, normal_Face);
   spec = energy_Conservation * pow(max(dot(viewDir, reflectDir), 0.0), kShininess);
  }

  else if(blinn_phong_active == true)
  {
   reflectDir = normalize(lightDir + viewDir);  /// THE REFLECTION HALFWAYDIR
   spec = pow(max(dot(normal_Face, reflectDir), 0.0), Mat.shiness);

  }

  else if(blinn_phong_active == false)
  {
    reflectDir = reflect(-lightDir, normal_Face);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
  }

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  //vec3 lightColor = vec3(1.0f);

  vec3 color;

  if(diffuseExist == true)
  {
  vec3 diffuseTexture = vec3(texture(Mat_1.texture_diffuse, coordTexOut));

  ambient = dirLight_Cal.ambient * Mat.ambient;
  //ambient = dirLight_Cal.ambient * diffuseTexture;
  diffuse = dirLight_Cal.diffuse * diff;
  //diffuse = dirLight_Cal.diffuse * diff * diffuseTexture;
  color = diffuseTexture;

  }

  else if(diffuseExist == false)
  {
  ambient = dirLight_Cal.ambient * Mat.ambient;
  diffuse = dirLight_Cal.diffuse * diff;
  //diffuse = dirLight_Cal.diffuse * diff * Mat.difusse;
  color = Mat.difusse;
  }

  if(specExist == true)
  {
   specular = dirLight_Cal.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut));
  }

  else if(specExist == false)
  {
   specular = dirLight_Cal.specular * spec * Mat.specular;
  }


  //vec3 lastCalcDir = (ambient + diffuse * specular) * shadow_dirLight;

  ///HERE SHADOW MAPPING COLOR
  float bias = max(0.05 * (1.0 - dot(normalize(normal_Face), lightDir)), 0.005);

  float shadow_dirLight = Shadow_calculation_dirLight(FragPos_LSM, bias);
  vec3 lastCalcDir = (ambient * (1.0 - shadow_dirLight) + (diffuse + specular)) * color;


  ///vec3 lastCalcDir = (ambient + diffuse * specular) * shadow_dirLight;
  return lastCalcDir;
}

vec3 CalcPointLight(point_Light pointLight, vec3 normal_Face, vec3 viewDir, bool diffuseExist, bool specExist)
{
  vec3 lightDir = normalize(pointLight.lightPos - FragPos);
  
  float diff = max(dot(lightDir, normal_Face), 0.0);

  vec3 reflectDir;
  float spec;

  if(automatic_LIGHT_shininess == true && blinn_phong_active == true)
  {
   const float energy_Conservation = (8.0 + kShininess) / (8.0 + pi);
   reflectDir = normalize(lightDir + viewDir);
   spec = energy_Conservation * pow(max(dot(normal_Face, reflectDir), 0.0), kShininess);
  }

  else if(automatic_LIGHT_shininess == true && blinn_phong_active == false)
  {
   const float energy_Conservation = (8.0 + kShininess) / (8.0 + pi);
   reflectDir = reflect(-lightDir, normal_Face);
   spec = energy_Conservation * pow(max(dot(viewDir, reflectDir), 0.0), kShininess);
  }

  else if(blinn_phong_active == true)
  {
    reflectDir = normalize(lightDir + viewDir);  /// THE REFLECTION HALFWAYDIR
    spec = pow(max(dot(normal_Face, reflectDir), 0.0), Mat.shiness);

  }

  else if(blinn_phong_active == false)
  {
     reflectDir = reflect(-lightDir, normal_Face);
     spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
  }


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
  specular = pointLight.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut)); ///a�adido de reflect
  }

  else if(specExist == false)
  {
  specular = pointLight.specular * spec * Mat.specular;///a�adido de reflect
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
  specular = spotLight.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut)) ; ///a�adido de reflect
  }
  else if(specExist == false)
  {
  specular = spotLight.specular * spec * Mat.specular ; ///a�adido de reflect
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

vec4 CalcSkyboxReflaction()
{
  float ratio = 1.0 / refractiveIndex;
  vec3 I = normalize(FragPos - viewPos); 
  vec3 R = refract(I, normalize(Normal),ratio );

  vec3 coordTex = transformation_SkyBox * R;

return vec4(texture(skybox, coordTex).rgb, 1.0);
}

vec4 opCalc_existTextures(vec4 result, bool existTexSpec, vec4 specMulti) 
{
vec4 finalResult = vec4(0.0);

if(activeSkybox == true)
{
  if(existTexSpec == true) 
  {
   finalResult = result + specMulti;
  }

}

if(existTexSpec == false)
{
 finalResult = result;

}

 return finalResult;
}

/////PROXIMAMENTE HACE TENER LA CAPACIDAD DE VER LAS TEXTURAS
void renderWithTextures(vec4 renderStandard, vec4 renderTextures)
{
  if(gl_FragCoord.x < 1000)
  {
    FragColor = renderStandard;
  }

  else if(gl_FragCoord.x <= 1000)
  {
    FragColor = renderTextures;
  }
}

vec4 renderFrontFacing(vec4 renderStandard)
{

vec4 renderFragColor = vec4(0.0);

  if(gl_FrontFacing == true || blendTexture == true)
  {
    renderFragColor = renderStandard;
  }
  
  else if(gl_FrontFacing == false)
  {
    renderFragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }

  return renderFragColor;
}

//////////////
void main()
{
calc_layerShadowMap(); ///TO CALCULATE THE LAYER OF THE SHADOW MAP;


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

if(Mat_1.use_texture_diffuse == false)
{
 diffExist = false;
}

if(texSpec.x == 0.0 && texSpec.y == 0.0 && texSpec.z == 0.0)
{
 specExist = false;
}

if(Mat_1.use_texture_specular == false)
{
 specExist = false;
}

if(NotTexture == true)
{
  diffExist = false;
  specExist = false;
}

////PARA saber si existe una textura en texDiff y no descartarla por completo
if(diffExist == true && texDiff.a < 0.1)
{
 discard;

}

vec3 result = vec3(0.0);

for(int i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
{
 result += CalcDirLight(directionalLight_1[i], normal_Face, viewDir, diffExist, specExist);

}

///Add point lights
for(int i = 0; i < NUM_POINT_LIGHTS; i++)
{

 result += CalcPointLight(pointLights_Array[i], normal_Face, viewDir, diffExist, specExist);

}

for(int i = 0; i < NUM_SPOT_LIGHTS; i++)
{ 
 if(spotLights_Array[i].lightState == 1)
 {
  result += CalcSpotLight(spotLights_Array[i], normal_Face, viewDir, diffExist, specExist);
 }

}
//result += DL_1;

vec4 resultVec4 = vec4(result, 1.0) * (vec4(vec3(1.0), 1.0) - vec4(vec3(depth), 1.0)) - vec4(vec3(depth), 1.0);

if(diffExist == true)
{
 resultVec4.w = texDiff.w;

}

vec4 skyBox_reflection = CalcSkyboxReflaction();

vec4 texSpecMulti = texSpec * skyBox_reflection;

if(activeSkybox == true)
{
  if(specExist == true) 
  {
 // vec4 texSpecMulti = texSpec * skyBox_reflection;
  // FragColor = resultVec4 + texSpecMulti;
  }

}

if(specExist == false)
{
//FragColor = resultVec4;

}

vec4 renderStandard = opCalc_existTextures(resultVec4, specExist, texSpecMulti);
vec4 renderCoordTextures = vec4(coordTexOut.x, coordTexOut.y, 0.0, 1.0);



 ///FragColor = renderFrontFacing(renderStandard);
  //FragColor = renderStandard;
  FragColor = resultVec4;



 //FragColor = vec4(1.0, 0.0, 0.0, 1.0);

//FragColor = vec4(1.0);

 //FragColor = vec4(coordTexOut.x, coordTexOut.y, 0.0, 1.0);

//  gl_FragDepth = gl_FragCoord.z + 50.0;
//renderWithTextures(renderStandard, renderCoordTextures);

//FragColor = opCalc_existTextures(resultVec4, specExist, texSpecMulti);

//if(gl_FragCoord.x < 1000)
//{
//FragColor = opCalc_existTextures(resultVec4, specExist, texSpecMulti);
//}
//else if (gl_FragCoord.x >= 1000)
//{
//FragColor = vec4(coordTexOut.x, coordTexOut.y, 0.0, 1.0);
//}
//FragColor = vec4(coordTexOut.x, coordTexOut.y, 0.0, 1.0);
//FragColor = resultVec4;

//FragColor = skyBox_reflection;

}




































































































