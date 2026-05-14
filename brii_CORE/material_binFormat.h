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
  //nameMaterial  ---STRING

  uint32_t albedo_PathLength{};
  uint32_t roughness_PathLength{};
  uint32_t metalness_PathLength{};
  uint32_t height_PathLength{};
  uint32_t ambientOclussion_PathLength{};
  uint32_t cavity_PathLength{};
  uint32_t opacity_PathLength{};
  uint32_t emissive_PathLength{};
};

#pragma pack(pop)

#endif //MATERIAL_BINFORMAT_h