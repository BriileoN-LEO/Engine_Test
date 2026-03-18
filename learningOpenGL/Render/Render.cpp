#include "Render.h"
#include "RenderData.h"
#include "threadSystem/thread_System.h"
#include "Edit_Modes/Edit_M.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "shadows_manager/shadows_D.h"
//#include "playTest.h"
//#include "Collision/ScreenHit.h"


namespace render
{
	bool oneTimeSee{};
	void render_ModelCreation_D()
	{
		for (auto& renderMCD : RenderData_Set::ModelCreation_D)
		{
			//renderMCD.second.renderMultipleModels(1, cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D, threadSystem::ControlPhysics_Events.timeInterpolation.alpha);
		//	renderMCD.second.drawModelMultiple(cameras::cameras_D[cameras::name_CurrentCamera], RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D, threadSystem::ControlPhysics_Events.timeInterpolation.alpha);
		}
	}

	void render_classicModelAssimp_D()
	{
	    RenderData_Set::ModelsScene_D->renderAll();  ///CHECK THIS
	}

	void render_ModelAssimp_D()
	{

		glEnable(GL_DEPTH_TEST); ////CONTINUE HERE

	//	std::map<std::string, float> meshesFar{}

		RenderData_Set::ModelsScene_D->render_nearPos(); // I HOPE IT WORKS

		/*
		float maxDist{ 10000.0f };

		for (int i = 0; i < static_cast<int>(meshesFar.size()); i++)
		{
			std::string meshToRender{};
			float currentDist{};

			for (auto& meshFar : meshesFar)
			{
				if (meshFar.second > currentDist && meshFar.second < maxDist)
				{
					currentDist = meshFar.second;
					meshToRender = meshFar.first;
					std::cout << meshToRender << '\n';
				}

			}

			maxDist = currentDist;
			bool breakLoop{ false };

			for (auto& renderMesh : RenderData_Set::AssimpModel_D)
			{
				std::vector<Assimp_D::Mesh>& meshesSearch{ renderMesh.second->outMeshes() };

				for (auto& mesh : meshesSearch)
				{
					if (mesh.textures.active_BlendMode == true)
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glDepthMask(GL_FALSE);
					}

					if (mesh.nameMesh == meshToRender)
					{
					//	mesh.Draw_WithLights02(RenderData_Set::shader_D["shaderT1"]); ///LAST WAY TO LOAD SHADERS

							if (oneTimeSee == false)
							{
					      	 SDL_Log(mesh.nameMesh.c_str());
							}
						mesh.Draw_WithLights(RenderData_Set::shader_D[renderMesh.second->shaders_set[pos_standardShader].name_shader],
							renderMesh.second->shaders_set[pos_normalShader].name_shader);

						//shading::shader& shaderUse{ renderMesh.second.outShader() };
						//mesh.Draw_WithLights(shaderUse);
						breakLoop = true;
						break;
					}

					if (mesh.textures.active_BlendMode == true)
					{
						glDepthMask(GL_TRUE);
						glDisable(GL_BLEND);
	
					}


				}

				if (breakLoop == true)
				{
					break;
				}
			}


		}

		oneTimeSee = true;
		*/
	}

	void render_MultiAssimp_D()
	{

		for (auto& multiAssimp_D : RenderData_Set::multi_AssimpModel)
		{
			multiAssimp_D.drawMultipleMesh();

		}
	}

	void render_MeshLights_D()
	{
		int posLight{ 0 };
		utilities_pointLight::entity_pL* pL{ nullptr };

		for (auto& renderMCD : RenderData_Set::MeshLights_MCD)
		{
			pL = RenderData_Set::pointLights_Scene_D->entity_by_Pos(posLight);
			renderMCD.renderMeshLight(cameras::cameras_D[cameras::name_CurrentCamera], pL->pL_entity->Color);
			++posLight;
		}

	}

	void render_AABB()
	{
		for (auto& renderAABB : AABB::meshBoundingBox)
		{
			renderAABB.draw();
		}
	}

	void render_Points()
	{
		for (auto& point : RenderData_Set::pointUI_D)
		{
			point.drawPoint();
		}

	}

