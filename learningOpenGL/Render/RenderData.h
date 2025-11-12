#ifndef RenderData
#define RenderData

#include <filesystem>
#include <fstream>
#include <iostream>
#include "learningOpenGL.h"
#include "ModelAssimp.h"
#include "configFilesTXT.h"
#include "Collision/CollisionAABB.h"
#include "2D_UI/2D_ScreenPlayer.h"

namespace RenderData_Set
{
	extern std::map<std::string, ObjCreation::ModelCreation> ModelCreation_D;
	extern std::map<std::string, Assimp_D::Model> AssimpModel_D;
	extern std::vector<ObjCreation::ModelCreation> MeshLights_MCD;
	extern std::vector<light::light1> pointLights_D;
	extern std::vector<light::DirectionalLight> directionalLights_D;
	extern std::map<std::string, light::SpotLight> spotLights_D;
	extern std::vector<screenUI::pointerScreen> pointUI_D;
	extern std::vector<individualComp::Multiple_AssimpMesh> multi_AssimpModel;
	extern std::map<std::string, shading::shader> shader_D;
	extern std::atomic<bool> finishLoadALL;

	namespace stencilTest
	{
		extern shading::shader stencilTest_shader;
		const void setStencilTest_Shader();
	}

	const std::map<std::string, ObjCreation::ModelCreation> setModelCreation_Data();

	const std::map<std::string, Assimp_D::Model> setModel_Data();

	const void loadCPU_Model_Data();
	const void insertData_toModel();
	const void insertSetting_toModel();////Continuar aqui para ir insertando los modelos 3D 
	template<typename modelSet>
	void running_LoadingModels(std::function<modelSet> functionLoad)
	{
		auto loading_Vals = []()
			{
				while (Assimp_D::loadToCPU::finishLoadModels.load() == false)
				{
					insertData_toModel();
					//std::cout << "LOADING_MODELS\n";
				}

				insertSetting_toModel();
				if (Assimp_D::loadToCPU::finishLoadModels.load() == true)
				{
					//insertSetting_toModel();
					std::cout << "INSERT::SETTINGS\n";
				//	Assimp_D::loadToCPU::finishLoadModels = true;
				}

			};

		functionLoad();
		loading_Vals();
		//std::thread loading_Models(loading_Vals);	
		//loading_Models.detach();

	}

	const void loadCPU_Shader();
	const void insertData_toShader();
	template<typename shaderSet>
	void running_LoadingShaders(std::function<shaderSet> functionShader)
	{
		auto loading_shader = []()
			{
				while (shading::loadToCPU::finishLoadShaders.load() == false)
				{
					insertData_toShader();
				}

				if (shading::loadToCPU::finishLoadShaders.load() == true)
				{
				//	shading::loadToCPU::finishLoadShaders = false;
				}

			};


		functionShader();
		
		loading_shader();
	//	std::thread loading_Shaders(loading_shader);
	//  loading_Shaders.detach();

	}
	const void loadAll_DataCPU();

	const std::vector<ObjCreation::ModelCreation> setMeshLight_ModelCreation_Data();
	const std::vector<light::light1> setPointLights();
	const std::vector<light::DirectionalLight> setDirectionalLights();
	const std::map<std::string, light::SpotLight> setSpotLights();
	const std::vector<screenUI::pointerScreen> setPointUI_2D();
	const std::vector<individualComp::Multiple_AssimpMesh> setMulti_AssimpModel();

	void setSettings_FileShader(const char* fragmentShader_Path, std::vector<std::string> values);
	void insertSettings_FileShader();

	const void set_AllObjects();
	const void running_AllObjects();
}



namespace cameras
{
	//camera::camera1 aerialCamera(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.1f, 100.0f);
	extern std::string name_CurrentCamera;
	extern camera::camera1 aerialCamera;
	extern camera::camera1 currentCamera;
	void setCameras();
	void updateStateCurrentCamera();

}

#endif RenderData