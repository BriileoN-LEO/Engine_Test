#ifndef MODEL_BINFORMAT_H
#define MODEL_BINFORMAT_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

struct model_LeoHeader
{
    char verifiedNumber[4]{};  ///LEOR [HEADER]
    uint32_t version{};

    uint32_t modelID{};
    uint32_t model_nameLenght{};
    //modelName  ---STRING
    uint32_t directory_pathLenght{}; ///ORIGINAL DIRECTORY OF THE MODEL .fbx, .obj, etc...
    //directory string

    uint32_t meshesCount{};

};

#pragma pack(pop)

struct meshAsset_register
{
    uint32_t meshID{};
    uint32_t leomesh_pathLength{};  ///path for the .leomesh of the mesh
    ///.leomesh path string
};


#endif