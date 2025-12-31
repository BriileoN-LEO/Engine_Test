

#include "2D_UI/Interface_UI.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include "learningOpenGL.h"
#include "config_window.h"
#include "data_save.h"
#include "control.h"
#include "SHADER_H.h"
#include "playTest.h"
#include "LIGHTS_test.h"
#include "ModelAssimp.h"
#include "stb_image.h"
#include "render/configFilesTXT.h"
#include "Render/RenderData.h"
#include "Render/Render.h"
#include "collision/ScreenHit.h"
#include "threadSystem/thread_System.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <cassert>
#include <filesystem>

SDL_Renderer* gRender{ nullptr };
SDL_GLContext contextOpenGl;
SDL_Window* gWindow{ nullptr };
SDL_Cursor* gCursor{ nullptr };
bool correct_init{ true };

void init()
{

	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		SDL_Log("<zno se pudo abrir la ventana");
		SDL_Log(SDL_GetError());
		assert(gWindow != nullptr);
		correct_init = false;
	}

	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		SDL_WindowFlags windowFlag{ SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY };

		gWindow = SDL_CreateWindow("LearningOpenGL", screenSettings::screen_w, screenSettings::screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE); //Creacion de la ventana

		if (gWindow == nullptr)
		{
			SDL_Log("No se pudo crear la ventana");
			SDL_Log(SDL_GetError());
			correct_init = false;
			assert(gWindow != nullptr);
		}

		else
		{
			contextOpenGl = SDL_GL_CreateContext(gWindow);  //Creacion del contexto

			if (contextOpenGl == NULL)
			{
				SDL_Log("Error al crear el contexto de OpenGL");
				SDL_Log(SDL_GetError());
				SDL_DestroyWindow(gWindow);
				SDL_Quit();
				correct_init = false;
				assert(contextOpenGl != nullptr);
			}

			else
			{
				
				if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) //Inializador de glad
				{
					SDL_Log("Fallo al inicializar glad");
					SDL_Log(SDL_GetError());
					bool correct_init = false;

					

				}
			}


		}

	}
}

enum class scenes
{
	distintFragmentShader_2 = 0,
	distintPos_3 = 1,
	seqTriangles_4 = 2,
	colorTriangle_5 = 3,
	VertexColorChange_6 = 4,
	sizeScenes = 5

};



int main(int argc, char* argv[])
{
	init();

	UI::init_imGUI(gWindow, contextOpenGl);

	openGL_render::viewportSet(0, 0, screenSettings::screen_w, screenSettings::screen_h);

	scenes scenesChange[static_cast<int>(scenes::sizeScenes)]
	{
		scenes::distintFragmentShader_2,
		scenes::distintPos_3,
		scenes::seqTriangles_4,
		scenes::colorTriangle_5,
		scenes::VertexColorChange_6
	};

	int controlScene{};

	camera::camState camP{ camera::camState::cameraAE };
	cameras::setCameras();
	//RenderData_Set::set_AllObjects(); DESACTIVADO TEMPORALMENTE
	RenderData_Set::running_AllObjects();
	testPlay::setTransformation_Objects();

	SDL_SetWindowRelativeMouseMode(gWindow, true);
	SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);
	//screenSettings::vSync::inFPS(screenSettings::fps); 

	openGL_render::setGlobalRender_OpenGL();

