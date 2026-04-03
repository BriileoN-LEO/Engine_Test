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


namespace log_Error_shader {

    GLint CompileShader_(GLuint& shader, const char* type, Uint8 typeTest, int line, const char* file)
    {
        GLint test_Shader{};

        if (typeTest == 0)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &test_Shader);
        }

        else if (typeTest == 1)
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &test_Shader);
        }

        if (test_Shader != GL_TRUE)
        {
            //	char infoLog[512];
            GLint logLength{};

          //  std::string logInfo{"logLength"};
            GLchar infoLog[512];

            if (typeTest == 0)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                glGetShaderInfoLog(shader, logLength, nullptr, infoLog);
            }

            if (typeTest == 1)
            {
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                glGetProgramInfoLog(shader, logLength, nullptr, infoLog);
            }


            std::string log_Error{};

            if (type != nullptr)
            {
                log_Error = type;
                log_Error += "::";
            }

            log_Error = "ERROR::SHADER::" + log_Error + "COMPILATION_FAILED --->" + infoLog;

            log_ErrorG::register_w_(file, line, log_Error.c_str());
            //SDL_Log(log_Error.c_str());
            //SDL_Log(infoLog);

        }

        else
        {
          std::string log_complete {type};
          log_complete = "SHADER::" + log_complete + "::COMPILATION_COMPLETED";
          SDL_Log(log_complete.c_str());

        }

        return test_Shader;
    }
}