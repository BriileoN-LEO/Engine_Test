#pragma once

#include "dataManager/dataTypes_brii.h"

enum class Brii_WindowType : uint16_t
 {
  PRINCIPAL_WINDOW = 0,
  VIEWPORT = 1,
  RENDER = 2,
  TREEWORK_VIEW = 3
 };


enum class Brii_WindowFlags : BT_ComponentFlags64
{
 //API
 BRII_WINDOW_OPENGL = 1,
 BRII_WINDOW_VULKAN = 1 << 1, 
 BRII_WINDOW_DIRECTX12 = 1 << 2,
 BRII_WINDOW_METAL = 1 << 3, 

 //WINDOW CONFIG
 BRII_WINDOW_RESIZABLE = 1 << 4,
 BRII_WINDOW_HIDDEN = 1 << 5, 
 BRII_WINDOW_HIGH_PIXEL_DENSITY = 1 << 6, 

};

template<> inline constexpr bool is_bitmask_flag<Brii_WindowFlags> = true;

//SDL_WindowFlags windowFlag{ SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY };

struct Brii_WindowSettings
{
 uint64_t windowName_ID{};

 int width_size{};
 int height_size{}; 
  
 Brii_WindowFlags window_Flags{};
};



