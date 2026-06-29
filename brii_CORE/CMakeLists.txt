cmake_minimum_required (VERSION 3.21)

project (briiCore)

add_executable(briiCore
 "mesh_binFormat.h"
 "material_binFormat.h"
 "model_binFormat.h"
 "glm_lib.h"
 "dataManager/convertion_DataManager.cpp"
 "dataManager/convertion_DataManager.h"
 "files_CoreManager/files_Core.cpp"
 "files_CoreManager/files_Core.h"
 "dataManager/dataTypes_brii.cpp"
 "dataManager/dataTypes_brii.h"
 "texturesPack_binFormat.h"
 "dataManager/algorithms_brii.cpp"
 "dataManager/algorithms_brii.h"
 "libs_in_CORE.h"
)

add_library(brii_CORE INTERFACE
 "mesh_binFormat.h"
 "material_binFormat.h"
 "model_binFormat.h"
 "glm_lib.h"
 "dataManager/convertion_DataManager.cpp"
 "dataManager/convertion_DataManager.h"
 "files_CoreManager/files_Core.cpp"
 "files_CoreManager/files_Core.h"
 "dataManager/dataTypes_brii.cpp"
 "dataManager/dataTypes_brii.h"
 "texturesPack_binFormat.h"
 "dataManager/algorithms_brii.cpp"
 "dataManager/algorithms_brii.h"
 "libs_in_CORE.h"
)

target_link_libraries(brii_CORE INTERFACE
assimp::assimp
glm::glm
)

target_link_libraries(briiCore PUBLIC
assimp::assimp
glm::glm
)

target_include_directories(brii_CORE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(briiCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/brii_CORE")
target_include_directories(briiCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})