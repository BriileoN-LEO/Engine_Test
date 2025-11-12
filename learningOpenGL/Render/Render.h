#ifndef Render
#define Render
#include "RenderData.h"

namespace render
{
	void render_ModelCreation_D();
	void render_classicModelAssimp_D();
	void render_ModelAssimp_D(std::string excludedMesh = "", std::string excludeModel = "");
	void render_MultiAssimp_D();
	void render_MeshLights_D();
	void render_AABB();
	void render_Points();

	void renderAll();
}

namespace openGL_render
{
	void setGlobalRender_OpenGL();
	void clearOpenGL();
	void viewportSet(int origenX, int origenY, int width, int height);

}

namespace renderSelection
{
	void renderSelection_Triangle();
	void renderSelection_Mesh();
	void renderSelection_Model();
}

namespace stencil_test
{

	void renderStencilTest();
	void renderScaleUp_ST(std::vector<std::string>& notVisible);

}

namespace refresh_Models
{
	void refreshAll_Models();
	///añadir aqui para refrescar las posiciones y los cambios de los modelos

}

namespace destroy
{
	void destroyModels();

}



#endif Render
