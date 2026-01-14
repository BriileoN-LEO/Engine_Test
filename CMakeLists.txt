# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.21)

add_compile_definitions(NOMINMAX)

#add_compile_definitions(WIN32_LEAN_AND_MEAN)

add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
project (Learning)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DATA_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_v_v1.vs" 
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_f_v1.fs"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_TEX/shader_v_Tex_v1.vs" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_TEX/shader_v_Tex_v1.fs"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_MODEL1/vshader_model_v1.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_MODEL1/fshader_model_v1.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_LIGHT/vShader_Light_v1.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_LIGHT/fShader_Light_v1.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_MODEL_T1/vShader_ModelT1.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_MODEL_T1/fShader_ModelT1.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_STANDARD/vShader_Standard_v1.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_STANDARD/fShader_Standard_v1.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_STENCIL_TEST/vShader_stencil_test.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_STENCIL_TEST/fShader_stencil_test.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADERS_AABB/vShader_AABB_All.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADERS_AABB/fShader_AABB_All.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_UI/vShader_Pointer.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_UI/fShader_Pointer.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_FRAMEBUFFER/vShader_framebuffer_V01.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_FRAMEBUFFER/fShader_framebuffer_V01.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_SKYBOX/vShader_skybox_v1.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/SHADER_SKYBOX/fShader_skybox_v1.frag"
"${CMAKE_CURRENT_SOURCE_DIR}/images/container.jpg"
"${CMAKE_CURRENT_SOURCE_DIR}/images/awesomeface.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/CubeGrass_Down.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/CubeGrass_laterals.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/CubeGrass_Top.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/grass.png"
"${CMAKE_CURRENT_SOURCE_DIR}/images/simple_glassWindow.png"
"C:/Users/Briil/3D_Models_Game Engine/backpack/backpack.obj"
"C:/Users/Briil/3D_Models_Game Engine/Floor/Floor.obj"
"C:/Users/Briil/3D_Models_Game Engine/Floor_2/Floor2.obj"
"C:/Users/Briil/3D_Models_Game Engine/Car_Formula1/Car_Formula1.fbx"
"C:/Users/Briil/3D_Models_Game Engine/flash-light-17mb/FlashLight/FlashLight.obj"
"C:/Users/Briil/3D_Models_Game Engine/Campo/Campo_Floor.obj"
"C:/Users/Briil/3D_Models_Game Engine/vegetacion_01/vegetacion_01.obj"
"C:/Users/Briil/3D_Models_Game Engine/tokio_place/source/testexport.fbx"
)

set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets_engine"
"${CMAKE_CURRENT_SOURCE_DIR}/Images"
"${CMAKE_CURRENT_SOURCE_DIR}/shaders")

set(ASSETS_DESTINATION_DIR "${CMAKE_BINARY_DIR}/assets_engine"
"${CMAKE_BINARY_DIR}/Images"
"${CMAKE_BINARY_DIR}/shaders")

file(COPY ${ASSETS_SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

set(RUTE_COMPLETE_FILES ${DATA_FILE_PATH})

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/Render/configFilesTXT.h"
  "${PROjECT_BINARY_DIR}/configFilesTXT.h"

)


#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}libs/SDL")

#//find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad_source STATIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c)

target_include_directories(glad_source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include)

#para la libreria de glm
include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main
)

set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
ktx_software
GIT_REPOSITORY https://github.com/KhronosGroup/KTX-Software.git
GIT_TAG v4.3.2

)

set(KTX_FEATURE_TOOLS OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_TEST OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_DOC OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_LOAD_TEST_IMAGES OFF CACHE BOOL "" FORCE)


FetchContent_Declare(
 glm
 GIT_REPOSITORY https://github.com/g-truc/glm.git
 GIT_TAG 1.0.1

)

