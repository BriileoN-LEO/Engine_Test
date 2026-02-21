#include "Render.h"
#include "RenderData.h"
#include "threadSystem/thread_System.h"
#include "Edit_Modes/Edit_M.h"
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
			renderMCD.second.drawModelMultiple(cameras::cameras_D[cameras::name_CurrentCamera], RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D, threadSystem::ControlPhysics_Events.timeInterpolation.alpha);
		}
	}

	void render_classicModelAssimp_D()
	{
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			renderMAD.second->Draw_WL();
		}
	}

	void render_ModelAssimp_D(std::vector<Assimp_D::excluded_Obj> excluded_Objs)
	{

		glEnable(GL_DEPTH_TEST);

		std::map<std::string, float> meshesNear{};
		std::map<std::string, float> meshesFar{};

		for (auto& modelSearch : RenderData_Set::AssimpModel_D)
		{
			bool pass_excluded_model{};

			for (auto excludedModel : excluded_Objs)
			{
				if (modelSearch.second->nameModel == excludedModel.nameModel && excludedModel.exclude_Type == Assimp_D::excludedOP::exclude_complete_model)
				{
					pass_excluded_model = true;
					break;
				}
			}

			if (pass_excluded_model == false)
			{
				std::vector<Assimp_D::Mesh>& meshesSearch{ modelSearch.second->outMeshes() };

				for (auto& meshS : meshesSearch)
					{
					bool pass_excluded_mesh{};
                    bool find_mesh{};

					for (auto excludedModel : excluded_Objs)
					{
						if (modelSearch.second->nameModel == excludedModel.nameModel)
						{
							for (auto excludedMesh : excludedModel.nameMeshes)
							{
								if (meshS.nameMesh == excludedMesh && excludedModel.exclude_Type == Assimp_D::excludedOP::exclude_only_meshes)
								{
									pass_excluded_mesh = true;
									find_mesh = true;
                                    break;
								}

							}

							if (find_mesh == true)
							{
								break;
							}
						}

					}

					if (pass_excluded_mesh == false)
					{
						float dist{ glm::distance(meshS.MeshCoord.posModel,  cameras::cameras_D[cameras::name_CurrentCamera].posCam) };
                     //   std::cout << dist << std::endl;s
                       // SDL_Log(std::to_string(dist).c_str());

					//	if (oneTimeSee == false)
					//	{
						// SDL_Log(meshS.nameMesh.c_str());
					//	}

						switch (meshS.renderP)
						{
						case Assimp_D::renderSeq::renderNear:
							meshesNear.emplace(meshS.nameMesh, dist);
							break;

						case Assimp_D::renderSeq::renderFar:
							meshesFar.emplace(meshS.nameMesh, dist);
							break;
						}
						/**
						if (meshS.renderP == Assimp::renderSeq::renderNear)
						{
							meshesNear.emplace(meshS.nameMesh, dist);
						}

						else if (meshS.renderP == Assimp::renderSeq::renderFar)
						{
							meshesFar.emplace(meshS.nameMesh, dist);

						}
						*/
					}
				}
			}
		}

		Uint8 pos_standardShader {static_cast<Uint8>(Assimp_D::shader_type::standard_Shader)};
		Uint8 pos_normalShader {static_cast<Uint8>(Assimp_D::shader_type::viewNormals_Shader)};
		float minNear{};

		for (int i = 0; i < static_cast<int>(meshesNear.size()); i++)
		{
			std::string meshToRender{};
			float maxNear{ 10000.0f };

			for (auto& meshNear : meshesNear)
			{
				if (meshNear.second < maxNear && meshNear.second > minNear)
				{
					maxNear = meshNear.second;
					meshToRender = meshNear.first;
				}

			}

			minNear = maxNear;
			bool breakLoop{ false };

			for (auto& renderMesh : RenderData_Set::AssimpModel_D)
			{
				std::vector<Assimp_D::Mesh>& meshesSearch{ renderMesh.second->outMeshes() };

				for (auto& mesh : meshesSearch)
				{
					if (mesh.nameMesh == meshToRender)
					{
						if (oneTimeSee == false)
						{
							SDL_Log(mesh.nameMesh.c_str());
						}
						//mesh.Draw_WithLights02(RenderData_Set::shader_D["shaderT1"]);  ///LAST WAY TO LOAD SHADERS
						mesh.Draw_WithLights(
							RenderData_Set::shader_D[renderMesh.second->shaders_set[pos_standardShader].name_shader],
				            renderMesh.second->shaders_set[pos_normalShader].name_shader);
						// mesh.Draw_WithLights(renderMesh.second.outShader()); //DESACTIVADO TEMPORALMENTE
						breakLoop = true;
						break;
					}
				}

				if (breakLoop == true)
				{
					break;
				}
			}
		}

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
		for (auto& renderMCD : RenderData_Set::MeshLights_MCD)
		{
			renderMCD.renderMeshLight(cameras::cameras_D[cameras::name_CurrentCamera], RenderData_Set::pointLights_D[posLight]);
			posLight++;
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


	namespace renderOP
	{
		void renderNormalScenario()
		{
			std::vector<Assimp_D::excluded_Obj> excluded_NormalScenario
			{
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),
			};


			//render_Skybox();

			render_Points();
			//render_classicModelAssimp_D();
			//render_ModelCreation_D();
			//render_ModelAssimp_D(excluded_NormalScenario);///LISTO_NEW_SHADER
			//render_MultiAssimp_D();///LISTO_NEW_SHADER
			render_MeshLights_D();
			render_AABB();
			render_ModelAssimp_D(excluded_NormalScenario);
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
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "Floor"),

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
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),

				};

				render_Points();
				renderSelection::renderSelection_Triangle(excluded_triangle);///LISTO_NEW_SHADER
				render_Skybox();

			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Mesh)
			{
				std::vector<Assimp_D::excluded_Obj> excluded_Mesh
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_only_meshes, data_HitAABB::selectedObj.first.nameModel,
						std::vector<std::string>({data_HitAABB::selectedObj.first.nameMesh}))

				};

				render_Points();
				renderSelection::renderSelection_Mesh(excluded_Mesh);///LISTO_NEW_SHADER
				render_Skybox();

			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Model)
			{
				std::vector<Assimp_D::excluded_Obj> excluded_Model
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, data_HitAABB::selectedObj.first.nameModel)
				};


				render_Points();
				renderSelection::renderSelection_Model(excluded_Model);///LISTO_NEW_SHADER
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
				std::vector<Assimp_D::excluded_Obj> excluded_Mesh
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_only_meshes, data_HitAABB::selectedObj.first.nameModel,
						std::vector<std::string>({data_HitAABB::selectedObj.first.nameMesh}))

				};

				render_Points();
				renderSelection::renderSelection_Mesh(excluded_Mesh);///LISTO_NEW_SHADER
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

			std::vector<Assimp_D::excluded_Obj> excluded_Mesh
			{
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),
				Assimp_D::excluded_Obj(
				Assimp_D::excludedOP::exclude_only_meshes,
				data_HitAABB::selectedObj.first.nameModel,
		        std::vector<std::string>({data_HitAABB::selectedObj.first.nameMesh}))
			};

		    excluded_Mesh.insert(excluded_Mesh.end(), edit_visualize::exclude_EditMeshes.begin(), edit_visualize::exclude_EditMeshes.end());

		//	for (auto meshesEx : excluded_Mesh)
			//{
		//	  SDL_Log(meshesEx.nameModel.c_str());
		//	}

			render_Points();
			///renderSelection::renderSelection_Mesh(excluded_Mesh);///LISTO_NEW_SHADER
			renderSelection::render_EditMode_SelectionMesh(excluded_Mesh);
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

		/*
		if (ControlScenarios::scene == ControlScenarios::stateScenarios::normalSceneario)
		{
			std::vector<Assimp_D::excluded_Obj> excluded_NormalScenario
			{
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),
			};


			//render_Skybox();

			render_Points();
			//render_classicModelAssimp_D();
			//render_ModelCreation_D();
			//render_ModelAssimp_D(excluded_NormalScenario);///LISTO_NEW_SHADER
			//render_MultiAssimp_D();///LISTO_NEW_SHADER
			render_MeshLights_D();	
			render_AABB();
			render_ModelAssimp_D(excluded_NormalScenario);
			//render_classicModelAssimp_D();
			//render_ModelAssimp_D(excluded_NormalScenario);///LISTO_NEW_SHADER

			render_Skybox();
			render_MultiAssimp_D();///LISTO_NEW_SHADER
		//	data_HitAABB::triangleStencil.drawTest_2();
	
		}

		else if (ControlScenarios::scene == ControlScenarios::stateScenarios::stencilTestAll)
		{
			std::vector<Assimp_D::excluded_Obj> excluded_Stencil
			{
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),
				Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "Floor"),
				
			};

			render_Points();
			stencil_test::renderStencilTest();///LISTO_NEW_SHADER
			render_Skybox();
		   // render_AABB();
		}

		else if (ControlScenarios::scene == ControlScenarios::stateScenarios::detectAABB)
		{
			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Triangle)
			{

				std::vector<Assimp_D::excluded_Obj> excluded_triangle
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),

				};

				render_Points();
				renderSelection::renderSelection_Triangle(excluded_triangle);///LISTO_NEW_SHADER
				render_Skybox();
		
			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Mesh)
			{
				std::vector<Assimp_D::excluded_Obj> excluded_Mesh
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_only_meshes, data_HitAABB::selectedObj.first.nameModel, 
						std::vector<std::string>({data_HitAABB::selectedObj.first.nameMesh}))

				};

				render_Points();
				renderSelection::renderSelection_Mesh(excluded_Mesh);///LISTO_NEW_SHADER
				render_Skybox();
		
			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Model)
			{
				std::vector<Assimp_D::excluded_Obj> excluded_Model
				{
					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, "mirror_01"),

					Assimp_D::excluded_Obj(Assimp_D::excludedOP::exclude_complete_model, data_HitAABB::selectedObj.first.nameModel)
				};


				render_Points();
				renderSelection::renderSelection_Model(excluded_Model);///LISTO_NEW_SHADER
				render_Skybox();
			
			}
 
		}

		else if (ControlScenarios::scene == ControlScenarios::stateScenarios::edit_Scene)
		{
			if (ControlScenarios::cleanScenario == true)
			{
				data_HitAABB::resetSelectedObj();
				ControlScenarios::cleanScenario = false;
			}

			if()

		}
		*/
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

		frameBuff_Obj::set_PlanarReflection_Dir(RenderData_Set::frameBuffers_D["mirror_01"].dataBuffer.nameAssimp.nameMesh, cameras::cameras_D[cameras::name_CurrentCamera]);
		render::renderAll();
		cameras::cameras_D[cameras::name_CurrentCamera].updateCameraOut();

	}

	void renderPhase()
	{
		openGL_render::clearOpenGL();

		//render::renderPlanarReflection(); ///Para renderizar el espejo invertido.
		//openGL_render::secondClearOpenGL();
		Shader_Set::set_All_UB();

		render::renderAll();

		render::render_brii_UI();

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
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearDepth(1.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

	}
	void secondClearOpenGL()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
	void renderSelection_Triangle(std::vector<Assimp_D::excluded_Obj> excluded_Objs)
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
			render::render_ModelAssimp_D(excluded_Objs);
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
			render::render_ModelAssimp_D(excluded_Objs);
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();
		}

	}
	void renderSelection_Mesh(std::vector<Assimp_D::excluded_Obj> excluded_Objs)  ////////CONTINUAR AQUI
	{

		if (data_HitAABB::renderSelection == true)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 1);///LISTO_NEW_SHADER

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			//	glDisable(GL_DEPTH_TEST);

			std::vector<Assimp_D::Mesh>& meshesData{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->outMeshes() };
			glm::mat4 modelMat{ glm::mat4(1.0f) };

			RenderData_Set::stencilTest::stencilTest_shader.use();

			for (auto& mesh : meshesData)
			{
				if (mesh.nameMesh == data_HitAABB::selectedObj.first.nameMesh)
				{
					RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh.MeshCoord.model);
					break;
				}

			}

			//glm::mat4 modelMesh = glm::mat4(1.0f);
			//modelMesh = glm::scale(modelMesh, glm::vec3(1.1f));
			//modelMesh = meshes.MeshCoord.model * modelMesh;
			//modelMesh = modelMesh * modelMat;
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

			RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(1.0));
			RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

			RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 0);///LISTO_NEW_SHADER

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
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
			render::render_ModelAssimp_D(excluded_Objs);
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();
		}

		else
		{
		//	render::render_ModelCreation_D();
			render::render_MultiAssimp_D();
			render::render_ModelAssimp_D();
			render::render_MeshLights_D();
		}
	}
	void renderSelection_Model(std::vector<Assimp_D::excluded_Obj> excluded_Objs)
	{

		if (data_HitAABB::renderSelection == true)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->Draw_WL();

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			std::vector<Assimp_D::Mesh>& Meshes{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->outMeshes() };
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
			render::render_ModelAssimp_D(excluded_Objs);
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();

		}

		else
		{
		//	render::render_ModelCreation_D();
			render::render_MultiAssimp_D();
			render::render_ModelAssimp_D();
			render::render_MeshLights_D();

		}


