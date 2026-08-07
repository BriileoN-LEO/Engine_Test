//
// Created by brii on 26/05/26.
//

#ifndef THIRDPARTY_LIBS_FILES_CORE_H
#define THIRDPARTY_LIBS_FILES_CORE_H

#include "systemManager/platform_Manager.h"
#include "systemManager/logger.h"
#include "dataManager/dataTypes_brii.h"
#include <iostream>
#include <filesystem>

namespace file_OP
{
 enum class writeFlags : uint32_t 
 {
  OVERWRITE = 0, //0001
  NEW_FILE = 1 << 0//0010
 }; 

}

template<> inline constexpr bool is_bitmask_flag<file_OP::writeFlags> = true;

namespace filesystem_manager
{
  void create_DirectoryFile(const std::string& filePath, uint32_t& directoryExist);
  void delete_file_sentence(std::string sentence, const std::string& filePath);
  uint32_t exist_filePath(const std::string& path); /// return 1 = exist file  ||  return 0 = not exist
}

namespace customFiles
{
 void clear_spaceKey(std::string& str);
 void standard_textureNameKTX(std::string& nameTexture); 
 void quit_double_underscore_txt(std::string& txt);

 void rename_fileExisting(std::string& dirOrigin);  ///nameFile with type file === example: file.txt
}




#if defined(__linux__) && defined(__unix) || defined(__unix) && defined(__MACH__)

namespace fileManager_POSIX
{
   uint32_t pwriting_handlingError(ssize_t bytes_writing, size_t size_bytes_file, const std::string& dir);
}

#endif

#endif //THIRDPARTY_LIBS_FILES_CORE_H
