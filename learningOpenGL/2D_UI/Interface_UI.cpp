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
		float& rot_skyBox{ RenderData_Set::skybox_D::skyBoxes_D["skyBox_day"].transform_SkyBox.rad };

		ImGui::SliderFloat("Rotation_skybox", &rot_skyBox, -360.0, 360.0);
		ImGui::End();

	}

	void render_SelectionCamera(SDL_Window* gWindow)
	{
		auto changeStateWindow = [&]()
			{
				if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
				{
					SDL_SetWindowRelativeMouseMode(gWindow, false);
				}

			};
		

		ImGui::Begin("CameraSelection");
		bool* returnSelection{ nullptr };
		returnSelection = new bool(ImGui::Button("Camera_firstPerson"));

		if (*returnSelection == true)
		{
			cameras::name_CurrentCamera = "cam1_firstPerson";
			changeStateWindow();
		}

		delete returnSelection;
		returnSelection = nullptr;

		returnSelection = new bool(ImGui::Button("Camera_EditMode"));
		
		if (*returnSelection == true)
		{
			cameras::name_CurrentCamera = "cam2_edit";
			changeStateWindow();
	
		}
		ImGui::End();

	}

	void render_All_ImGui(SDL_Window* gWindow)
	{
		renderFirst_WindowUI();
		render_SelectionCamera(gWindow);

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