//
// Created by brii on 26/05/26.
//

#include "files_Core.h"

namespace filesystem_manager
{
    namespace fs = std::filesystem;
    void create_DirectoryFile(const std::string& filePath, bool& directoryExist)
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

  void delete_file_sentence(std::string fileName, const std::string& filePath)
  {
    std::cout << "deleting file = " << fileName;
  
    std::filesystem::path pt {filePath};
    std::filesystem::remove(pt);
  }
}

namespace customFiles
{
   void clear_spaceKey(std::string& str)
  {
    bool success{};

    while (!success)
    {
     size_t find_pos {str.find_first_of(" ")};

     if (find_pos != std::string::npos)
     {
       str.erase(find_pos, 1);
     }

     else if (find_pos == std::string::npos)
     {
       success = true;
     }
    }

  }

      void standard_textureNameKTX(std::string& nameTexture)
    {
      std::string f_name {nameTexture};
      std::string new_name {};

      bool success{};
      while (!success) {
        size_t find_pos{f_name.find_first_of("_")};

        if (find_pos != std::string::npos) 
        {

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

 void quit_double_underscore_txt(std::string& txt) 
 { 
      std::string f_name {txt};
      std::string new_name {};

      bool success{};
      while (!success) {
        size_t find_pos{f_name.find_first_of("_")};

        if (find_pos != std::string::npos) 
        {

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

     txt = new_name;

 }

}

#if defined(__linux__) && defined(__unix) || defined(__unix) && defined(__MACH__)

   uint32_t fileManager_POSIX::pwriting_handlingError(ssize_t bytes_writing, size_t size_bytes_file, const std::string& dir)
  { 
  if(bytes_writing < 0)
  {
   std::cerr << "ERROR POSIX::WRITING IN FILE::DIRECTION ---> " << dir << "\n";
   return 0;
  }
  else if(bytes_writing != size_bytes_file)
  {
   std::cerr << "ERROR POSIX::MEMORY IS FULL OF SPACE BEFORE WRITING IN FILE::LIBERATE SPACE:: ERROR DIRECTION ---> " << dir << "\n";
   return 0;
  }
 
  return 1;
  }
#endif 
