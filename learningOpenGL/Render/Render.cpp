#include "Render.h"
#include "RenderData.h"

namespace render
{
	void render_ModelCreation_D() 
	 {
		for(auto& renderMCD : RenderData_Set::ModelCreation_D)
		{
				renderMCD.second.renderMultipleModels(1, cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D);
		}
	}

	void render_ModelAssimp_D() 
	 {
		for (auto& renderMAD : RenderData_Set::AssimpModel_D)
		{
			renderMAD.second.Draw_WL(cameras::aerialCamera, RenderData_Set::pointLights_D, RenderData_Set::directionalLights_D);
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