	void render_Skybox()
	{
		//RenderData_Set::skybox_D::currentSkyBox_D->draw_Skybox();
		if (RenderData_Set::skybox_D::skyBox_Current.active == true && !RenderData_Set::skybox_D::skyBox_Current.nameSkybox.empty())
		{
			RenderData_Set::skybox_D::skyBoxes_D[RenderData_Set::skybox_D::skyBox_Current.nameSkybox].draw_Skybox();
		}
	
	}

	void render_brii_UI()
	{
		for (auto& UI : brii_UI::editMode_UI_D)
		{
			UI.second.renderBatch_UI();
		}

	}

	namespace lights
	{
		void render_PointLight_D()
		{
		  RenderData_Set::pointLights_Scene_D->renderAll();
		}
	}

	namespace shadows
	{

		void render_AllShadowMap_dL(const std::string* shader_shadowMap)
		{
			RenderData_Set::ModelsScene_D->render_nearPos_depthMapShadow(shader_shadowMap);
			shader_shadowMap = nullptr;
		}

		void set_renderShadowMap_dL()
		{
			RenderData_Set::dL_shadows_D->draw_ShadowMap();

			const std::string* shaderID { RenderData_Set::dL_shadows_D->out_shaderID()};

			if (shaderID != nullptr)
			{
				render_AllShadowMap_dL(shaderID);
			}

			else if (shaderID == nullptr)
			{
				register_error_RM::register_error_withSentence("ERROR FIND SHADER FOR SHADOW MAPPING");
			}

			//glViewport(0, 0, screenSettings::screen_w, screenSettings::screen_h);
			openGL_render::viewportSet(0, 0, screenSettings::screen_w, screenSettings::screen_h);

		}
	}

	namespace renderOP
	{
		void renderNormalScenario()
		{


			//render_Skybox();

			render_Points();
			//render_classicModelAssimp_D();
			//render_ModelCreation_D();
			//render_ModelAssimp_D(excluded_NormalScenario);///LISTO_NEW_SHADER
			//render_MultiAssimp_D();///LISTO_NEW_SHADER
			render_MeshLights_D();
			lights::render_PointLight_D();
		//	render_AABB();
			render_ModelAssimp_D();
		//	render_classicModelAssimp_D();
			//render_classicModelAssimp_D();
			//render_ModelAssimp_D();

			//render_classicModelAssimp_D();
			//render_ModelAssimp_D(excluded_NormalScenario);///LISTO_NEW_SHADER

			render_Skybox();
			render_MultiAssimp_D();///LISTO_NEW_SHADER
			//	data_HitAABB::triangleStencil.drawTest_2();

		}
		void renderStencilTest()
		{
			std::vector<Assimp_D::excluded_Obj> excluded_Stencil
			{
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, FNV::str_to_hash("mirror_01")),
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, FNV::str_to_hash("Floor")),

			};

