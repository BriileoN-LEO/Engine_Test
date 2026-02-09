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
 	  NOT_BUTTOM = 2,
    };

	struct exit_Buttom_EM
	{
	  editMode_S buttom{editMode_S::NOT_BUTTOM};
	  bool press{ false };
	};

extern bool buttom_active;
extern std::array<exit_Buttom_EM, 2> lastButtom;

 void render_SelectionObj();

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