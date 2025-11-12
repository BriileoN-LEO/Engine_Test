#include "RenderData.h"
#include "Collision/ScreenHit.h"
#include "playTest.h"
//#include "2D_UI/2D_ScreenPlayer.h"

namespace RenderData_Set
{
	using matSettings = light::lightShader;
	  
	std::map<std::string, ObjCreation::ModelCreation> ModelCreation_D{};
	std::map<std::string, Assimp_D::Model> AssimpModel_D{};
	std::vector<ObjCreation::ModelCreation> MeshLights_MCD{};

	std::vector<light::light1> pointLights_D{};
	std::vector<light::DirectionalLight> directionalLights_D{};
	std::map<std::string, light::SpotLight> spotLights_D{};

	std::vector<screenUI::pointerScreen> pointUI_D{};
	std::vector<individualComp::Multiple_AssimpMesh>multi_AssimpModel{};

	std::map<std::string, shading::shader>shader_D;
	std::atomic<bool> finishLoadALL{ false };


	namespace stencilTest
	{
		shading::shader stencilTest_shader{};
		const void setStencilTest_Shader()
		{
			stencilTest_shader.shaderCreation(vShader_StencilTest.c_str(), fShader_StencilTest.c_str());
		}
	}

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
	const std::map<std::string, Assimp_D::Model> setModel_Data()
	{

		Assimp_D::shaderSettings ss_Model_v1
		{
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(0.8f),
			32.0f
		};
		Assimp_D::coordModel coordBackPack{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 210.0f };

		unsigned int aiProcessFlags_BP{ aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_ImproveCacheLocality
			| aiProcess_CalcTangentSpace
			| aiProcess_GenSmoothNormals
			| aiProcess_GenNormals
			| aiProcess_SortByPType };


		std::filesystem::path pathBackpack{ backpack_Model };
		Assimp_D::Model modelBackpack(pathBackpack.string(), vShader_ModelT1.c_str(), fShader_ModelT1.c_str(), coordBackPack, ss_Model_v1, aiProcessFlags_BP);
		modelBackpack.setNameModel("backPack");
		int numMeshes{ modelBackpack.numMeshes() };
		for (int i = 0; i < numMeshes; i++)
		{
			modelBackpack.SetShinessTex_Mesh(i, 32.0f);
		}

		/////////////////////CREACION DE FLOOR MODEL ///////////

		Assimp_D::shaderSettings ss_Model_v2
		{
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f),
			glm::vec3(0.8f),
			glm::vec3(0.5f),
			64.0f

		};

		Assimp_D::coordModel coord_FloorModel{ glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
		std::filesystem::path path_FloorModel{ floor2_Model };
		Assimp_D::Model model_Floor(path_FloorModel.string(), vShader_Standard_v1.c_str(), fShader_Standard_v1.c_str(), coord_FloorModel, ss_Model_v2, aiProcessFlags_BP);
		model_Floor.setNameModel("Floor");

		//////////////////Creacion de la lampara de mano////////////////////////

		Assimp_D::shaderSettings ss_FlashLight
		{
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f),
			glm::vec3(0.8f),
			glm::vec3(0.5f),
			32.0f
		};

		unsigned int aiProcessFlags_FL{ aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_ImproveCacheLocality
			| aiProcess_CalcTangentSpace
			| aiProcess_GenSmoothNormals
			| aiProcess_GenNormals
			| aiProcess_SortByPType
			| aiProcess_FlipWindingOrder };
		
		Assimp_D::coordModel coord_FlashLight{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.001f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
		std::filesystem::path path_FlashLight{ flashLight_Model };
		Assimp_D::Model model_FlashLight(path_FlashLight.string(), vShader_ModelT1.c_str(), fShader_ModelT1.c_str(), coord_FlashLight, ss_FlashLight, aiProcessFlags_FL);
		model_FlashLight.setNameModel("FlashLight");


		///////////////////Creacion de plantas//////////////////////////
		
		unsigned int aiProcessFlags_CV{ aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_ImproveCacheLocality
			| aiProcess_CalcTangentSpace
			| aiProcess_GenSmoothNormals
			| aiProcess_GenNormals
			| aiProcess_SortByPType
			| aiProcess_FlipWindingOrder };


		Assimp_D::shaderSettings ss_Campo01
		{
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f),
			glm::vec3(0.8f),
			glm::vec3(0.5f),
			32.0f
		};

