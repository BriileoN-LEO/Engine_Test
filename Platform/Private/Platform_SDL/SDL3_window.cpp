#pragma once

#include "Public/Platform_Brii/Platform_Window.h"
#include "systemManager/logger.h" 
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

//SDL_Window* gWindow{ nullptr };

///CONTINUE HERE TO IMPLEMENT ALL THE DATA CREATION OF WINDOW CONTEXT IN SDL3 

void create_SDL_GL_Window(SDL_Window* Window)
{
 if (SDL_Init(SDL_INIT_VIDEO) == false)
 {
  SDL_Log("no se pudo abrir la ventana");
  SDL_Log(SDL_GetError());
  assert(gWindow != nullptr);
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
		SDL_SetWindowAlwaysOnTop(gWindow, true);
		SDL_SetWindowPosition(gWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

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
			SDL_GL_SetSwapInterval(1); ///TEMPORAL

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

//ADD THIS IN THE FUTURE 
void create_SDL_Vulkan_Window(SDL_Window* Window)
{

}

class SDL_Window_Context
{
 private: 
 SDL_Window* Window{ nullptr };
  
 public:

 SDL_Window_Context();
 ~SDL_Window_Context();
 
 void createWindow(brii_Renderer_API* API)
 {
  switch (*API)
 {
  case(brii_Renderer_API::OpenGL) :
  create_SDL_GL_Window(
  break;
	
  case(brii_Renderer_API::Vulkan) :
  create_SDL_Vulkan_Window();

   
   
 
 }

 void destroy()
 {
  SDL_DestroyWindow(Window);
  SDL_Quit();
 }

};

SDL_Window_Context window{};


//	SDL_GL_DestroyContext(contextOpenGl);
//	SDL_DestroyWindow(gWindow);
//	SDL_Quit();



brii_Platform_Window::brii_Platform_Window(){};
brii_Platform_Window::~brii_Platform_Window(){};

void brii_Platform_Window::Initialize(brii_Renderer_API* API)
{
  window.createWindow(API); 
}



