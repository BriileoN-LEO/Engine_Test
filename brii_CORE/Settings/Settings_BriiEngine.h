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

 struct Graphics_Settings 
 { 
   brii_Renderer_API API{};

   //work int OpenGL and Vulkan
   int API_majorVersion{};
   int API_minorVersion{};
  
 
 };

 private:
 
 Graphics_Settings Graphics_brii{};

 public:

 brii_EngineSettings();
 ~brii_EngineSettings();

 const Graphics_Settings* get_GraphicsSet()
 {
  return &Graphics_brii;
 }


};

//SEE WHERE I COULD SAVE THE SETTINGS THE CHANGE OF API CONTEXT 

