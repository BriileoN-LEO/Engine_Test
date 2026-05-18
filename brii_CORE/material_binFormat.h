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

  uint32_t materialID{};
  uint32_t material_nameLenght{};

  float albedo_base[4]{};
  float normals_base[3]{};
  float RMA_base[3]{};
  float height_base{};
  float emission[3]{};
  //nameMaterial  ---STRING

  uint32_t albedo_PathLength{};
  uint32_t normals_PathLength{};
  uint32_t RMA_PathLength{}; ///[ROUGHNESS] [METALLIC] [AMBIENT OCLUSSION]
  uint32_t height_PathLength{};
  uint32_t emission_PathLength{};
};

#pragma pack(pop)

#endif //MATERIAL_BINFORMAT_h