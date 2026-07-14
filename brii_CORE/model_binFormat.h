#ifndef MODEL_BINFORMAT_H
#define MODEL_BINFORMAT_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

constexpr size_t MAX_SIZE_STR_BIN_MODEL = 512;

struct model_LeoHeader
{
    char verifiedNumber[4]{};  ///LEOR [HEADER]
    uint32_t version{};

    uint64_t modelID{};
    char modelName[MAX_SIZE_STR_BIN_MODEL]{};
    
    uint64_t meshesCount{};
};
//struct meshAsset_register 
//{ 
 //   uint64_t meshID{}; ////ALL THE ID MESHES IN SEQUENCE
//};
#pragma pack(pop)


#endif
