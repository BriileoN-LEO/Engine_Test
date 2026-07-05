#ifndef MATERIAL_BINFORMAT_h
#define MATERIAL_BINFORMAT_h
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
  char verifiedNumber[4]{};  ///LEOA
  uint32_t version{};
  uint64_t material_bin_ID{};
  char nameMaterial_Header[MAX_SIZE_STR_BIN_MATERIAL] {};
  uint32_t materialCount{};
 };

struct matPack_data_register
{
  uint64_t materialID{};
  char nameMaterial[MAX_SIZE_STR_BIN_MATERIAL]{};

  float albedo_base[4]{};
  float normals_base[3]{};
  float RMA_base[3]{};
  float height_base{};
  float emission{};

  uint64_t albedo_hash{};
  uint64_t normals_hash{};
  uint64_t RMA_hash{}; ///[ROUGHNESS] [METALLIC] [AMBIENT OCLUSSION]
  uint64_t height_hash{};
  uint64_t emission_hash{};
};

#pragma pack(pop)

namespace standardMat_base
{
  const float albedo_SM[4]{255.0f, 255.0f, 255.0f, 255.0f};
  const float normal_SM[3]{0.0f, 0.0f, 0.0f};
  const float RMA_SM[3]{255.0f, 255.0f, 255.0f};
  const float height_base{0.0f};
  const float emission{0.0f};
}

namespace data_MatCore
{ 
  using matPack_ptr = matPack_data_register*;
}

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


#endif //MATERIAL_BINFORMAT_h

