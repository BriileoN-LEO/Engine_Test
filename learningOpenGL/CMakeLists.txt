# CMakeList.txt : CMake project for learningOpenGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

# Add source to this project's executable.
#add_executable (learningOpenGL "learningOpenGL.cpp" "learningOpenGL.h")

add_executable(Learning "learningOpenGL.cpp" "learningOpenGL.h"
"libFile.h"
"config_window.h" "config_window.cpp" "data_save.h" "data_save.cpp"
"control.cpp" "control.h"
"SHADER_H.cpp" "SHADER_H.h"
"GLM_test.h" "GLM_test.cpp"
"LIGHTS_test.h" "LIGHTS_test.cpp"
"playTest.h" "playTest.cpp"
"Model_Assimp/ModelAssimp.cpp" "Model_Assimp/ModelAssimp.h"
"Render/Render.cpp" "Render/Render.h"
"Render/RenderData.cpp" "Render/RenderData.h"
"Collision/ScreenHit.cpp" "Collision/ScreenHit.h"
"Collision/CollisionAABB.cpp" "Collision/CollisionAABB.h"
"2D_UI/2D_screenPlayer.cpp" "2D_UI/2D_screenPlayer.h"
"Render/frameBuffers.cpp" "Render/frameBuffers.h"
"threadSystem/thread_System.h" "threadSystem/thread_System.cpp"
"2D_UI/Interface_UI.cpp" "2D_UI/Interface_UI.h"
"2D_UI/Interface_generalUI.cpp" "2D_UI/Interface_generalUI.h"
"Edit_Modes/Edit_M.cpp" "Edit_Modes/Edit_M.h"
"textureData/textureManager.cpp" "textureData/textureManager.h"
"2D_UI/system_UI/system_UI_B.cpp" "2D_UI/system_UI/system_UI_B.h"
"resource_Manager/resourceManager.h" "resource_Manager/resourceManager.cpp"
"optimize_Algorithmics/optimizeAlgorithmics.h" "optimize_Algorithmics/optimizeAlgorithmics.cpp"
"Model_Assimp/Assimp_lib.h"  "Model_Assimp/Assimp_lib.cpp"
"shadows_manager/shadows_D.h"  "shadows_manager/shadows_D.cpp"
"STB_IMAGE_IMPLEMENT.cpp"
        "Model_Assimp/Assimp_lib.h" "Model_Assimp/Assimp_lib.cpp"
"2D_geo/basic_geometry_D.cpp" "2D_geo/basic_geometry_D.h"
"log_Errors/log_error_General.cpp" "log_Errors/log_error_General.h"
"libCust_openGL/lib_openGL.cpp" "libCust_openGL/lib_openGL.h"
"Render/Clustered_Shading_Render.cpp" "Render/Clustered_Shading_Render.h"
"files_manager/files_DataManager.cpp" "files_manager/files_DataManager.h"
#"brii_CORE/Types_import.h"
        #"libs/SDL/lib/arm64"

)

add_library(Learning_lib INTERFACE
"optimize_Algorithmics/optimizeAlgorithmics.h"
)

target_include_directories(Learning_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

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
target_include_directories(Learning PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(Learning PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ThirdPartyLibs")
target_include_directories(Learning PUBLIC ${glm_SOURCE_DIR})
target_include_directories(Learning PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/Model_Assimp")
target_include_directories(Learning PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL"
        "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/Edit_Modes"
        "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/resource_Manager"
        "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/optimize_Algorithmics"
        "${CMAKE_CURRENT_SOURCE_DIR}/learningOpenGL/Model_Assimp"
)
target_include_directories(Learning PUBLIC ${stb_SOURCE_DIR})

target_link_libraries(Learning PRIVATE
brii_CORE
SDL3::SDL3-shared
glad_source
OpenGL::GL
assimp::assimp
glm::glm
ktx
)

target_include_directories(Learning PUBLIC ${glm_SOURCE_DIR})

if(TARGET assimp)
    target_compile_options(assimp PRIVATE -Wno-error)
    target_compile_options(assimp PRIVATE -w)
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(Learning PRIVATE
            m
            pthread
            dl
    )


    target_compile_options(Learning PRIVATE
            -Wall
            -Wextra
            -Werror
    )


endif()

#add_custom_command(
 #   TARGET Learning POST_BUILD
 #   COMMAND ${CMAKE_COMMAND} -E copy
  #          $<TARGET_RUNTIME_DLLS:Learning>
  #          $<TARGET_FILE_DIR:Learning>
  #  COMMENT "Copiando los DLLs necesarios..."
#)

message(STATUS "SDL3 encontrado en: ${SDL3_DIR}")
message(STATUS "OpenGl encontrado en: ${OpenGL_DIR}")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-error)
    target_compile_options(Learning PRIVATE -Wno-error)
endif()

#if(WIN32)
#    target_link_options(Learning PRIVATE "/SUBSYSTEM:WINDOWS")
#endif()


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET learningOpenGL PROPERTY CXX_STANDARD 20)
#endif()

