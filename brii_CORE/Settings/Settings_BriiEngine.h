#pragma once

#include "Settings_WindowEngine.h"
#include <iostream>

///SELECTION OF THE GRAPHICS API

 enum class Brii_Renderer_API : uint8_t
 {
  OpenGL = 0,
  Vulkan,
  DirectX12,
  Metal
 };

 struct Brii_GraphicsSettings 
 { 
   Brii_Renderer_API API{};

    //work API version | OpenGL and Vulkan
   int API_majorVersion{};
   int API_minorVersion{};
 
   
 };


////CONTINUE HERE 28/08/2026
////SEE WHAT I WILL DO WITH THIS, is posible to make
///Make a vector of window settings an the window will get a pointer of the Brii_WindowSettings
///Update the window by only call the window update, and in this place only modify the windows_Setting, because all is link via the pointer 
///Only have one Brii_GraphicsSettings
class Brii_EngineSettings
{
  private:
 
  Brii_WindowSettings Window_Settings{};
  Brii_GraphicsSettings Graphics_Set{};

  public:

  Brii_EngineSettings();
  ~Brii_EngineSettings();

  ////THIS
  void insert_WindowSettings(Brii_WindowSettings wSet);  
  void insert_GraphicsSettings(Brii_GraphicsSettings gSet);

  const Brii_Renderer_API* get_RendererAPI() const
  {
   return &Graphics_Set.API;
  }

  const Brii_GraphicsSettings* get_GraphicsSet()
  {
   return &Graphics_Set;
  }
  const Brii_WindowSettings* get_WindowSettings()
  {
   return &Window_Settings;
  }
 };

//SEE WHERE I COULD SAVE THE SETTINGS THE CHANGE OF API CONTEXT 

