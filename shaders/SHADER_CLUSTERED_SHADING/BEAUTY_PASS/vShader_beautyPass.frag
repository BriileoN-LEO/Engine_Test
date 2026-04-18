#version 460 core

in vec2 texCoords_quadScreen;
out vec4 FragColor;

uniform sampler2D texPosition;
uniform sampler2D texNormal;
uniform sampler2D texFragPosViewSpace_and_shiness;
uniform sampler2D texDiffuse_And_Spec;

uniform vec3 viewPos;


uniform vec2 screenSize;
uniform float zNear;
uniform float zFar;
uniform float scaleZ;
uniform float biasZ;
uniform uint gridDimX;
uniform uint gridDimY;
#define tileSize 16.0

const bool automatic_LIGHT_shininess = true;  ////relative brightness
const float pi = 3.14159265;
const float kShininess = 16.0;

uniform vec3 indirect_light;

  layout(std140, binding = 2) uniform shading_settings
 {
  vec3 pad_001;
  bool blinn_phong_active;
 };


struct Light
{
 vec4 lightPos_radius; // 0-12  ---> .w == radius
 vec4 Color_And_Intensity; // 76-80
 vec4 Direction;
 int lightState; // state of the light
 int light_Type; /// 0 == DIRECTIONAL LIGHT /// 1 == POINT LIGHT /// 2 == SPOT LIGHT
 float cutOff;
 float outerCutOff;
};

layout (std430, binding = 5) buffer Lights_D
{
 Light Lights_Array[];

};

struct Cluster_LightsData
{
  uint offset;
  uint count;
};

layout (std430, binding = 6) buffer light_GridBuffer
{
  Cluster_LightsData cluster_Lights[];
};

layout (std430, binding = 7) buffer global_ID_LightBuffer
{
  uint global_ID_lights[];
};

vec3 CalcDirLight(Light DirLight, vec3 diffuse, float specular, float shiness_m, vec3 normal_Face, vec3 viewDir)
{
   vec3 lightDir = normalize(DirLight.lightPos_radius.rgb);    ///////////////////////THIS IS THE GENERAL DIRECTION OF THE DIRECTIONAL LIGHT

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
   spec = pow(max(dot(normal_Face, reflectDir), 0.0), shiness_m);

  }

  else if(blinn_phong_active == false)
  {
    reflectDir = reflect(-lightDir, normal_Face);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness_m);
  }

  float specularCalc = spec * specular;

  vec3 lastCalcDir = (indirect_light + diff * specularCalc) * diffuse;

  ///HERE SHADOW MAPPING COLOR
 // float bias = max(0.05 * (1.0 - dot(normalize(normal_Face), lightDir)), 0.005);

 // float shadow_dirLight = Shadow_calculation_dirLight(FragPos_LSM, bias);
  //vec3 lastCalcDir = (ambient * (1.0 - shadow_dirLight) + (diffuse + specular)) * color;


  ///vec3 lastCalcDir = (ambient + diffuse * specular) * shadow_dirLight;
  return lastCalcDir;
}

vec3 CalcPointLight(Light PointLight, vec3 diffuse, float specular, float shiness_m, vec3 normal_Face, vec3 viewDir)
{
  vec3 lightDir = normalize(PointLight.lightPos_radius.rgb - FragPos);

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
    spec = pow(max(dot(normal_Face, reflectDir), 0.0), shiness_m);

  }

  else if(blinn_phong_active == false)
  {
     reflectDir = reflect(-lightDir, normal_Face);
     spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness_m);
  }


  //float distance_attenuation = length(PointLight.lightPos - FragPos);
  float distance_attenuation = PointLight.lightPos_radius.a;
  float attenuation = 1.0 / (PointLight.constant + PointLight.linear * distance_attenuation + PointLight.quadratic * (distance_attenuation * distance_attenuation));

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  ambient = PointLight.ambient * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
  diffuse = PointLight.diffuse * diff * vec3(texture(Mat_1.texture_diffuse, coordTexOut));
  specular = PointLight.specular * spec * vec3(texture(Mat_1.texture_specular, coordTexOut)); ///a�adido de reflect



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



void main()
{
   vec3 gPosition = texture(texPosition, texCoords_quadScreen).rgb;
   vec3 gNormal = texture(texNormal, texCoords_quadScreen).rgb;
   vec3 gFragPosViewsSpace = texture(texFragPosViewSpace_and_shiness, texCoords_quadScreen).rgb;
   vec3 gDiffuse = texture(texDiffuse_And_Spec, texCoords_quadScreen).rgb;
   float gSpecular = texture(texDiffuse_And_Spec, texCoords_quadScreen).a;
   float gShiness = texture(texFragPosViewSpace_and_shiness, texCoords_quadScreen).a;

   ///CALCULATE WHERE CLUSTER MATCH WITH THE PIXEL
   uint tileX = uint(gl_FragCoord.x / tileSize);
   uint tileY = uint(gl_FragCoord.y / tileSize);

   float view_Z = -gFragPosViewsSpace.z;
   uint sliceZ = uint(max(0.0, log2(view_Z) * scaleZ + biasZ));

   uint clusterIndex = tileX + (tileY * gridDimX) + (sliceZ * gridDimX * gridDimY);

   uint lightStartPoint = cluster_Lights[clusterIndex].offset;
   uint lightCounter_size = cluster_Lights[clusterIndex].count;

   for(int i = 0; i < lightCounter_size; i++)
   {
      Light light_D = Lights_Array[global_ID_lights[lightStartPoint + i]];


   }


}