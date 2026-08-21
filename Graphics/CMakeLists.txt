cmake_minimum_required (VERSION 3.21)

project(Brii_GraphicsInterface CXX)

#set_target_properties(Brii_GraphicsInterface PROPERTIES LINKER_LANGUAGE CXX)

set(GRAPHICS_INTERFACE
Public/Graphics_Interface/IGraphics_brii.h
)

set(GRAPHICS_OPENGL
Private/IG_OpenGL/IGraphics_OpenGL.h
Private/IG_OpenGL/IGraphics_OpenGL.cpp
)

set(GRAPHICS_VULKAN

)

set(GRAPHICS_API)

if(WIN32 OR UNIX)
list(APPEND GRAPHICS_API 
${GRAPHICS_OPENGL}
${GRAPHICS_VULKAN}
)
endif()



add_library(Brii_GraphicsInterface STATIC  
${GRAPHICS_INTERFACE} 
${GRAPHICS_API}
)

#set_common_target_properties(Brii_GraphicsInterface)


target_include_directories(Brii_GraphicsInterface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Brii_GraphicsInterface PUBLIC
${GRAPHICS_INTERFACE}
${GRAPHICS_API}
)


