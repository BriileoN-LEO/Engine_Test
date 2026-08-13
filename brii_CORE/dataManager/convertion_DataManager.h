//
// Created by brii on 14/05/26.
//

#ifndef LEARNING_CONVERTION_DATAMANAGER_H
#define LEARNING_CONVERTION_DATAMANAGER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "systemManager/logger.h"
#include "glm_lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

namespace convert_dataTypes
{
  std::array<float, 16> aiMat4_to_arrayFloat16(aiMatrix4x4& aiMat);
  void aiMat4_to_rawArrayFloat16(aiMatrix4x4& aiMat, float (&rawArray)[16]);
  glm::mat4 rawArrayFloat16_to_glmMat4(float (&rawArray)[16]);  
  glm::mat4 arrayFloat16_to_glmMat4(std::array<float, 16>& arrayFloat16);

  template<typename T, size_t sT>
  void copy_rawArrayData(T (&dataIN)[sT], T (&dataOUT)[sT]) /////COPY RAW ARRAY TO ANOTHER RAW ARRAY WITH THE SAME TYPE AND SIZE
  {
    std::copy(dataIN, dataIN + sT, dataOUT);
  } 
   
  template<typename T, size_t sT>
  glm::vec2 rawArray_to_GLMvec2(T rawArray[sT])
  {
    glm::vec2 conv{};
    int max{ (sT < 2) ? sT : 2};
    for(int i = 0; i < max; i++)
   {
    conv[i] = static_cast<float>(rawArray[i]);
   }
     
   return conv;
  }

  template<typename T, size_t sT>
  glm::vec3 rawArray_to_GLMvec3(T rawArray[sT])
  {
    glm::vec3 conv{};
    int max{ (sT < 3) ? sT : 3};
    for(int i = 0; i < max; i++)
   {
    conv[i] = static_cast<float>(rawArray[i]);
   }
     
   return conv;
  }

  template<typename T, size_t sT>
  glm::vec4 rawArray_to_GLMvec4(T rawArray[sT])
  {
    glm::vec4 conv{};
    int max{ (sT < 4) ? sT : 4};
    for(int i = 0; i < max; i++)
   {
    conv[i] = static_cast<float>(rawArray[i]);
   }
     
   return conv;
  }

 
}

namespace convert_str
{
 bool find_badCharacters_filePath(std::string& str, size_t& pos_BC);
 void quit_repetitive_char(std::string& str, char char_quit);
 void quit_first_End_Spaces_str(std::string& str);
 uint32_t contains_characters_str(std::string& str);
 void remplace_char_in_str(std::string& txt, std::string char_t, size_t position, uint32_t expand_str, uint32_t deleteRepetition); ///expand_str = if the position is biggest thant the size of txt, it will be expand the txt with '\0' (empty spaces) if its declare with (1), if is (0) the last character of txt will be replaced by the char_t


 template <size_t sT>
 void conv_str_to_rawArray(std::string& str, char (& rawArray)[sT])
 {  
   if(!str.empty() && contains_characters_str(str) == 1)
  {
   //size_t find_p {str.find_first_of('\0')};
  // memset(rawArray, '\0',  sT);
   std::fill(std::begin(rawArray), std::end(rawArray), '\0');
   strcpy(rawArray, str.c_str());

   return; 
  }

  log_System::fileLogger.warning("cannot convert str to rawArray[] | empty str");
 }

}


#endif //LEARNING_CONVERTION_DATAMANAGER_H
