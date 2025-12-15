#ifndef Interface_UI
#define Interface_UI


#include "Render/RenderData.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"


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




#endif Interface_UI