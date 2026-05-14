cmake_minimum_required (VERSION 3.21)

project(briiCooker)

add_executable(briiCooker "main_Cooker.cpp"
        "manager_briiCooker.cpp"
        "manager_briiCooker.h"
)

target_include_directories(briiCooker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(briiCooker PRIVATE
Learning_lib
briiCore
assimp::assimp
glm::glm
)

if(TARGET assimp)
    target_compile_options(assimp PRIVATE -Wno-error)
    target_compile_options(assimp PRIVATE -w)
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(briiCooker PRIVATE
            m
            pthread
            dl
    )


    target_compile_options(briiCooker PRIVATE
            -Wall
            -Wextra
            -Werror
    )


endif()