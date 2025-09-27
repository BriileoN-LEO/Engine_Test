#version 330 core

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

void main()
{ 
 vec2 coordTex1 = coordTexOut;
 vec2 coordTex2 = coordTexOut;

 coordTex1 = coordTexOut * texTransform1;
 coordTex2 = coordTexOut * texTransform2;


 ///AMBIENT LIGHT
float ambientStrength = 0.4;
vec3 ambientLight = ambientStrength * lightColor;

///DIFUSSE LIGHT
vec3 normal_Face = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);

float diff = max(dot(lightDir, normal_Face), 0.0);
vec3 diffuseLight = diff * lightColor;


///SPECULAR LIGHT

float specular_intensity = 2.0;

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, normal_Face);

float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128);
vec3 specularLight = specular_intensity * specular * lightColor;

/////////////////

vec3 resultBasicLight = (ambientLight + diffuseLight + specularLight)  * objectColor;


//ragColor = mix(texture(texture1, coordTexOut), texture(texture2, coordTexOut), 0.2);

FragColor = texture(texture1, coordTexOut) * vec4(resultBasicLight.xyz, 1.0);
//FragColor = vec4(resultBasicLight, 1.0);

}