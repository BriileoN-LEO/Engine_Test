//
// Created by brii on 26/05/26.
//

#include "files_Core.h"

namespace filesystem_manager
{
    namespace fs = std::filesystem;
    void create_DirectoryFile(std::string& filePath, bool& directoryExist)
    {
     fs::path finalPath {filePath};
     fs::path directoryFather {finalPath.parent_path()};

     if (!directoryFather.empty() && fs::exists(directoryFather))
     {
       std::cout << "FILESYSTEM::DIRECTORY NOT EXISTS::CREATING DIRECTORY FILE -----> " + filePath << std::endl;

       fs::create_directories(directoryFather);
       directoryExist = false;

       return;
     }

     directoryExist = true;
     std::cout << "FILESYSTEM::DIRECTORY EXISTS" << std::endl;

    }
}

namespace customFiles
{
    void standard_textureNameKTX(std::string& nameTexture)
    {
      std::string f_name {nameTexture};
      std::string new_name {};

      bool success{};
      while (!success) {
        size_t find_pos{f_name.find_first_of("_")};

        if (find_pos != std::string::npos) {

          size_t find_pCopy{find_pos + 1};
          new_name += f_name.substr(0, find_pos);

          if (f_name[find_pCopy] == '_')
          {
            f_name = f_name.substr(find_pCopy + 1);
          }

          else
          {
            f_name = f_name.substr(find_pCopy);
          }
        }

        else if (find_pos == std::string::npos)
        {
          new_name += f_name.substr(0);
          success = true;
        }

      }

     nameTexture = new_name;
    }

}