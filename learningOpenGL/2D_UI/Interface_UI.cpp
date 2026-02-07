#include "Interface_UI.h"
#include "2D_UI/Interface_generalUI.h"
#include "Edit_Modes/Edit_M.h"

namespace UI_EditMode
{
	bool buttom_active{};
	editMode_S lastButtom{editMode_S::NOT_BUTTOM};

	void render_SelectionObj() {
		ImGui::Begin("EditMode");

		if (lastButtom == editMode_S::SELECTION_ON_OFF)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0, 0.5f, 0.0f, 1.0f));
	//	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(1.0f, 0.6f, 0.0f, 1.0f));

		bool* selection_on_off {new bool(ImGui::Button("selection : ON/OFF")) };

		std::array<bool*, 1> selections
		{
			std::move(selection_on_off)
		};

		int fS{};
		auto find_S = std::ranges::find_if(selections,
			[&](bool* detect) {
				++fS;
				return *detect == true;
			});

		if (find_S != selections.end())
		{
		   --fS;
           editMode_S typeEditSelection { dataConvert::s_cast<editMode_S>(fS) };

			switch (typeEditSelection)
			{
				case (editMode_S::SELECTION_ON_OFF) :

					if (lastButtom == editMode_S::SELECTION_ON_OFF)
					{
						edit_visualize::selectionMode = false;
						lastButtom = editMode_S::NOT_BUTTOM;
					}

					else if (lastButtom == editMode_S::NOT_BUTTOM)
					{
						edit_visualize::selectionMode = true;
						lastButtom = editMode_S::SELECTION_ON_OFF;
					}

					break;
			}

		}

		  ImGui::PopStyleColor(2);


		ImGui::End();
	}
}


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
			ControlScenarios::scene = ControlScenarios::stateScenarios::normalSceneario;
			ControlScenarios::cleanScenario = true;
		}

		delete returnSelection;
		returnSelection = nullptr;

		returnSelection = new bool(ImGui::Button("Camera_EditMode"));
		
		if (*returnSelection == true)
		{
			cameras::name_CurrentCamera = "cam2_edit";
			changeStateWindow();
			ControlScenarios::scene = ControlScenarios::stateScenarios::editMode_advance;  ////THIS CHANGE THE MODE OF VIEW
			ControlScenarios::cleanScenario = true;
		}
		ImGui::End();

	}

	void render_All_ImGui(SDL_Window* gWindow)
	{
		renderFirst_WindowUI();
		render_SelectionCamera(gWindow);

		if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
		{
          UI_EditMode::render_SelectionObj();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void destroyUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		for (auto& destroy_UI : brii_UI::editMode_UI_D)
		{
			destroy_UI.second.destroy();
		}
	}


}
