#include "LIGHTS_test.h"

namespace light
{
	light1::light1() {};
	light1::light1(posicion lightPosicion, color lightColor)
	{
		setLightsSettings(lightPosicion, lightColor);
	}

	void light1::setLightsSettings(posicion lightPosicion, color lightColor)
	{
		Posicion = lightPosicion;
		Color = lightColor;
	}
	void light1::setAttenuation(float constant, float linear, float quadratic)
	{
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}
	void light1::setMatProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		Mat.ambient = ambient;
		Mat.diffuse = diffuse;
		Mat.specular = specular;
	}

	DirectionalLight::DirectionalLight() {};
	DirectionalLight::DirectionalLight(posicion Posicion, direction Direction, color color)
	{
		set_DirectionalLight(Posicion, Direction, color);
	}

	void DirectionalLight::set_DirectionalLight(posicion Posicion, direction Direction, color Color)
	{
		this->Posicion = Posicion;
	//	this->Direction = glm::normalize(Direction);
		this->Direction = Direction;
		lastDirection = Direction;
		this->Color = Color;
	}

	void DirectionalLight::setMatProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		Mat.ambient = ambient;
		Mat.diffuse = diffuse;
		Mat.specular = specular;
	}

	
	SpotLight::SpotLight() {};
	SpotLight::SpotLight(posicion Posicion, direction Direction, lightShader Mat, float cutOff, float outerCutOff)
	{
		this->Posicion = Posicion;
		this->Direction = Direction;
		this->Mat = Mat;
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
	}

	void SpotLight::updateLight(posicion Posicion, direction Direction)
	{
		this->Posicion = Posicion;
		this->Direction = Direction;
	}
	void SpotLight::setAttenuation(float constant, float linear, float quadratic)
	{
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;

	}
	void SpotLight::setMatProperties(lightShader Mat)
	{
		this->Mat = Mat; 
	}

	pointLight_PBR::pointLight_PBR() = default;


	SpotLight_PBR::SpotLight_PBR() = default;
	SpotLight_PBR::SpotLight_PBR(glm::vec3 Position, glm::vec3 Direction, glm::vec3 Color, float Radio, float cutOff, float outerCutOff) :
	data(Position, Direction, Color, Radio, cutOff, outerCutOff)
	{};

	void SpotLight_PBR::update_position(glm::vec3 Position)
	{
		data.Position = Position;
	}
	void SpotLight_PBR::update_direction(glm::vec3 Direction)
	{
		data.Direction = Direction;
	}
	void SpotLight_PBR::update_color(glm::vec3 Color)
	{
     data.Color = Color;
	}

	const data_SpotLightPBR& SpotLight_PBR::out_Data()
	{
       return data;
	}



	inline void duplicate_Lights(std::vector<posicion> posLights, auto& typeLight, auto& meshLight)
	{

	}
}