			render_Points();
			stencil_test::renderStencilTest();///LISTO_NEW_SHADER
			render_Skybox();
			// render_AABB();
		}
		void renderDetectAABB()
		{

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Triangle)
			{

				std::vector<Assimp_D::excluded_Obj> excluded_triangle
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, FNV::str_to_hash("mirror_01")),

				};

				render_Points();
				renderSelection::renderSelection_Triangle();///LISTO_NEW_SHADER
				render_Skybox();

			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Mesh)
			{
				std::vector<uint32_t> meshID{};
				meshID.emplace_back(data_HitAABB::selectedObj.first.mesh_ID);

				std::vector<Assimp_D::excluded_Obj> excluded_Mesh
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, FNV::str_to_hash("mirror_01")),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_only_meshes, data_HitAABB::selectedObj.first.model_ID,
						meshID)

				};

				render_Points();
				renderSelection::renderSelection_Mesh();///LISTO_NEW_SHADER
				render_Skybox();

			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Model)
			{
				std::vector<Assimp_D::excluded_Obj> excluded_Model
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, FNV::str_to_hash("mirror_01")),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, data_HitAABB::selectedObj.first.model_ID)
				};


				render_Points();
				renderSelection::renderSelection_Model();///LISTO_NEW_SHADER
				render_Skybox();

			}

		}
		void renderEditMode()
		{
			if (ControlScenarios::cleanScenario == true)
			{
				data_HitAABB::resetSelectedObj();
				ControlScenarios::cleanScenario = false;
			}

			if (data_HitAABB::renderSelection == true)
			{
				std::vector<uint32_t> ms{};
				ms.emplace_back(data_HitAABB::selectedObj.first.mesh_ID);

				std::vector<Assimp_D::excluded_Obj> excluded_Mesh
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, FNV::str_to_hash("mirror_01")),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_only_meshes, data_HitAABB::selectedObj.first.model_ID,
						ms)

				};

				render_Points();
				renderSelection::renderSelection_Mesh();///LISTO_NEW_SHADER
				render_Skybox();

			}

			else if (data_HitAABB::renderSelection == false)
			{
				renderNormalScenario();
			}
		}
		void renderEditMode_Advance()
		{
			if (ControlScenarios::cleanScenario == true)
			{
				data_HitAABB::resetSelectedObj();
				ControlScenarios::cleanScenario = false;
			}

			std::vector<uint32_t> ms{};
			ms.emplace_back(data_HitAABB::selectedObj.first.mesh_ID);

			std::vector<Assimp_D::excluded_Obj> excluded_Mesh
			{
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, FNV::str_to_hash("mirror_01")),
				Assimp_D::excluded_Obj(
				Assimp_D::excludedOP::exclude_only_meshes,
				data_HitAABB::selectedObj.first.model_ID,
		        ms)
			};

		    excluded_Mesh.insert(excluded_Mesh.end(), edit_visualize::exclude_EditMeshes.begin(), edit_visualize::exclude_EditMeshes.end());

		//	for (auto meshesEx : excluded_Mesh)
			//{
		//	  SDL_Log(meshesEx.nameModel.c_str());
		//	}

			render_Points();
			///renderSelection::renderSelection_Mesh(excluded_Mesh);///LISTO_NEW_SHADER
			renderSelection::render_EditMode_SelectionMesh();
			render_Skybox();
		}

	}

	void renderAll()
	{

		switch (ControlScenarios::scene)
		{

		case ControlScenarios::stateScenarios::normalSceneario:
			 
			renderOP::renderNormalScenario();
			break;

		case ControlScenarios::stateScenarios::stencilTestAll:

			renderOP::renderStencilTest();
			break;

		case ControlScenarios::stateScenarios::detectAABB:

			renderOP::renderDetectAABB();
			break;

		case ControlScenarios::stateScenarios::edit_Scene:
		
			renderOP::renderEditMode();
		    break;

			case ControlScenarios::stateScenarios::editMode_advance :

		    renderOP::renderEditMode_Advance();

		}

	}
	void renderInvertAll()
	{
		//cameras::startInvertCurrentCamera();

		cameras::startInvertCurrentCamera();
		renderAll();
		cameras::endInvertCurrentCamera();

	}
	void renderPlanarReflection()
	{

		frameBuff_Obj::set_PlanarReflection_Dir(RenderData_Set::frameBuffers_D["mirror_01"].dataBuffer.nameAssimp.mesh_ID, cameras::cameras_D[cameras::name_CurrentCamera]); ///CHECK IF THIS WORKS
		render::renderAll();
		cameras::cameras_D[cameras::name_CurrentCamera].updateCameraOut();

	}

	void renderPhase()
	{

		//Shader_Set::set_All_UB();

		////HERE SET THE RENDER SHADOWS

		//shadows::set_renderShadowMap_dL();

		RenderData_Set::frameBuffers_D["screen"].bindFrameBuffer(); ///TO BIND THE FRAMEBUFFER
		openGL_render::clearOpenGL();

		//render::renderPlanarReflection(); ///Para renderizar el espejo invertido.
		//openGL_render::secondClearOpenGL();
		Shader_Set::set_All_UB();

		render::renderAll();

	//	render::render_brii_UI();

		openGL_render::secondClearOpenGL();  ///SECOND CLEAR
		RenderData_Set::frameBuffers_D["screen"].useFrameBufferScreen(); ///TO USE THE FRAMEBUFFER BIND

     //	RenderData_Set::frameBuffers_D["mirror_01"].useFrameBufferModel();


	}

}

