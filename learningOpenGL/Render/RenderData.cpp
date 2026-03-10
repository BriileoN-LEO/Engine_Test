#include "RenderData.h"
#include "Collision/ScreenHit.h"
#include "playTest.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "Edit_Modes/Edit_M.h"
#include "2D_geo/basic_geometry_D.h"
//#include "2D_UI/Interface_UI.h"
//#include "2D_UI/2D_ScreenPlayer.h"



namespace RenderData_Set
{
	using matSettings = light::lightShader;


	//std::vector<Assimp_D::Model> models_D{};


	std::map<std::string, ObjCreation::ModelCreation> ModelCreation_D{};

	//std::map<std::string, Assimp_D::Model> AssimpModel_D{};  ////OFF FOR ONE MOMENT
	std::optional<resourceManager::manager_Model> AssimpModel_D;
	std::optional<utilities::scene> ModelsScene_D;

	std::optional<resourceManager::manager_PointLights> pointLights_D;
	std::optional<utilities_pointLight::scene_pointLights> pointLights_Scene_D;

	std::vector<ObjCreation::ModelCreation> MeshLights_MCD{};

	//std::vector<light::light1> pointLights_D{};
	std::vector<light::DirectionalLight> directionalLights_D{};
	std::map<std::string, light::SpotLight> spotLights_D{};

	std::vector<screenUI::pointerScreen> pointUI_D{};
	std::vector<individualComp::Multiple_AssimpMesh>multi_AssimpModel{};

	std::map<std::string, shading::shader>shader_D;
	std::atomic<bool> finishLoadALL{ false };

	std::map<std::string, frameBuff::frameBuffer> frameBuffers_D{};
	frameBuff::frameBuffer testFrameBuffer{};

	Assimp_D::Mesh error_MeshF{};  ///DELETE THIS IN THE FUTURE


	namespace skybox_D
	{

		std::map<std::string, sky::cubeMap_Skybox> skyBoxes_D{};
	//	std::unique_ptr<sky::cubeMap_Skybox> currentSkyBox_D{};
		std::string nameSkybox{};
		activeSkybox skyBox_Current{};

		const std::map<std::string, sky::cubeMap_Skybox> setSkyBoxes_D()
		{
			std::vector<std::string> nameFiles
			{
				"right.jpg",
				"left.jpg",
				"top.jpg",
				"bottom.jpg",
				"front.jpg",
				"back.jpg"
			};

			sky::cubeMap_Skybox skyBox_Day("skyBox_day", skyBox_Directory_01, nameFiles, "shaderSkybox_01");

			std::map<std::string, sky::cubeMap_Skybox> SB
			{
				{"skyBox_day", skyBox_Day}
			};


			return SB;
		}

	}


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
		model_Plant01.SetTexture_Mesh(image_GlassWindow.c_str(), "plant01_1", texDataManager::typeTexture::diffuse);
		model_Plant01.SetOrderRender_Mesh("plant01_1", Assimp_D::renderSeq::renderFar);
		model_Plant01.BlendModeTexture_Mesh("plant01_1", true);

        ///////////////// 


		std::map<std::string, Assimp_D::Model> AssimpModels{};
		/*{
			{"CampoVegetacion", model_Campo01},
			{"plant01", model_Plant01},
			{"backPack", modelBackpack},
		//	{"Floor", model_Floor},
			{"FlashLight", model_FlashLight}
		};*/

