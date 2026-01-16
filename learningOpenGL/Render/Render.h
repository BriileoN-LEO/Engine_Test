#ifndef Render
#define Render
#include "RenderData.h"

namespace render
{

	void render_ModelCreation_D();
	void render_classicModelAssimp_D();
	void render_ModelAssimp_D(std::vector<Assimp_D::excluded_Obj> excluded_Objs = std::vector<Assimp_D::excluded_Obj>());
	void render_MultiAssimp_D();
	void render_MeshLights_D();
	void render_AABB();
	void render_Points();
	void render_Skybox();
	void render_brii_UI();

	namespace renderOP
	{
		void renderNormalScenario();
		void renderStencilTest();
		void renderDetectAABB();
		void renderEditMode();
	}

	void renderAll();
	void renderInvertAll();
	void renderPlanarReflection();
	void renderPhase();

}


namespace openGL_render
{
	void setGlobalRender_OpenGL();
	void clearOpenGL();
	void secondClearOpenGL();
	void viewportSet(int origenX, int origenY, int width, int height);

}

namespace renderSelection
{
	void renderSelection_Triangle(std::vector<Assimp_D::excluded_Obj> excluded_Objs = std::vector<Assimp_D::excluded_Obj>());
	void renderSelection_Mesh(std::vector<Assimp_D::excluded_Obj> excluded_Objs = std::vector<Assimp_D::excluded_Obj>());
	void renderSelection_Model(std::vector<Assimp_D::excluded_Obj> excluded_Objs = std::vector<Assimp_D::excluded_Obj>());
}

namespace stencil_test
{

	void renderStencilTest(std::vector<Assimp_D::excluded_Obj> excluded_Objs = std::vector<Assimp_D::excluded_Obj>());
	void renderScaleUp_ST(std::vector<std::string>& notVisible);

}

namespace refresh_Models
{
	void refreshAll_Models();
	void refreshUI_point();
	void refreshAll_LastModels();
	///añadir aqui para refrescar las posiciones y los cambios de los modelos

}

namespace destroy
{
	void destroyModels();

}



#endif Render
