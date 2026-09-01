#include "IGraphics_OpenGL.h"

OpenGL_RHI::OpenGL_RHI(){};
OpenGL_RHI::~OpenGL_RHI(){};


void OpenGL_RHI::Initialize_API(void* windowHandle)
{
 SDL_Window* window {static_cast<SDL_Window*>(windowHandle)};
 context_SDL3 = SDL_GL_CreateContext(window);  
}
void OpenGL_RHI::destroy_RHI()
{
  SDL_GL_DestroyContext(context_SDL3); ////REMPLACE THIS CONTEXT OPENGL 
}






