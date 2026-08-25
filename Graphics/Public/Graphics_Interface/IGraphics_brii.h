#pragma once

#include <cstdint>
///INTERFACE GRAPHICS GLOBAL API 
class brii_RHI
{
public: 
 
 brii_RHI() = default;
 virtual ~brii_RHI() = 0; 

 //virtual void init_RHI() = 0;
 virtual void Initialize_API(void* windowHandle) = 0; 
 virtual void destroy_RHI() = 0;

};



