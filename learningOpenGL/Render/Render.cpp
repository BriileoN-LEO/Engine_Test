#include "Render.h"
#include "RenderData.h"
#include "Collision/ScreenHit.h"


namespace render
{
	void render_ModelCreation_D() 
	 {
		for(auto& renderMCD : RenderData_Set::ModelCreation_D)
		{
			renderMCD.second.renderMultipleModels(1, cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D);
		}
	}

	void render_ModelAssimp_D() 
	 {
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			renderMAD.second.Draw_WL(cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D);
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

	void renderAll()
	{
		render_ModelCreation_D();
		render_ModelAssimp_D();
		render_MeshLights_D();
	}


}

namespace openGL_render
{
	void setGlobalRender_OpenGL()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		

	//	glDepthFunc(GL_LESS);
	//	glDepthMask(GL_FALSE);
		
	}

	void clearOpenGL()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	}

	void viewportSet(int origenX, int origenY, int width, int height)
	{
		glViewport(origenX, origenY, width, height);
		viewportOpenGL = glm::vec4(origenX, origenY, width, height);

	}
     


}

namespace stencil_test
{

	void renderStencilTest()
	{
		
		calculateAllScreenHit();

		glStencilMask(0x00);
		std::string back_Excluded{ "Floor" };
		RenderData_Set::AssimpModel_D[back_Excluded].Draw_WL(cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D);
		render::render_ModelCreation_D();
		render::render_MeshLights_D();


		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		//render::render_ModelCreation_D();

		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			if (renderMAD.first != back_Excluded)
			{
				renderMAD.second.Draw_WL(cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D, RenderData_Set::spotLights_D);
			}
		}
		
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
				std::vector<Assimp::Mesh>& meshesData{ renderMAD.second.outMeshes() };
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

					meshes.Draw_Alone();

				}
			}
		}

	}

}

namespace destroy
{
	void destroyModels()
	{
		for (auto& val : RenderData_Set::AssimpModel_D)
		{
			RenderData_Set::AssimpModel_D[val.first].destroyModel();
		}
	}

}