FetchContent_Declare(
assimp
GIT_REPOSITORY https://github.com/assimp/assimp.git
#GIT_TAG v5.4.1
GIT_TAG v6.0.2
GIT_SHALLOW ON
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

FetchContent_Declare(
imgui
GIT_REPOSITORY https://github.com/ocornut/imgui
GIT_TAG docking # Use the 'docking' branch or any desired tag
)

FetchContent_MakeAvailable(
SDL3
ktx_software
glm
assimp
imgui
)

#target_ling_libraries(imgui PUBLIC SDL3::SDL3 OpenGL::GL )

add_executable(Learning "learningOpenGL/learningOpenGL.cpp" "learningOpenGL/config_window.h" "learningOpenGL/config_window.cpp" "learningOpenGL/data_save.h" "learningOpenGL/data_save.cpp"
"learningOpenGL/control.cpp" "learningOpenGL/control.h" 
"learningOpenGL/SHADER_H.cpp" "learningOpenGL/SHADER_H.h" 
"learningOpenGL/GLM_test.h" "learningOpenGL/GLM_test.cpp"
"learningOpenGL/LIGHTS_test.h" "learningOpenGL/LIGHTS_test.cpp"
"learningOpenGL/playTest.h" "learningOpenGL/playTest.cpp"
"learningOpenGL/Model_Assimp/ModelAssimp.cpp" "learningOpenGL/Model_Assimp/ModelAssimp.h"
"learningOpenGL/Render/Render.cpp" "learningOpenGL/Render/Render.h"
"learningOpenGL/Render/RenderData.cpp" "learningOpenGL/Render/RenderData.h"
"learningOpenGL/Collision/ScreenHit.cpp" "learningOpenGL/Collision/ScreenHit.h"
"learningOpenGL/Collision/CollisionAABB.cpp" "learningOpenGL/Collision/CollisionAABB.h"
"learningOpenGL/2D_UI/2D_screenPlayer.cpp" "learningOpenGL/2D_UI/2D_screenPlayer.h"
"learningOpenGL/Render/frameBuffers.cpp" "learningOpenGL/Render/frameBuffers.h"
"learningOpenGL/threadSystem/thread_System.h" "learningOpenGL/threadSystem/thread_System.cpp"
"learningOpenGL/2D_UI/Interface_UI.cpp" "learningOpenGL/2D_UI/Interface_UI.h"
"learningOpenGL/2D_UI/Interface_generalUI.cpp" "learningOpenGL/2D_UI/Interface_generalUI.h"
"learningOpenGL/textureData/textureManager.cpp" "learningOpenGL/textureData/textureManager.h"
"learningOpenGL/libFile.h"
"learningOpenGL/STB_IMAGE_IMPLEMENT.cpp"

"libs/SDL/lib/arm64"
)


target_sources(Learning PRIVATE
${imgui_SOURCE_DIR}/imgui.cpp
${imgui_SOURCE_DIR}/imgui_draw.cpp
${imgui_SOURCE_DIR}/imgui_tables.cpp
${imgui_SOURCE_DIR}/imgui_demo.cpp
${imgui_SOURCE_DIR}/imgui_widgets.cpp

${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)


target_include_directories(Learning PRIVATE
${imgui_SOURCE_DIR}
${imgui_SOURCE_DIR}/backends
)
target_include_directories(Learning PRIVATE "${PROJECT_BINARY_DIR}")
target_include_directories(Learning PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdPartyLibs")
target_include_directories(Learning PRIVATE ${glm_SOURCE_DIR})
target_include_directories(Learning PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/Model_Assimp")
target_include_directories(Learning PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL")


target_link_libraries(Learning PRIVATE
SDL3::SDL3-shared
glad_source
OpenGL::GL
assimp::assimp
ktx
)

#add_custom_command(
 #   TARGET Learning POST_BUILD
 #   COMMAND ${CMAKE_COMMAND} -E copy
  #          $<TARGET_RUNTIME_DLLS:Learning>
  #          $<TARGET_FILE_DIR:Learning>
  #  COMMENT "Copiando los DLLs necesarios..."
#)


message(STATUS "SDL3 encontrado en: ${SDL3_DIR}")
message(STATUS "OpenGl encontrado en: ${OpenGL_DIR}")


#if(WIN32)
#    target_link_options(Learning PRIVATE "/SUBSYSTEM:WINDOWS")
#endif()