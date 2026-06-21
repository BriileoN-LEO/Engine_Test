//
// Created by brii on 19/06/26.
//

#ifndef TEXTURESPACK_BINFORMAT_H
#define TEXTURESPACK_BINFORMAT_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

struct texturesPack_LeoHeader
{
    char verifiedNumber[4]{};  ///LEOT
    uint32_t version{};

    uint32_t texturesID{};
    uint32_t textures_nameLenght{};
    ///--name texture pack in string

    uint32_t texturesCount{};
};

struct texturePack_data_register
{
    uint64_t textureID{};
    uint64_t texture_nameLenght{};
    ///--name texture - STRING

    uint64_t offset_texBin{};
    uint64_t size_texBin{};

};

#pragma pack(pop)



#endif //TEXTURESPACK_BINFORMAT_H