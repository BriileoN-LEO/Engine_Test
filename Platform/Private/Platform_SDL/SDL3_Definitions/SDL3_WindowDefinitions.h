
//SDL Libraries
#include "Settings/Settings_BriiEngine.h"
#include "SDL3_libraries.h"

//CPP Libraries
#include <memory>
#include <vector>
#include <string>


void create_SDL_GL_Window(SDL_Window* Window);

void create_SDL_Vulkan_Window(SDL_Window* Window);

class SDL_Window_Context
{
 private: 
 SDL_Window* Window{ nullptr };
 std::string Window_Name{};
 uint64_t windowName_ID{}; /// TO GET THE WINDOW ID WHERE IT COMES FROM 

 public:

 SDL_Window_Context();
 ~SDL_Window_Context();

 void createWindow(const std::string nameWindow, const brii_EngineSettings::Graphics_Settings* API_settings);
 SDL_Window* get_SDLWindow();
 
 void destroy();

};

///CONTINUE HERE 25/08/2026
///Define a windows manager to save all the posible windows
///Define an enum class to save the type of windows created and save it in the SDL_Window_Context

namespace SDL_WindowDef
{
  class SDL_WindowManager
 {
  std::vector<uint64_t> windows_ID{};
  std::vector<std::unique_ptr<SDL_Window_Context>> window_SDL3{};
 
  SDL_WindowManager

 };

}

