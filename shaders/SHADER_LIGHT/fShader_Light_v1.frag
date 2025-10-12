#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;

float near = 0.001;
float far = 500.0; 

float linearDepth(float depth)
{
  float z = depth * 2.0 - 1.0;

  return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{
 
  float depth = linearDepth(gl_FragCoord.z) / far;

  vec4 result = vec4(lightColor.xyz * 1.5, 1.0) * vec4((vec3(1.0f) - vec3(depth)) - vec3(depth), 1.0);

  FragColor = result;

}