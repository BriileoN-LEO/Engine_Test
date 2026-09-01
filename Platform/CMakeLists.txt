cmake_minimum_required (VERSION 3.21)

project(Brii_Platform CXX)


set(PLATFORM_BRII 
Public/Platform_Brii/Platform_Window.h
Public/Platform_Brii/Platform_Input.h
Public/Platform_Brii/Platform_FileSystem.h
)

add_library(Brii_Platform STATIC  
${PLATFORM_BRII} 
)
#SDL
#-Call the OS to make the events, inputs and outputs
#-PC
if(TARGET_PLATFORM STREQUAL "PC")

set(PLATFORM_PC
Private/Platform_SDL/Public/SDL3_WindowDefinitions.h
Private/Platform_SDL/Public/SDL3_WindowDefinitions.cpp 
Private/Platform_SDL/Public/SDL3_libraries.h
Private/Platform_SDL/Public/SDL3_window.cpp
Private/Platform_SDL/Public/SDL3_Input.cpp
)

target_sources(Brii_Platform PRIVATE ${PLATFORM_PC})
target_link_libraries(Brii_Platform PRIVATE SDL3::SDL3-shared)
target_include_directories(Brii_Platform PRIVATE ${PLATFORM_PC})


if(UNIX AND NOT APPLE)

set(PLATFORM_LINUX
Private/Platform_SDL/Private/Linux/SDL3_WindowDef_Linux.cpp
Private/FileSystem_OS/Linux/FileSystem_POSIX.cpp
)

target_sources(Brii_Platform PRIVATE ${PLATFORM_LINUX})
target_include_directories(Brii_Platform PRIVATE ${PLATFORM_LINUX})

endif()


#in FUTURE add = 
#PS5
#Nintendo Switch 

endif()

#TO GET A STRING WITH THE DIRECTION OF THE ENGINE, IN THE ENGINE CONVERT THIS IN A STRING
target_compile_definitions(Brii_Platform PUBLIC
SOURCE_STRINGS_ENGINE_BRII="${SOURCE_LOCALIZATION_STRING}"
)

target_compile_definitions(Brii_Platform PUBLIC
SOURCE_STRINGS_USER_ENGINE_BRII="${SOURCE_LOCALIZATION_STRING_USER}"
)

target_include_directories(Brii_Platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Brii_Platform PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Platform")
target_include_directories(Brii_Platform PRIVATE 
${PLATFORM_BRII}
#${PLATFORM_SDL}
)

target_link_libraries(Brii_Platform PRIVATE
brii_CORE
)


