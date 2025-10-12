#ifndef Render
#define Render
#include "RenderData.h"

namespace render
{
	 void render_ModelCreation_D();
	 void render_ModelAssimp_D();
	 void render_MeshLights_D();

	 void renderAll();
}


namespace openGL_render
{
	void setGlobalRender_OpenGL();
	void clearOpenGL();
	void viewportSet(int origenX, int origenY, int width, int height);

}

namespace stencil_test
{

	void renderStencilTest();
	void renderScaleUp_ST(std::vector<std::string>& notVisible);

}

namespace destroy
{
	void destroyModels();

}



#endif Render
