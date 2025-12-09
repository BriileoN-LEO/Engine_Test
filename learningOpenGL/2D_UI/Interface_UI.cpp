#include "Interface_UI.h"

namespace UI
{
	std::unique_ptr<ImGuiIO> IO{ nullptr };
	std::unique_ptr<ImGuiStyle> style_UI{ nullptr };


	void init_imGUI(SDL_Window* window, SDL_GLContext contextOpenGL)
	{
		float scaleGUI{ SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()) };
		const char* version_glsl{ "#version 130" };

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
	    IO = std::make_unique< ImGuiIO>(ImGui::GetIO());
		//ImGuiIO& IO = ImGui::GetIO();

		IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();

		style_UI = std::make_unique<ImGuiStyle>(ImGui::GetStyle());
		//ImGuiStyle& style_UI = ImGui::GetStyle();

		style_UI->ScaleAllSizes(scaleGUI);
		style_UI->FontScaleDpi = scaleGUI;

		ImGui_ImplSDL3_InitForOpenGL(window, contextOpenGL);
		ImGui_ImplOpenGL3_Init(version_glsl);
		
	}

	void start_NewFrameUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void renderFirst_WindowUI()
	{
		ImGui::Begin("DemoWindow");
		ImGui::Button("Hello");
		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void destroyUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

	}


}