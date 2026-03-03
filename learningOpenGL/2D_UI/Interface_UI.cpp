#include "Interface_UI.h"
#include "2D_UI/Interface_generalUI.h"
#include "Edit_Modes/Edit_M.h"

namespace UI_EditMode
{
	bool buttom_active{};
	std::array<exit_Buttom_EM, 4> lastButtom
	{
		exit_Buttom_EM(editMode_S::SELECTION_ON_OFF, false),
		exit_Buttom_EM(editMode_S::SELECTION_EXPLODE, false),
		exit_Buttom_EM(editMode_S::SELECTION_SHOW_NORMALS, false),

		exit_Buttom_EM(editMode_S::SHADING_BLINN_PHONG_ON_OFF, false)

	};


	 std::array<bool*, 4> buttom_detect
	{
	  new bool(false),
      new bool(false),
	  new bool(false),
	  new bool(false)
	};

//	bool* selection_on_off { nullptr };
//	bool* selection_explode { nullptr };
//	bool* selection_show_normals { nullptr };

	void render_colorButtom(const bool& buttom_press)
	{
		if (buttom_press == true)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
		}
		else if (buttom_press == false)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0, 0.5f, 0.0f, 1.0f));

	}

	void render_Buttom(const editMode_S& buttom)
	{
		uint32_t pos_buttom{static_cast<uint32_t>(buttom)};

		switch (buttom)
		{
			case (editMode_S::SELECTION_ON_OFF)	:
				buttom_detect[pos_buttom] = nullptr;
				buttom_detect[pos_buttom] = new bool(ImGui::Button("selection : ON/OFF"));
				break;
			case (editMode_S::SELECTION_EXPLODE) :
				buttom_detect[pos_buttom]  = nullptr;
				buttom_detect[pos_buttom]  = new bool(ImGui::Button("selection explode"));
				break;
			case (editMode_S::SELECTION_SHOW_NORMALS) :
				buttom_detect[pos_buttom]  = nullptr;
				buttom_detect[pos_buttom]  = new bool(ImGui::Button("selection show normals"));
				break;
			case(editMode_S::SHADING_BLINN_PHONG_ON_OFF) :
				const char* str_blinn {nullptr};

				if (lastButtom[pos_buttom].press == false)
				{
					str_blinn = "Blinn Phong : ON";
				}

				else if (lastButtom[pos_buttom].press == true)
				{
					str_blinn = "Blinn Phong : OFF";
				}

				buttom_detect[pos_buttom]  = nullptr;
				buttom_detect[pos_buttom]  = new bool(ImGui::Button(str_blinn));  ///TEST THIS

		}
	}

	void render_SelectionObj() {
		//bool* selection_on_off { new bool(ImGui::Button("selection : ON/OFF")) };

		ImGui::Begin("EditMode");


		uint32_t pos {static_cast<uint32_t>(editMode_S::SHADING_BLINN_PHONG_ON_OFF)};

		for (uint32_t i = 0; i < pos; i++)
		{
			render_colorButtom(lastButtom[i].press);
			render_Buttom(lastButtom[i].buttom);
		 }

		std::array<bool*, 3> selections
		{
			buttom_detect[static_cast<uint32_t>(editMode_S::SELECTION_ON_OFF)],
			buttom_detect[static_cast<uint32_t>(editMode_S::SELECTION_EXPLODE)],
			buttom_detect[static_cast<uint32_t>(editMode_S::SELECTION_SHOW_NORMALS)],
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

					if (lastButtom[fS].press == true)
					{
						edit_visualize::selectionMode = false;
						lastButtom[fS].press = false;
					}

					else if (lastButtom[fS].press == false)
					{
						edit_visualize::selectionMode = true;
				        lastButtom[fS].press = true;
					}

					break;

				case (editMode_S::SELECTION_EXPLODE) :
					if (lastButtom[fS].press == true)
					{
						edit_visualize::exploded_objs_Active = false;
						lastButtom[fS].press = false;
					}

					else if (lastButtom[fS].press == false)
					{
						edit_visualize::exploded_objs_Active = true;
						lastButtom[fS].press = true;
					}
				break;

				case (editMode_S::SELECTION_SHOW_NORMALS) :
					if (lastButtom[fS].press == true)
					{
						edit_visualize::show_normals_active = false;
						lastButtom[fS].press = false;
					}

					else if (lastButtom[fS].press == false)
					{
						edit_visualize::show_normals_active = true;
						lastButtom[fS].press = true;
					}
					break;
			}

		}

		for (uint32_t i = 0; i < static_cast<uint32_t>(selections.size()); i++)
		{
			selections[i] = nullptr;
		}

		  ImGui::PopStyleColor(2 * static_cast<int>(selections.size()));


		ImGui::End();
	}

	void render_shading_selection()
	{
		ImGui::Begin("shading_settings");

		uint32_t pos {static_cast<uint32_t>(editMode_S::SHADING_BLINN_PHONG_ON_OFF)};

		for (uint32_t i = pos; i < pos + 1; i++)
		{
			render_colorButtom(lastButtom[i].press);
			render_Buttom(lastButtom[i].buttom);
		}

		std::array<bool*, 1> selections
		{
			buttom_detect[static_cast<uint32_t>(editMode_S::SHADING_BLINN_PHONG_ON_OFF)],
		};

		int fS{dataConvert::cast_int<editMode_S>(editMode_S::SHADING_BLINN_PHONG_ON_OFF)};
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
				case (editMode_S::SHADING_BLINN_PHONG_ON_OFF) :

					if (lastButtom[fS].press == false)
					{
						edit_visualize::shading_Blinn_Phong_Active = true;
						lastButtom[fS].press = true;

					}

					else if (lastButtom[fS].press == true)
					{
						edit_visualize::shading_Blinn_Phong_Active = false;
						lastButtom[fS].press = false;
                        SDL_Log("PRESS::BUTTOM");
					}

				break;

			}

		}

		for (uint32_t i = 0; i < static_cast<uint32_t>(selections.size()); i++)
		{
			*selections[i] = false;
			selections[i] = nullptr;
		}

		ImGui::PopStyleColor(2 * static_cast<int>(selections.size()));


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
		  UI_EditMode::render_shading_selection();
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
