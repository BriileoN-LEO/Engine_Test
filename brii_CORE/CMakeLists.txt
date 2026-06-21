cmake_minimum_required (VERSION 3.21)

project (brii_CORE)

add_library(brii_CORE INTERFACE
 "mesh_binFormat.h"
 "material_binFormat.h"
 "model_binFormat.h"
 "dataManager/convertion_DataManager.cpp"
 "dataManager/convertion_DataManager.h"
 "glm_lib.h"
 "files_CoreManager/files_Core.cpp"
 "files_CoreManager/files_Core.h"
 "dataManager/dataTypes_brii.cpp"
 "dataManager/dataTypes_brii.h"
  "texturesPack_binFormat.h"
)
target_link_libraries(brii_CORE INTERFACE
assimp::assimp
glm::glm
)

target_include_directories(brii_CORE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})