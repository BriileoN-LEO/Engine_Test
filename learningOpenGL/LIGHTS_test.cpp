#include "LIGHTS_test.h"

namespace light
{
	light1::light1() {};
	light1::light1(glm::vec3 lightPosicion, glm::vec3 lightColor)
	{
		setLightsSettings(lightPosicion, lightColor);
	}

	void light1::setLightsSettings(glm::vec3 lightPosicion, glm::vec3 lightColor)
	{
		Posicion = lightPosicion;
		Color = lightColor;
	}
}