namespace openGL_render
{
	void setGlobalRender_OpenGL()
	{
		/*
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		*/
		
		glEnable(GL_DEPTH_TEST);
	   // glDepthMask(GL_TRUE);
		glEnable(GL_PROGRAM_POINT_SIZE);
//		glEnable(GL_BLEND);
		glDepthFunc(GL_LESS);
		glClearDepth(1.0);
		//glDepthMask(GL_TRUE);

		
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
//if (ControlScenarios::scene == ControlScenarios::stateScenarios::detectAABB ||
	//ControlScenarios::scene == ControlScenarios::stateScenarios::stencilTestAll
//	)
//{
			glEnable(GL_STENCIL_TEST);
			//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//

	//lse if(ControlScenarios::scene == ControlScenarios::stateScenarios::normalSceneario)
	//
		//lEnable(GL_STENCIL_TEST);
	//glDisable(GL_STENCIL_TEST);
	//
		//
	//f (ControlScenarios::scene == ControlScenarios::stateScenarios::stencilTestAll ||
	//ControlScenarios::scene == ControlScenarios::stateScenarios::detectAABB
	//)

	//
		//}
		


/////nooooo
	//	glDepthFunc(GL_LESS);
	//	glDepthMask(GL_FALSE);
		
	}

	void clearOpenGL()
	{
	//RenderData_Set::testFrameBuffer.bindFrameBuffer();
	//	RenderData_Set::frameBuffers_D["mirror_01"].bindFrameBuffer();  ///se blindea el Framebuffer para recibir el render
		glDepthMask(GL_TRUE);
		//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearDepth(1.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

	}
	void secondClearOpenGL()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);
//		RenderData_Set::frameBuffers_D["Mirror"].useFrameBufferModel();
		
	    

	}

	void viewportSet(int origenX, int origenY, int width, int height)
	{
		glViewport(origenX, origenY, width, height);
		viewportOpenGL = glm::vec4(origenX, origenY, width, height);

	}
     


}

namespace renderSelection
{
	void renderSelection_Triangle()
	{
	
		if (data_HitAABB::renderSelection == true)
		{
		
		//	glStencilFunc(GL_ALWAYS, 1, 0xFF);

	//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			/*
			for (auto& renderMAD : RenderData_Set::AssimpModel_D)
			{
				renderMAD.second.Draw_WL();
			}
			*/
			

			if (data_HitAABB::setNewTriangle == true)
			{
				data_HitAABB::triangleStencil.insertTriangle();
				data_HitAABB::setNewTriangle = false;
			}

			glStencilFunc(GL_ALWAYS, 1, 0xFF);    ////PROVISIONAL OFF
			glStencilMask(0xFF);   ////PROVISIONAL OFF
			data_HitAABB::triangleStencil.drawTest_2(); ///LISTO_NEW_SHADER

			//std::cout << data_HitAABB::triangleStencil.vertex[0].posicion.x << '\t' << data_HitAABB::triangleStencil.vertex[0].posicion.y << '\t' << data_HitAABB::triangleStencil.vertex[0].posicion.z << '\n';

  ////PROVISIONAL OFF
			
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			render::lights::render_PointLight_D();
			render::render_ModelAssimp_D();
		//	render::render_ModelAssimp_D();
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();
			

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0xFF);
			glDisable(GL_DEPTH_TEST);
			data_HitAABB::triangleStencil.drawSelection();

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glEnable(GL_DEPTH_TEST);
		
		
		}

