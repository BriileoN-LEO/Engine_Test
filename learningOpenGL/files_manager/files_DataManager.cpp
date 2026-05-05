//
// Created by brii on 03/04/26.
//

#include "files_DataManager.h"
#include "log_Errors/log_error_General.h"
#include "data_save.h"

namespace filesData
{
    std::string read_FileData_str_(const char* file_path, int line, const char* file)
    {

        std::ifstream StreamFile;

        StreamFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::stringstream file_Str_Stream;

        try
        {
            StreamFile.open(file_path);
            file_Str_Stream << StreamFile.rdbuf();
            StreamFile.close();
        }
        catch (const std::ifstream::failure& e)
        {
           // std::string log_error {"ERROR::FILE_NOT_SUCCESFULLY_READ ---->" +  static_cast<std::string>(file_path) + '\n'};
           // log_ErrorG::register_w_(file, line, log_error.c_str());
            std::cout << e.what();
            //SDL_Log(e.what());
        }

        std::string file_str { file_Str_Stream.str() };

        resolve_Errors::quit_BOM_UFT_8(file_str);  ///RESOLVE DE BOM UTF 8 OR OTHER PROBLEMS

        std::string log_success {file_path};
        log_success = "READ_FILE::COMPLETED---->" + log_success;
        SDL_Log(log_success.c_str());

        return file_str;
    }
}