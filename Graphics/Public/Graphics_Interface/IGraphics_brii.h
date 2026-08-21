#pragma once

#include <cstdint>

///SELECTION OF THE GRAPHICS API 
enum class brii_Renderer_API : uint8_t
{
 OpenGL = 0,
 Vulkan,
 DirectX12,
 Metal
};


///INTERFACE GRAPHICS GLOBAL API 
class brii_RHI
{
public: 
 
 brii_RHI() = default;
 virtual ~brii_RHI() = default; 

 virtual void init_RHI() = 0;

 virtual void destroy_RHI() = 0; 
};
