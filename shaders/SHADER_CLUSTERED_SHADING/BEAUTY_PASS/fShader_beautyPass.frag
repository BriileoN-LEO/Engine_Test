#version 460 core

in vec2 texCoords_quadScreen;
out vec4 FragColor;

uniform sampler2D texPosition;
uniform sampler2D texNormal;
uniform sampler2D texFragPosViewSpace_and_shiness;
uniform sampler2D texDiffuse_And_Spec;
uniform sampler2D texEditMode;  ///NEW

uniform sampler2D tex_zDepth_Models; ///NEW
uniform sampler2D tex_zDepth_EditMode; ///NEW

uniform vec3 viewPos;

uniform vec2 screenSize;
uniform float zNear;
uniform float zFar;
uniform float scaleZ;
uniform float biasZ;
uniform uint gridDimX;
uniform uint gridDimY;
uniform uint gridDimZ;
//#define tileSize 16.0

const float pi = 3.14159265359;
const float kShininess = 16.0;

uniform vec3 indirect_light;

layout (std140, binding = 0) uniform matrices_cam
{
 mat4 view_c;
 mat4 projection_c;
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
   vec3 gEditMode = texture(texEditMode, texCoords_quadScreen).rgb;

   float gZDEPTH_MODELS = texture(tex_zDepth_Models, texCoords_quadScreen).r; ///NEW
   float gZDEPTH_EDITMODE = texture(tex_zDepth_EditMode, texCoords_quadScreen).r; ///NEW

   ///CALCULATE WHERE CLUSTER MATCH WITH THE PIXEL

    vec2 tileSize = screenSize / vec2(gridDimX, gridDimY);
   //uint tileS_X = uint(screenSize.x / gridDimX);
   //uint tileS_Y = uint(screenSize.y / gridDimY);

   uint tileX = uint(gl_FragCoord.x / tileSize.x);
   uint tileY = uint(gl_FragCoord.y / tileSize.y);
   //vec2 tileSize = screenSize / vec2(gridDimX, gridDimY);

   tileX = uint(clamp(tileX, 0u, gridDimX - 1u));
   tileY = uint(clamp(tileY, 0u, gridDimY - 1u));

   float view_Z = -gFragPosViewsSpace.z;
   uint sliceZ = uint(max(0.0, log2(view_Z) * scaleZ + biasZ));
   sliceZ = clamp(sliceZ, 0u, gridDimZ - 1u);

   uint clusterIndex = tileX + (tileY * gridDimX) + (sliceZ * gridDimX * gridDimY);

   uint lightStartPoint = cluster_Lights[clusterIndex].offset;
   uint lightCounter_size = cluster_Lights[clusterIndex].count;

   vec3 N = normalize(gNormal);
   vec3 V = normalize(viewPos - gPosition);

   float metallic = 0.0;
   vec3 F0 = mix(vec3(0.04f), gDiffuse, metallic); ///Here comes the metallic texture

   vec3 Lo = vec3(0.0);

   mat4 inv_view_c = inverse(view_c);
   for(uint i = 0; i < lightCounter_size; i++)
   {
      Light light_current = Lights_Array[global_ID_lights[lightStartPoint + i]];

       vec3 light_pos = vec4(inv_view_c * vec4(light_current.lightPos_radius.xyz, 1.0)).xyz;
     //  vec3 light_pos = light_current.lightPos_radius.xyz;

           vec3 halfwayDir_lightAndFrag = light_pos - gPosition;

           vec3 L = normalize(halfwayDir_lightAndFrag);
           vec3 H = normalize(V + L);

           float radius = light_current.lightPos_radius.w;
           float distance = length(halfwayDir_lightAndFrag);

           float dist_sqr = distance * distance;
           float inv_Square = 1.0 / max(dist_sqr, 0.0001);
           float rad_sqr = radius * radius;
           float factor = dist_sqr / rad_sqr;
           float factor2 = factor * factor;
           float windowing = clamp(1.0 - factor2, 0.0, 1.0);
           windowing *= windowing;

           float attenuation = inv_Square * windowing;
           //float attenuation = clamp(1.0 - (dist_sqr / rad_sqr), 0.0, 1.0);
           //attenuation *= attenuation;
           //  float attenuation = 1.0 / (distance * distance);

           vec3 radiance = light_current.Color_And_Intensity.xyz * light_current.Color_And_Intensity.w * attenuation;

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
           kD *= 1.0 - metallic;  // -----------FOR METALLIC TEXTURE

           float NdotL = max(dot(N, L), 0.0);
           Lo += (kD * gDiffuse / pi + specular) * radiance * NdotL;
      // Lo += vec3(0.1);
   }

    vec3 ambient = vec3(0.03) * gDiffuse; ////ADD AO(ambient oclution)
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));


    bool test_1 = gEditMode.rgb == vec3(0.0, 0.0, 0.0) || gZDEPTH_EDITMODE == 0.0;
    bool test_2 = test_1 ? true : gZDEPTH_EDITMODE >= gZDEPTH_MODELS ;

    if(test_2 == true)
    {
      FragColor = vec4(color, 1.0);///RESOLVE THE PBR/////////////////////////
    }

    else
    {
      FragColor = vec4(gEditMode, 1.0);
    }
}