//		stencil_test::renderStencilTest(); ///test por el momento

	}

	void render_AdvanceSelection_Mesh(std::vector<Assimp_D::excluded_Obj>excluded_Objs)
	{
			if (data_HitAABB::renderSelection == true)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);

				RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 1);///LISTO_NEW_SHADER

				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				//	glDisable(GL_DEPTH_TEST);

				std::vector<Assimp_D::Mesh>& meshesData{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->outMeshes() };
				glm::mat4 modelMat{ glm::mat4(1.0f) };

				RenderData_Set::stencilTest::stencilTest_shader.use();

				for (auto& mesh : meshesData)
				{
					if (mesh.nameMesh == data_HitAABB::selectedObj.first.nameMesh)
					{
						RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh.MeshCoord.model);
						break;
					}

				}

				//glm::mat4 modelMesh = glm::mat4(1.0f);
				//modelMesh = glm::scale(modelMesh, glm::vec3(1.1f));
				//modelMesh = meshes.MeshCoord.model * modelMesh;
				//modelMesh = modelMesh * modelMat;
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);


			//	RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.937f, 0.505f, 0.078f));  ////ORANGE

				RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.356f, 0.725f, 0.909f));  ////BLUE


				RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

				RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel]->DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 0);///LISTO_NEW_SHADER
			}

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

			render::render_ModelAssimp_D(excluded_Objs);
			render::render_MultiAssimp_D();
		//	render::render_ModelCreation_D();
			render::render_MeshLights_D();

	}
	void render_EditMode_SelectionMesh(std::vector<Assimp_D::excluded_Obj> excluded_Objs) {
		////MODIFICAR PARA SELECCIONAR


		//int size_SM { dataConvert::cast_int<decltype(edit_visualize::nameSelect_Model.size())>(edit_visualize::nameSelect_Model.size()) };
		int size_SM { static_cast<int>(edit_visualize::nameSelect_Model.size()) };


		std::vector<std::unique_ptr<Assimp_D::structModelName>> getNameSelected_Models(size_SM);
		//std::vector<Assimp_D::structModelName> getNameSelected_Models{};

		glm::vec3& posCamera{cameras::cameras_D[cameras::name_CurrentCamera].posCam};

		std::vector<float> pos_Detect{};

		for (auto sMP : edit_visualize::nameSelect_Model)
		{
		   Assimp_D::Mesh& getMesh {RenderData_Set::AssimpModel_D[sMP.names.nameModel]->outSpecificMesh(sMP.names.nameMesh) };

			if (static_cast<int>(getMesh.nameMesh.size()) != 0)
			{
				glm::vec3& posObj_P {getMesh.MeshCoord.posModel};

				float dist_t{glm::distance2(posObj_P, posCamera)};

				pos_Detect.emplace_back(dist_t);
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
			std::unique_ptr<Assimp_D::structModelName> container_Sp{std::make_unique<Assimp_D::structModelName>(edit_visualize::nameSelect_Model[pos_D].names)};
			getNameSelected_Models.emplace_back(std::move(container_Sp));
		}

		for (int sP = 0; sP < static_cast<int>(getNameSelected_Models.size()); sP++)
		{
			if (getNameSelected_Models[sP] != nullptr) {
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask( 0xFF);

				RenderData_Set::AssimpModel_D[getNameSelected_Models[sP]->nameModel]->DrawSingleMesh(getNameSelected_Models[sP]->nameMesh, 1);///LISTO_NEW_SHADER

				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				Assimp_D::Mesh& mesh_Model {RenderData_Set::AssimpModel_D[getNameSelected_Models[sP]->nameModel]->outSpecificMesh(getNameSelected_Models[sP]->nameMesh)};

				RenderData_Set::stencilTest::stencilTest_shader.use();
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh_Model.MeshCoord.model);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);


			   if (getNameSelected_Models[sP]->nameModel == data_HitAABB::selectedObj.first.nameModel &&
			   	getNameSelected_Models[sP]->nameMesh == data_HitAABB::selectedObj.first.nameMesh)
			    {
			    	RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.937f, 0.505f, 0.078f));
			    }
				else
				{
					RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(0.8f, 5.0f, 0.3f));
				}

				RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

				RenderData_Set::AssimpModel_D[getNameSelected_Models[sP]->nameModel]->DrawSingleMesh(getNameSelected_Models[sP]->nameMesh, 0);
			}
		}

		getNameSelected_Models.clear();

		render_AdvanceSelection_Mesh(excluded_Objs);

	}
}

