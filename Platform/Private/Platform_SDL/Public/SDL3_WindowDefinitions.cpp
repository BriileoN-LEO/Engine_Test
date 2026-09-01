
#include "SDL3_WindowDefinitions.h"
//SDL_Window* gWindow{ nullptr };

///CONTINUE HERE TO MAKE THE IMPLEMENTATION of OpenGL to SDL

namespace SDL_WindowSets
{
void create_SDL_GL_Window(SDL_Window* Window, const Brii_WindowSettings* Window_Settings)
{
 if (SDL_Init(SDL_INIT_VIDEO) == false)
 { 
  std::cout << "ERROR OpenGL | not init video\n";
  std::string errorGL {SDL_GetError()};
  std::cout << errorGL << "\n";
  assert(gWindow != nullptr); ///CHANGE this to other way to stop process 
 }

 else
 {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
 
  SDL_WindowFlags windowFlag{ SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY };

  Window = SDL_CreateWindow("LearningOpenGL", screenSettings::screen_w, screenSettings::screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE); //Creacion de la ventana
  SDL_SetWindowAlwaysOnTop(Window, true);
  SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

   if (Window == nullptr)
  {
   std::cout << "ERROR OpenGL | not init window\n";
   std::string errorGL {SDL_GetError()};
   std::cout << errorGL << "\n"; 
  }

   else
  {
   contextOpenGl = SDL_GL_CreateContext(Window);  //Creacion del contexto
   SDL_GL_SetSwapInterval(1); ///TEMPORAL

   if (contextOpenGl == NULL)
  {
   SDL_Log("Error al crear el contexto de OpenGL");
   SDL_Log(SDL_GetError());
   SDL_DestroyWindow(gWindow);
   SDL_Quit();
   correct_init = false;
   assert(contextOpenGl != nullptr);
  }

   else
  {
   if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) //Inializador de glad
   {
    SDL_Log("Fallo al inicializar glad");
    SDL_Log(SDL_GetError());
    bool correct_init = false;
   }
  }
 }
}

}

}
