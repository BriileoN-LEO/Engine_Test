#ifndef MODEL_BINFORMAT_H
#define MODEL_BINFORMAT_H
#include "systemManager/platform_Manager.h"
#include "dataManager/convertion_DataManager.h"
#include "files_CoreManager/files_Core.h"
#include "systemManager/logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

constexpr size_t MAX_SIZE_STR_BIN_MODEL = 512;
constexpr char vN_LEOR[4] {'L', 'E', 'O', 'R'};

struct model_LeoHeader
{
 char modelName[MAX_SIZE_STR_BIN_MODEL]{};
 size_t meshesCount{};
 char verifiedNumber[4]{vN_LEOR[0], vN_LEOR[1], vN_LEOR[2], vN_LEOR[3]};  ///LEOR [HEADER]
 uint32_t version{};

  auto& get_modelName()
 {
   return modelName;
 }

 const auto& get_meshesCount()
 {
  return meshesCount;
 }
 
  const auto& get_vefNumber()
 {
  return verifiedNumber;
 }

  auto& get_version()
 {
  return version;
 }


   // uint64_t modelID{};
};
///REGISTER NUMBER OF ID MESHES
//uint64_t meshID

//struct meshAsset_register 
//{ 
 //   uint64_t meshID{}; ////ALL THE ID MESHES IN SEQUENCE
//};
#pragma pack(pop)

namespace data_modelCore
{
  const std::string vefNum {".leor"};
  const std::string temp_verfCreationFileDir {"tempBinModel"};
 
  constexpr size_t get_maxSize_nameModelArray()
 { 
  return MAX_SIZE_STR_BIN_MODEL;
 }

}

 #if defined(__unix__) || defined(__unix) && defined(__MACH__)

 inline uint32_t pack_binModel_posix_overwrite(const model_LeoHeader& headerModel, const std::vector<uint64_t>& meshes_ID, const std::string& outDir,const std::string& dirOrigin)
 { 
  std::string dirOrigin_new {dirOrigin};
  
   //convert_str::remplace_char_in_str(dirOrigin_new, "/", dirOrigin_new.size(), 0, 1);
  
  std::string temp_modelFileDir {headerModel.modelName + data_modelCore::temp_verfCreationFileDir + data_modelCore::vefNum};
  temp_modelFileDir = dirOrigin + temp_modelFileDir;
  convert_str::quit_repetitive_char(temp_modelFileDir, '/');

  std::string outDir_re {outDir};
  convert_str::quit_repetitive_char(outDir_re, '/');

  int binModel {open(temp_modelFileDir.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)};

  if(binModel < 0)
  {
   log_System::fileLogger.error("error writing modelBin | not open modelBin | modelBin = " + temp_modelFileDir);
   return 0;
  }
  
  log_System::fileLogger.info("writing file | file = " + temp_modelFileDir);

  size_t bytes_sizeHeader {sizeof(headerModel)};
  size_t bytes_sizeMeshesID {sizeof(uint64_t) * meshes_ID.size()};

  size_t bytesRest_write {bytes_sizeHeader + bytes_sizeMeshesID};

   /////////SEE IF THERES IS SPACE TO ALLOCATE THE BINARY AND WRITE
  int reserve_space = posix_fallocate(binModel, 0, bytesRest_write);
  if(reserve_space != 0)
  {
   log_System::fileLogger.error("not enought space to write modelBin | modelBin = " + temp_modelFileDir);
   close(binModel);
   filesystem_manager::delete_file_sentence(headerModel.modelName, temp_modelFileDir);
   return 0;
  }
  
  off_t offset_bin{};

  //FASE 1 == Model bin header
  const void* buffer_header {reinterpret_cast<const void*>(&headerModel)};
  ssize_t countBytes_HeaderWrite {pwrite(binModel, buffer_header, bytes_sizeHeader, offset_bin)};
 
  std::cout << "BYTES WRITE FILE = " << countBytes_HeaderWrite << '\n';
  std::cout << "BYTES HOPE TO WRITE = " << bytes_sizeHeader << '\n';

  if(fileManager_POSIX::pwriting_handlingError(countBytes_HeaderWrite, bytes_sizeHeader, temp_modelFileDir) == 0)
  {
   close(binModel);
   filesystem_manager::delete_file_sentence(headerModel.modelName, temp_modelFileDir);
   return 0;
  }

  offset_bin = bytes_sizeHeader;

  //FASE 2 == Add meshes ID
  const void* buffer_meshesID {reinterpret_cast<const void*>(meshes_ID.data())};
  ssize_t countBytes_meshesID_write {pwrite(binModel, buffer_meshesID, bytes_sizeMeshesID, offset_bin)};

  std::cout << "BYTES meshesID model WRITE FILE = " << countBytes_meshesID_write << '\n';
  std::cout << "BYTES meshesID model HOPE TO WRITE = " <<  bytes_sizeMeshesID * meshes_ID.size() << '\n';

   if(fileManager_POSIX::pwriting_handlingError(countBytes_meshesID_write, bytes_sizeMeshesID, temp_modelFileDir) == 0)
  {
   close(binModel);
   filesystem_manager::delete_file_sentence(headerModel.modelName, temp_modelFileDir);
   return 0;
  }
  
  close(binModel);

  if(access(outDir_re.c_str(), F_OK) == 0)
  {
   std::filesystem::remove(outDir_re); ///REMOVE THE EXISTING FILE THAT WILL BE OVERWRITE
  }
   std::filesystem::rename(temp_modelFileDir, outDir_re); ////RENAME FILE WITH THE NAME THAT OVERWRITES THE FILE  

  log_System::modelCooker_logger.success("writing binary success | modelBin = " + outDir_re);
  return 1;
 }

