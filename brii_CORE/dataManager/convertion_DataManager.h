//
// Created by brii on 14/05/26.
//

#ifndef LEARNING_CONVERTION_DATAMANAGER_H
#define LEARNING_CONVERTION_DATAMANAGER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm_lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

namespace convert_dataTypes
{
  std::array<float, 16> aiMat4_to_arrayFloat16(aiMatrix4x4& aiMat);
  glm::mat4 arrayFloat16_to_glmMat4(std::array<float, 16>& arrayFloat16);
}

namespace convert_str
{
 bool find_badCharacters_filePath(std::string& str, size_t& pos_BC);

}


#endif //LEARNING_CONVERTION_DATAMANAGER_H