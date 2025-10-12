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
	DirectionalLight::DirectionalLight(direction Direction, color color)
	{
		set_DirectionalLight(Direction, color);
	}

	void DirectionalLight::set_DirectionalLight(direction Direction, color Color)
	{
		this->Direction = glm::normalize(Direction);
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

	inline void duplicate_Lights(std::vector<posicion> posLights, auto& typeLight, auto& meshLight)
	{

	}
}