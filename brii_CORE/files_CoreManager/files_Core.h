//
// Created by brii on 26/05/26.
//

#ifndef THIRDPARTY_LIBS_FILES_CORE_H
#define THIRDPARTY_LIBS_FILES_CORE_H

#include <iostream>
#include <filesystem>

namespace filesystem_manager
{
  void create_DirectoryFile(const std::string& filePath, bool& directoryExist);

}

namespace customFiles
{
 void clear_spaceKey(std::string& str);
 void standard_textureNameKTX(std::string& nameTexture);

}


#endif //THIRDPARTY_LIBS_FILES_CORE_H