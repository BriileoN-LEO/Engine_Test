#include "RHI_Factory.h"
#include "Private/IG_OpenGL/IGraphics_OpenGL.h"

void createGlobal_API_RHI(brii_RHI* RHI, brii_Renderer_API* API)
{
  if(*API == brii_Renderer_API::OpenGL)
 {
  RHI = new OpenGL_RHI(); 
 }
}

