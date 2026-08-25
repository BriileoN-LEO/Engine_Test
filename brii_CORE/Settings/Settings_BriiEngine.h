#pragma once

#include <iostream>

///SELECTION OF THE GRAPHICS API 
enum class brii_Renderer_API : uint8_t
{
 OpenGL = 0,
 Vulkan,
 DirectX12,
 Metal
};


class brii_EngineSettings
{
public:

 struct Window_Settings
 {
  int width_size{};
  int height_size{};
  
 };

 struct Graphics_Settings 
 { 
  brii_Renderer_API API{};

   //work API version | OpenGL and Vulkan
  int API_majorVersion{};
  int API_minorVersion{};
  
  
 
 };

 private:
 
 Window_Settings Window_brii{};
 Graphics_Settings Graphics_brii{};

 public:

 brii_EngineSettings();
 ~brii_EngineSettings();

 ////THIS
 void insert_WindowSettings(Window_Settings wSet);  
 void insert_GraphicsSettings(Graphics_Settings gSet);

 const Graphics_Settings* get_GraphicsSet()
 {
  return &Graphics_brii;
 }


};

//SEE WHERE I COULD SAVE THE SETTINGS THE CHANGE OF API CONTEXT 

