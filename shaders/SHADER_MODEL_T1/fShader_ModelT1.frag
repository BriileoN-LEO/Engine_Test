#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 coordTexOut;
//in vec3 resultBasicLight;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

struct material_standard
{
vec3 ambient;
vec3 difusse;
vec3 specular;
float shiness;
};

uniform material_standard Mat;

struct material_maps
{
sampler2D texture_diffuse;
sampler2D texture_specular;
float shiness;
};

uniform material_maps Mat_1;

void main()
{ 
 ///AMBIENT LIGHT

vec3 ambientLight = (vec3(0.8) * Mat.ambient) * lightColor;
vec3 ambientSet = ambientLight * vec3(texture(Mat_1.texture_diffuse, coordTexOut));

///DIFUSSE LIGHT
vec3 normal_Face = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);

float diff = max(dot(lightDir, normal_Face), 0.0);
vec3 diffuseLight = (diff * Mat.difusse) * lightColor;

vec3 diffuse_set = diffuseLight * diff * vec3(texture(Mat_1.texture_diffuse, coordTexOut));

///SPECULAR LIGHT

float specular_intensity = 2.0;

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, normal_Face);

float specular = pow(max(dot(viewDir, reflectDir), 0.0), Mat.shiness);
vec3 specularLight = (Mat.specular * specular) * lightColor;

vec3 specular_set = specularLight * specular * vec3(texture(Mat_1.texture_specular, coordTexOut));
/////////////////


vec3 resultBasicLight = (ambientSet + diffuse_set + specular_set)  * objectColor;


//ragColor = mix(texture(texture1, coordTexOut), texture(texture2, coordTexOut), 0.2);
//FragColor = mix(texture(texture_diffuse1, coordTexOut), texture(texture_diffuse2, coordTexOut), texture(texture_diffuse3, coordTexOut), texture(texture_specular1, coordTexOut), 0.2) * vec4(resultBasicLight.xyz, 1.0);


//vec4 mixDiff = mix(texture(texture_diffuse1, coordTexOut), texture(texture_diffuse2, coordTexOut), 0.2);
//mixDiff = mix(texture(texture_diffuse1, coordTexOut), mixDiff, 0.2);

//vec4 mixSpecular = mix(texture(texture_specular1, coordTexOut), texture(texture_specular2, coordTexOut), 0.2);

//vec4 mixAll = mix(mixDiff, mixSpecular, 0.2);

FragColor =   vec4(resultBasicLight.xyz, 1.0);
//FragColor = mixAll * vec4(resultBasicLight, 1.0);
//FragColor = mix(texture(texture_diffuse1, coordTexOut),  * vec4(resultBasicLight, 1.0);

}