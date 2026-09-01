#pragma once

//Engine Links libraries
#include "Public/Platform_Brii/Platform_Window.h"
#include "SDL3_WindowDefinitions.h"
#include "systemManager/logger.h"


//CPP libraries 
#include <cassert>


///CONTINUE HERE 25/08/2026
///Define a windows manager to save all the posible windows
///Define an enum class to save the type of windows created and save it in the SDL_Window_Context
//FOLLOW THE INSTRUCTIONS IN OBSIDIAN

struct Brii_WindowHandle
{
 SDL_Window* Window {nullptr};
};

 Brii_Window::Brii_Window(){};
 Brii_Window::~Brii_Window(){ destroy(); };
 
 bool Brii_Window::create_Window(Brii_WindowSettings* Window_Settings)
{      
  SDL_WindowFlags window_Flags {};
  
  ///GET THE API FLAG FOR SDL3
  if(!SDL_WindowDef::link_windowFlags(&Window_Settings->window_Flags, &window_Flags))
 {
  std::cout << "[ERROR][WINDOW] stop creation window\n";
  return false;
 }
  
 ///consult the string of the name of the window with the windowID 
 Window_Context.Window = SDL_CreateWindow( 

 ///CONTINUE HERE TO CREATE THE WINDOW TO SDL3 30/08/2026
  ///CREATE THE HASH TABLE TO SAVE ALL THE STRINGS IN RAM
  ///use the convetion type std::wstring to save special characters, defined in dataTypes_brii.h

  ////HERE CALL TO STOP THuE CREATION WINDOW IF DETECTS IF CALLING THE API

  SDL_WindowFlags windowFlag{ SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY };

  Window = SDL_CreateWindow("LearningOpenGL", screenSettings::screen_w, screenSettings::screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE); //Creacion de la ventana
  SDL_SetWindowAlwaysOnTop(Window, true);
  SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

   if (Window == nullptr)
  {
   std::cout << "ERROR OpenGL | not init window\n";
   std::string errorGL {SDL_GetError()};
   std::cout << errorGL << "\n"; 
  } 
  
 }

 void* Brii_Window::get_WindowHandle() const; 
 {
   return static_cast<void*>(Window_Context.Window);
 }

  const BT_WindowID* Brii_Window::get_WindowID() const
 {
  return windows_ID;
 }

void Brii_Window::destroy()
 {  
  SDL_DestroyWindow(Window_Context->Window);
  SDL_Quit();
 }

};


 Brii_WindowManager::Brii_WindowManager(){};
 Brii_WindowManager::~Brii_WindowManager(){};

  void Brii_WindowManager::create_WindowContext(Brii_WindowSettings* API_settings)
{
   
  
}

 bool Brii_WindowManager::contains_window(BT_WindowID window_ID)
{
 for(auto windowFind : windows_container)
 {
  const BT_WindowID* window_get {windowFind.get_WindowID()};
  if(*window_get == window_ID)
  {
   return true;
  }
 }
 return false; 
}


Brii_Window* Brii_WindowManager::get_WindowContext(BT_WindowID window_ID) const
{

}
const Brii_Window* Brii_WindowManager::get_ConstWindowContext(BT_WindowID window_ID) const
{

}







 ///CHANGE THIS BRII PLATFORM | SUBSITUTE FOR BRII PLATFORM MANAGER 
/*
brii_Platform_Window::brii_Platform_Window(){};
brii_Platform_Window::~brii_Platform_Window(){};
void brii_Platform_Window::Initialize(void* API_settings)
{
 sets_GraphicsEngine::Graphics_Settings* SDL_SettingsGraphics {static_cast<sets_GraphicsEngine::Graphics_Settings*>(API_settings)};  
 
 window_SDL3.createWindow(SDL_SettingsGraphics); 
}

void* brii_Platform_Window::get_HandleWindow()
{
 return static_cast<void*>(window_SDL3.get_SDLWindow());
}
*/


