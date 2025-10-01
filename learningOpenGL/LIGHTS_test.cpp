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

	DirectionalLight::DirectionalLight() {};
	DirectionalLight::DirectionalLight(lightShader LS_set, posicion pos, color color)
	{
		set_DirectionalLight(LS_set, pos, color);
	}

	void DirectionalLight::set_DirectionalLight(lightShader LS_set, posicion pos, color color)
	{
		lightShader_set = LS_set;
		Posicion = pos;
		Color = color;
	}
	inline void duplicate_Lights(std::vector<posicion> posLights, auto& typeLight, auto& meshLight)
	{

	}
}