//
// Created by brii on 19/06/26.
//

#ifndef TEXTURESPACK_BINFORMAT_H
#define TEXTURESPACK_BINFORMAT_H
#include <ktx.h> ///NUEVA LIBRERIA PARA CARGAR IMAGENES
#include <KHR/khr_df.h>
#include "cmake-build-debug/_deps/ktx_software-src/lib/vkformat_enum.h" /////CHANGE THIS FOR A WAY TO REMPLACE THIS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <memory>

#pragma pack(push, 1)

constexpr size_t MAX_SIZE_STR_BIN_TEXTURE { 256 };

struct texPack_LeoHeader
{
    char verifiedNumber[4]{};  ///LEOT
    uint32_t version{};

  //  uint32_t texturesID{};
   // uint32_t textures_nameLenght{};
    ///--name texture pack in strin

    uint32_t texturesCount{};
};

struct texPack_data_register
{
    char textureName[MAX_SIZE_STR_BIN_TEXTURE]{};
    uint64_t textureID{};
   // uint64_t texture_nameLenght{};
    ///--name texture - STRING
    uint64_t offset_texBin{};
    uint64_t size_texBin{};
    uint32_t width{};
    uint32_t height{};
    uint32_t nrChannels{};
};

#pragma pack(pop)

struct ktx2_deleter
{
 void operator()(ktxTexture2* tex) const
 {
    if (tex)
    {
      ktxTexture2_Destroy(tex);
    }
 }
};

using texKTX2_ptr = std::unique_ptr<ktxTexture2, ktx2_deleter>;
using ktxBytes_ptr = std::unique_ptr<uint8_t>;

struct loadTexture_Memory
{
  std::string name{};
  texKTX2_ptr texData{ nullptr };
  uint8_t* ktx_Bytes{ nullptr };  ///THIS ONLY USE MALLOC --- use free() to liberate memory
  texPack_data_register dataTex {};

  loadTexture_Memory(){};
  loadTexture_Memory(loadTexture_Memory&& texM) noexcept
  {
      //name = texM.name;
      if (texData != nullptr)
      {
          texData.reset();
          texData = nullptr;
      }
      texData = std::move(texM.texData);
      texM.texData = nullptr;

      if (texData != nullptr)
      {
          free(ktx_Bytes);
          ktx_Bytes = nullptr;
      }
      ktx_Bytes = std::move(texM.ktx_Bytes);
      texM.ktx_Bytes = nullptr;

      dataTex = texM.dataTex;
  };
  loadTexture_Memory(loadTexture_Memory& texM)
  {
     // name = texM.name;
      if (texData != nullptr)
      {
        texData.reset();
        texData = nullptr;
      }
      texData = std::move(texM.texData);
      texM.texData = nullptr;

      if (texData != nullptr)
      {
          free(ktx_Bytes);
          ktx_Bytes = nullptr;
      }
      ktx_Bytes = std::move(texM.ktx_Bytes);
      texM.ktx_Bytes = nullptr;

      dataTex = texM.dataTex;
  };

  ~loadTexture_Memory()
  {
   if (texData != nullptr)
   {
      free(ktx_Bytes);
      texData = nullptr;
      delete[] ktx_Bytes;
   }

   if (texData != nullptr)
   {
      texData.reset();
      texData = nullptr;
   }
  }

  void clearDataMem()
  {

      if (texData != nullptr)
      {
          free(ktx_Bytes);
          ktx_Bytes = nullptr;
      }

      if (texData != nullptr)
      {
          texData.reset();
          texData = nullptr;
      }
  }

};

inline uint32_t pack_binTextures_KTX2(std::vector<loadTexture_Memory>& texKTX2, const std::string& outDir, uint32_t version, std::vector<int>& posTex)
{
    texPack_LeoHeader header{};
    header.verifiedNumber[0] = 'L';
    header.verifiedNumber[1] = 'E';
    header.verifiedNumber[2] = 'O';
    header.verifiedNumber[3] = 'T';
    header.version = version;
    ///ADD TE OFFSET  ////////CONTINUE HERE

    for (int i = 0; i < static_cast<int>(texKTX2.size()); i++)
    {
        ktxTexture2* ktxTex {texKTX2[i].texData.get()};

        ktx_uint8_t* albedo_bytes{ nullptr };
        ktx_size_t albedo_size{};
        KTX_error_code result { ktxTexture_WriteToMemory(ktxTexture(ktxTex), &albedo_bytes, &albedo_size) };

        if (result != KTX_SUCCESS)
        {
          std::cerr << "ERROR::WRITING KTX2 TO MEMORY::NOT IN BIN:: TEXTURE--->" << texKTX2[i].name << "\n";
          ktxTex = nullptr;
          albedo_bytes = nullptr;
          texKTX2[i].texData.reset();
          continue;
        }

        std::cout << "WRITTING KTX2 TO MEMORY::WRITING TEXTURE BIN FILE:: TEXTURE --->" << texKTX2[i].name << "\n";

        texKTX2[i].ktx_Bytes = albedo_bytes;
        albedo_bytes = nullptr;

        texKTX2[i].dataTex.size_texBin = albedo_size;

        posTex.emplace_back(i);
    }

    uint64_t offset_state { sizeof(texPack_LeoHeader) + (posTex.size() * sizeof(texPack_data_register)) };

    for (auto& num : posTex)
    {
        texKTX2[num].dataTex.offset_texBin = offset_state;  /// BEGGINS WITH THE DATA IN BINARY FILE
        offset_state += texKTX2[num].dataTex.size_texBin;  ///SUM TO CALCULATE THE FINAL OF THE KTX2 DATA OF THE TEXTURE
    }

    header.texturesCount = posTex.size();


    //////////////WRITING BINARY FILE/////////////////
    std::ofstream binTex (outDir, std::ios::binary);
    if (!binTex.is_open())
    {
      std::cerr << "ERROR::OPENING FILE --->" << outDir;
      return 0;
    }

    ////FASE 1 --- WRITE HEADER
    binTex.write(reinterpret_cast<const char*>(&header), sizeof(texPack_LeoHeader));

    ////FASE 2 --- WRITE THE DESCRIPTION OF ALL TEXTURES
    for (auto& num : posTex)
    {
     binTex.write(reinterpret_cast<const char*>(&texKTX2[num].dataTex), sizeof(texPack_data_register));
    }

    ////FASE 3 --- WRITE ALL BINARIES OF KTX2 TEXTURES
    for (auto& num : posTex)
    {
     std::cout << "WRITTING BYTES::TEXTURE BINARY::TEXTURE --->" << texKTX2[num].name << "\n";
     binTex.write(reinterpret_cast<const char*>(texKTX2[num].ktx_Bytes),  texKTX2[num].dataTex.size_texBin);

     texKTX2[num].clearDataMem();
    }

    ////REVIEW THIS FUNCTION

  binTex.close();
  std::cout << "BINARY SUCCESSFULLY CREATED:: FILE ---> " << outDir << "\n";
  return 1;
}

#endif //TEXTURESPACK_BINFORMAT_H