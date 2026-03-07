//
// Created by brii on 07/03/26.
//      glm::mat4 mat4{glm::mat4(1.0f)};
#include "Assimp_lib.h"


namespace data_Assimp
{

    glm::mat4 aiMat4_to_glmMat4(aiMatrix4x4& aiMat)
    {
       glm::mat4 mat4{glm::mat4(1.0f)};

        mat4[0][0] = aiMat.a1; mat4[1][0] = aiMat.a2; mat4[2][0] = aiMat.a3; mat4[3][0] = aiMat.a4;
        mat4[0][1] = aiMat.b1; mat4[1][1] = aiMat.b2; mat4[2][1] = aiMat.b3; mat4[3][1] = aiMat.b4;
        mat4[0][2] = aiMat.c1; mat4[1][2] = aiMat.c2; mat4[2][2] = aiMat.c3; mat4[3][2] = aiMat.c4;
        mat4[0][3] = aiMat.d1; mat4[1][3] = aiMat.d2; mat4[2][3] = aiMat.d3; mat4[3][3] = aiMat.d4;

     return mat4;
    }
}