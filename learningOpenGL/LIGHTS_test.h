#ifndef LIGHTS_test
#define LIGHTS_test

#include "GLM_test.h"
#include "data_save.h"
#include "ModelAssimp.h"
//#include "SHADER_H.h"

namespace light
{
	using color = glm::vec3;
	using posicion = glm::vec3;

	struct lightShader
	{
		glm::vec3 direction{};
		glm::vec3 ambient{};
		glm::vec3 diffuse{};
		glm::vec3 specular{};
	};
    
	class light1
	{
	public:

		posicion Posicion{};
		color Color{};

		light1();
		light1(posicion lightPosicion, color lightColor);

		void setLightsSettings(posicion lightPosicion, color lightColor);

		//void setLightMeshPos(glm:: ////hacer una actualizacion de la luz mediante el la posicion del mesh con sus vertices, hay que calcular el centro de mi modelo 3D
				///void translatePosLightMesh(glm::mat4 meshPosicion) ////Actualizar la traslacion del mesh con la matriz de traslacion del objecto
	};
	
	class DirectionalLight
	{
	public:

		lightShader lightShader_set{};
		color Color{};

		DirectionalLight();
		DirectionalLight(lightShader LS_set, posicion pos, color color);

		void set_DirectionalLight(lightShader LS_set, posicion pos, color color);

	};
	
	inline void duplicate_Lights(std::vector<posicion> posLights, auto& typeLight, auto& meshLight);

}



#endif LIGHTS_test