		else
		{
			//render::render_ModelAssimp_D();
			render::lights::render_PointLight_D();
			render::render_ModelAssimp_D();
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();
		}

	}
	void renderSelection_Mesh()  ////////CONTINUAR AQUI
	{

		if (data_HitAABB::renderSelection == true)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			RenderData_Set::ModelsScene_D->render_singleMesh(data_HitAABB::selectedObj.first.model_ID, data_HitAABB::selectedObj.first.mesh_ID, 1); //CHECK
		//	RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 1);///LISTO_NEW_SHADER

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			//	glDisable(GL_DEPTH_TEST);

			Assimp_D::Mesh* mesh_out {RenderData_Set::ModelsScene_D->out_mesh_fromModel(data_HitAABB::selectedObj.first.model_ID, data_HitAABB::selectedObj.first.mesh_ID)};
		//	std::vector<Assimp_D::Mesh>& meshesData{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->outMeshes() };
			glm::mat4 modelMat{ glm::mat4(1.0f) };

			RenderData_Set::stencilTest::stencilTest_shader.use();

		//	for (auto& mesh : meshesData)
		//	{
		//		if (mesh.nameMesh == data_HitAABB::selectedObj.first.nameMesh)
		//		{
			if (mesh_out != nullptr)
			{
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh_out->MeshCoord.model);
				mesh_out = nullptr;
			}

			else if (mesh_out == nullptr)
			{
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", modelMat);
			}
		//			break;
			//	}

		//	}

			//glm::mat4 modelMesh = glm::mat4(1.0f);
			//modelMesh = glm::scale(modelMesh, glm::vec3(1.1f));
			//modelMesh = meshes.MeshCoord.model * modelMesh;
			//modelMesh = modelMesh * modelMat;
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

			RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(1.0));
			RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

			//RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 0);///LISTO_NEW_SHADER
			RenderData_Set::ModelsScene_D->render_singleMesh(data_HitAABB::selectedObj.first.model_ID, data_HitAABB::selectedObj.first.mesh_ID, 0);//CHECK

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

			mesh_out = nullptr;
			//glEnable(GL_DEPTH_TEST);
	
			/*
			for (auto& renderMAD : RenderData_Set::AssimpModel_D)
			{
				if (renderMAD.second.nameModel != data_HitAABB::selectedObj.first.nameModel)
				{
					renderMAD.second.Draw_WL();
				}

				else if (renderMAD.second.nameModel == data_HitAABB::selectedObj.first.nameModel)
				{
					renderMAD.second.DrawExcludeMesh(data_HitAABB::selectedObj.first.nameMesh);
				}

			}
			*/


			//render::render_ModelAssimp_D(data_HitAABB::selectedObj.first.nameMesh); ///ExcludeMesh
			render::render_ModelAssimp_D();
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();
			render::lights::render_PointLight_D();
		}

		else
		{
		//	render::render_ModelCreation_D();
			render::render_MultiAssimp_D();
			render::render_ModelAssimp_D();
			render::render_MeshLights_D();
			render::lights::render_PointLight_D();
		}
	}
	void renderSelection_Model()
	{

		if (data_HitAABB::renderSelection == true)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

		//	RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->Draw_WL();
			RenderData_Set::ModelsScene_D->render_singleModel(data_HitAABB::selectedObj.first.model_ID);  //CHECK IF IT RENDERS

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

            utilities::entity* entity_model {RenderData_Set::ModelsScene_D->out_entity_model(data_HitAABB::selectedObj.first.model_ID)};

			//std::vector<Assimp_D::Mesh>& Meshes{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->outMeshes() };
			std::vector<Assimp_D::Mesh>& Meshes{ entity_model->model_entity->outMeshes() };

			for (auto& mesh : Meshes)
			{
				RenderData_Set::stencilTest::stencilTest_shader.use();

				RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh.MeshCoord.model);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

				RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(1.0));
				RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);
				mesh.Draw_Alone();
			}

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			entity_model = nullptr;
			/*
			for (auto& renderMAD : RenderData_Set::AssimpModel_D)
			{
				if (renderMAD.second.nameModel != data_HitAABB::selectedObj.first.nameModel)
				{
					renderMAD.second.Draw_WL();

				}

			}
		   */
			//render::render_ModelAssimp_D("", data_HitAABB::selectedObj.first.nameModel); //Exclude Model ///LISTO_NEW_SHADER
			render::render_ModelAssimp_D();
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();
			render::lights::render_PointLight_D();

		}

		else
		{
		//	render::render_ModelCreation_D();
			render::render_MultiAssimp_D();
			render::render_ModelAssimp_D();
			render::render_MeshLights_D();
			render::lights::render_PointLight_D();

		}


