#include "RenderData.h"

namespace RenderData_Set
{

	std::map<std::string, ObjCreation::ModelCreation> ModelCreation_D{};
	std::map<std::string, Assimp::Model> AssimpModel_D{};
	std::vector<ObjCreation::ModelCreation> MeshLights_MCD{};
	std::vector<light::light1> pointLights_D{};
	std::vector<light::DirectionalLight> directionalLights_D{};

	const std::map<std::string, ObjCreation::ModelCreation> setModelCreation_Data()
	{

		std::vector<std::array<float, 24>> vertex{};
		vertex.emplace_back(vertexCreationData::cube::Tri1_face1);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face1);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face2);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face2);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face3);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face3);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face4);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face4);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face5);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face5);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face6);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face6);

		ObjCreation::ModelCreation FirstModel(vertex);
		FirstModel.BuildVertexShader(vShader_Model_V1.c_str(), fShader_Model_V1.c_str());

		FirstModel.LoadVertexTex(image_Wood.c_str(), 0, false);
		FirstModel.LoadVertexTex(image_happyFace.c_str(), 1, false);

		////////////////////SETEAR LAS TEXTURAS MULTIPLES PARA CREAR EL CUBO DE MINECRAFT////////////////
		FirstModel.LoadMultipleTex(image_MineGrass_lateral.c_str(), texture::textureUnits::TEXTURE0, false);
		FirstModel.LoadMultipleTex(image_MineGrass_top.c_str(), texture::textureUnits::TEXTURE0, false);
		FirstModel.LoadMultipleTex(image_MineGrass_down.c_str(), texture::textureUnits::TEXTURE0, false);

		FirstModel.SetTextures("texture1", 0);
		FirstModel.SetTextures("texture2", 1);

		FirstModel.setColorModel(glm::vec3(0.5f, 0.5f, 0.4f));

		FirstModel.shaderColor.GLM_scaleTex("texTransform1", glm::vec2(1.0f, 1.0f));
		FirstModel.shaderColor.GLM_scaleTex("texTransform2", glm::vec2(0.5f, 0.5f));

		FirstModel.createVAO();
		//FirstModel.setModelProjection();
		FirstModel.setPosModel(100);

		std::map<std::string, ObjCreation::ModelCreation> ModelCreation_Mesh
		{
			{"MinecraftCube", FirstModel}

		};

		return ModelCreation_Mesh;
	}
	const std::map<std::string, Assimp::Model> setModel_Data()
	{
	
		Assimp::shaderSettings ss_Model_v1
		{
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(0.8f),
			32.0f
		};
		Assimp::coordModel coordBackPack{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 210.0f };

		std::filesystem::path pathBackpack{ backpack_Model };
		Assimp::Model modelBackpack(pathBackpack.string(), vShader_ModelT1.c_str(), fShader_ModelT1.c_str(), coordBackPack, ss_Model_v1);
		int numMeshes{ modelBackpack.numMeshes() };
		for (int i = 0; i < numMeshes; i++)
		{
			modelBackpack.SetShinessTex_Mesh(i, 32.0f);
		}

		/////////////////////CREACION DE FLOOR MODEL ///////////

		Assimp::shaderSettings ss_Model_v2
		{
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f),
			glm::vec3(0.8f),
			glm::vec3(0.5f),
			64.0f

		};

		Assimp::coordModel coord_FloorModel{ glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
		std::filesystem::path path_FloorModel{ floor2_Model };
		Assimp::Model model_Floor(path_FloorModel.string(), vShader_Standard_v1.c_str(), fShader_Standard_v1.c_str(), coord_FloorModel, ss_Model_v2);

		std::map<std::string, Assimp::Model> AssimpModels
		{
			{"backPack", modelBackpack},
			{"Floor", model_Floor}
		};

		return AssimpModels;
	}
	const std::vector<ObjCreation::ModelCreation> setMeshLight_ModelCreation_Data()
	{

		std::vector<std::array<float, 9>> vertexLight{};
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face1);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face1);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face2);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face2);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face3);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face3);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face4);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face4);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face5);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face5);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face6);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face6);

		ObjCreation::ModelCreation MeshLight_01;
		MeshLight_01.insertVertices_Fase1(vertexLight);
		MeshLight_01.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());
		//para colocar el color del objeto y de la luz

		//BasicLight.setPosModeslTransforms(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.7f), glm::vec3(0.0f, 1.0f, 0.0f), 2.0f);
		MeshLight_01.createVAO_Fase1();
		glm::vec3 randomPivotMesh{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
		glm::vec3 randomPosLight{ glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f))) };
		MeshLight_01.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		
		ObjCreation::ModelCreation MeshLight_02;
		MeshLight_02.insertVertices_Fase1(vertexLight);
		MeshLight_02.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());
	
		MeshLight_02.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 2.0f;
		MeshLight_02.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);


		ObjCreation::ModelCreation MeshLight_03;
		MeshLight_03.insertVertices_Fase1(vertexLight);
		MeshLight_03.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());

		MeshLight_03.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 1.5f;
		MeshLight_03.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);


		ObjCreation::ModelCreation MeshLight_04;
		MeshLight_04.insertVertices_Fase1(vertexLight);
		MeshLight_04.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());

		MeshLight_04.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 2.5f;
		MeshLight_04.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		////////////////////////////

		ObjCreation::ModelCreation MeshLight_05;
		MeshLight_05.insertVertices_Fase1(vertexLight);
		MeshLight_05.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());

		MeshLight_05.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 3.0f;
		MeshLight_05.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		////////////////////////////

		ObjCreation::ModelCreation MeshLight_06;
		MeshLight_06.insertVertices_Fase1(vertexLight);
		MeshLight_06.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());

		MeshLight_06.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 3.0f;
		MeshLight_06.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		////////////////////////////

		ObjCreation::ModelCreation MeshLight_07;
		MeshLight_07.insertVertices_Fase1(vertexLight);
		MeshLight_07.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());

		MeshLight_07.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 3.0f;
		MeshLight_07.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		////////////////////////////

		ObjCreation::ModelCreation MeshLight_08;
		MeshLight_08.insertVertices_Fase1(vertexLight);
		MeshLight_08.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());

		MeshLight_08.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 3.0f;
		MeshLight_08.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		////////////////////////////

		ObjCreation::ModelCreation MeshLight_09;
		MeshLight_09.insertVertices_Fase1(vertexLight);
		MeshLight_09.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());

		MeshLight_09.createVAO_Fase1();
		randomPivotMesh = randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		randomPosLight = glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		randomPosLight *= 3.0f;
		MeshLight_09.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		std::vector<ObjCreation::ModelCreation> MeshLight
		{
			MeshLight_01,
			MeshLight_02,
			MeshLight_03,
			MeshLight_04,
			MeshLight_05,
			MeshLight_06,
			MeshLight_07,
			MeshLight_08,
			MeshLight_09
		};

		return MeshLight;
	}
	const std::vector<light::light1> setPointLights()
	{

		glm::vec3 purpleLight{ 0.7f, 0.5f, 1.0f };
		glm::vec3 witheLight{ 1.0f, 1.0f, 1.0f };
		glm::vec3 green_blue{ 1.02f, 1.18f, 1.44f };
		glm::vec3 randomColor1{ 0.5, 0.6, 0.8 };
		glm::vec3 randomColor2{ 0.8, 0.4, 0.2 };
		glm::vec3 randomColor3{ 0.8, 0.4, 0.6 };
		glm::vec3 randomColor4{ 0.4, 0.7, 0.8 };
		glm::vec3 randomColor5{ 0.5, 0.4, 0.6 };
		glm::vec3 randomColor6{ 0.6, 0.3, 0.6 };
		glm::vec3 randomColor7{ 0.2, 0.7, 0.5 };
		glm::vec3 randomColor8{ 0.7, 0.8, 0.3 };	

		light::light1 pointLight_01(glm::vec3(3.0f, 3.0f, 3.0f), witheLight);
		pointLight_01.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_01.setMatProperties(witheLight, witheLight, witheLight);
	
		light::light1 pointLight_02(glm::vec3(9.0f, 4.0f, 9.0f), purpleLight);
		pointLight_02.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_02.setMatProperties(purpleLight, purpleLight, purpleLight);
		
		light::light1 pointLight_03(glm::vec3(15.0f, 5.0f, 15.0f), randomColor1);
		pointLight_03.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_03.setMatProperties(randomColor1, randomColor1, randomColor1);
		
		light::light1 pointLight_04(glm::vec3(21.0f, 6.0f, 21.0f), randomColor2);
		pointLight_04.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_04.setMatProperties(randomColor2, randomColor2, randomColor2);

		light::light1 pointLight_05(glm::vec3(27.0f, 7.0f, 27.0f), randomColor3);
		pointLight_05.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_05.setMatProperties(randomColor3, randomColor3, randomColor3);

		light::light1 pointLight_06(glm::vec3(27.0f, 8.0f, 27.0f), randomColor4);
		pointLight_06.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_06.setMatProperties(randomColor4, randomColor4, randomColor4);

		light::light1 pointLight_07(glm::vec3(27.0f, 9.0f, 27.0f), randomColor5);
		pointLight_07.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_07.setMatProperties(randomColor5, randomColor5, randomColor5);
		
		light::light1 pointLight_08(glm::vec3(27.0f, 10.0f, 27.0f), randomColor6);
		pointLight_08.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_08.setMatProperties(randomColor6, randomColor6, randomColor6);

		light::light1 pointLight_09(glm::vec3(27.0f, 3.0f, 27.0f), randomColor7);
		pointLight_09.setAttenuation(1.0f, 0.8f, 0.0035f);
		pointLight_09.setMatProperties(randomColor7, randomColor7, randomColor7);

		std::vector<light::light1> pointLights
		{
			pointLight_01,
			pointLight_02,
			pointLight_03,
			pointLight_04,
			pointLight_05,
			pointLight_06,
			pointLight_07,
			pointLight_08,
			pointLight_09
		};


		return pointLights;
	}
	const std::vector<light::DirectionalLight> setDirectionalLights()
	{
		glm::vec3 white_Color{ 1.0f, 1.0f, 1.0f };

		light::DirectionalLight directionalLight_1(glm::vec3(0.0f, -1.0f, 2.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		directionalLight_1.setMatProperties(white_Color * 0.3f, white_Color * 0.3f, white_Color * 0.3f);

		std::vector<light::DirectionalLight> DirectionalLights
		{
			directionalLight_1
		};


		return DirectionalLights;
	}
	void setSettings_FileShader(const char* fragmentShader_Path, std::vector<std::string> values)
	{

		std::ifstream readShader;

		readShader.exceptions(std::ios::failbit | std::ios::badbit);

		std::vector<std::string> renglones{};
		std::string renglon{};
		int numSkips{ 1 };

		try
		{
			readShader.open(fragmentShader_Path);

			while (std::getline(readShader, renglon))
			{
				renglones.emplace_back(renglon + '\n');
			}

			readShader.close();
		}

		catch (std::ifstream::failure iff)
		{
			SDL_Log("ERROR::READ::SETTINGS::SHADER");
			SDL_Log(iff.what());
		}

		for (auto& set : values)
		{
			set += '\n';
			renglones[numSkips] = set;
			numSkips++;
		}

		for (auto& ren : renglones)
		{

			SDL_Log(ren.c_str());
		}

		std::ofstream writeShader;
		
		writeShader.exceptions(std::ios::failbit | std::ios::badbit);
	
		try
		{
			writeShader.open(fragmentShader_Path);
			
			
				for (auto& set : renglones)
				{
					writeShader << set;
				}

			writeShader << std::endl;
			writeShader.close();
		}

		catch (std::fstream::failure ff)
		{
			SDL_Log("ERROR::OVERWRITE::SETTINGS::SHADER");
			SDL_Log(ff.what());
		}
		

	}
	void insertSettings_FileShader()
	{
		
		std::string numberPointLights{ "#define NUM_POINT_LIGHTS " + std::to_string(pointLights_D.size()) };
		std::string numberDirectionalLights{ "#define NUM_DIRECTIONAL_LIGHTS " + std::to_string(directionalLights_D.size()) };

		std::vector<std::string> setSettings_Filer
		{
			numberPointLights,
			numberDirectionalLights
		};

		setSettings_FileShader(fShader_ModelT1.c_str(), setSettings_Filer);
		setSettings_FileShader(fShader_Standard_v1.c_str(), setSettings_Filer);
		setSettings_FileShader(fShader_Model_V1.c_str(), setSettings_Filer);


	}
	
	const void set_AllObjects()
	{
		pointLights_D = setPointLights();
		directionalLights_D = setDirectionalLights();

		insertSettings_FileShader();

		ModelCreation_D = setModelCreation_Data();
		AssimpModel_D = setModel_Data();
		MeshLights_MCD = setMeshLight_ModelCreation_Data();
	}
}

namespace cameras
{

	extern camera::camera1 aerialCamera{};

	 void setCameras()
	{
		 aerialCamera.setSettingsCamera(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.1f, 100.0f);

	}

}

