#ifndef MODEL_BINFORMAT_H
#define MODEL_BINFORMAT_H
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
  constexpr size_t get_maxSize_nameModelArray()
 { 
  return MAX_SIZE_STR_BIN_MODEL;
 }


}


#endif
