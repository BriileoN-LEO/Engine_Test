#pragma once

//Engine Links libraries
#include "Public/Platform_Brii/Platform_Window.h"
#include "SDL3_Definitions/SDL3_WindowDefinitions.h"
#include "systemManager/logger.h"


//CPP libraries 
#include <cassert>



brii_Platform_Window::brii_Platform_Window(){};
brii_Platform_Window::~brii_Platform_Window(){};
void brii_Platform_Window::Initialize(const brii_EngineSettings::Graphics_Settings* API_settings)
{
  window_SDL3.createWindow(API_settings); 
}

void* brii_Platform_Window::get_HandleWindow()
{
 return static_cast<void*>(window_SDL3.get_SDLWindow());
}



