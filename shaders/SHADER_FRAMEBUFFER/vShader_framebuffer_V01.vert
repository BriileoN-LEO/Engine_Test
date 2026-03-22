#version 460 core

//layout (location = 0) in vec2 aPos; 
layout (location = 0) in vec3 aPos; //Agregado
//layout (location = 1) in vec3 aNormal; //Agregado
layout (location = 1) in vec2 aTexCoords;


out vec2 texCoords;

//layout (std140, binding = 0) uniform matrices_cam
//{
 //mat4 view_c;
 //mat4 projection_c;
//};


//uniform mat4 model; //Agregado
//uniform mat4 view;  //Agregado
//uniform mat4 projection;


void main()
{

//gl_Position = projection_c * view_c * vec4(aPos, 1.0);
 gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
 texCoords = aTexCoords;


}

