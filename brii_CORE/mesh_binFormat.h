#ifndef MESH_BINFORMAT_H
#define MESH_BINFORMAT_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

constexpr size_t MAX_SIZE_STR_BIN_MESH = 512;

struct mesh_LeoHeader
{
 char verifiedNumber[4]{};  ///LEOM [HEADER]
 uint32_t version{};

 uint64_t meshID{};
 char nameMesh[MAX_SIZE_STR_BIN_MESH] {};

 uint64_t vertexCount{};
 uint64_t indexCount{};
 uint64_t material_ID{};

 float mesh_transformation[16]{};
};

#pragma pack(pop)

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

////ALL INDICES IN THE BINARY [INDICES]

#endif //MESH_BINFORMAT_H
