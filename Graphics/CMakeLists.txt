cmake_minimum_required (VERSION 3.21)

project(Brii_GraphicsInterface CXX)

#set_target_properties(Brii_GraphicsInterface PROPERTIES LINKER_LANGUAGE CXX)

set(GRAPHICS_INTERFACE
Public/Graphics_Interface/IGraphics_brii.h
Public/RHI_Settings/RHI_Factory.h
Public/RHI_Settings/RHI_Factory.cpp
)

set(GRAPHICS_OPENGL
Private/IG_OpenGL/IGraphics_OpenGL.h
Private/IG_OpenGL/IGraphics_OpenGL.cpp
)

set(GRAPHICS_VULKAN
#IN THE FUTURE IMPLEMENT THE API VULKAN 
)

set(GRAPHICS_API)

#IF WIN32 OR LINUX(UNIX)
#API's = OpenGL | Vulkan
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

if(WIN32 OR UNIX)
target_link_libraries(Brii_GraphicsInterface 
glad_source
OpenGL::GL
)
endif()

target_link_libraries(Brii_GraphicsInterface
brii_CORE
)

#set_common_target_properties(Brii_GraphicsInterface)


target_include_directories(Brii_GraphicsInterface PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Brii_GraphicsInterface PUBLIC
${GRAPHICS_INTERFACE}
)
target_include_directories(Brii_GraphicsInterface PRIVATE 
${GRAPHICS_API}
)


