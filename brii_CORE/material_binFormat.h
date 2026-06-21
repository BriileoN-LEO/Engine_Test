#ifndef MATERIAL_BINFORMAT_h
#define MATERIAL_BINFORMAT_h
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

struct material_LeoHeader
{
  char verifiedNumber[4]{};  ///LEOA

  uint64_t materialID{};
  uint64_t material_nameLenght{};
  //nameMaterial  ---STRING

  float albedo_base[4]{};
  float normals_base[3]{};
  float RMA_base[3]{};
  float height_base{};
  float emission[3]{};

  uint64_t albedo_hash{};
  uint64_t normals_hash{};
  uint64_t RMA_hash{}; ///[ROUGHNESS] [METALLIC] [AMBIENT OCLUSSION]
  uint64_t height_hash{};
  uint64_t emission_hash{};
};

#pragma pack(pop)

#endif //MATERIAL_BINFORMAT_h