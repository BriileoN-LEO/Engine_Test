//LINUX
//Description specifications:
//*The unique API Available = OpenGL, Vulkan

//ENGINE HEADERS
#include "Private/IG_OpenGL/IGraphics_OpenGL.h"
#include "Public/RHI_Settings/RHI_Factory.h"

namespace Brii_RHI_Factory
{
 void create_API_RHI(Brii_RHI* RHI, const Brii_Renderer_API* API)
 {
  switch (*API)
 {
  case (Brii_Renderer_API::OpenGL) :
   {
    RHI = new OpenGL_RHI(); 
    return; 
   }
  
  case (Brii_Renderer_API::Vulkan) :
   {
   std::cout << "Vulkan API is not available\n";
    return;
   }

  case (Brii_Renderer_API::DirectX12) :
  {
    std::cout << "DirectX12 is not available in Linux";
  }

   case (Brii_Renderer_API::Metal) :
  {
    std::cout << "Metal is not available in Linux";
  }
 }
  
 }

}
