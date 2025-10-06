#ifndef RenderData
#define RenderData
#include "learningOpenGL.h"
#include "ModelAssimp.h"
#include "configFilesTXT.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace RenderData_Set
{
	extern std::map<std::string, ObjCreation::ModelCreation> ModelCreation_D;
	extern std::map<std::string, Assimp::Model> AssimpModel_D;
	extern std::vector<ObjCreation::ModelCreation> MeshLights_MCD;
	extern std::vector<light::light1> pointLights_D;
	extern std::vector<light::DirectionalLight> directionalLights_D;

	const std::map<std::string, ObjCreation::ModelCreation> setModelCreation_Data();
	const std::map<std::string, Assimp::Model> setModel_Data();
	const std::vector<ObjCreation::ModelCreation> setMeshLight_ModelCreation_Data();
	const std::vector<light::light1> setPointLights();
	const std::vector<light::DirectionalLight> setDirectionalLights();
	void setSettings_FileShader(const char* fragmentShader_Path, std::vector<std::string> values);
	void insertSettings_FileShader();

	const void set_AllObjects();
}
namespace cameras
{
	//camera::camera1 aerialCamera(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.1f, 100.0f);
	extern camera::camera1 aerialCamera;
	void setCameras();

}

#endif RenderData