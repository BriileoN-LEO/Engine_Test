#include "LIGHTS_test.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"

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


	void directionalLight_PBR::setData(glm::vec3 Direction, glm::vec3 Color, float Intensity, bool stateLight)
	{
		data.Direction = Direction;
		data.Color = Color;
		data.Intensity = Intensity;
		data.stateLight = stateLight;
	}

    directionalLight_PBR::directionalLight_PBR() = default;
	directionalLight_PBR::directionalLight_PBR(glm::vec3 Direction, glm::vec3 Color, float Intensity, bool stateLight)
	{
		setData(Direction, Color, Intensity, stateLight);
	}

	void directionalLight_PBR::setID(std::string dL_ID)
	{
		ID = FNV::str_to_hash(dL_ID);
	}
	void directionalLight_PBR::setID_by_int(uint32_t dL_ID)
	{
      ID = dL_ID;
	}
	void directionalLight_PBR::update_direction(glm::vec3 Direction)
	{
	  data.Direction = Direction;
	}
	void directionalLight_PBR::update_color(glm::vec3 Color)
	{
	  data.Color = Color;
	}
	void directionalLight_PBR::update_intensity(float Intensity)
	{
	  data.Intensity = Intensity;
	}
	void directionalLight_PBR::update_stateLight(bool stateLight)
	{
	  data.stateLight = stateLight;
	}

	uint32_t& directionalLight_PBR::get_ID_ref()
	{
		return ID;
	}
	const uint32_t& directionalLight_PBR::get_ID_c() const
	{
		return ID;
	}
	const data_directionalLightPBR& directionalLight_PBR::out_Data()
	{
      return data;
	}

	void pointLight_PBR::setData(glm::vec3 Position, glm::vec3 Color, float Radio, float Intensity, bool stateLight)
	{
		data.Position = Position;
		data.Color = Color;
		data.Radio = Radio;
		data.Intensity = Intensity;
		data.stateLight = stateLight;
	}

	pointLight_PBR::pointLight_PBR() = default;
	pointLight_PBR::pointLight_PBR(glm::vec3 Position, glm::vec3 Color, float Radio, float Intensity, bool stateLight)
	{
		setData(Position, Color, Radio, Intensity, stateLight);
	}

	void pointLight_PBR::setID(std::string pL_ID)
	{
		ID = FNV::str_to_hash(pL_ID);
	}
	void pointLight_PBR::setID_by_int(uint32_t pL_ID)
	{
		ID = pL_ID;
	}
	void pointLight_PBR::update_position(glm::vec3 Position)
	{
		data.Position = Position;
	}
	void pointLight_PBR::update_color(glm::vec3 Color)
	{
		data.Color = Color;
	}
	void pointLight_PBR::update_radio(float Radio)
	{
		data.Radio = Radio;
	}
	void pointLight_PBR::update_intensity(float Intensity)
	{
        data.Intensity = Intensity;
	}
	void pointLight_PBR::update_stateLight(bool stateLight)
	{
        data.stateLight = stateLight;
	}

	uint32_t& pointLight_PBR::get_ID_ref()
	{
		return ID;
	}
	const uint32_t& pointLight_PBR::get_ID_c() const
	{
		return ID;
	}
	const data_pointLightPBR& pointLight_PBR::out_Data() const
	{
        return data;
	}

	SpotLight_PBR::SpotLight_PBR() = default;
	SpotLight_PBR::SpotLight_PBR(glm::vec3 Position, glm::vec3 Direction, glm::vec3 Color, float Radio, float cutOff, float outerCutOff) :
	data(Position, Direction, Color, Radio, cutOff, outerCutOff)
	{};

	void SpotLight_PBR::setID(std::string sL_ID)
	{
		ID = FNV::str_to_hash(sL_ID);
	}
	void SpotLight_PBR::setID_by_int(uint32_t sL_ID)
	{
		ID = sL_ID;
	}
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
	void SpotLight_PBR::update_stateLight(bool stateLight)
	{
		data.stateLight = stateLight;
	}

	uint32_t& SpotLight_PBR::get_ID_ref()
	{
	 return ID;
	}
	const uint32_t& SpotLight_PBR::get_ID_c() const
	{
		return ID;
	}
	const data_SpotLightPBR& SpotLight_PBR::out_Data()
	{
       return data;
	}



	inline void duplicate_Lights(std::vector<posicion> posLights, auto& typeLight, auto& meshLight)
	{

	}
}