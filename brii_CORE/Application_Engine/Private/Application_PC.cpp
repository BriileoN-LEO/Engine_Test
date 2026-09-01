

#include "Application_Engine/Public/Application.h"
#include "Public/Graphics_Interface/IGraphics_brii.h"
#include "Public/Platform_Brii/Platform_Window.h" 
#include <SDL3/SDL.h> 

namespace Application
{

void BriiEngine::init_Window()
{
  Brii_RHI* RHI_ptr { RHI_Manager->get_RHI_RawPtr() };
  const Brii_Renderer_API* API_ptr {Engine_Settings.get_RendererAPI()}; 
  Brii_RHI_Factory::create_API_RHI(RHI_ptr, API_ptr);

  if (SDL_Init(SDL_INIT_VIDEO) == false)
 { 
  std::cout << "ERROR OpenGL | not init video\n";
  std::string errorGL {SDL_GetError()};
  std::cout << errorGL << "\n";
  return;
 } 

   

}

void BriiEngine::init_EngineBrii()
{
 
  
 ////HERE PUT THE ATTRIBUTES OPENGL 

}

void BriiEngine::destroy_EngineBrii()
{

}

}
