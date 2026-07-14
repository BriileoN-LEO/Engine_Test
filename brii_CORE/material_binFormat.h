#ifndef MATERIAL_BINFORMAT_h
#define MATERIAL_BINFORMAT_h
#include "systemManager/platform_Manager.h"
#include "files_CoreManager/files_Core.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
//#include <sstream>
//#include <cmath>
#include <fstream>

#pragma pack(push, 1)

constexpr size_t MAX_SIZE_STR_BIN_MATERIAL = 256;

struct material_LeoHeader
{
  char nameMaterial_Header[MAX_SIZE_STR_BIN_MATERIAL] {}; //256 bytes
  uint64_t material_bin_ID{}; //8 bytes
  char verifiedNumber[4]{};  ///LEOA || //4 bytes
  uint32_t version{}; //4 bytes
  uint32_t materialCount{}; //4 bytes
 };

//// IN ORDER TO OPTIMIZED
struct matPack_data_register
{
 // uint64_t materialID{};
  
  char nameMaterial[MAX_SIZE_STR_BIN_MATERIAL]{};

  float albedo_base[4]{255.0f, 255.0f, 255.0f, 255.0f};
  float normals_base[4]{};
  float RMA_base[4]{255.0f, 255.0f, 255.0f, 0.0f};

  uint64_t materialID{};

  uint64_t albedo_hash{};
  uint64_t normals_hash{};
  uint64_t RMA_hash{}; ///[ROUGHNESS] [METALLIC] [AMBIENT OCLUSSION]
  uint64_t height_hash{};
  uint64_t emission_hash{};

  float height_base{};
  float emission{};
};

#pragma pack(pop)

namespace standardMat_base
{
  const float albedo_SM[4]{255.0f, 255.0f, 255.0f, 255.0f};
  const float normal_SM[4]{};
  const float RMA_SM[4]{255.0f, 255.0f, 255.0f, 0.0f};
  const float height_base{};
  const float emission{};
}

namespace data_MatCore
{ 
  using matPack_ptr = matPack_data_register*;
  const std::string vefNum {"leoa"};
  const std::string temp_verfCreationFileDir {"tempBinMat"};
}



//USE OF POSIX TO WRITE FILES
#if defined(__unix__) || defined(__unix) && defined(__MACH__)

inline uint32_t pack_binMaterial_posix_overwrite(const material_LeoHeader& mat_header, const std::vector<matPack_data_register>& mat_pack,  const std::string& outDir, const std::string dir_origin)
{

//  std::vector<funcPtr::func_str_constStrAmp> func_Delete{};
//  func_Delete.resize(2);
//  func_Delete[0] = &filesystem_manager::delete_file_sentence;
 // func_Delete[1] = &funcPtr::empty_func::str_constStrAmp; 
  ////WRITE IN EXTERNAL FILE
  std::string temp_creationFileDir {mat_header.nameMaterial_Header + data_MatCore::temp_verfCreationFileDir};
  temp_creationFileDir = dir_origin + "/" + temp_creationFileDir + "." + data_MatCore::vefNum;

  int binMat {open(temp_creationFileDir.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)};
  
  if(binMat < 0)
  {
   std::cerr << "ERROR::OPEN FILE::DIRECTION ---> " << temp_creationFileDir << "\n";
   return 0;
  }
  
  off_t offset_bin{};
  size_t bytes_sizeHeader{sizeof(mat_header)};
  const void* buffer_header {reinterpret_cast<const void*>(&mat_header)};

  std::cout << "WRITING IN FILE ---> " << temp_creationFileDir << "\n";
  ssize_t countBytes_HeaderWrite {pwrite(binMat, buffer_header, bytes_sizeHeader, offset_bin)};

  ////TEST HERE IF THE COMPARATION OF THE SIZE IN BITS IS BAD WHEN I CAST SIZE_T(long - signed value) to uint64_t(unsigned int64) 
  ///REMEMBER, not cast negative values to unsigned int --- ERRORS WILL SHOW
  
  if(fileManager_POSIX::pwriting_handlingError(countBytes_HeaderWrite, bytes_sizeHeader, temp_creationFileDir) == 0)
  {
   close(binMat);
   filesystem_manager::delete_file_sentence(mat_header.nameMaterial_Header, temp_creationFileDir);
   return 0;
  }
 
  offset_bin = countBytes_HeaderWrite;
  size_t bytes_sizeMatPack {sizeof(matPack_data_register)};

  for(int i = 0; i < static_cast<int>(mat_pack.size()); i++)
  {
   const void* buffer_matPack {reinterpret_cast<const void*>(&mat_pack[i])};
   ssize_t countBytes_matPackWrite {pwrite(binMat, buffer_matPack, bytes_sizeMatPack, offset_bin)}; 
   

  if(fileManager_POSIX::pwriting_handlingError(countBytes_matPackWrite, bytes_sizeMatPack, temp_creationFileDir) == 0)
  {
   close(binMat);
   filesystem_manager::delete_file_sentence(mat_header.nameMaterial_Header, temp_creationFileDir);
   return 0;
  }
    
   offset_bin += bytes_sizeMatPack;  
  }

 close(binMat);

if(access(outDir.c_str(), F_OK) == 0)
{
 std::filesystem::remove(outDir); ///REMOVE THE EXISTING FILE THAT WILL BE OVERWRITE
}
   std::filesystem::rename(temp_creationFileDir, outDir); ////RENAME FILE WITH THE NAME THAT OVERWRITES THE FILE 
  /*
  if(access(outDir.c_str(), F_OK) != 0)
 {
  ////OPEN THE DIRECTION OF THE FILE TO OVERWRITE
  int open_newDir{ open(outDir.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644) }; ///CREATE IF NOT EXISTS | OPEN 
  
  if(open_newDir < 0)
  {
   std::cerr << "ERROR::OPEN FILE::DIRECTION ---> " << outDir << "\n";
   std::filesystem::remove(temp_creationFileDir);
   return 0;
  }
 }

  bool copy_matBin {std::filesystem::copy_file(temp_creationFileDir, outDir, std::filesystem::copy_options::overwrite_existing)};
  std::filesystem::remove(temp_creationFileDir);

  if(!copy_matBin)
  {
    std::cerr << "ERROR::OVERWRITING DATA::NOT SUCCESSFULLY CREATE FILE:: DIRECTION ---> " << outDir << "\n";
    std::filesystem::remove(outDir);
  }
*/

  //CONTINUE HERE============

  return 1;
}

