#ifndef Render_H
#define Render_H
#include "RenderData.h"

namespace shadowsManager
{
	enum class shadow_render : uint8_t;
}

namespace render
{
    extern bool oneTimeSee;

	void render_ModelCreation_D();
	void render_classicModelAssimp_D();
	void render_ModelAssimp_D();
	void render_MultiAssimp_D();
	void render_MeshLights_D();
	void render_AABB();
	void render_Points();
	void render_Skybox();
	void render_brii_UI();

	namespace lights
	{
	  void render_PointLight_D();
	  void renderPrepassCS_PointLights_D(shading::shader& shader);
	}

	namespace shadows
	{
      void render_AllShadowMap_dL(shading::shader* shader_shadowMap);
	  void set_renderShadowMap_dL(shadowsManager::shadow_render shadowRenderType);
      void render_currentShadowMap_dL();
	}

	namespace renderOP
	{
		void renderNormalScenario();
		void renderStencilTest();
		void renderDetectAABB();
		void renderEditMode();

		void renderEditMode_Advance();  ///////MAKING A ADVANCE SELECTION MESH TO RENDER ALL THE MESHES THAT I SELECTED
	}

	void renderAll();
	void renderInvertAll();
	void renderPlanarReflection();
	void renderPhase();

}

namespace render_ClusteredShading
{
   void update_CS();
   void render_All_CS();
}

namespace openGL_render
{
	extern bool RENDER_CS_OR_FS;  ////RENDER CLUSTERED SHADING(CS) == TRUE / RENDER FOWARD SHADING(FS) == FALSE

	void setGlobalRender_OpenGL();
	void clearOpenGL();
	void secondClearOpenGL();
	void viewportSet(int origenX, int origenY, int width, int height);

    void DRAW_RENDER();

}

namespace renderSelection
{
	void renderSelection_Triangle();
	void renderSelection_Mesh();
	void renderSelection_Model();

	void render_AdvanceSelection_Mesh();
	void render_EditMode_SelectionMesh();
}

namespace stencil_test
{

	void renderStencilTest();
	void renderScaleUp_ST(std::vector<std::string>& notVisible);

}

namespace refresh_Models
{
	void refreshAll_Models();
	void refreshUI_point();
	void refreshAll_LastModels();
	void refreshAll_LastCameras();
	///a�adir aqui para refrescar las posiciones y los cambios de los modelos

}

namespace destroy
{
	void destroyModels();
	void destroyTextures();
	void destroyLights();
    void destroyFrameBuffers();
}



#endif //Render