		Assimp_D::coordModel coord_Campo01{ glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(2.0f), glm::vec3(1.0f), 0.0f };
		std::filesystem::path path_campo_01{ campo_01 };
		Assimp_D::Model model_Campo01(path_campo_01.string(), vShader_ModelT1.c_str(), fShader_ModelT1.c_str(), coord_Campo01, ss_Campo01, aiProcessFlags_CV);
		model_Campo01.setNameModel("CampoVegetacion");
//		model_Campo01.SetTexture_Mesh(



		Assimp_D::shaderSettings ss_Plant01
		{
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f),
			glm::vec3(0.8f),
			glm::vec3(0.5f),
			32.0f
		};

		unsigned int aiProcessFlags_PL{ aiProcess_Triangulate
	| aiProcess_FlipUVs
	| aiProcess_ImproveCacheLocality
	| aiProcess_CalcTangentSpace
	| aiProcess_GenSmoothNormals
	| aiProcess_GenNormals
	| aiProcess_SortByPType
	| aiProcess_FlipWindingOrder };


		Assimp_D::coordModel coord_Plant01{ glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(1.0f), 0.0f };
		std::filesystem::path path_plant_01{ vegetacion_01 };
		Assimp_D::Model model_Plant01(path_plant_01.string(), vShader_ModelT1.c_str(), fShader_ModelT1.c_str(), coord_Plant01, ss_Plant01, aiProcessFlags_PL);
		model_Plant01.setNameModel("plant01");
		model_Plant01.SetTexture_Mesh(image_GlassWindow.c_str(), "plant01_1", texture::typeTextures::diffuse);
		model_Plant01.SetOrderRender_Mesh("plant01_1", Assimp_D::renderSeq::renderFar);
		model_Plant01.BlendModeTexture_Mesh("plant01_1", true);

        ///////////////// 

		std::map<std::string, Assimp_D::Model> AssimpModels
		{
			{"CampoVegetacion", model_Campo01},
			{"plant01", model_Plant01},
			{"backPack", modelBackpack},
		//	{"Floor", model_Floor},
			{"FlashLight", model_FlashLight}
		};

		return AssimpModels;
	}

	const void loadCPU_Model_Data()
	{
		unsigned int aiProcessFlags{ aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_ImproveCacheLocality
		| aiProcess_CalcTangentSpace
		| aiProcess_GenSmoothNormals
		| aiProcess_GenNormals
		| aiProcess_SortByPType };

	//	std::filesystem::path pathBackpack{ backpack_Model };
		Assimp_D::loadToCPU::insertProcessModel back_Pack
		{
			"backPack",
			backpack_Model,
			"shaderT1",
			aiProcessFlags

		};

		//std::filesystem::path path_FloorModel{ floor2_Model };
		Assimp_D::loadToCPU::insertProcessModel Floor
		{
			"Floor",	
			floor2_Model,
			"shaderT1",
			aiProcessFlags

		};


		//std::filesystem::path path_FlashLight{ flashLight_Model };
		Assimp_D::loadToCPU::insertProcessModel FlashLight
		{
			"FlashLight",
			flashLight_Model,
			"shaderT1",
			aiProcessFlags

		};


	//	std::filesystem::path path_campo_01{ campo_01 };
		Assimp_D::loadToCPU::insertProcessModel CampoVegetacion
		{
			"CampoVegetacion",
			campo_01,
			"shaderT1",
			aiProcessFlags

		};

		//std::filesystem::path path_plant_01{ vegetacion_01 };
		Assimp_D::loadToCPU::insertProcessModel plant01
		{
			"plant01",
			vegetacion_01,
			"shaderT1",
			aiProcessFlags

		};


		std::vector<Assimp_D::loadToCPU::insertProcessModel> models
		{
			back_Pack,
			//Floor,
			FlashLight,
			CampoVegetacion,
			plant01

		};

		Assimp_D::loadToCPU::atomic_sizeModels.fetch_add(static_cast<int>(models.size()));

		//std::cout << Assimp_D::loadToCPU::atomic_sizeModels.load() << '\n';

		std::thread loadThread_Models(Assimp_D::loadToCPU::loadModelsThread, models);
		
		loadThread_Models.detach();

	}
	const void insertData_toModel()
	{
		int actual_countSizeModel{ Assimp_D::loadToCPU::atomic_CounterModel.load() };

		if (actual_countSizeModel > 0)
		{
			Assimp_D::loadToCPU::mutexModel.lock();
			
			if (!Assimp_D::loadToCPU::modelsData.empty())
			{
				Assimp_D::loadToCPU::ModelData_loadCPU model{ Assimp_D::loadToCPU::modelsData.front() };
				
				Assimp_D::loadToCPU::modelsData.pop();

				Assimp_D::loadToCPU::mutexModel.unlock();

				Assimp_D::loadToCPU::atomic_CounterModel--;

				AssimpModel_D.emplace(model.nameModel, Assimp_D::Model(model));

				std::cout << "LOADING::MODEL---->" << model.nameModel << '\n';
			}
			else
			{
				Assimp_D::loadToCPU::mutexModel.unlock();
			}

		}
		
		if (Assimp_D::loadToCPU::flagsAtomic.load())
		{
			int actual_sizeModels{ Assimp_D::loadToCPU::atomic_sizeModels.load() };
			//std::cout << "finishLoad::" << actual_sizeModels << '\n';

			if (static_cast<int>(AssimpModel_D.size()) == actual_sizeModels)
			{
				Assimp_D::loadToCPU::finishLoadModels.store(true);
				///std::cout << "finishLoad::" << Assimp_D::loadToCPU::finishLoadModels.load() << '\n';
				//Assimp_D::loadToCPU::flagsAtomic = false;
			}
		}

	}
	const void insertSetting_toModel()
	{
			Assimp_D::shaderSettings ss_Model_v1
			{
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f),
				glm::vec3(1.0f),
				glm::vec3(0.8f),
				32.0f
			};
			Assimp_D::coordModel coordBackPack{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 210.0f };
			AssimpModel_D["backPack"].setModelSettings(coordBackPack, ss_Model_v1);
			//AssimpModel_D["backPack"].loadTemporalShaders(vShader_ModelT1.c_str(), fShader_ModelT1.c_str());

			///////////////////////

			Assimp_D::shaderSettings ss_Model_v2
			{
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(0.5f),
				glm::vec3(0.8f),
				glm::vec3(0.5f),
				64.0f

			};
			Assimp_D::coordModel coord_FloorModel{ glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
			AssimpModel_D["Floor"].setModelSettings(coord_FloorModel, ss_Model_v2);
			//AssimpModel_D["Floor"].loadTemporalShaders(vShader_Standard_v1.c_str(), fShader_Standard_v1.c_str());

			/////////////////////////

			Assimp_D::shaderSettings ss_FlashLight
			{
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(0.5f),
				glm::vec3(0.8f),
				glm::vec3(0.5f),
				32.0f
			};
			Assimp_D::coordModel coord_FlashLight{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.001f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
			AssimpModel_D["FlashLight"].setModelSettings(coord_FlashLight, ss_FlashLight);
			//AssimpModel_D["FlashLight"].loadTemporalShaders(vShader_ModelT1.c_str(), fShader_ModelT1.c_str());

			///////////////////////////

			Assimp_D::shaderSettings ss_Campo01
			{
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(0.5f),
				glm::vec3(0.8f),
				glm::vec3(0.5f),
				32.0f
			};
			Assimp_D::coordModel coord_Campo01{ glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(2.0f), glm::vec3(1.0f), 0.0f };
			AssimpModel_D["CampoVegetacion"].setModelSettings(coord_Campo01, ss_Campo01);
			//AssimpModel_D["CampoVegetacion"].loadTemporalShaders(vShader_ModelT1.c_str(), fShader_ModelT1.c_str());

			Assimp_D::shaderSettings ss_Plant01
			{
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(0.5f),
				glm::vec3(0.8f),
				glm::vec3(0.5f),
				32.0f
			};
			Assimp_D::coordModel coord_Plant01{ glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(1.0f), 0.0f };
			AssimpModel_D["plant01"].setModelSettings(coord_Plant01, ss_Plant01);
			AssimpModel_D["plant01"].SetTexture_Mesh(image_GlassWindow.c_str(), "plant01_1", texture::typeTextures::diffuse);
			AssimpModel_D["plant01"].SetOrderRender_Mesh("plant01_1", Assimp_D::renderSeq::renderFar);
			AssimpModel_D["plant01"].BlendModeTexture_Mesh("plant01_1", true);
			//AssimpModel_D["plant01"].loadTemporalShaders(vShader_ModelT1.c_str(), fShader_ModelT1.c_str());

			testPlay::setTransformation_Objects();

			//Assimp_D::loadToCPU::finishLoadModels = false;
	}

	const void loadCPU_Shader()
	{
		shading::loadToCPU::shaderData_loadCPU shaderT1("shaderT1", vShader_ModelT1.c_str(), fShader_ModelT1.c_str());
		shading::loadToCPU::shaderData_loadCPU shaderStandard("shaderStandard", vShader_Standard_v1.c_str(), fShader_Standard_v1.c_str());

		std::vector<shading::loadToCPU::shaderData_loadCPU> shadersLoad
		{
			shaderT1,
			shaderStandard,
		};

		shading::loadToCPU::atomic_sizeShader.fetch_add(static_cast<int>(shadersLoad.size()));

		std::thread loadShader_Thread(shading::loadToCPU::loadShadersThread, shadersLoad);

		loadShader_Thread.detach();
	}
	const void insertData_toShader()
	{
		int atomic_CounterShader{ shading::loadToCPU::atomic_CounterShader.load() };

		if (atomic_CounterShader > 0)
		{
			shading::loadToCPU::mutexShader.lock();

			if (!shading::loadToCPU::shaderData.empty())
			{

				shading::loadToCPU::shaderData_loadCPU shaD{ shading::loadToCPU::shaderData.front() };

				shading::loadToCPU::shaderData.pop();

				shading::loadToCPU::mutexShader.unlock();

				shading::loadToCPU::atomic_CounterShader--;

				shader_D.emplace(shaD.nameShader, shading::shader(shaD.vertexShader_name, shaD.fragmentShader_name));
			   
				std::cout << "LOADING::SHADER---->" << shaD.nameShader << '\n';
			}

			else
			{
				shading::loadToCPU::mutexShader.unlock();
			}
		}

		if (shading::loadToCPU::flagsAtomic.load() == true)
		{
			int actualSize_counterShader{ shading::loadToCPU::atomic_sizeShader.load() };

			if (static_cast<int>(shader_D.size()) == actualSize_counterShader)
			{
				shading::loadToCPU::finishLoadShaders.store(true);
				//shading::loadToCPU::flagsAtomic = false;
			}
		

		}

	}

	const void loadAll_DataCPU()
	{
		running_LoadingModels<void()>(loadCPU_Model_Data);
		running_LoadingShaders<void()>(loadCPU_Shader);
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
		directionalLight_1.setMatProperties(white_Color * 0.1f, white_Color * 0.1f, white_Color * 0.1f);

		std::vector<light::DirectionalLight> DirectionalLights
	 	{
			directionalLight_1
		};


		return DirectionalLights;
	}
	const std::map<std::string, light::SpotLight> setSpotLights()
	{
		matSettings matSP_1
		{
		  glm::vec3(0.0f),
		  glm::vec3(0.92f, 0.83f, 0.38f),
		  glm::vec3(0.92f, 0.83f, 0.38f),
		};

	//	glm::vec3 whiteLight{ 1.0f, 1.0f, 1.0f };
		light::SpotLight spotLight_1(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -0.1f), matSP_1, 17.5f, 21.5f);
		spotLight_1.setAttenuation(1.0f, 0.8f, 0.0035f);
	
		std::map<std::string, light::SpotLight> spotLights
		{
			{"FlashLight_SpotLight", spotLight_1}
		};

		return spotLights;
	}
	const std::vector<screenUI::pointerScreen> setPointUI_2D()
	{

		glm::vec3 posScreen{ screenUI::screenWorldPos::getScreenPos(static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_h) * 0.5f, 0.0f) };
		posScreen = cameras::aerialCamera.posCam - posScreen;  //para normalizar la posicion en 0 del punto 
		posScreen *= 2.0f;

		glm::vec3 pointColor{ 0.5f, 0.8f, 0.7f };
		float size{ 5.0f };
		screenUI::pointerScreen centerPoint_UI(vShader_Pointer.c_str(), fShader_Pointer.c_str(), size, glm::vec3(0.0f), pointColor);

		std::vector<screenUI::pointerScreen> PointsUI
		{
			centerPoint_UI
		};
		
		return PointsUI;
	}
	const std::vector<individualComp::Multiple_AssimpMesh> setMulti_AssimpModel()
	{
		////Aqui colocar el seteo de los objetos que contendran Multiples Models
		Assimp_D::structModelName sM_Plant01
		{
			"plant01",
			"plant01_1",
			true
		};	

		std::vector<glm::vec3> pos_Plant01
		{
			{4.0f, 5.0f, 2.0f},
			{3.0f, 5.0f, 1.0f},
			{2.0f, 5.0f, 3.0f},
			{2.5f, 5.0f, 4.0f},
			{4.5f, 5.0f, 1.0f},
			{1.0f, 5.0f, 3.0f},
		};

		individualComp::Multiple_AssimpMesh plant01(sM_Plant01, pos_Plant01);

		std::vector<individualComp::Multiple_AssimpMesh> multiMesh
		{
			plant01
		};

		return multiMesh;
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
		std::string numberSpotLights{ "#define NUM_SPOT_LIGHTS " + std::to_string(spotLights_D.size()) };

		std::vector<std::string> setSettings_Filer
		{
			numberPointLights,
			numberDirectionalLights,
			numberSpotLights
		};

		setSettings_FileShader(fShader_ModelT1.c_str(), setSettings_Filer);
		setSettings_FileShader(fShader_Standard_v1.c_str(), setSettings_Filer);
		setSettings_FileShader(fShader_Model_V1.c_str(), setSettings_Filer);


	}

	
	const void set_AllObjects()   /////////Cambiar esta funcion para que pueda utilizar la nueva carga de Modelos
	{
		pointLights_D = setPointLights();
		directionalLights_D = setDirectionalLights();
		spotLights_D = setSpotLights();

		insertSettings_FileShader();
		stencilTest::setStencilTest_Shader();

		ModelCreation_D = setModelCreation_Data();
		//AssimpModel_D = setModel_Data();   ////DESACTIVADO TEMPORALMENTE
		MeshLights_MCD = setMeshLight_ModelCreation_Data();

		multi_AssimpModel = setMulti_AssimpModel();

		//Creacion del boundingBox
		AABB::create_BoundingBox_Mesh();
		//AABB::test_BoundingBos();
		AABB::setShader_AABB();

		pointUI_D = setPointUI_2D();

	}
	const void running_AllObjects()
	{
		loadAll_DataCPU();

		while (!finishLoadALL.load())
		{
			//std::cout << "START_LOADED" << '\n';
			if (Assimp_D::loadToCPU::finishLoadModels.load() == true && shading::loadToCPU::finishLoadShaders.load() == true)
			{
				std::cout << "START_LOADED" << '\n';
				set_AllObjects();
				finishLoadALL = true;
			}

		}


	}
	
}

namespace cameras
{
	std::string name_CurrentCamera{};
	camera::camera1 aerialCamera{};
	camera::camera1 currentCamera{};

	void setCameras()
	{
		aerialCamera.setSettingsCamera(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.001, 100.0f);
		currentCamera = aerialCamera;
		name_CurrentCamera = "aerialCamera";

	}
	void updateStateCurrentCamera()
	{
		currentCamera = aerialCamera;
		name_CurrentCamera = "aerialCamera";

	}

}

