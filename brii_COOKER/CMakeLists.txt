cmake_minimum_required (VERSION 3.21)

project(brii_COOKER)

add_executable(brii_COOKER "main_Cooker.cpp"
        "manager_briiCooker.cpp"
        "manager_briiCooker.h"
        "dataCooker.cpp"
        "dataCooker.h"
)

target_include_directories(brii_COOKER PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#TO GET A STRING WITH THE DIRECTION OF THE ENGINE, IN THE ENGINE CONVERT THIS IN A STRING
target_compile_definitions(brii_COOKER PRIVATE 
SOURCE_DIR_ENGINE_B="${SOURCE_DIR_ENGINE}"
)
 
target_link_libraries(brii_COOKER PRIVATE
Learning_lib
brii_CORE
assimp::assimp
glm::glm
ktx
thirdParty_Libs
)

if(TARGET assimp)
    target_compile_options(assimp PRIVATE -Wno-error)
    target_compile_options(assimp PRIVATE -w)
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(brii_COOKER PRIVATE
            m
            pthread
            dl
    )

    target_compile_options(brii_COOKER PRIVATE
            -Wall
            -Wextra
            -Werror
    )


endif()
