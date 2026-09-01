#pragma once

#include "dataManager/dataTypes_brii.h"
#include <cstdint>

///INTERFACE GRAPHICS GLOBAL API 
class Brii_RHI
{
 //DESCRIPTION
 //Virtual class, derivate clases per Graphics API
 //Derivate RHI class:
 //-IGraphics_OpenGL.h

public: 
 
 Brii_RHI() = default;
 virtual ~Brii_RHI() = 0; 

 //virtual void init_RHI() = 0;
 virtual void Initialize_API(void* windowHandle) = 0;
 virtual void destroy_RHI() = 0;

};

//CONTINUE HERE TO DEFINE THE FUNCTIONS OF RHI 
class Brii_RHI_Manager
{
 ///DESCRIPION 
 ///definitions in IGraphics_brii.cpp
 ///Owns a unique pointer from an virtual class depending of the the Graphics API

 private:

 BT_UniquePtr<Brii_RHI> graphics_RHI{nullptr};
 
 public:

 Brii_RHI* get_RHI_RawPtr() const; 

};



