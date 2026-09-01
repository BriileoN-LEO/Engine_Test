#pragma once

#include "dataManager/dataTypes_brii.h"
#include "Settings/Settings_BriiEngine.h"

/*
 class Brii_Window
i{
 
 private

 public: 
 
 brii_Platform_Window();
 ~brii_Platform_Window();
 
 void Initialize_GraphicsAPI(void* API_settings);
 //void create_Window( 

 void* get_HandleWindow();

};*/

//this class is defined in SDL3_Window.cpp or other type

 struct Brii_WindowHandle;

 class Brii_Window
{
  private:
  Brii_WindowHandle* Window_Context{ nullptr };
  BT_WindowID* windows_ID{ nullptr };

  public:

  Brii_Window();
  ~Brii_Window(); 
 
 bool create_Window(Brii_WindowSettings* Window_Settings);
 void* get_WindowHandle() const;
 const BT_WindowID* get_WindowID() const;
 void destroy();
};

///THE MANAGER UTILITIES
///only see if the window exist or not


////CONTINUE HERE TO DECLARE 

 class Brii_WindowManager
{
  private:

  BT_Vector<Brii_Window> windows_container{};
 
  public:
  Brii_WindowManager();
  ~Brii_WindowManager();

  void create_WindowContext(Brii_WindowSettings* API_settings);
  bool contains_window(BT_WindowID window_ID); 

  Brii_Window* get_WindowContext(BT_WindowID window_ID) const;
  const Brii_Window* get_ConstWindowContext(BT_WindowID window_ID) const; 
 
};




 

///Probably create here the general window manager that the application requieres to create
//multiple windows