//		stencil_test::renderStencilTest(); ///test por el momento

	}

	void render_AdvanceSelection_Mesh()
	{
			if (data_HitAABB::renderSelection == true)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);

			//	RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 1);///LISTO_NEW_SHADER
				RenderData_Set::ModelsScene_D->render_singleMesh(data_HitAABB::selectedObj.first.model_ID, data_HitAABB::selectedObj.first.mesh_ID, 1);

				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				//	glDisable(GL_DEPTH_TEST);

				Assimp_D::Mesh* meshesData{	RenderData_Set::ModelsScene_D->out_mesh_fromModel(data_HitAABB::selectedObj.first.model_ID, data_HitAABB::selectedObj.first.mesh_ID)};   ////////PROBABLY THIS IS THE ERROR
				//std::vector<Assimp_D::Mesh>& meshesData{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->outMeshes() };
				glm::mat4 modelMat{ glm::mat4(1.0f) };

				RenderData_Set::stencilTest::stencilTest_shader.use();

			//	for (auto& mesh : meshesData)
			//	{
					if (meshesData != nullptr)
					{
						RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", meshesData->MeshCoord.model);

						meshesData = nullptr;
						//break;
					}

				    else if (meshesData == nullptr)
				    {
				    	RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", modelMat);
				    }


				//}

				//glm::mat4 modelMesh = glm::mat4(1.0f);
				//modelMesh = glm::scale(modelMesh, glm::vec3(1.1f));
				//modelMesh = meshes.MeshCoord.model * modelMesh;
				//modelMesh = modelMesh * modelMat;
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);


			//	RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.937f, 0.505f, 0.078f));  ////ORANGE

				RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.356f, 0.725f, 0.909f));  ////BLUE


				RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

				//RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 0);///LISTO_NEW_SHADER
				RenderData_Set::ModelsScene_D->render_singleMesh(data_HitAABB::selectedObj.first.model_ID, data_HitAABB::selectedObj.first.mesh_ID, 0);  ////////PROBABLY THIS IS THE ERROR
			}

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

			render::render_ModelAssimp_D();
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();
		    render::lights::render_PointLight_D();

	}
	void render_EditMode_SelectionMesh() {
		////MODIFICAR PARA SELECCIONAR


		//int size_SM { dataConvert::cast_int<decltype(edit_visualize::nameSelect_Model.size())>(edit_visualize::nameSelect_Model.size()) };
		int size_SM { static_cast<int>(edit_visualize::nameSelect_Model.size()) };


		//std::vector<std::unique_ptr<Assimp_D::structModelName>> getNameSelected_Models(size_SM);
		std::vector<Assimp_D::structModelName*> getNameSelected_Models(size_SM);
		//std::vector<Assimp_D::structModelName> getNameSelected_Models{};

		glm::vec3& posCamera{cameras::cameras_D[cameras::name_CurrentCamera].posCam};

		std::vector<float> pos_Detect{};

		for (auto sMP : edit_visualize::nameSelect_Model)
		{

		   //Assimp_D::Mesh& getMesh {RenderData_Set::AssimpModel_D[sMP.names.nameModel]->outSpecificMesh(sMP.names.nameMesh) };
             Assimp_D::Mesh* getMesh {RenderData_Set::ModelsScene_D->out_mesh_fromModel(sMP.names.model_ID, sMP.names.mesh_ID )};

			if (getMesh != nullptr)
			{
				glm::vec3& posObj_P {getMesh->MeshCoord.posModel};

				float dist_t{glm::distance2(posObj_P, posCamera)};

				pos_Detect.emplace_back(dist_t);

				getMesh = nullptr;
			}
		}


		for (int i = 0; i < size_SM; i++)
		{
			int pos_D{ size_SM - 1 };

		   for (int d = 0; d < size_SM; d++)
		   {
			 if (pos_Detect[i] < pos_Detect[d] && i != d)
			 {
			   --pos_D;
			 }
		   }
			//getNameSelected_Models.emplace_back(std::make_unique<Assimp_D::structModelName>(edit_visualize::nameSelect_Model[pos_D]));

			//std::unique_ptr<Assimp_D::structModelName> container_Sp{std::make_unique<Assimp_D::structModelName>(edit_visualize::nameSelect_Model[pos_D].names)};
			//getNameSelected_Models.emplace_back(std::move(container_Sp));
			getNameSelected_Models.emplace_back(&edit_visualize::nameSelect_Model[pos_D].names);
		}

		for (int sP = 0; sP < static_cast<int>(getNameSelected_Models.size()); sP++)
		{
			if (getNameSelected_Models[sP] != nullptr)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask( 0xFF);

			//	RenderData_Set::AssimpModel_D[getNameSelected_Models[sP]->nameModel]->DrawSingleMesh(getNameSelected_Models[sP]->nameMesh, 1);///LISTO_NEW_SHADER
				RenderData_Set::ModelsScene_D->render_singleMesh(getNameSelected_Models[sP]->model_ID, getNameSelected_Models[sP]->mesh_ID, 1);

				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
			//	Assimp_D::Mesh& mesh_Model {RenderData_Set::AssimpModel_D[getNameSelected_Models[sP]->nameModel]->outSpecificMesh(getNameSelected_Models[sP]->nameMesh)};
				Assimp_D::Mesh* mesh_Model {RenderData_Set::ModelsScene_D->out_mesh_fromModel(getNameSelected_Models[sP]->model_ID, getNameSelected_Models[sP]->mesh_ID )};

				RenderData_Set::stencilTest::stencilTest_shader.use();
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh_Model->MeshCoord.model);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

				mesh_Model = nullptr;

			   if (getNameSelected_Models[sP]->model_ID == data_HitAABB::selectedObj.first.model_ID &&
			   	getNameSelected_Models[sP]->mesh_ID == data_HitAABB::selectedObj.first.mesh_ID)
			    {
			    	RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.937f, 0.505f, 0.078f));
			    }
				else
				{
					RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.8f, 5.0f, 0.3f));
				}

				RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

			//	RenderData_Set::AssimpModel_D[getNameSelected_Models[sP]->nameModel]->DrawSingleMesh(getNameSelected_Models[sP]->nameMesh, 0);
				RenderData_Set::ModelsScene_D->render_singleMesh(getNameSelected_Models[sP]->model_ID, getNameSelected_Models[sP]->mesh_ID, 0);

				getNameSelected_Models[sP] = nullptr;
			}
		}

		getNameSelected_Models.clear();

		render_AdvanceSelection_Mesh();

	}
}

