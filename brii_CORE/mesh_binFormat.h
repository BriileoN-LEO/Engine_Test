#ifndef MESH_BINFORMAT_H
#define MESH_BINFORMAT_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

constexpr size_t MAX_SIZE_STR_BIN_MESH = 512;

////EACH MODEL WILL HAVE ONE BINARY WITH ALL MESHES
struct mesh_LeoHeader
{
 uint64_t meshesCount{};
 uint32_t version{};
 char verifiedNumber[4]{};  ///LEOM [HEADER]
};

struct meshPack_Register
{
 uint64_t meshID{};

 uint64_t vertexCount{};
 uint64_t indexCount{};
 uint64_t material_ID{};

 uint64_t offset_meshBin{};
 uint64_t size_meshBin{};
 
 float mesh_transformation[16]{};

 char nameMesh[MAX_SIZE_STR_BIN_MESH]{};  
};

struct vertex_D //[VERTICES]
{
 float position[3]{};
 float normal[3]{};
 float uv[2]{};

 vertex_D(){};
 vertex_D(float position[3], float normal[3], float uv[2])
 {
    this->position[0] = position[0];
    this->position[1] = position[1];
    this->position[2] = position[2];

    this->normal[0] = normal[0];
    this->normal[1] = normal[1];
    this->normal[2] = normal[2];

    this->uv[0] = uv[0];
    this->uv[1] = uv[1];
 }
};


#pragma pack(pop)


////ALL INDICES IN THE BINARY [INDICES]

#endif //MESH_BINFORMAT_H