namespace stencil_test
{

	void renderStencilTest(std::vector<Assimp_D::excluded_Obj> excluded_Objs)
	{
		
		//calculateAllScreenHit();

		glStencilMask(0x00);
		std::string back_Excluded{ "Floor" };
		RenderData_Set::AssimpModel_D[back_Excluded]->Draw_WL(); //LISTO_NEW_SHADER
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

		render::render_ModelAssimp_D(excluded_Objs);
			
		
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

	}

}

namespace refresh_Models
{
	void refreshAll_Models()
	{
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			renderMAD.second->updateModel();
		}

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
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			renderMAD.second->ModelCoord.lastModel = renderMAD.second->ModelCoord.model;
		//	renderMAD.second.ModelCoord.lastTranslateM = renderMAD.second.ModelCoord.translateM;
		//	renderMAD.second.ModelCoord.lastScaleS = renderMAD.second.ModelCoord.scaleS;
		//	renderMAD.second.ModelCoord.lastRotateR = renderMAD.second.ModelCoord.rotateR;

		}
	}

	///añadir aqui para refrescar las posiciones y los cambios de los modelos

}


namespace destroy
{
	void destroyModels()
	{
		for (auto& val : RenderData_Set::AssimpModel_D)
		{
			RenderData_Set::AssimpModel_D[val.first]->destroyModel();
		}
		
		for (auto& AABB_Mesh : AABB::meshBoundingBox)
		{
			AABB_Mesh.destroy();
		}

	}

}
