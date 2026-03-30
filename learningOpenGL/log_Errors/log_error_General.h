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


#endif //LEARNING_LOG_ERROR_GENERAL_H