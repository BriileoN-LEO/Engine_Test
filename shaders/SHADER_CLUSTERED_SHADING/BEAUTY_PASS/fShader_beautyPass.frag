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
uniform uint gridDimZ;
#define tileSize 16.0

const float pi = 3.14159265359;
const float kShininess = 16.0;

uniform vec3 indirect_light;

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

float calc_DistributionGGX(vec3 N, vec3 H, float roughness)
{
   float a = roughness * roughness;
   float a2 = a*a;
   float NdotH = max(dot(N, H), 0.0);
   float NdotH2 = NdotH * NdotH;

   //float num = a2;
   float denom = (NdotH2 * (a2 - 1.0) + 1.0);
   denom = pi * (denom * denom);

   return a2 / denom;
}

float calc_GeometrySchlickGGX(float NdotV, float roughness)
{
  float k_indirect = roughness + 1.0;
  k_indirect = (k_indirect * k_indirect) / 8.0;

  float num = NdotV;
  float denum = NdotV * (1.0 - k_indirect) + k_indirect;

  return num / denum;
}

float calc_GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
 float NdotV = max(dot(N, V), 0.0);
 float NdotL = max(dot(N, L), 0.0);
 float GGX_NV = calc_GeometrySchlickGGX(NdotV, roughness);
 float GGX_NL = calc_GeometrySchlickGGX(NdotL, roughness);

 return GGX_NV * GGX_NL;
}

vec3 calc_FresnelSchlick(float cosTheta, vec3 F0)
{
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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

   tileX = clamp(tileX, 0u, gridDimX - 1u);
   tileY = clamp(tileY, 0u, gridDimY - 1u);

   float view_Z = -gFragPosViewsSpace.z;
   uint sliceZ = uint(max(0.0, log2(view_Z) * scaleZ + biasZ));
   sliceZ = clamp(sliceZ, 0u, gridDimZ - 1u);

   uint clusterIndex = tileX + (tileY * gridDimX) + (sliceZ * gridDimX * gridDimY);

   uint lightStartPoint = cluster_Lights[clusterIndex].offset;
   uint lightCounter_size = cluster_Lights[clusterIndex].count;

   vec3 N = normalize(gNormal);
   vec3 V = normalize(viewPos - gPosition);
   vec3 F0 = mix(vec3(0.04f), gDiffuse, gSpecular);

   vec3 Lo = vec3(0.0);
   for(int i = 0; i < lightCounter_size; i++)
   {
      Light light_D = Lights_Array[global_ID_lights[lightStartPoint + i]];

           vec3 halfwayDir_lightAndFrag = light_D.lightPos_radius.xyz - gPosition;

           vec3 L = normalize(halfwayDir_lightAndFrag);
           vec3 H = normalize(V + L);

           float radius = light_D.lightPos_radius.w;
           float distance = length(halfwayDir_lightAndFrag);

           float dist_sqr = distance * distance;
           float rad_sqr = radius * radius;
           float attenuation = clamp(1.0 - (dist_sqr / rad_sqr), 0.0, 1.0);
           attenuation * attenuation;
         //  float attenuation = 1.0 / (distance * distance);

           vec3 radiance = light_D.Color_And_Intensity.xyz * light_D.Color_And_Intensity.w * attenuation;

           //float roughness = 1.0 - gSpecular;
           float roughness = 0.2;


           float NDF = calc_DistributionGGX(N, H, roughness);
           float G = calc_GeometrySmith(N, V, L, roughness);
           vec3 F = calc_FresnelSchlick(max(dot(H, V), 0.0), F0);

           vec3 numerator = NDF * G * F;
           float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
           vec3 specular = numerator / denominator;

           vec3 kS = F;
           vec3 kD = vec3(1.0) - kS;

           //kD *= 1.0 - metallic   -----------FOR METALLIC TEXTURE

           float NdotL = max(dot(N, L), 0.0);
           Lo += (kD * gDiffuse / pi + specular) * radiance * NdotL;

      // Lo += vec3(0.1);
   }

    vec3 ambient = vec3(0.03) * gDiffuse; ////ADD AO(ambient oclution)
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);  ///RESOLVE THE PBR/////////////////////////
}