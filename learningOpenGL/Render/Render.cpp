#include "Render.h"
#include "RenderData.h"
//#include "playTest.h"
//#include "Collision/ScreenHit.h"


namespace render
{
	void render_ModelCreation_D() 
	 {
		for(auto& renderMCD : RenderData_Set::ModelCreation_D)
		{
			renderMCD.second.renderMultipleModels(1, cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D);
		}
	}

	void render_classicModelAssimp_D()
	{
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
			{
				renderMAD.second.Draw_WL();
		    }
	}

	void render_ModelAssimp_D(std::string excludedMesh, std::string excludeModel)
	 {
		
//		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
//		{
//			renderMAD.second.Draw_WL();
//		}


		///////RESOLVER AQUIIII AHHHHHHS 

		std::map<std::string, float> meshesNear{};
		std::map<std::string, float> meshesFar{};

		for (auto& modelSearch : RenderData_Set::AssimpModel_D)
		{
			if (modelSearch.second.nameModel != excludeModel)
			{
				std::vector<Assimp_D::Mesh>& meshesSearch{ modelSearch.second.outMeshes() };

				for (auto& meshS : meshesSearch)
				{
					if (meshS.nameMesh != excludedMesh)
					{
						float dist{ glm::distance(meshS.MeshCoord.posModel, cameras::aerialCamera.posCam) };

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


		float minNear{};		

		for (int i = 0; i < static_cast<int>(meshesNear.size()); i++)
		{
			std::string meshToRender{};
			float maxNear{ 10000 };

			 for(auto& meshNear : meshesNear)
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
				 std::vector<Assimp_D::Mesh>& meshesSearch{ renderMesh.second.outMeshes() };

				 for (auto& mesh : meshesSearch)
				 {
					 if (mesh.nameMesh == meshToRender)
					 {
					
						 mesh.Draw_WithLights(RenderData_Set::shader_D[renderMesh.second.nameShader]);
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

		float maxDist{ 10000 };

		for (int i = 0; i < static_cast<int>(meshesFar.size()); i++)
		{
			std::string meshToRender{};
			float currentDist{};

			for (auto& meshFar : meshesFar)
			{
				if(meshFar.second > currentDist && meshFar.second < maxDist)
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
				std::vector<Assimp_D::Mesh>& meshesSearch{ renderMesh.second.outMeshes() };

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
						mesh.Draw_WithLights(RenderData_Set::shader_D[renderMesh.second.nameShader]);
						//shading::shader& shaderUse{ renderMesh.second.outShader() };
						//mesh.Draw_WithLights(shaderUse);
						breakLoop = true;
						break;
					}

					if (mesh.textures.active_BlendMode == true)
					{
						glDisable(GL_BLEND);
						glDepthMask(GL_TRUE);
					}


				}

				if (breakLoop == true)
				{
					break;
				}
			}
			

		}
	

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
			renderMCD.renderMeshLight(cameras::aerialCamera, RenderData_Set::pointLights_D[posLight]);
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

	void renderAll()
	{
		if (ControlScenarios::scene == ControlScenarios::stateScenarios::normalSceneario)
		{
			render_Points();
			//render_classicModelAssimp_D();
			render_ModelCreation_D();
			render_ModelAssimp_D();///LISTO_NEW_SHADER
			render_MultiAssimp_D();///LISTO_NEW_SHADER
			render_MeshLights_D();	
			render_AABB();
		//	data_HitAABB::triangleStencil.drawTest_2();
	
		}

		else if (ControlScenarios::scene == ControlScenarios::stateScenarios::stencilTestAll)
		{
			render_Points();
			stencil_test::renderStencilTest();///LISTO_NEW_SHADER
		   // render_AABB();
		}

		else if (ControlScenarios::scene == ControlScenarios::stateScenarios::detectAABB)
		{
			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Triangle)
			{
				render_Points();
				renderSelection::renderSelection_Triangle();///LISTO_NEW_SHADER
		
			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Mesh)
			{
				render_Points();
				renderSelection::renderSelection_Mesh();///LISTO_NEW_SHADER
		
			}

			if (ControlScenarios::sceneAABB == ControlScenarios::scenarioAABB::Model)
			{
				render_Points();
				renderSelection::renderSelection_Model();///LISTO_NEW_SHADER
			
			}
 
		}

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
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
			

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			data_HitAABB::triangleStencil.drawTest_2(); ///LISTO_NEW_SHADER


			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			render::render_ModelAssimp_D();
			render::render_ModelCreation_D();
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
			render::render_ModelAssimp_D();
			render::render_ModelCreation_D();
			render::render_MeshLights_D();
		}

	}
	void renderSelection_Mesh()  ////////CONTINUAR AQUI
	{

		if (data_HitAABB::renderSelection == true)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel].DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 1);///LISTO_NEW_SHADER

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			//	glDisable(GL_DEPTH_TEST);

			std::vector<Assimp_D::Mesh>& meshesData{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel].outMeshes() };
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
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::aerialCamera.cam);
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::aerialCamera.camProjection);

			RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 0);

			RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel].DrawSingleMesh(data_HitAABB::selectedObj.first.nameMesh, 0);///LISTO_NEW_SHADER

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
			render::render_ModelAssimp_D(data_HitAABB::selectedObj.first.nameMesh); ///ExcludeMesh

			render::render_ModelCreation_D();
			render::render_MeshLights_D();
		}

		else
		{
			render::render_ModelCreation_D();
			render::render_ModelAssimp_D();
			render::render_MeshLights_D();
		}
	}
	void renderSelection_Model()
	{

		if (data_HitAABB::renderSelection == true)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel].Draw_WL();

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			std::vector<Assimp_D::Mesh>& Meshes{ RenderData_Set::AssimpModel_D[data_HitAABB::selectedObj.first.nameModel].outMeshes() };
			for (auto& mesh : Meshes)
			{
				RenderData_Set::stencilTest::stencilTest_shader.use();

				RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", mesh.MeshCoord.model);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::aerialCamera.cam);
				RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::aerialCamera.camProjection);

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
			render::render_ModelAssimp_D("", data_HitAABB::selectedObj.first.nameModel); //Exclude Model ///LISTO_NEW_SHADER

			render::render_ModelCreation_D();
			render::render_MeshLights_D();

		}

		else
		{
			render::render_ModelCreation_D();
			render::render_ModelAssimp_D();
			render::render_MeshLights_D();

		}


//		stencil_test::renderStencilTest(); ///test por el momento

	}

}

namespace stencil_test
{

	void renderStencilTest()
	{
		
		//calculateAllScreenHit();

		glStencilMask(0x00);
		std::string back_Excluded{ "Floor" };
		RenderData_Set::AssimpModel_D[back_Excluded].Draw_WL(); //LISTO_NEW_SHADER
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

		render::render_ModelAssimp_D("", back_Excluded);
			
		
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
				std::vector<Assimp_D::Mesh>& meshesData{ renderMAD.second.outMeshes() };
				for (auto& meshes : meshesData)
				{
					glm::mat4 modelMesh = glm::mat4(1.0f);
					modelMesh = glm::scale(modelMesh, glm::vec3(1.2f));
			//modelMesh = meshes.MeshCoord.model * modelMesh;
					modelMesh = modelMesh * meshes.MeshCoord.model;

					RenderData_Set::stencilTest::stencilTest_shader.use();

					RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", meshes.MeshCoord.model);
					RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::aerialCamera.cam);
					RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::aerialCamera.camProjection);

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
			renderMAD.second.updateModel();
		}


		for (auto& renderPoint : RenderData_Set::pointUI_D)
		{
			renderPoint.updatePoint();

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
			RenderData_Set::AssimpModel_D[val.first].destroyModel();
		}
		
		for (auto& AABB_Mesh : AABB::meshBoundingBox)
		{
			AABB_Mesh.destroy();
		}

	}

}