		return AssimpModels;
	}

	const void loadSettings_LayoutUni()
	{
		shading::settings_LayoutUni.emplace(
			shading::layoutType::MATRIX_OBJ,
			std::pair<std::string, int>("matrices_cam", 0)
		);

		shading::settings_LayoutUni.emplace(
			shading::layoutType::LIGHTS,
			std::pair<std::string, int>("lights", 1)
		);

		shading::settings_LayoutUni.emplace(  /////RESOLVE WHY I QUIT THIS AND ALL CONVERTS TO INVICIBLE
		shading::layoutType::SHADER_SETTINGS,
	    std::pair<std::string, int>("shading_settings", 2)
		);

	}
	const void loadLayoutBuffer_shader()
	{

		for (auto& layout_SS : shading::settings_LayoutUni)
		{
			const shading::layoutType& layoutT{ layout_SS.first };
			const std::string& nameBlockD{ layout_SS.second.first };
			const int& indexP{ layout_SS.second.second };

			shading::shaders_LayoutB.emplace(
				layoutT,
				shading::layouts_Data(layoutT, nameBlockD, indexP));
		}
	}

	const void loadCPU_Model_Data()
	{
		unsigned int aiProcessFlags{ aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_ImproveCacheLocality
	//	| aiProcess_CalcTangentSpace
		| aiProcess_GenSmoothNormals
		//| aiProcess_GenNormals
		| aiProcess_SortByPType 
		| aiProcess_JoinIdenticalVertices
	//	| aiProcess_PreTransformVertices
	};

		unsigned int aiProcessFlags_backpack{
			aiProcess_Triangulate 
			| aiProcess_FlipUVs 
			| aiProcess_GenSmoothNormals 
			//| aiProcess_JoinIdenticalVertices
		};

		//NEW_SET SHADERS
		std::vector<Assimp_D::shader_SetType> shaders{};
		shaders.emplace_back("shaderT1", Assimp_D::shader_type::standard_Shader);
		shaders.emplace_back("normals_shaderT1", Assimp_D::shader_type::viewNormals_Shader);


	//	std::filesystem::path pathBackpack{ backpack_Model };

		Assimp_D::loadToCPU::insertProcessModel Sponza_model
		{
		 "sponza_model",
	     sponza_2_model,
		 shaders,
		 aiProcessFlags

		};

		Assimp_D::loadToCPU::insertProcessModel back_Pack
		{
			"backPack",
			backpack_Model,
			shaders,
			aiProcessFlags

		};

		//std::filesystem::path path_FloorModel{ floor2_Model };
		Assimp_D::loadToCPU::insertProcessModel Floor
		{
			"Floor",	
			floor2_Model,
			shaders,
			aiProcessFlags

		};


		//std::filesystem::path path_FlashLight{ flashLight_Model };
		Assimp_D::loadToCPU::insertProcessModel FlashLight
		{
			"FlashLight",
			flashLight_Model,
			shaders,
			aiProcessFlags

		};


	//	std::filesystem::path path_campo_01{ campo_01 };
		Assimp_D::loadToCPU::insertProcessModel CampoVegetacion
		{
		    "CampoVegetacion",
			campo_01,
			shaders,
			aiProcessFlags
		};

		//std::filesystem::path path_plant_01{ vegetacion_01 };
		Assimp_D::loadToCPU::insertProcessModel plant01
		{
			"plant01",
			vegetacion_01,
			shaders,
			aiProcessFlags

		};

		Assimp_D::loadToCPU::insertProcessModel mirror_01
		{
			"mirror_01",
			vegetacion_01,
			shaders,
			aiProcessFlags

		};

		std::queue<Assimp_D::loadToCPU::insertProcessModel> models;
		models.push(back_Pack);
		//models.push(Floor);	//Floor,
		models.push(Sponza_model);
		models.push(FlashLight);
		models.push(CampoVegetacion);
		models.push(plant01);
		models.push(mirror_01);

		//models_D.reserve(static_cast<int>(models.size()));// LAST_WAY ---- MODELS DATA
		AssimpModel_D->reserve_size(static_cast<int>(models.size()));

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
			//ssimp_D::loadToCPU::mutexImporter.unlock();

			if (!Assimp_D::loadToCPU::modelsData.empty())
			{
				Assimp_D::loadToCPU::ModelData_loadCPU model{ Assimp_D::loadToCPU::modelsData.front() };
				
				Assimp_D::loadToCPU::modelsData.pop();

				Assimp_D::loadToCPU::mutexModel.unlock();

				--Assimp_D::loadToCPU::atomic_CounterModel;

			//	Assimp_D::Model modelP(model);  ///CORREGIR AQUI

				//AssimpModel_D.emplace(model.nameModel, std::move(modelP));
				//AssimpModel_D.emplace(model.nameModel, Assimp_D::Model(model));

				AssimpModel_D->insertModel(model.nameModel, model);

				std::cout << "LOADING::MODEL---->" << model.nameModel << '\n';

				//Assimp_D::loadToCPU::importer.FreeScene();
				Assimp_D::loadToCPU::manageImporter.notify_one();

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

			if (static_cast<int>(AssimpModel_D->size_models_D()) == actual_sizeModels)
			{
				Assimp_D::loadToCPU::finishLoadModels.store(true);
				///std::cout << "finishLoad::" << Assimp_D::loadToCPU::finishLoadModels.load() << '\n';
				//Assimp_D::loadToCPU::flagsAtomic = false;
			}
		}

	}
	const void insertSetting_toModel()
	{
		Assimp_D::Model* model {nullptr};

			Assimp_D::shaderSettings ss_Model_v1
			{
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f),
				glm::vec3(1.0f),
				glm::vec3(0.8f),
				32.0f
			};

		    Assimp_D::coordModel coordSponza { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.2f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
		    model = AssimpModel_D->model_by_str("sponza_model");
		    if (model != nullptr)
		    {
			  model->setModelSettings(coordSponza, ss_Model_v1);
		      model = nullptr;
		    }

			Assimp_D::coordModel coordBackPack{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 210.0f };
		    model = AssimpModel_D->model_by_str("backPack");
		    if (model != nullptr)
		    {
			   model->setModelSettings(coordBackPack, ss_Model_v1);
		   // 	delete model;
		    	model = nullptr;
		    }


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
			//AssimpModel_D["Floor"].setModelSettings(coord_FloorModel, ss_Model_v2);
		    model = AssimpModel_D->model_by_str("Floor");
	     	if (model != nullptr)
		    {
		    	model->setModelSettings(coord_FloorModel, ss_Model_v2);
	     	//	delete model;
	     		model = nullptr;
		    }

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
			Assimp_D::coordModel coord_FlashLight{ glm::vec3(0.0f), glm::vec3(0.001f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
			//AssimpModel_D["FlashLight"].setModelSettings(coord_FlashLight, ss_FlashLight);
		    model = AssimpModel_D->model_by_str("FlashLight");
		    if (model != nullptr)
	     	 {
		     	model->setModelSettings(coord_FlashLight, ss_FlashLight);
		    //	delete model;
		    	model = nullptr;
		     }



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
		//	AssimpModel_D["CampoVegetacion"].setModelSettings(coord_Campo01, ss_Campo01);

		     model = AssimpModel_D->model_by_str("CampoVegetacion");
		     if (model != nullptr)
		     {
			  model->setModelSettings(coord_Campo01, ss_Campo01);
		    // 	delete model;
		     	model = nullptr;
		     }


			//AssimpModel_D["CampoVegetacion"].loadTemporalShaders(vShader_ModelT1.c_str(), fShader_ModelT1.c_str());

			Assimp_D::shaderSettings ss_Plant01
			{
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(0.5f),
				glm::vec3(0.8f),
				glm::vec3(0.5f),
				32.0f
			};
			Assimp_D::coordModel coord_Plant01{ glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(1.0f), 0.0f };
		///	AssimpModel_D["plant01"].setModelSettings(coord_Plant01, ss_Plant01);
		///	AssimpModel_D["plant01"].SetTexture_Mesh(image_GlassWindow.c_str(), "plant01_1", texDataManager::typeTexture::diffuse);
		///	AssimpModel_D["plant01"].SetOrderRender_Mesh("plant01_1", Assimp_D::renderSeq::renderFar);
		///	AssimpModel_D["plant01"].BlendModeTexture_Mesh("plant01_1", true);

		    model = AssimpModel_D->model_by_str("plant01");
		    if (model != nullptr)
		    {
		    	model->setModelSettings(coord_Plant01, ss_Plant01);
		    	model->SetTexture_Mesh(image_GlassWindow.c_str(), "plant01_1", texDataManager::typeTexture::diffuse);
		    	model->SetOrderRender_Mesh("plant01_1", Assimp_D::renderSeq::renderFar);
		    	model->BlendModeTexture_Mesh("plant01_1", true);

		    //	delete model;
		    	model = nullptr;
		    }


			//AssimpModel_D["plant01"].loadTemporalShaders(vShader_ModelT1.c_str(), fShader_ModelT1.c_str());

			//////Model para el cristal
			Assimp_D::coordModel coord_Mirror_01{ glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(15.0f), glm::vec3(1.0f), 0.0f };
	//		AssimpModel_D["mirror_01"].setModelSettings(coord_Mirror_01, ss_Plant01);
		    model = AssimpModel_D->model_by_str("mirror_01");
		    if (model != nullptr)
		    {
		    	model->setModelSettings(coord_Mirror_01, ss_Plant01);
		//    	delete model;
		    	model = nullptr;
		    }

			Assimp_D::coordModel coord_TokioPlace{ glm::vec3(0.0f), glm::vec3(0.01f), glm::vec3(1.0f), 0.0f };
			///AssimpModel_D["tokio_Place"].setModelSettings(coord_TokioPlace, ss_Plant01);
		    model = AssimpModel_D->model_by_str("tokio_Place");
		    if (model != nullptr)
		    {
		    	model->setModelSettings(coord_TokioPlace, ss_Plant01);
		    //	delete model;
		    	model = nullptr;
	     	}



			testPlay::setTransformation_Objects();

			//Assimp_D::loadToCPU::finishLoadModels = false;
	}

	const void loadCPU_Shader()
	{
		std::vector <shading::layoutType> LB_01
		{
			 shading::layoutType::MATRIX_OBJ,
			 shading::layoutType::LIGHTS,
			 shading::layoutType::SHADER_SETTINGS
		};

		std::vector <shading::layoutType> LB_MAT
		{
			shading::layoutType::MATRIX_OBJ
		};

		std::vector <shading::layoutType> LB_02
		{
		    shading::layoutType::NONE
		};

		shading::loadToCPU::shaderData_loadCPU shaderT1("shaderT1", vShader_ModelT1.c_str(), fShader_ModelT1.c_str(), LB_01, gShader_ModelT1.c_str());  //TEST
		shading::loadToCPU::shaderData_loadCPU shaderT1_normals("normals_shaderT1", vShader_ModelT1_Normals.c_str(), fShader_ModelT1_Normals.c_str(), LB_MAT, gShader_ModelT1_Normals.c_str());
		shading::loadToCPU::shaderData_loadCPU shaderStandard("shaderStandard", vShader_Standard_v1.c_str(), fShader_Standard_v1.c_str(), LB_02);
		shading::loadToCPU::shaderData_loadCPU shaderFramebuffer("shaderFramebuffer", vShader_Framebuffer_V01.c_str(), fShader_Framebuffer_V01.c_str(), LB_MAT);
		shading::loadToCPU::shaderData_loadCPU shaderPoint("shaderPoint", vShader_Pointer.c_str(), fShader_Pointer.c_str(), LB_02);
		shading::loadToCPU::shaderData_loadCPU shaderSkybox_01("shaderSkybox_01", vShader_Skybox_V01.c_str(), fShader_Skybox_V01.c_str(), LB_02);
		shading::loadToCPU::shaderData_loadCPU shader_briiUI_01("brii_UI_01", vShader_briiUI_V01.c_str(), fShader_briiUI_V01.c_str(), LB_02);

		std::vector<shading::loadToCPU::shaderData_loadCPU> shadersLoad
		{
			shaderT1,
			shaderT1_normals,
			shaderStandard,
			shaderFramebuffer,
			shaderPoint,
			shaderSkybox_01,
			shader_briiUI_01,
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

				--shading::loadToCPU::atomic_CounterShader;

			    if (shaD.geometryShader_name == nullptr) /// NOT HAVE GEOMETRY SHADER
			    {
				    shader_D.emplace(shaD.nameShader, shading::shader(shaD.vertexShader_name, shaD.fragmentShader_name, shaD.data_Layout));
			    }

				else if (shaD.geometryShader_name != nullptr)  /// HAVE GEOMETRY SHADER
				{
					shader_D.emplace(shaD.nameShader, shading::shader(shaD.vertexShader_name, shaD.fragmentShader_name, shaD.geometryShader_name, shaD.data_Layout));
				}

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

	const void loadCPU_UI_editMode()
	{
		std::string shader_01{ "brii_UI_01" };


		texDataManager::TextureData_File textureArrowUI
		{
			"textureArrowUI_01",
			texDataManager::typeTexture::diffuse,
			LeftArrow_editModeUI,
			texDataManager::formatImage::standard,
			false,
			0
		};

		brii_UI::insertSpriteUI sprite_ArrowShader_State
		{
		brii_UI::typeSection_UI::arrow_compareShader,
		glm::vec2(screenSettings::screen_w, screenSettings::screen_h * 0.5f),
		std::pair<brii_UI::coord_x, brii_UI::coord_y>(20.0f, 20.0f),
		"textureArrowUI_01",
		0,
		textureArrowUI

		};
		
		std::vector<brii_UI::insertSpriteUI> sprites_editMode_CS
		{
			sprite_ArrowShader_State
		};

		brii_UI::midLevel_SpriteUI editMode_midLevel_01
		{
			sprites_editMode_CS,
			brii_UI::sectionsUI::controlShaders,
			"controlShaders"
		};

		std::vector<brii_UI::midLevel_SpriteUI> sprites_EditMenu_mid ///SPRITES DE EDITMODE-MID LEVEL 
		{
		   editMode_midLevel_01

		};
		 
		std::map<std::string, texDataManager::TextureData_File> textures_controlShaders ///A�ADIR TEXTURAS
		{
			{textureArrowUI.nameTexture, textureArrowUI}
		};

		; ///INSERCION DE LA CONFIGURACION
		brii_UI::editMode_UI_D.emplace(
			brii_UI::sectionsUI::controlShaders, 
			brii_UI::spriteUI_Dynamic(
				brii_UI::sectionsUI::controlShaders,
				static_cast<int>(sprites_editMode_CS.size()),
		    	texDataManager::TextureData_File_UI("controlShaders", textures_controlShaders),
			    shader_01) ///SHADER NAME
		); 


		brii_UI::maxLevel_SpriteUI sprites_EditMenu ///SPRITES DE EDITMODE-MAX LEVEL 
		{
			sprites_EditMenu_mid,
			brii_UI::menuSpriteUI::editMenu,
			"editMenu"
		};

		std::vector<brii_UI::maxLevel_SpriteUI> allSprites
		{
			sprites_EditMenu
		};
		
		brii_UI::loadToCPU::atomic_sizeCounterUI.store(static_cast<int>(allSprites.size()));

		std::thread threadUI(brii_UI::loadToCPU::loadUIbrii_thread, allSprites);
		threadUI.detach();

	}
    const void insertData_UI()
	{
		int counter{ brii_UI::loadToCPU::atomic_CounterUI.load()};

		if (counter > 0)
		{
			brii_UI::loadToCPU::mutex_UI.lock();

			if (!brii_UI::loadToCPU::UI_data.empty())
			{
				brii_UI::maxLevel_SpriteUI spriteToLoad{ brii_UI::loadToCPU::UI_data.front() };

				brii_UI::loadToCPU::UI_data.pop();

				brii_UI::loadToCPU::mutex_UI.unlock();

				--brii_UI::loadToCPU::atomic_CounterUI;

				if (spriteToLoad.menuType == brii_UI::menuSpriteUI::editMenu)
				{
					for (auto sprite : spriteToLoad.midLevels_UI)
					{
						for (auto infoSprite : sprite.basicLevels_UI)
						{
							brii_UI::editMode_UI_D[sprite.seccion].sprites.emplace(
								infoSprite.name,
								brii_UI::spriteUI(
									infoSprite.name,
									infoSprite.textureName,
									infoSprite.posicion,
									infoSprite.posSC,
									infoSprite.layerTexture
								)
							);

							std::cout << "LOADING::TEXTURE ----> " + infoSprite.textureName << '\n';
						}
					}

				}

			}

			else
			{
				brii_UI::loadToCPU::mutex_UI.unlock();
			}
		}

		if (brii_UI::loadToCPU::flagsReady.load())
		{
			int st{ brii_UI::loadToCPU::atomic_sizeCounterUI.load() };

			int st_f  ///sum all
			{
				static_cast<int>(brii_UI::editMode_UI_D.size())
			};

			if (st == st_f)
			{
				brii_UI::loadToCPU::finishLoadUI.store(true);
			}


		}


	
	}
	const void loadAll_DataCPU()
	{
		running_LoadingModels<void()>(loadCPU_Model_Data);
		running_LoadingShaders<void()>(loadCPU_Shader);
		running_Loading_UI<void()>(loadCPU_UI_editMode);
	}
	
	 std::vector<ObjCreation::ModelCreation> setMeshLight_ModelCreation_Data()
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
	 void setPointLights() ///SEE
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

		std::unique_ptr<light::light1> pt_light {std::make_unique<light::light1>(pointLight_01)};
		pointLights_D->insert_PL("pointLight_01", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_02);
		pointLights_D->insert_PL("pointLight_02", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_03);
		pointLights_D->insert_PL("pointLight_03", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_04);
		pointLights_D->insert_PL("pointLight_04", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_05);
		pointLights_D->insert_PL("pointLight_05", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_06);
		pointLights_D->insert_PL("pointLight_06", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_07);
		pointLights_D->insert_PL("pointLight_07", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_08);
		pointLights_D->insert_PL("pointLight_08", std::move(pt_light));

		pt_light = nullptr;
		pt_light = std::make_unique<light::light1>(pointLight_09);
		pointLights_D->insert_PL("pointLight_09", std::move(pt_light));

	}
	 void setPointLights_Scene()
	{
	  const uint32_t& size {pointLights_D->out_size() };
      for (uint32_t i = 0; i < size; i++)
      {
      	pointLights_Scene_D->insert(pointLights_D->pL_by_num(i));
      }

	  std::unique_ptr<geo_2D::point_geo> pointGEO{
	  	std::make_unique<geo_2D::point_geo>(
	  		glm::vec3(0.0f),
            glm::vec3(1.0f),
            8.0f,
            "shaderPoint"
	  	)};

		pointLights_Scene_D->setPoint_geo(std::move(pointGEO));

	}

	 std::vector<light::DirectionalLight> setDirectionalLights()
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
	 std::map<std::string, light::SpotLight> setSpotLights()
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
	 std::vector<screenUI::pointerScreen> setPointUI_2D()
	{

		glm::vec3 posScreen{ screenUI::screenWorldPos::getScreenPos(static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_h) * 0.5f, 0.0f) };
		posScreen = cameras::cameras_D[cameras::name_CurrentCamera].posCam - posScreen;  //para normalizar la posicion en 0 del punto 
		posScreen *= 2.0f;

		glm::vec3 pointColor{ 0.5f, 0.8f, 0.7f };
		float size{ 5.0f };
		screenUI::pointerScreen centerPoint_UI("shaderPoint", size, glm::vec3(0.0f), pointColor);
		
		glm::vec3 pointColor_02{ 0.4, 0.5, 0.8 };
	
		screenUI::pointerScreen pointDirPlane("shaderPoint", 20.0f, glm::vec3(0.0f), pointColor_02);

		std::vector<screenUI::pointerScreen> PointsUI
		{
			centerPoint_UI,
			//pointDirPlane
		};
		
		return PointsUI;
	}
	 std::vector<individualComp::Multiple_AssimpMesh> setMulti_AssimpModel()
	{
		////Aqui colocar el seteo de los objetos que contendran Multiples Models

		uint32_t model_name {FNV::str_to_hash("plant01")};
		uint32_t mesh_name {FNV::str_to_hash("plant01_1")};
		Assimp_D::structModelName sM_Plant01
		{
			model_name,
			mesh_name,
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

		individualComp::Multiple_AssimpMesh plant01_B(sM_Plant01, pos_Plant01);

		std::vector<individualComp::Multiple_AssimpMesh> multiMesh;

		//multiMesh.reserve(1);
		multiMesh.push_back(std::move(plant01_B));

		return multiMesh;
	}

	 std::map<std::string, frameBuff::frameBuffer> setFrameBuffers()
	{
		frameBuff::frameBuffer frameBufferScreen("screen", frameBuff::typeFrameBuffer::bufferScreen);

		Assimp_D::structModelName nameFrameBuff_01("mirror_01", "mirror_01_1");
		frameBuff::frameBuffer frameBufferModel_01("mirror_01", frameBuff::typeFrameBuffer::bufferAssimp, nameFrameBuff_01);

		std::map<std::string, frameBuff::frameBuffer> FB;
		FB.emplace("screen", std::move(frameBufferScreen));
		FB.emplace("mirror_01", std::move(frameBufferModel_01));

		return FB;
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

		//	SDL_Log(ren.c_str());
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
		
		std::string numberPointLights{ "#define NUM_POINT_LIGHTS " + std::to_string(pointLights_Scene_D->num_pointLights()) };  ///this is the size of the pointLights per scene
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

	void init_managerResources()
	{
		AssimpModel_D.emplace();
		ModelsScene_D.emplace();

		pointLights_D.emplace();
		pointLights_Scene_D.emplace();
	}

	void setModels_to_scene()
	{
		int s_MD {AssimpModel_D->size_models_D()};  ///CHECK IF THIS IS CORRECT

       for (int i = 0; i < s_MD; i++)
       {
       	Assimp_D::Model* model_ptr {AssimpModel_D->model_by_num(i)}; ///CHECK IF THIS IS CORRECT
        ModelsScene_D->insert_entity_model(std::move(model_ptr));

       	model_ptr = nullptr;

       }

	}

	 void set_AllObjects()   /////////Cambiar esta funcion para que pueda utilizar la nueva carga de Modelos
	{

		setPointLights();
		setPointLights_Scene();

		directionalLights_D = setDirectionalLights();
		spotLights_D = setSpotLights();

		insertSettings_FileShader();
		stencilTest::setStencilTest_Shader();

		SDL_Log("STATE_1");

		ModelCreation_D = setModelCreation_Data();
		SDL_Log("STATE_2");
		//AssimpModel_D = setModel_Data();   ////DESACTIVADO TEMPORALMENTE

		MeshLights_MCD = setMeshLight_ModelCreation_Data();   /////TO CREATE THE MESH OF THE SHADERS
		SDL_Log("STATE_3");


		skybox_D::skyBoxes_D = skybox_D::setSkyBoxes_D();
		//skybox_D::currentSkyBox_D = std::make_unique<sky::cubeMap_Skybox>(skybox_D::skyBoxes_D["skyBox_day"]);
		skybox_D::nameSkybox = "skyBox_day";
		skybox_D::skyBox_Current = skybox_D::activeSkybox("skyBox_day", true);
		SDL_Log("STATE_5");

		//Creacion del boundingBox
		AABB::create_BoundingBox_Mesh();
		SDL_Log("STATE_6");
		//AABB::test_BoundingBos();
		AABB::setShader_AABB();
		SDL_Log("STATE_7");

		pointUI_D = setPointUI_2D();
		SDL_Log("STATE_8");

		testFrameBuffer.loadFrameBuffer();
		SDL_Log("STATE_9");
		frameBuffers_D = setFrameBuffers();
		SDL_Log("STATE_10");

		textureCache::loadAll_PreLoadedTexturesToCache(); ///CARGA DE LAS TEXTURAS EN EL CACHE.
		SDL_Log("STATE_11");
		brii_UI::resizeUI_textures(); //TO RESIZE ALL THE TEXTURES THAT I HAVE IN MY UI
		SDL_Log("STATE_12");

		shading::register_Errors_SS::debug_BufferLayout(shader_D["shaderT1"].outID(), "lights");
		SDL_Log("STATE_13");

		setModels_to_scene();  /////PUT TO THE END TO SAVE ALL THE POINTERS CORRECTLY
		SDL_Log("STATE_14");

		multi_AssimpModel = setMulti_AssimpModel();
		SDL_Log("STATE_15");

	}

	 void running_AllObjects()
	{
		//loadLayoutBuffer_shader();  ///TO LOAD THE LAYOUTS OF UNIFORM BUFFERS IN THE SHADER

		loadSettings_LayoutUni(); ///LOADED THE BUFFER SETTINGS

		loadAll_DataCPU();  ///CRITICAL FASE TO LOAD ALL THE DATA, MODELS, SHADERS, ETC

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

		loadLayoutBuffer_shader();


	}
	
}

namespace cameras
{
	std::map<std::string, camera::camera1> cameras_D{};

	std::string name_CurrentCamera{};
	//camera::camera1 aerialCamera{};
//	camera::camera1 currentCamera{};

	void setCameras()
	{
		camera::camera1 firstPersonCam(camera::typeCam::firstPerson, glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.01, 100.0f);
		camera::camera1 editCam(camera::typeCam::editMode, glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.01, 100.0f);
//      aerialCamera.setSettingsCamera(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.001, 100.0f);
//		currentCamera = aerialCamera;

		cameras_D.emplace("cam1_firstPerson", firstPersonCam);
		cameras_D.emplace("cam2_edit", editCam);

		name_CurrentCamera = "cam1_firstPerson";

	}

	void updateStateCurrentCamera()
	{
		//currentCamera = aerialCamera;
		name_CurrentCamera = "aerialCamera";

	}

	void startInvertCurrentCamera()
	{
		//LEGACY CAM
		/*
		aerialCamera.yaw += 180.0f;
		aerialCamera.updateCameraOut();
		*/
		if (cameras_D[name_CurrentCamera].type == camera::typeCam::firstPerson)
		{
			cameras_D[name_CurrentCamera].yaw += 180.0f;
			cameras_D[name_CurrentCamera].updateCameraOut();
		}
	}

	void endInvertCurrentCamera()
	{
		//LEGACY CAM
		/*
		aerialCamera.yaw -= 180.0f;
		aerialCamera.updateCameraOut();
		*/
		if (cameras_D[name_CurrentCamera].type == camera::typeCam::firstPerson)
		{
			cameras_D[name_CurrentCamera].yaw -= 180.0f;
			cameras_D[name_CurrentCamera].updateCameraOut();
		}

	}

}

namespace Shader_Set
{
	std::map<shading::layoutType, bool> usageExistence_LT{
		{ shading::layoutType::MATRIX_OBJ, true },
		{ shading::layoutType::LIGHTS, true},
        { shading::layoutType::SHADER_SETTINGS, true}
	};

	void setUB_MatCam(bool& check_Exist)
	{
		if (shading::shaders_LayoutB.find(shading::layoutType::MATRIX_OBJ) != shading::shaders_LayoutB.end())
		{
			unsigned int& useUBO{ shading::shaders_LayoutB[shading::layoutType::MATRIX_OBJ].use() };

			glBindBuffer(GL_UNIFORM_BUFFER, useUBO);

			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cameras::cameras_D[cameras::name_CurrentCamera].cam));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cameras::cameras_D[cameras::name_CurrentCamera].camProjection));

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			
		}

		else
		{
			check_Exist = false;
		}
			/*
			if (shader_set.second.out_DataLayout() == shading::layoutType::MATRIX_OBJ)
			{
				shader_set.second.setMat4_UB("matrices_cam", "view_c", cameras::cameras_D[cameras::name_CurrentCamera].cam);
				shader_set.second.setMat4_UB("matrices_cam", "projection_c", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
			}
			*/

	}

	void setLights_SSBO(bool& check_Exist)
	{
		if (shading::shaders_LayoutB.find(shading::layoutType::LIGHTS) != shading::shaders_LayoutB.end())
		{

			std::vector<shading::setShader_DirectionLight> SS_DL{};
			std::vector<shading::setShader_PointLight> SS_PL{};
			std::vector<shading::setShader_SpotLight> SS_SL{};

			////CONTINUAR COLOCANDO LAS LUCES PARA CREAR EL BUFFER DE LUCES

			for (auto& dirLight : RenderData_Set::directionalLights_D)
			{

				SS_DL.emplace_back(
					glm::vec3(dirLight.Direction),
					0.0f,
					glm::vec3(dirLight.Mat.ambient),
					0.0f,
					glm::vec3(dirLight.Mat.diffuse),
					0.0f,
					glm::vec3(dirLight.Mat.specular),
					0
				);

			}

		    const uint32_t& size_pL{ RenderData_Set::pointLights_Scene_D->num_pointLights() };
			utilities_pointLight::entity_pL* entity_pL{ nullptr };
			for (uint32_t i = 0; i < size_pL; i++)
			{
				entity_pL = RenderData_Set::pointLights_Scene_D->entity_by_Pos(i);

				if (entity_pL != nullptr)
				    {  /// see this

					SS_PL.emplace_back(
						glm::vec3(entity_pL->pL_entity->Posicion),
						0.0f,
						glm::vec3(entity_pL->pL_entity->Mat.ambient),
						entity_pL->pL_entity->constant,
						glm::vec3(entity_pL->pL_entity->Mat.diffuse),
						entity_pL->pL_entity->linear,
						glm::vec3(entity_pL->pL_entity->Mat.specular),
						entity_pL->pL_entity->quadratic,
						glm::vec3(0.0f),
						static_cast<int>(entity_pL->pL_entity->stateLight)
					);
					entity_pL = nullptr;
				}

				else if (entity_pL == nullptr) {

					std::string log_findError {"NOT FIND THE POINT LIGHT IN POS:: POS = " + std::to_string(i)};
					register_error_RM::register_error_withSentence(log_findError.c_str());
				}

			}

			for (auto& spotLight : RenderData_Set::spotLights_D)
			{
				SS_SL.emplace_back(
					glm::vec3(spotLight.second.Posicion),
					glm::cos(glm::radians(spotLight.second.cutOff)),
					glm::vec3(spotLight.second.Direction),
					glm::cos(glm::radians(spotLight.second.outerCutOff)),
					glm::vec3(spotLight.second.Mat.ambient),
					spotLight.second.constant,
					glm::vec3(spotLight.second.Mat.diffuse),
					spotLight.second.linear,
					glm::vec3(spotLight.second.Mat.specular),
					spotLight.second.quadratic,
					glm::vec3(0.0f),
					static_cast<int>(spotLight.second.stateLight)
				);

			}
			

			size_t rangeSize_01{ static_cast<size_t>(SS_DL.size() * sizeof(shading::setShader_DirectionLight)) };
		    size_t rangeSize_02{ static_cast<size_t>(SS_PL.size() * sizeof(shading::setShader_PointLight)) };
			size_t rangeSize_03{ static_cast<size_t>(SS_SL.size() * sizeof(shading::setShader_SpotLight)) };

			size_t total{ rangeSize_01 + rangeSize_02 + rangeSize_03 };
			////CHECK IF I CAN LOAD THE LIGHTS WITH THE BUFFER 

			unsigned int& ID_SSBO{ shading::shaders_LayoutB[shading::layoutType::LIGHTS].use() };
		//	const int& indexP{ shading::shaders_LayoutB[shading::layoutType::LIGHTS].outIndexP() };

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID_SSBO);

			//glBufferData(GL_SHADER_STORAGE_BUFFER, total, nullptr, GL_DYNAMIC_DRAW);

			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, rangeSize_01, SS_DL.data());

			//static_assert(sizeof(shading::setShader_PointLight) == 64, "NO EQUAL SIZE::POINT LIGHT::BUFFER");

			//if (sizeof(shading::setShader_PointLight) != 64)
			//{
			//	std::cout << "ERROR SIZE--->" << sizeof(shading::setShader_PointLight) << '\n';
			//}

			glBufferSubData(GL_SHADER_STORAGE_BUFFER, rangeSize_01, rangeSize_02, SS_PL.data());
			

			glBufferSubData(GL_SHADER_STORAGE_BUFFER, rangeSize_01 + rangeSize_02, rangeSize_03, SS_SL.data());

			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, indexP, ID_SSBO);
			//glBindBufferRange(GL_UNIFORM_BUFFER, 1, UBO, (num_PointLights * sizeof(setShader_PointLight)) + (num_DirLights * sizeof(setShader_DirectionLight)), 2 * sizeof(glm::mat4));

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		else
		{
			check_Exist = false;
		}

	}

	void setShader_settings(bool& check_Exist)
	{

		if (shading::shaders_LayoutB.find(shading::layoutType::SHADER_SETTINGS) != shading::shaders_LayoutB.end())
		{
			shading::shading_settings_LU s_Set;
			s_Set.blinn_phong_active = static_cast<int>(edit_visualize::shading_Blinn_Phong_Active);

			unsigned int& useUBO{ shading::shaders_LayoutB[shading::layoutType::SHADER_SETTINGS].use() };

			glBindBuffer(GL_UNIFORM_BUFFER, useUBO);

			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(shading::shading_settings_LU), &s_Set);
		//	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(int), glm::value_ptr(b));

			glBindBuffer(GL_UNIFORM_BUFFER, 0);

		}

		else
		{
			check_Exist = false;
		}
	}

	void set_All_UB()
	{
		if (usageExistence_LT[shading::layoutType::MATRIX_OBJ] == true)
		{
			setUB_MatCam(usageExistence_LT[shading::layoutType::MATRIX_OBJ]);
		}

		if (usageExistence_LT[shading::layoutType::LIGHTS] == true)
		{
			setLights_SSBO(usageExistence_LT[shading::layoutType::LIGHTS]);
		}

		if (usageExistence_LT[shading::layoutType::SHADER_SETTINGS] == true)
		{
			setShader_settings(usageExistence_LT[shading::layoutType::SHADER_SETTINGS]);
		}

	}

}