#ifndef LIGHTS_test
#define LIGHTS_test

#include "GLM_test.h"
#include "data_save.h"
//#include "SHADER_H.h"

namespace light
{
	class light1
	{
	public:
		
		glm::vec3 Posicion{};
		glm::vec3 Color{};

		light1();
		light1(glm::vec3 lightPosicion, glm::vec3 lightColor);

		void setLightsSettings(glm::vec3 lightPosicion, glm::vec3 lightColor);

		//void setLightMeshPos(glm:: ////hacer una actualizacion de la luz mediante el la posicion del mesh con sus vertices, hay que calcular el centro de mi modelo 3D
				///void translatePosLightMesh(glm::mat4 meshPosicion) ////Actualizar la traslacion del mesh con la matriz de traslacion del objecto
	};

}



#endif LIGHTS_test