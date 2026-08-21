#ifndef IGRAPHICS_OPENGL
#define IGRAPHICS_OPENGL

#include "Public/Graphics_Interface/IGraphics_brii.h"

class OpenGL_RHI : public brii_RHI
{
 public: 
 OpenGL_RHI();
 ~OpenGL_RHI() override;

  void destroy_RHI() override;

};


#endif