//penGL_render::setGlobalRender_OpenGL();
	 
	//threadSystem::ControlPhysics_System ControlPhysics_Events(60);
	threadSystem::ControlPhysics_Events.initializeThread(60);
	screenSettings::vSync syncFPS(screenSettings::fps);
	syncFPS.syncThread_Time(threadSystem::ControlPhysics_Events.cv_start, threadSystem::ControlPhysics_Events.startSystem);

	if (correct_init == true)
	{

		SDL_Event event;
		bool loopEvent{ false };

		while (!loopEvent)
		{

			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL3_ProcessEvent(&event);

				if (event.type == SDL_EVENT_QUIT)
				{
					loopEvent = true;
				}

				if (event.type == SDL_EVENT_WINDOW_RESIZED)
				{
					screenSettings::framebuffer_size_callback(gWindow);

					if (screenSettings::detectChangeSizeWindow == true)
					{
						glViewport(0, 0, screenSettings::screen_w, screenSettings::screen_h);
						screenSettings::detectChangeSizeWindow = false;

					}
				}

				if (event.type == SDL_EVENT_KEY_DOWN)
				{
					//para cambiar la escena
					if (event.key.key == SDLK_O)
					{
						controlScene += 1;

						if (controlScene == static_cast<int>(scenes::sizeScenes))
						{
							controlScene = 0;

						}

					}

					if (!controlMove::detectSDLK_code::detectKey(event))
					{
						if (controlMove::detectMove == false)
						{
							controlMove::detectMove = true;
						}

						//	playingTest::moveRecSeq_Key(seqTriangles, event);

					}
					if (!controlMove::detectSDLK_code::detectKeyColor(event))
					{
						if (shading_2::changeColorTest == false)
						{
							shading_2::changeColorTest = true;

						}

					}
					if (controlMove::detectSDLK_code::detectKeyScale(event))
					{
						//	TexVertex.pressScaleTex(&event);

					}
					if (controlMove::detectSDLK_code::detectKeyTranslate(&event))
					{
						//					TexVertex.vertexTransform.detectTranslate(&event);

						if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
						{
							cameras::cameras_D[cameras::name_CurrentCamera].moveCameraTest = true;
						}
					}

					if (controlMove::detectSDLK_code::detectKeyUI(event))
					{
						if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
						{
							if (controlMove::detectSDLK_code::keys_UI[SDLK_TAB] == true)
							{
								screenSettings::outWindow = true;
								SDL_SetWindowRelativeMouseMode(gWindow, false);
								
							}

							else if (controlMove::detectSDLK_code::keys_UI[SDLK_TAB] == false)
							{
								screenSettings::outWindow = false;
								SDL_SetWindowRelativeMouseMode(gWindow, true);

							}
						}
					}

					if (event.key.key == SDLK_F) ////Temporal para controlar el apagado y encendido de la luz
					{
						if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
						{
							if (RenderData_Set::spotLights_D["FlashLight_SpotLight"].stateLight == true)
							{
								RenderData_Set::spotLights_D["FlashLight_SpotLight"].stateLight = false;

							}

							else if (RenderData_Set::spotLights_D["FlashLight_SpotLight"].stateLight == false)
							{
								RenderData_Set::spotLights_D["FlashLight_SpotLight"].stateLight = true;

							}
						}

						else if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
						{
							cameras::cameras_D[cameras::name_CurrentCamera].resetPos();
						}
					}

					ControlScenarios::detectScenario_Key(&event);

					//			TexVertex.vertexTransform.detectRot(&event);
					//			TexVertex.vertexTransform.detectScale(&event);

				}

				if (event.type == SDL_EVENT_KEY_UP)
				{

					controlMove::detectSDLK_code::resetKeys();

					if (controlMove::detectMove == true)
					{
						controlMove::detectMove = false;
					}

					if (shading_2::changeColorTest == true)
					{
						shading_2::changeColorTest = false;

					}


					//	if (TexVertex.scaleTest == true)
						//{
					//		TexVertex.outScaleTex();
					//	}

					//	TexVertex.vertexTransform.resetTests();

					if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
					{
						cameras::cameras_D[cameras::name_CurrentCamera].resetTest();
					}

				}

				if (event.type == SDL_EVENT_MOUSE_MOTION)
				{
					//FirstModel.cam1.detectRotCamMouse(controlMouse::getDistanceMotionMouse());

					if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
					{
						cameras::cameras_D[cameras::name_CurrentCamera].detectRotCamMouse(controlMouse::getDistanceMotionMouse());
						cameras::cameras_D[cameras::name_CurrentCamera].moveCameraTest = true;
					}

					else if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
					{
					    
						SDL_Keymod modStateKey = SDL_GetModState();
						bool stateAlt{ static_cast<bool>(modStateKey & SDL_KMOD_ALT) };
						bool stateClickMouse{ static_cast<bool>(event.motion.state & SDL_BUTTON_LMASK) }; //CLICK IZQUIERDO DEL MOUSE

						///ROTACION 
						if (stateAlt && stateClickMouse)
						{
							if (cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos == false)
							{
								cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos = true;
							}

							cameras::cameras_D[cameras::name_CurrentCamera].rotateCam_EditMode(controlMouse::getCurrentPosMouse());
						//SDL_Log("ROTATE::CAMERA::EDIT_MODE\n");
								///funcion para editar el movimiento de la camara

						}

						stateClickMouse = static_cast<bool>(event.motion.state & SDL_BUTTON_MMASK); //CLICK DE LA RUEDITA DEL MOUSE
					    
						///TRASLACION
						if (stateAlt && stateClickMouse)
						{
							if (cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos == false)
							{
								cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos = true;
							}
							cameras::cameras_D[cameras::name_CurrentCamera].translateCam_EditMode(controlMouse::getCurrentPosMouse());

						}

						stateClickMouse = static_cast<bool>(event.motion.state & SDL_BUTTON_RMASK); //CLICK DE LA RUEDITA DEL MOUSE

						///TRASLACION
						if (stateAlt && stateClickMouse)
						{
							if (cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos == false)
							{
								cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos = true;
							}
							cameras::cameras_D[cameras::name_CurrentCamera].dollyCam_EditMode(controlMouse::getCurrentPosMouse());

						}

					}
					
				}
				else if(event.type != SDL_EVENT_MOUSE_MOTION)
				{
					if (cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos == true)
					{
						cameras::cameras_D[cameras::name_CurrentCamera].editMode_Cam.stopDetectCurrentPos = false;
					}

				}

				if (event.type == SDL_EVENT_MOUSE_WHEEL)
				{
					SDL_Keymod modStateKey = SDL_GetModState();
					if (modStateKey & SDL_KMOD_ALT)
					{
						if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
						{
							cameras::cameras_D[cameras::name_CurrentCamera].cameraProjection(&event);
						}
					}
				}

				if (event.key.key == SDLK_ESCAPE)
				{
					loopEvent = true;

				}

				////para controlar la salida del mouse de la pantalla
				if (event.key.key == SDLK_LGUI || event.key.key == SDLK_RGUI)
				{
					screenSettings::outWindow = true;
					SDL_SetWindowRelativeMouseMode(gWindow, false);
				}

				if (screenSettings::outWindow == false && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
				{
					if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
					{
						data_HitAABB::selectObj = true;  ///ACTIVAR LA SELECCION DE INTERSECCION DEL OBJETO.
					}

				}

				if (screenSettings::outWindow == true && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
				{
		
					if (controlMove::detectSDLK_code::keys_UI[SDLK_TAB] == false)
					{
						screenSettings::outWindow = false;
						
						if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
						{
							SDL_SetWindowRelativeMouseMode(gWindow, true);
							SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);

						}

						else if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
						{
							SDL_SetWindowRelativeMouseMode(gWindow, false);
						}
					}
				}




			}

			//ControlPhysics_Events.update_ControlSystem();

			//cameras::updateStateCurrentCamera();


			///CONTROL DE EVENTS CAMERA.
			if (screenSettings::outWindow == false)
			{
			//	SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);

				if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::firstPerson)
				{
					SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);
					//camera_Transforms::attachObject_Cam(RenderData_Set::AssimpModel_D["FlashLight"].ModelCoord, cameras::aerialCamera);
				}
				cameras::cameras_D[cameras::name_CurrentCamera].updateLastPosCam();
				cameras::cameras_D[cameras::name_CurrentCamera].controlEventsCamera();
			}
		//	refresh_Models::refreshUI_point();

			//ControlPhysics_Events.update_ControlSystem();
			threadSystem::ControlPhysics_Events.controlExternal_System();

			if (syncFPS.frameT == true)
			{
				//openGL_render::clearOpenGL();

				UI::start_NewFrameUI();

				threadSystem::ControlPhysics_Events.timeInterpolation.loopAcomulator();
				////AQUI IBA LA SECCION DE CONTROL DE EVENTS CAMERA.


				/*
				std::vector<light::light1> pointLights
				{
					lightTest_01
				};

				std::vector<light::DirectionalLight> directionalLights
				{
					directionalLight_1
				};

				//FirstModelTest
				testTransLight.transformMeshLight(&BasicLight, &lightTest_01);
				testTranforms.transformUniqueModel(&FirstModel, aerialCamera, lightTest_01);
				FirstModel.renderMultipleModels(1, aerialCamera, lightTest_01);/////Colocamos el light para saber la posicion del light
				BasicLight.renderMeshLight(aerialCamera, lightTest_01);

				///Draw Backpack
				//modelBackpack.Draw(aerialCamera, lightTest_01);
				modelBackpack.Draw_WL(aerialCamera, pointLights, directionalLights);
				testBackPack.transformModel_test1(&coordBackPack);
				modelBackpack.setModelCoord(coordBackPack);

				///Draw Floor model
				//model_Floor.Draw(aerialCamera, lightTest_01);
				model_Floor.Draw_WL(aerialCamera, pointLights, directionalLights); ///para renderizar con directional lights
				testFloor.moveModel_Test(&coord_FloorModel);
				model_Floor.setModelCoord(coord_FloorModel);
				*/

				/*
				refresh_Models::refreshAll_Models();
				AABB::updateCoordAABB_All();
				ScreenCalc_Hit::calc_IntersectALL();
				*/
				//AABB::updateCoordAABB_All();

				//render::renderAll();

		//		stencil_test::renderStencilTest();
				//testPlay::renderTranformations_Objects();
		//		AABB::updateCoordAABB_All();
	
				if (RenderData_Set::finishLoadALL == true)
				{
					//ControlPhysics_Events.update_ControlSystem();

					//testPlay::renderTranformations_Objects();

					//cameras::updateStateCurrentCamera();
					refresh_Models::refreshUI_point();
					//refresh_Models::refreshUI_point();
					//testPlay::transformation_handCamara();
					testPlay::transformation_handCamara();
					refresh_Models::refreshAll_Models();
					//AABB::updateCoordAABB_All();
					//ScreenCalc_Hit::calc_IntersectALL();

					render::renderPhase();
					refresh_Models::refreshAll_LastModels();
					
					UI::render_All_ImGui(gWindow);
					//testPlay::renderTranformations_Objects();
				}
				

				//openGL_render::secondClearOpenGL();

				SDL_UpdateWindowSurface(gWindow);
				SDL_GL_SwapWindow(gWindow);

				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				syncFPS.frameT = false;
				syncFPS.stopTimeNS = SDL_GetTicksNS();
			}

			syncFPS.countTimeRender();

		}

	}

	//multiplesTriangles.destroy();
	UI::destroyUI();
	threadSystem::ControlPhysics_Events.destroy();
	destroy::destroyModels();
	SDL_GL_DestroyContext(contextOpenGl);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	return 0;
}
