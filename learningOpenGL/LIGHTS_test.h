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

		uint32_t ID{};

		posicion Posicion{};
		transformation_basics::basics_Model3D lightCoord{};

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

		posicion Posicion{};
		direction Direction{};
		direction lastDirection{};

		color Color{};
		lightShader Mat{};

		bool stateLight{ true };

		DirectionalLight();
		DirectionalLight(posicion Posicion, direction Direction, color lightColor);

		void set_DirectionalLight(posicion Posicion, direction Direction, color lightColor);
		void setMatProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	};

	class SpotLight
	{
	public:

		posicion Posicion{};
		direction Direction{};
		lightShader Mat{};

		float cutOff{};
		float outerCutOff{}; 
		float constant{};
		float linear{};
		float quadratic{};

		bool stateLight{ true };

		SpotLight();
		SpotLight(posicion Posicion, direction Direction, lightShader Mat, float cutOff, float outerCutOff);

		void updateLight(posicion Posicion, direction Direction);
		void setAttenuation(float constant, float linear, float quadratic);
		void setMatProperties(lightShader Mat);
		

	};


	enum class typeLight : uint8_t
	{
      DIRECTIONAL_LIGHT = 0,
	  POINT_LIGHT = 1,
	  SPOT_LIGHT = 2
	};


	struct data_directionalLightPBR
	{
	  glm::vec3 Direction{};
	  glm::vec3 Color{};
	  float Intensity{};

	  bool stateLight{ true };
	};

	class directionalLight_PBR
	{
	private :
		data_directionalLightPBR data{};

		void setData(glm::vec3 Direction, glm::vec3 Color, float Intensity, bool stateLight);

	public :

		directionalLight_PBR();
		directionalLight_PBR(glm::vec3 Direction, glm::vec3 Color, float Intensity, bool stateLight);

		void update_direction(glm::vec3 Direction);
		void update_color(glm::vec3 Color);
		void update_intensity(float Intensity);
		void update_stateLight(bool stateLight);

		const data_directionalLightPBR& out_Data();
	};

	struct data_pointLightPBR
	{
		glm::vec3 Position{};
		glm::vec3 Color{};
		float Radio{};
		float Intensity{};

		bool stateLight{ true };
	};

	class pointLight_PBR  /////CONTINUE HERE
	{
	private:

		data_pointLightPBR data{};

		void setData(glm::vec3 Position, glm::vec3 Color, float Radio, float Intensity, bool stateLight);

	public:
		pointLight_PBR();
		pointLight_PBR(glm::vec3 Position, glm::vec3 Color, float Radio, float Intensity, bool stateLight);

	    void update_position(glm::vec3 Position);
		void update_color(glm::vec3 Color);
		void update_radio(float Radio);
		void update_intensity(float Intensity);
		void update_stateLight(bool stateLight);

		const data_pointLightPBR& out_Data();
	};

	struct data_SpotLightPBR
	{
		glm::vec3 Position{};
		glm::vec3 Direction{};
		glm::vec3 Color{};

		float Radio{};
		float cutOff{};
		float outerCutOff{};

		bool stateLight{ true };
	};

	class SpotLight_PBR
	{
	private:

        data_SpotLightPBR data{};

	public:
		SpotLight_PBR();
		SpotLight_PBR(glm::vec3 Position, glm::vec3 Direction, glm::vec3 Color, float Radio, float cutOff, float outerCutOff);

		void update_position(glm::vec3 Position);
		void update_direction(glm::vec3 Direction);
		void update_color(glm::vec3 Color);

	    const data_SpotLightPBR& out_Data();

	};

	
	inline void duplicate_Lights(std::vector<posicion> posLights, auto& typeLight, auto& meshLight); /// hacer un testeo despues 
	//inline void setFile_PointLights();
  

}



#endif // LIGHTS_test