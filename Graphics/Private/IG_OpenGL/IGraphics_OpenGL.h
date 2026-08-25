#ifndef IGRAPHICS_OPENGL
#define IGRAPHICS_OPENGL

#include "Public/Graphics_Interface/IGraphics_brii.h"
#include <SDL3/SDL_video.h>

class OpenGL_RHI : public brii_RHI
{
 private:
 SDL_GLContext context_SDL3;

 public:

 OpenGL_RHI();
 ~OpenGL_RHI() override;
 
 ///CONTINUE HERE 22/08/2026 
 ///See how to implement the initilization of OpenGL with SDL3
 ///Avoid implement SDL3 in IGraphics_brii.h, it could cause conflic with others platforms diferent from SDL3 

 void Initialize_API(void* windowHandle) override;
 void destroy_RHI() override;

};


#endif