namespace stencil_test
{

	void renderStencilTest()
	{
		
		//calculateAllScreenHit();

		glStencilMask(0x00);
		std::string back_Excluded{ "Floor" };
		//RenderData_Set::AssimpModel_D[back_Excluded]->Draw_WL(); //LISTO_NEW_SHADER
		RenderData_Set::ModelsScene_D->render_singleModel(FNV::str_to_hash(back_Excluded));  ///CONTINUE HERE

		render::render_ModelCreation_D();
		render::render_MeshLights_D();


		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		//render::render_ModelCreation_D();

		/*
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			if (renderMAD.first != back_Excluded)
			{
				renderMAD.second.Draw_WL();
			}
		}
		*/

		render::render_ModelAssimp_D();
			
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		std::vector<std::string> notModelsStencil
		{
			back_Excluded
		};
		renderScaleUp_ST(notModelsStencil);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	void renderScaleUp_ST(std::vector<std::string>& notVisible)
	{

		for (auto& nameNV : notVisible)
		{
		    uint32_t model_ID_conv {FNV::str_to_hash(nameNV)};

            utilities::entity* model_entity{RenderData_Set::ModelsScene_D->out_entity_model(model_ID_conv)}; //MAKING ALL THE CHANGES

			if (model_entity != nullptr)
			{
             std::vector<Assimp_D::Mesh>& meshes_entities {model_entity->model_entity->outMeshes()};

			 for (auto& mesh : meshes_entities)
			 {
			 	glm::mat4 modelMesh = glm::mat4(1.0f);
			 	modelMesh = glm::scale(modelMesh, glm::vec3(1.2f));
			 	//modelMesh = meshes.MeshCoord.model * modelMesh;
			 	modelMesh = modelMesh * mesh.MeshCoord.model;

			 	RenderData_Set::stencilTest::stencilTest_shader.use();

			 	RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh.MeshCoord.model);
			 	RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
			 	RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

			 	RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

			 	mesh.Draw_Alone();
			 }

				model_entity = nullptr;
			}

			else if (model_entity == nullptr)
			{
				std::string log_error {"MISSING MODEL::NOT FIND THE MODEL::(" + nameNV + ")"};
				register_error_RM::register_error_withSentence(log_error.c_str());
			}


		}



/*
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			bool pass{ false };
			
			for (int i = 0; i < static_cast<int>(notVisible.size()); i++)
			{
				if(renderMAD.first == notVisible[i])
				{
					pass = true;
				}

			}
			
			if (pass == false)
			{
				std::vector<Assimp_D::Mesh>& meshesData{ renderMAD.second->outMeshes() };
				for (auto& meshes : meshesData)
				{
					glm::mat4 modelMesh = glm::mat4(1.0f);
					modelMesh = glm::scale(modelMesh, glm::vec3(1.2f));
			//modelMesh = meshes.MeshCoord.model * modelMesh;
					modelMesh = modelMesh * meshes.MeshCoord.model;

					RenderData_Set::stencilTest::stencilTest_shader.use();

					RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", meshes.MeshCoord.model);
					RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
					RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

					RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

					meshes.Draw_Alone();

				}
			}
		}
		*/
	}

}

