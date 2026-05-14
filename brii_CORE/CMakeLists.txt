cmake_minimum_required (VERSION 3.21)

project (briiCore)

add_library(briiCore INTERFACE
 "mesh_binFormat.h"
 "material_binFormat.h"
 "model_binFormat.h"
 "dataManager/convertion_DataManager.cpp"
 "dataManager/convertion_DataManager.h"
 "glm_lib.h"
)
target_link_libraries(briiCore INTERFACE
assimp::assimp
glm::glm
)

target_include_directories(briiCore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})