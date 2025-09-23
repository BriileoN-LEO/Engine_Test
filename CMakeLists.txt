# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.16)

project (Learning)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DATA_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_v_v1.vs" 
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_f_v1.fs"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_TEX/shader_v_Tex_v1.vs" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_TEX/shader_v_Tex_v1.fs"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_MODEL1/vshader_model_v1.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_MODEL1/fshader_model_v1.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/images/container.jpg"
"${CMAKE_CURRENT_SOURCE_DIR}/images/awesomeface.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/CubeGrass_Down.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/CubeGrass_laterals.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/CubeGrass_Top.png"
)

set(RUTE_COMPLETE_FILES ${DATA_FILE_PATH})

configure_file(

  "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/configFilesTXT.h"
  "${PROjECT_BINARY_DIR}/configFilesTXT.h"

)

list(APPEND CMAKE_PREFIX_PATH "C:/Users/Briil/SDKs_libraries/SDL")

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad_source STATIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c)

target_include_directories(glad_source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include)

#para la libreria de glm
include(FetchContent)

FetchContent_Declare(
 glm
 GIT_REPOSITORY https://github.com/g-truc/glm.git
 GIT_TAG 1.0.1

)

FetchContent_MakeAvailable(glm)

add_executable(Learning "learningOpenGL/learningOpenGL.cpp" "learningOpenGL/config_window.h" "learningOpenGL/config_window.cpp" "learningOpenGL/data_save.h" "learningOpenGL/data_save.cpp"
"learningOpenGL/control.cpp" "learningOpenGL/control.h" 
"learningOpenGL/SHADER_H.h" "learningOpenGL/SHADER_H.cpp"
"learningOpenGL/GLM_test.h" "learningOpenGL/GLM_test.cpp"
)

target_include_directories(Learning PRIVATE "${PROJECT_BINARY_DIR}")
target_include_directories(Learning PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdPartyLibs")
target_include_directories(Learning PRIVATE ${glm_SOURCE_DIR})

target_link_libraries(Learning PRIVATE
SDL3::SDL3 
glad_source
OpenGL::GL
glm
)

message(STATUS "SDL3 encontrado en: ${SDL3_DIR}")
message(STATUS "OpenGl encontrado en: ${OpenGL_DIR}")


#if(WIN32)
#    target_link_options(Learning PRIVATE "/SUBSYSTEM:WINDOWS")
#endif()