namespace refresh_Models
{
	void refreshAll_Models()
	{

		const uint32_t& size_m {RenderData_Set::ModelsScene_D->size_VM()};


		for (uint32_t r = 0; r < size_m; r++)
		{
			utilities::entity* entity_model{RenderData_Set::ModelsScene_D->out_entity_model_byPos(r)};

           if (entity_model != nullptr) {
	           entity_model->model_entity->updateModel();
           	entity_model = nullptr;
           }

		}

	//	for (auto& renderMAD : RenderData_Set::AssimpModel_D)
	//	{
	//		renderMAD.second->updateModel();
	//	}

		//RenderData_Set::pointUI_D[0].updatePoint();
		
		/*
		for (auto& renderPoint : RenderData_Set::pointUI_D)
		{
			renderPoint.updatePoint();

		}
		*/
	}
	void refreshUI_point()
	{
		RenderData_Set::pointUI_D[0].updatePoint();

	}

	void refreshAll_LastModels()
	{
		const uint32_t& size_m {RenderData_Set::ModelsScene_D->size_VM()};


		for (uint32_t r = 0; r < size_m; r++)
	    {
			utilities::entity* entity_model{RenderData_Set::ModelsScene_D->out_entity_model_byPos(r)};

			if (entity_model != nullptr)
			{
              entity_model->model_entity->ModelCoord.lastModel =  entity_model->model_entity->ModelCoord.model;  //NEW METHOD
			}

		}

//		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
//		{
//			renderMAD.second->ModelCoord.lastModel = renderMAD.second->ModelCoord.model; //LAST METHOD
		//	renderMAD.second.ModelCoord.lastTranslateM = renderMAD.second.ModelCoord.translateM;
		//	renderMAD.second.ModelCoord.lastScaleS = renderMAD.second.ModelCoord.scaleS;
		//	renderMAD.second.ModelCoord.lastRotateR = renderMAD.second.ModelCoord.rotateR;

//		}
	}

	///añadir aqui para refrescar las posiciones y los cambios de los modelos

}


namespace destroy
{
	void destroyModels()
	{

		RenderData_Set::ModelsScene_D->cleanAll_scene();
		RenderData_Set::AssimpModel_D->clean_data();
	//	const uint32_t& size_m {RenderData_Set::ModelsScene_D->size_VM()};


	//	for (uint32_t r = 0; r < size_m; r++)
	//	{
	//		utilities::entity* entity_model{RenderData_Set::ModelsScene_D->out_entity_model_byPos(r)};

	//		if (entity_model != nullptr)
	//		{
	//			entity_model->model_entity->destroyModel();  //NEW METHOD
	//		}

		//}

//		for (auto& val : RenderData_Set::AssimpModel_D)
//		{
//			RenderData_Set::AssimpModel_D[val.first]->destroyModel(); ///LAST METHOD
//		}

		for (auto& AABB_Mesh : AABB::meshBoundingBox)
		{
			AABB_Mesh.destroy();
		}

	}
	void destroyLights()
	{
		RenderData_Set::pointLights_Scene_D->clean_data();
	    RenderData_Set::pointLights_D->clean_data();
	}
	void destroyFrameBuffers()
	{
	   for (auto& FB : RenderData_Set::frameBuffers_D)
	   {
         FB.second.clear();
	   }
	}
}