inline uint32_t pack_binModel_posix_newFile(const model_LeoHeader& headerModel, const std::vector<uint64_t>& meshes_ID, const std::string& outDir,const std::string& dirOrigin)
{ ///REVIEW AND CONTINUE HERE==========
 std::string newDir {outDir}; 
 customFiles::rename_fileExisting(newDir);
 
 return pack_binModel_posix_overwrite(headerModel, meshes_ID, newDir, dirOrigin);
}
 #elif defined(_WIN32)

 #endif

 /////////////CONTINUE HERE TO IMPLEMENT THE SAVE OF THE BINARY MODEL
inline uint32_t writeFile_ModelBinary(const model_LeoHeader& headerModel, const std::vector<uint64_t>& meshes_ID, const std::string& dirOrigin, file_OP::writeFlags fileT)
{

  uint32_t w_test {};
  std::string nameModel {headerModel.modelName};
  nameModel += data_modelCore::vefNum;
  std::string dir_create {dirOrigin}; 
  //convert_str::remplace_char_in_str(dir_create, "/", dir_create.size(), 0, 1);
  dir_create = dir_create + nameModel;
  
  log_System::modelCooker_logger.info("creating model binary | modelBin = " + nameModel); 

  #if defined(__unix__) || defined(__unix) && defined(__MACH__)
  
   using typeWriteFlags = std::underlying_type_t<file_OP::writeFlags>;
   if(!static_cast<typeWriteFlags>(fileT & file_OP::writeFlags::OVERWRITE))
  {
    log_System::modelCooker_logger.info("overwriting modelBin | modelBin = " + nameModel); 
    w_test = pack_binModel_posix_overwrite(headerModel, meshes_ID, dir_create, dirOrigin);
    return w_test;
  }

   if(!static_cast<typeWriteFlags>(fileT & file_OP::writeFlags::NEW_FILE))
  {
    log_System::modelCooker_logger.info("new file modelBin | modelBin = " + nameModel); 
    w_test = pack_binModel_posix_newFile(headerModel, meshes_ID, dir_create, dirOrigin);
    return w_test;
  }

 #elif defined(_WIN32)

 #endif 

 return w_test;
}



#endif
