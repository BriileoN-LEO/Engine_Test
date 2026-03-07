//
// Created by brii on 05/03/26.
//

#ifndef ASSIMP_LIB
#define ASSIMP_LIB

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "learningOpenGL.h"


namespace data_Assimp
{

    glm::mat4 aiMat4_to_glmMat4(aiMatrix4x4& aiMat);


}


#endif //LEARNING_ASSIMP_LIB_H