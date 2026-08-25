cmake_minimum_required (VERSION 3.21)

project(brii_CORE CXX)

set(APLICATION_ENGINE
 Application_Engine/Public/Application.h
)

set(SETTINGS_ENGINE
 Settings/Settings_BriiEngine.h
)

set(DATA_MANAGER
dataManager/algorithms_brii.h
dataManager/algorithms_brii.cpp
dataManager/containerTypes_manager.h
dataManager/containerTypes_manager.cpp
dataManager/convertion_DataManager.h
dataManager/convertion_DataManager.cpp
dataManager/dataTypes_brii.h
dataManager/dataTypes_brii.cpp
)

set(FILES_CORE_MANAGER
files_CoreManager/files_Core.cpp
files_CoreManager/files_Core.h
)

set(SYSTEM_MANAGER
systemManager/logger.h          #////===MULTIPLATFORM LIBRARY====
systemManager/logger.cpp
systemManager/platform_Manager.h
)

set(THREAD_CORE
threadCore/thread_Manager.h
threadCore/thread_Manager.cpp
)

set(BINARIES_FORMATS
material_binFormat.h
mesh_binFormat.h
model_binFormat.h
texturesPack_binFormat.h
)


#add_executable(briiCore
#${APLICATION_ENGINE}
#${SETTINGS_ENGINE}
#${DATA_MANAGER}
#${FILES_CORE_MANAGER}
#${SYSTEM_MANAGER}
#${THREAD_CORE}
#${0BINARIES_FORMATS}
#glm_lib.h
#libs_in_CORE.h
#main_core.h
#main_core.cpp
#)

add_library(brii_CORE STATIC
 ${APLICATION_ENGINE}
 ${SETTINGS_ENGINE}
 ${DATA_MANAGER}
 ${FILES_CORE_MANAGER}
 ${SYSTEM_MANAGER}
 ${THREAD_CORE}
 ${BINARIES_FORMATS}
 glm_lib.h
 libs_in_CORE.h
 main_core.h
 main_core.cpp
)


if(TARGET_PLATFORM STREQUAL "PC")

set(APPLICATION_PC
 Application_Engine/Private/Application_PC.cpp
)

target_sources(brii_CORE PRIVATE ${APPLICATION_PC})
target_link_libraries(brii_CORE PRIVATE SDL3::SDL3-shared)
target_include_directories(brii_CORE PRIVATE ${APPLICATION_PC})

endif()

#target_link_libraries(briiCore PUBLIC
#assimp::assimp
#glm::glm
#ktx
#)


target_include_directories(brii_CORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(brii_CORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/brii_CORE")

target_include_directories(brii_CORE PRIVATE
 ${APLICATION_ENGINE}
 ${SETTINGS_ENGINE}
 ${DATA_MANAGER}
 ${FILES_CORE_MANAGER}
 ${SYSTEM_MANAGER}
 ${THREAD_CORE}
 ${BINARIES_FORMATS}
 glm_lib.h
 libs_in_CORE.h
 main_core.h
 main_core.cpp
)

target_link_libraries(brii_CORE PRIVATE
assimp::assimp
glm::glm
ktx
Brii_Platform
Brii_GraphicsInterface
)

#target_include_directories(briiCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/brii_CORE")
#target_include_directories(briiCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
