//
// Created by brii on 26/05/26.
//

#include "files_Core.h"

namespace filesystem_manager
{
    namespace fs = std::filesystem;
    void create_DirectoryFile(const std::string& filePath, uint32_t& directoryExist)
    {
     fs::path finalPath {filePath};
     fs::path directoryFather {finalPath.parent_path()};

     if (!directoryFather.empty() && fs::exists(directoryFather))
     {
       log_System::fileLogger.warning("directory not exists | creating directory file = " + filePath);
       //std::cout << "FILESYSTEM::DIRECTORY NOT EXISTS::CREATING DIRECTORY FILE -----> " + filePath << std::endl;

       fs::create_directories(directoryFather);
       directoryExist = 0;

       return;
     }

     directoryExist = 1;
     log_System::fileLogger.info("directory exists | stop creating directory = " + filePath);
    }

  void delete_file_sentence(std::string fileName, const std::string& filePath)
  {
    std::cout << "deleting file = " << fileName;
  
    std::filesystem::path pt {filePath};
    std::filesystem::remove(pt);
  }

   uint32_t exist_filePath(const std::string& path)
  {

  #if defined(__linux__) && defined(__unix) || defined(__unix) && defined(__MACH__)

   return (access(path.c_str(), F_OK) == 0) ? 1 : 0;

  #elif defined(_WIN32)

   return std::filesystem::exists(path) ? 1 : 0;

  #endif
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


  void rename_fileExisting(std::string& dirOrigin)
 {
  
   if(filesystem_manager::exist_filePath(dirOrigin) == 1)  // 1 = EXISTS | 0 = NOT EXISTS 
  { 
   size_t pos_maxName {dirOrigin.find_last_of('.')};
   size_t pos_minName {dirOrigin.find_last_of('/')};

   if((pos_maxName == std::string::npos) || (pos_minName == std::string::npos))
   {
     std::cerr << "ERROR::FILE_CORE:: not rename file existing:: cause = not path\n";
    return;
   }

   size_t dirO_size { dirOrigin.size() - 1};

   ///name size = 10;  -- range = 6 - 8 -- range_pos = 5 - 7
   //pos_minName = 5;
   //pos_maxName = 7;
   //(9 - (9-7)) - (5+1) = 1
  
    //name size = 20; -- range = 14 - 17 --- range pos = 13 - 16
    //(19 - (19 - 16)) - (13 + 1) = 2
   
   size_t size_nameFile { (dirO_size - (dirO_size - pos_maxName)) - (pos_minName + 1)};

   std::string nameFile { dirOrigin.substr((pos_minName + 1), size_nameFile) };
   std::string vefNumT { dirOrigin.substr(pos_maxName) };
   std::string originDir { dirOrigin.substr(0, pos_minName + 1) };

   std::string newDir{};
   uint32_t access_out{};

   uint32_t val_count{};
   while(access_out == 1) ////CHANGE name adding number to test if file exists 
   {
     newDir = originDir + nameFile + std::to_string(val_count) + vefNumT;
     access_out = filesystem_manager::exist_filePath(newDir);  
     val_count++;
   }

   dirOrigin = newDir;
   return;
 }

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
