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
    void aiMat4_to_rawArrayFloat16(aiMatrix4x4& aiMat, float (&matArray)[16])
    {
       matArray[0] = aiMat.a1; matArray[1] = aiMat.a2; matArray[2] = aiMat.a3; matArray[3] = aiMat.a4;
       matArray[4] = aiMat.b1; matArray[5] = aiMat.b2; matArray[6] = aiMat.b3; matArray[7] = aiMat.b4;
       matArray[8] = aiMat.c1; matArray[9] = aiMat.c2; matArray[10] = aiMat.c3; matArray[11] = aiMat.c4;
       matArray[12] = aiMat.d1; matArray[13] = aiMat.d2; matArray[14] = aiMat.d3; matArray[15] = aiMat.d4;
    }

    glm::mat4 arrayFloat16_to_glmMat4(std::array<float, 16>& arrayFloat16)
    {
        glm::mat4 mat4_D {glm::mat4(1.0f)};

        mat4_D[0][0] = arrayFloat16[0]; mat4_D[1][0] = arrayFloat16[1]; mat4_D[2][0] = arrayFloat16[2]; mat4_D[3][0] = arrayFloat16[3];
        mat4_D[0][1] = arrayFloat16[4]; mat4_D[1][1] = arrayFloat16[5]; mat4_D[2][1] = arrayFloat16[6]; mat4_D[3][1] = arrayFloat16[7];
        mat4_D[0][2] = arrayFloat16[8]; mat4_D[1][2] = arrayFloat16[9]; mat4_D[2][2] = arrayFloat16[10]; mat4_D[3][2] = arrayFloat16[11];
        mat4_D[0][3] = arrayFloat16[12]; mat4_D[1][3] = arrayFloat16[13]; mat4_D[2][3] = arrayFloat16[14]; mat4_D[3][3] = arrayFloat16[15];
     
     return mat4_D;
    }

   template<typename T, size_t sT>
   void copy_rawArrayData(T (&dataIN)[sT], T (&dataOUT)[sT])
   {
    std::copy(dataIN, dataIN + sT, dataOUT);
   }

}

namespace convert_str
{
    bool find_badCharacters_filePath(std::string& str, size_t& pos_BC)
    {
        constexpr std::string_view badCharacters{ "./"};
        size_t find_BC {str.find_first_of(badCharacters)};

        if (!str.empty() || find_BC != std::string::npos)
        {
            pos_BC = find_BC;

          return true;
        }

        return false;
    }

   void quit_first_End_Spaces_str(std::string& str)
   {
    //make this algorithm in the future
    //INSTRUCTIONS:
    //*quit all the spaces ('\0') in string before the first character.
    //*quit all the spaces ('\0') ins string after the last character.
    /*
     for(int i = 0; i < static_cast<int>(str); i++)
     {
      size_t posFirst { str.find_first_of('\0')};
      size_t posLast { str.find_last_of('\0')};
    
      if(pos_First
        ////CONTINUE HERE TO QUIT SPACE STARTED AND TO THE END OF THE MAXIMUM AND MINIMUM CHARACTERS IN STRING
     }
    */
   }

  uint32_t contains_characters_str(std::string& str)
  {
    for(int i = 0; i < static_cast<int>(str.size()); i++)
    {
      if(static_cast<int>(str[i] ^ '\0') != 0)
      {
       return 1;
      }
    }

    return 0;
  }
 
void remplace_char_in_str(std::string& txt, std::string char_t, size_t position, uint32_t expand_str, uint32_t deleteRepetition)
{ ////CONTINUE HERE 
  auto delete_repetition = [&](size_t minPos_newChar, size_t maxPos_newChar)
    { 
      if(minPos_newChar > 0)
      {
       int test_min {(txt[minPos_newChar - 1] == txt[minPos_newChar]) ? 1 : 0};  
       txt.erase(minPos_newChar, test_min);
       maxPos_newChar -= test_min;
      }

      if(maxPos_newChar < (txt.size() - 1))
      {
       int test_max {(txt[maxPos_newChar + 1] == txt[maxPos_newChar]) ? 1 : 0};
       txt.erase(maxPos_newChar, test_max); ///test if i put (0), doesnt eliminate nothing
      }
       
    };

  if(expand_str == 1)
  {    
    txt.insert(position, char_t);
  }

  else if(expand_str == 0)
  {  
   size_t total_size_new {(position + 1) + char_t.size()};
   size_t size_txt_new{ total_size_new > txt.size() ? total_size_new : txt.size()};
   txt.resize(size_txt_new);
   txt.replace(position, char_t.size(), char_t);
  }

  if(deleteRepetition == 1)
  {
   size_t max_sizePos {position + (char_t.size() - 1)};  
   delete_repetition(position, max_sizePos);
  }

}




}