inline uint32_t pack_binMaterial_posix_newFile(const material_LeoHeader& mat_header, const std::vector<matPack_data_register>& mat_pack, const std::string outDir, const std::string& dir_origin)
{

 std::string newDir {outDir}; 

  if(access(outDir.c_str(), F_OK) == 0)  ////THIS FUNCTION REVIEW IF FILE EXISTS | 0 = file exists
 { 
  uint32_t access_out{};

  uint32_t val_count{};
  while(access_out == 0) ////CHANGE name adding number to test if file exists 
  {
    newDir = outDir + "/" + (mat_header.nameMaterial_Header + std::to_string(val_count)) + "." +  data_MatCore::vefNum;
    val_count = access(newDir.c_str(), F_OK);
    val_count++;
  } 
 }
 
 return pack_binMaterial_posix_overwrite(mat_header, mat_pack, newDir, dir_origin);
}

#elif defined(_WIN32)

inline uint32_t pack_binMaterial_win(const material_LeoHeader& mat_header, const std::vector<matPack_data_register>& mat_pack,  const std::string& outDir)
{
  ////write this function in windows
}

#endif

///USE when writing files linearity needed
inline uint32_t pack_binMaterial(const material_LeoHeader& mat_header, const std::vector<matPack_data_register>& mat_pack,  const std::string& outDir)
{
  
  std::ofstream binMat(outDir, std::ios::binary);
 
  if(!binMat.is_open() == true)
  {
   std::cerr << "ERROR::OPEN FILE::DIRECTION ---> " << outDir << "\n";
   return 0;
  }

  //std::string nameMatBin{mat_header.nameMaterial_Header};
  //nameMatBin.append(mat_header.nameMaterial_Header);

  std::cout << "WRITTING BINARY FILE:: MATERIAL BIN ---> " << mat_header.nameMaterial_Header << '\n';
 
  binMat.write(reinterpret_cast<const char*>(&mat_header), sizeof(material_LeoHeader)); 
  
  for(auto& mat : mat_pack)
  {  
    binMat.write(reinterpret_cast<const char*>(&mat), sizeof(matPack_data_register));
    std::cout << "   WRITTING MATERIAL:: MATERIAL ---> " << mat.nameMaterial; 
  }

  binMat.close();
  std::cout << "BINARY MATERIAL SUCCESSFULLY CREATED:: MATERIAL BIN ---> " << mat_header.nameMaterial_Header << "\n";
  return 1;
}

inline uint32_t writeFile_binMaterial(const material_LeoHeader& mat_header, const std::vector<matPack_data_register>& mat_pack, const std::string& outDir, file_OP::writeFlags& fileT)
{
  std::array<std::string, 2> sen_t{};
  sen_t[0] = "==UNSUCCESSFULL WRITING==\n";
  sen_t[1] = "==COMPLETE WRITING==\n"; 

  uint32_t w_test {};

  std::string nameMat {mat_header.nameMaterial_Header};
  nameMat += "." + data_MatCore::vefNum;
  std::string dir_create = outDir + "/" + nameMat; ///origin/name.leoa --- DIRECTION LEO MATERIAL
  
  std::cout << "CREATING MATERIAL BINARY::MATBIN ---> " << mat_header.nameMaterial_Header << "\n";

  #if defined(__unix__) || defined(__unix) && defined(__MACH__)

  using typeWriteFlags = std::underlying_type_t<file_OP::writeFlags>;
   if(static_cast<typeWriteFlags>(fileT & file_OP::writeFlags::OVERWRITE))
  {
    w_test = pack_binMaterial_posix_overwrite(mat_header, mat_pack, dir_create, outDir);
    std::cout << sen_t[w_test];
    return w_test;
  }

   if(static_cast<typeWriteFlags>(fileT & file_OP::writeFlags::NEW_FILE))
  {
    w_test = pack_binMaterial_posix_newFile(mat_header, mat_pack, dir_create, outDir);
    std::cout << sen_t[w_test];
    return w_test;
  }
 
 #elif(_WIN32)


 #endif


 return w_test;
}




#endif //MATERIAL_BINFORMAT_h

