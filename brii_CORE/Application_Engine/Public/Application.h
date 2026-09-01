#pragma once

#include "Public/Platform_Brii/Platform_Window.h"
#include "Public/Platform_Brii/Platform_FileSystem.h"
#include "Public/RHI_Settings/RHI_Factory.h"
//#include "Settings/Settings_BriiEngine.h"
//#include "Settings/Settings_WindowEngine.h" 
#include <iostream> 

namespace Application
{

  class BriiEngine
 {
  private:
 
  
  BT_UniquePtr<Brii_WindowManager> window_Manager{}; 
  BT_UniquePtr<Brii_RHI_Manager> RHI_Manager{};

  //Main settings for the principal Window of the Engine 
	 Brii_EngineSettings Engine_Settings{};

  public:

  BriiEngine();
  ~BriiEngine();

  void insert_Settings(Brii_EngineSettings Engine_Settings);
 
  void init_Window();
  void init_EngineBrii();
  void destroy_EngineBrii();

 };

////QUIT THIS IN THE FUTURE 
///EXAMPLE IF I WILL HAVE MULTIPLE WINDOWS, HOW I WILL BE CREATE
 class Brii_UI_Engine
{
 private: 

  ///THIS LINK IS FOR THE WINDOW GET BY THE ENGINE, WITH THE WindowManager 
 Brii_Window* link_Window{};
 Brii_WindowSettings Window_Settings{};
  
 public:


};


}
