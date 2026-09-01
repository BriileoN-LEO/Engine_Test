#ifndef IGRAPHICS_OPENGL
#define IGRAPHICS_OPENGL

#include "Public/Graphics_Interface/IGraphics_brii.h"
#include <SDL3/SDL_video.h>

class OpenGL_RHI : public Brii_RHI
{
 private:
 SDL_GLContext context_SDL3;

 public:

 OpenGL_RHI();
 ~OpenGL_RHI() override;
 
 void Initialize_API(void* windowHandle) override;
 void destroy_RHI() override;

};


#endif

