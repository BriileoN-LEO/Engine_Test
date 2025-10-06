#ifndef LIGHTS_test
#define LIGHTS_test

#include "GLM_test.h"
#include "data_save.h"
//#include "ModelAssimp.h"
//#include "SHADER_H.h"

using color = glm::vec3;
using posicion = glm::vec3;
using direction = glm::vec3;

namespace light
{
	
	struct lightShader
	{
		glm::vec3 ambient{};
		glm::vec3 diffuse{};
		glm::vec3 specular{};
	};
    
	class light1
	{
	public:

		posicion Posicion{};
		color Color{};

		lightShader Mat{};

		float constant{};
		float linear{};
		float quadratic{};

		bool stateLight{ true };

		light1();
		light1(posicion lightPosicion, color lightColor);

		void setLightsSettings(posicion lightPosicion, color lightColor);
		void setAttenuation(float constant, float linear, float quadratic);
		void setMatProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

		//void setLightMeshPos(glm:: ////hacer una actualizacion de la luz mediante el la posicion del mesh con sus vertices, hay que calcular el centro de mi modelo 3D
				///void translatePosLightMesh(glm::mat4 meshPosicion) ////Actualizar la traslacion del mesh con la matriz de traslacion del objecto
	};
	
	class DirectionalLight
	{
	public:

		direction Direction{};
		color Color{};
		lightShader Mat{};

		bool stateLight{ true };

		DirectionalLight();
		DirectionalLight(direction Direction, color lightColor);

		void set_DirectionalLight(direction Direction, color lightColor);
		void setMatProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	};
	
	inline void duplicate_Lights(std::vector<posicion> posLights, auto& typeLight, auto& meshLight); /// hacer un testeo despues 
	//inline void setFile_PointLights();

}



#endif LIGHTS_test