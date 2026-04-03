//
// Created by brii on 27/03/26.
//

#ifndef LEARNING_LOG_ERROR_GENERAL_H
#define LEARNING_LOG_ERROR_GENERAL_H

#include "learningOpenGL.h"

namespace log_ErrorG
{

    void register_w_(const char* file, int line, const char* sentence);
    #define register_w(sen) register_w_(__FILE__, __LINE__, sen)
}

namespace log_Error_shader
{
    GLint CompileShader_(GLuint& shader, const char* type, Uint8 typeTest, int line, const char* file);
#define CompileShader(shader, type, typeTest) CompileShader_(shader, type, typeTest, __LINE__, __FILE__)
}


#endif //LEARNING_LOG_ERROR_GENERAL_H