//
// Created by brii on 14/05/26.
//

#include "convertion_DataManager.h"

namespace convert_dataTypes
{
    std::array<float, 16> aiMat4_to_arrayFloat16(aiMatrix4x4& aiMat)
    {
       std::array<float, 16> matArray{};

       matArray[0] = aiMat.a1; matArray[1] = aiMat.a2; matArray[2] = aiMat.a3; matArray[3] = aiMat.a4;
       matArray[4] = aiMat.b1; matArray[5] = aiMat.b2; matArray[6] = aiMat.b3; matArray[7] = aiMat.b4;
       matArray[8] = aiMat.c1; matArray[9] = aiMat.c2; matArray[10] = aiMat.c3; matArray[11] = aiMat.c4;
       matArray[12] = aiMat.d1; matArray[13] = aiMat.d2; matArray[14] = aiMat.d3; matArray[15] = aiMat.d4;

       return matArray;
    }

    glm::mat4 arrayFloat16_to_glmMat4(std::array<float, 16>& arrayFloat16)
    {
        glm::mat4 mat4_D {glm::mat4(1.0f)};

        mat4_D[0][0] = arrayFloat16[0]; mat4_D[1][0] = arrayFloat16[1]; mat4_D[2][0] = arrayFloat16[2]; mat4_D[3][0] = arrayFloat16[3];
        mat4_D[0][1] = arrayFloat16[4]; mat4_D[1][1] = arrayFloat16[5]; mat4_D[2][1] = arrayFloat16[6]; mat4_D[3][1] = arrayFloat16[7];
        mat4_D[0][2] = arrayFloat16[8]; mat4_D[1][2] = arrayFloat16[9]; mat4_D[2][2] = arrayFloat16[10]; mat4_D[3][2] = arrayFloat16[11];
        mat4_D[0][3] = arrayFloat16[12]; mat4_D[1][3] = arrayFloat16[13]; mat4_D[2][3] = arrayFloat16[14]; mat4_D[3][3] = arrayFloat16[15];

    }
}