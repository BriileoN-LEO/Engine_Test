#ifndef Interface_UI
#define Interface_UI

#include "Render/RenderData.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"


namespace UI_EditMode
{
 enum class editMode_S : uint8_t
	{
      SELECTION_ON_OFF = 0,
 	  SELECTION_EXPLODE = 1,
 	  SELECTION_SHOW_NORMALS = 2,

 	  SHADING_BLINN_PHONG_ON_OFF = 3,
 	  NOT_BUTTOM = 4,
    };

	struct exit_Buttom_EM
	{
	  editMode_S buttom{editMode_S::NOT_BUTTOM};
	  bool press{ false };
	};

extern bool buttom_active;
extern std::array<exit_Buttom_EM, 4> lastButtom;
extern std::array<bool*, 4> buttom_detect;

//extern bool* selection_on_off;
//extern bool* selection_explode;
//extern bool* selection_show_normals;

//extern bool* selection_blinn_phong_on_off;

 void render_colorButtom(const bool& buttom_press);
 void render_Buttom(const editMode_S& buttom);

 void render_SelectionObj();
 void render_shading_selection();

 void render_pLRadio_Slider();

}

namespace UI
{
	extern std::unique_ptr<ImGuiIO> IO;
	extern std::unique_ptr<ImGuiStyle> style_UI;

	void init_imGUI(SDL_Window* window, SDL_GLContext contextOpenGL);

	void start_NewFrameUI();

	void renderFirst_WindowUI();

	void render_SelectionCamera(SDL_Window* gWindow);
	
	void render_All_ImGui(SDL_Window* gWindow);

	void destroyUI();
 
}



#endif //Interface_UI