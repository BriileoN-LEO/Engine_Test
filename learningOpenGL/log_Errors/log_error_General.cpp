//
// Created by brii on 27/03/26.
//

#include "log_error_General.h"

namespace log_ErrorG
{

    void register_w_(const char* file, int line, const char* sentence)
    {
        std::string str_error {sentence};
        str_error += " | ";
        str_error += + file;
        str_error += "---> line: " + std::to_string(line);


        SDL_Log(str_error.c_str());
    }


}