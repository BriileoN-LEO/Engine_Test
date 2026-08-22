#pragma once

//Engine Links libraries
#include "Public/Platform_Brii/Platform_Window.h"
#include "systemManager/logger.h"

//SDL Libraries
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

//CPP libraries 
#include <cassert>


//SDL_Window* gWindow{ nullptr };

///CONTINUE HERE TO MAKE THE IMPLEMENTATION of OpenGL to SDL  

void create_SDL_GL_Window(SDL_Window* Window)
{
 if (SDL_Init(SDL_INIT_VIDEO) == false)
 { 
  std::cout << "ERROR OpenGL | not init video\n";
  std::string errorGL {SDL_GetError()};
  std::cout << errorGL << "\n";
  assert(gWindow != nullptr); ///CHANGE this to other way to stop process 
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

  Window = SDL_CreateWindow("LearningOpenGL", screenSettings::screen_w, screenSettings::screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE); //Creacion de la ventana
  SDL_SetWindowAlwaysOnTop(Window, true);
  SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

   if (Window == nullptr)
  {
   std::cout << "ERROR OpenGL | not init window\n";
   std::string errorGL {SDL_GetError()};
   std::cout << errorGL << "\n";
   assert(gWindow != nullptr);
  }

   else
  {
   contextOpenGl = SDL_GL_CreateContext(Window);  //Creacion del contexto
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
 
 void createWindow(const brii_EngineSettings::Graphics_Settings* API_settings)
 {
   switch (API_settings->API)
  {
   case(brii_Renderer_API::OpenGL) :
   create_SDL_GL_Window(Window);
   break;
	
   case(brii_Renderer_API::Vulkan) :
   create_SDL_Vulkan_Window(Window);
 
   case(brii_Renderer_API::DirectX12) : 
   std::cout << "ERROR API::DirectX12 not support\n";
  
   case(brii_Renderer_API::Metal) : 
   std::cout << "ERROR API::Metal not support\n";
  }  
 
 }

 void destroy()
 {
  SDL_GL_DestroyContext(contextOpenGl); ////REMPLACE THIS CONTEXT OPENGL
  SDL_DestroyWindow(Window);
  SDL_Quit();
 }

};

///SEE WHERE SAVE THIS VARIABLES
SDL_Window_Context window{};

brii_Platform_Window::brii_Platform_Window(){};
brii_Platform_Window::~brii_Platform_Window(){};
void brii_Platform_Window::Initialize(const brii_EngineSettings::Graphics_Settings* API_settings)
{
  window.createWindow(API_settings); 
}



