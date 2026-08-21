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


class BriiEngine_Settings
{
 private:
 brii_Renderer_API Graphics_API{};

 public:

};

//SEE WHERE I COULD SAVE THE SETTINGS THE CHANGE OF API CONTEXT 

