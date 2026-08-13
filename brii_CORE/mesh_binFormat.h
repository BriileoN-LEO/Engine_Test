#ifndef MESH_BINFORMAT_H
#define MESH_BINFORMAT_H
#include "systemManager/platform_Manager.h"
#include "files_CoreManager/files_Core.h"
#include "dataManager/convertion_DataManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#pragma pack(push, 1)

constexpr size_t MAX_SIZE_STR_BIN_MESH = 512;
constexpr size_t MAX_SIZE_STR_HEADER = 256;
constexpr char vN_LEOM[4] {'L', 'E', 'O', 'M'};

////EACH MODEL WILL HAVE ONE BINARY WITH ALL MESHES
struct mesh_LeoHeader
{
 char nameMeshBin[MAX_SIZE_STR_HEADER]{}; ////256 bytes
 float generalMesh_transformation[16]{};
 size_t meshesCount{}; // 8 bytes
 char verifiedNumber[4]{vN_LEOM[0], vN_LEOM[1], vN_LEOM[2], vN_LEOM[3]};  ///LEOM [HEADER] || 4 bytes
 uint32_t version{}; //4 bytes
	
 auto& get_nameMeshBin()
 {
  return nameMeshBin;
 }
 
 auto& get_generalMatTrans()
 {
   return generalMesh_transformation;
 }

 const auto& get_meshesCount()
 {
  return meshesCount;
 }
 
 const auto& get_vefNum()
 {
  return verifiedNumber;
 }

 const auto& get_version()
 {
  return version;
 }
 
 void print_info()
 {
  std::cout << "======================\n";
  std::cout << "[INFO MESH BINARY]\n";
  std::cout << "name = " << nameMeshBin << '\n';
  std::cout << "meshes count = " << meshesCount << '\n';
  std::cout << "vef number = " << verifiedNumber[0] <<  verifiedNumber[1] <<  verifiedNumber[2] <<  verifiedNumber[3] << '\n';
  std::cout << "version = " << version << '\n';
  std::cout << "======================\n";
}

} ;

struct meshPack_Register
{
 uint64_t meshID{};

 uint64_t vertexCount{};
 uint64_t indexCount{};
 uint64_t material_ID{};

 size_t offset_meshBin{}; ///START OF CONTENT VERTEX_D
 size_t offset_startIndices{}; ///START OF CONTENT INDICES
 size_t size_meshBin{};
 
 size_t sizeNameMesh{};//NEW
 
 float mesh_transformation[16]{};

 char nameMesh[MAX_SIZE_STR_BIN_MESH]{};

 auto& get_nameMesh()
 {
  return nameMesh;
 }
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

  void print_info()
 {
  std::cout << "=========================\n";
  std::cout << "[VERTEX DATA]\n";
  std::cout << "position = {" << position[0] << ", " << position[1] << ", " << position[2] << "}\n";
  std::cout << "normal = {" << normal[0] << ", " << normal[1] << ", " << normal[2] << "}\n";
  std::cout << "uv = {" << uv[0] << ", " << uv[1] << "}\n";
  std::cout << "=========================\n"; 
 }

};
////ALL INDICES IN THE BINARY [INDICES]
///extract the indices viewing the sizeof(unsigned int)

/////INTRODUCE VERTICES AND INDICES IN ONE PLACE

#pragma pack(pop)

namespace data_meshCore
{
  const std::string vefNum {".leom"};
  const std::string temp_verfCreationFileDir {"tempBinMesh"};
 
 constexpr size_t get_maxSize_meshHeaderArray()
 {
  return MAX_SIZE_STR_HEADER; 
 }

 constexpr size_t get_maxSize_nameMeshArray()
 {
  return MAX_SIZE_STR_BIN_MESH;
 }

 struct mesh_info
 {
  std::vector<vertex_D> vertex{};
  std::vector<unsigned int> indices{}; 
 };
 
  struct import_meshBinary
 {
  meshPack_Register mesh_data{};
  std::vector<vertex_D> vertex{};
  std::vector<unsigned int> indices{};
 };
}

struct data_meshF
{
 std::vector<vertex_D>* dataVertex{ nullptr };
 std::vector<unsigned int>* dataIndices{ nullptr };
 meshPack_Register* mesh_info{ nullptr }; //////CHANGE NAME

 data_meshF(){};
 data_meshF(std::vector<vertex_D>& dataVertex, std::vector<unsigned int>&dataIndices, meshPack_Register& mesh_info) : dataVertex(&dataVertex), dataIndices(&dataIndices), mesh_info(&mesh_info){};
 ~data_meshF()
 {
  destroy();
 }

 data_meshF operator<<(data_meshF& move_dMF) ///OPERATOR FOR MOVE THE DATA
 {
   dataVertex = std::move(move_dMF.dataVertex);
   dataIndices = std::move(move_dMF.dataIndices);
   mesh_info = std::move(move_dMF.mesh_info);
   move_dMF.clear();

  return *this;
 }
 
 void insert_NewData_ptr(std::vector<vertex_D>& dataVertex, std::vector<unsigned int>&dataIndices, meshPack_Register& mesh_info)
 {
  this->dataVertex = nullptr;
  this->dataIndices = nullptr;
  this->mesh_info = nullptr;

  this->dataVertex = &dataVertex;
  this->dataIndices = &dataIndices;
  this->mesh_info = &mesh_info;
 }

 void clear()
 {
  dataVertex = nullptr;
  dataIndices = nullptr;
  mesh_info = nullptr;
 }
 void destroy()
 {
  dataVertex = nullptr;
  dataIndices = nullptr;
  mesh_info = nullptr;

  delete dataVertex;
  delete dataIndices;
  delete mesh_info;
 }

};

#if defined(__unix__) || defined(__unix) && defined(__MACH__)

inline uint32_t pack_binMesh_posix_overwrite(const mesh_LeoHeader& headerMesh, std::vector<data_meshF>& data_meshes, const std::string& outDir, const std::string&dir_origin)
{ 

  std::string temp_creationFileDir {headerMesh.nameMeshBin + data_meshCore::temp_verfCreationFileDir};
  temp_creationFileDir = dir_origin + temp_creationFileDir + data_meshCore::vefNum;
 
  convert_str::quit_repetitive_char(temp_creationFileDir, '/'); 

  std::string outDir_re{outDir};
  convert_str::quit_repetitive_char(outDir_re, '/');

  int binMesh {open(temp_creationFileDir.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)};

  if(binMesh < 0)
  {
   log_System::fileLogger.error("erro open file | file = " + temp_creationFileDir);
   return 0;
  }

    ///DETERMINADED SIZE
  size_t bytes_sizeHeader { sizeof(mesh_LeoHeader) };
  size_t bytes_sizeMeshPR { sizeof(meshPack_Register) };
//  size_t bytes_sizeVertex { sizeof(vertex_D)};
  size_t bytes_sizeMeshVertex { sizeof(vertex_D) };
  size_t bytes_sizeMeshIndices { sizeof(unsigned int) };

  //uint64_t bytes_sizeHeader64 {static_cast<uint64_t>(sizeof(mesh_LeoHeader))};
  //uint64_t bytes_sizemeshPR {static_cast<uint64_t>(sizeof(meshPack_Register))};
  //uint64_t bytes_sizeMeshVertex {static_cast<uint64_t>(sizeof(vertex_D))};
  //uint64_t bytes_sizeMeshIndices {static_cast<uint64_t>(sizeof(unsigned int))};
	 
  size_t meshesPR_size {data_meshes.size() * bytes_sizeMeshPR};
  size_t vertex_D_offsets {bytes_sizeHeader + meshesPR_size}; //////////OFFSETS for binary information

  size_t bytesRest_write {vertex_D_offsets}; ////THE TOTAL SIZE TO WRITE IN BINARY
  for(auto& mesh_D : data_meshes)
  {
   bytesRest_write += mesh_D.dataVertex->size() * bytes_sizeMeshVertex;
   bytesRest_write += mesh_D.dataIndices->size() * bytes_sizeMeshIndices;	
  }

  /////////SEE IF THERES IS SPACE TO ALLOCATE THE BINARY AND WRITE
  int reserve_space = posix_fallocate(binMesh, 0, bytesRest_write);
  if(reserve_space != 0)
  {
   log_System::fileLogger.error("not enought space to write modelBin | modelBin = " + temp_creationFileDir);
   close(binMesh);
   filesystem_manager::delete_file_sentence(headerMesh.nameMeshBin, temp_creationFileDir);
   return 0;
  }

  off_t offset_bin{};//STARTING OFFSET, CURRENT WRITING OFFSET
  const void* buffer_header {reinterpret_cast<const void*>(&headerMesh)};

  //FASE 1 === ADD MESH BIN HEADER
  std::cout << "WRITING IN FILE ---> " << temp_creationFileDir << "\n";
  ssize_t countBytes_HeaderWrite {pwrite(binMesh, buffer_header, bytes_sizeHeader, offset_bin)}; ///CORREGIR bytesRest_write

  //////////////////CONTINUE HEREEE////////

  if(fileManager_POSIX::pwriting_handlingError(countBytes_HeaderWrite, bytes_sizeHeader, temp_creationFileDir) == 0)
  {
   close(binMesh);
   filesystem_manager::delete_file_sentence(headerMesh.nameMeshBin, temp_creationFileDir);
   return 0;
  }

  offset_bin = countBytes_HeaderWrite;
//  bytesRest_write -= bytes_sizeHeader;

 ///FASE 2 === ADD MESHES PACK INFO REGISTER 
 for(auto& mesh_D : data_meshes)
 {
   size_t size_dataVertexD {bytes_sizeMeshVertex * mesh_D.dataVertex->size()};
   size_t size_indices { bytes_sizeMeshIndices * mesh_D.dataIndices->size()};
   size_t meshD_size { size_dataVertexD + size_indices };

   mesh_D.mesh_info->size_meshBin = meshD_size;
   mesh_D.mesh_info->offset_meshBin = vertex_D_offsets;
   mesh_D.mesh_info->offset_startIndices = vertex_D_offsets + size_dataVertexD;
  
   const void* buffer_MeshR{reinterpret_cast<const void*>(mesh_D.mesh_info)};
   ssize_t countBytes_registerMesh {pwrite(binMesh, buffer_MeshR, bytes_sizeMeshPR, offset_bin)};
 
  if(fileManager_POSIX::pwriting_handlingError(countBytes_registerMesh, bytes_sizeMeshPR, temp_creationFileDir) == 0)
  {
   close(binMesh);
   filesystem_manager::delete_file_sentence(headerMesh.nameMeshBin, temp_creationFileDir);
   return 0;
  }

   offset_bin += countBytes_registerMesh;
 //  bytesRest_write -= countBytes_registerMesh;
   vertex_D_offsets += meshD_size; // sum the sizeof bites
 } 
  
  ///FASE 3 === ADD MESHES DATA (vertex, indices)
 for(auto& mesh_D : data_meshes)
 { 
 
   size_t size_vertexArray_write {mesh_D.dataVertex->size() * bytes_sizeMeshVertex};

    ///FASE 3.1 === VERTEX	 
    const void* buffer_vertex{reinterpret_cast<const void*>(mesh_D.dataVertex->data())};
    ssize_t countBytes_rVertex{pwrite(binMesh, buffer_vertex, size_vertexArray_write, offset_bin)};

    if(fileManager_POSIX::pwriting_handlingError(countBytes_rVertex, size_vertexArray_write, temp_creationFileDir) == 0)
     {
      close(binMesh);
      filesystem_manager::delete_file_sentence(headerMesh.nameMeshBin, temp_creationFileDir);
      return 0;
     }

   offset_bin += countBytes_rVertex;
   //bytesRest_write -= countBytes_rVertex; 

   size_t size_indicesArray_write {mesh_D.dataIndices->size() * bytes_sizeMeshIndices};
    ///FASE 3.2 === INDICES
    const void* buffer_indices{reinterpret_cast<const void*>(mesh_D.dataIndices->data())};
    ssize_t countBytes_indices{pwrite(binMesh, buffer_indices, size_indicesArray_write, offset_bin)};

    if(fileManager_POSIX::pwriting_handlingError(countBytes_indices, size_indicesArray_write, temp_creationFileDir) == 0)
     {
      close(binMesh);
      filesystem_manager::delete_file_sentence(headerMesh.nameMeshBin, temp_creationFileDir);
      return 0;
     }

   offset_bin += countBytes_indices;
   //bytesRest_write -= countBytes_rVertex; 
 }

 close(binMesh);

if(access(outDir_re.c_str(), F_OK) == 0)
{
 std::filesystem::remove(outDir_re); ///REMOVE THE EXISTING FILE THAT WILL BE OVERWRITE
}
 std::filesystem::rename(temp_creationFileDir, outDir_re); ////RENAME FILE WITH THE NAME THAT OVERWRITES THE FILE  
// std::cout << "SUCCESSFULLY complete writing MeshBin file:: file = " << headerMesh.nameMeshBin << "\n"; 
  log_System::meshCooker_logger.success("writing binary success | meshBin = " + outDir_re);
 return 1;
}

inline uint32_t pack_binMesh_posix_newFile(const mesh_LeoHeader& headerMesh, std::vector<data_meshF>& data_meshes, const std::string outDir, const std::string& dir_origin)
{
 std::string newDir {outDir}; 
 customFiles::rename_fileExisting(newDir);
 
 return pack_binMesh_posix_overwrite(headerMesh, data_meshes, newDir, dir_origin);
}


////CREATE FILE MANAGER BIN MESH HERE, LIKE MATERIAL BIN

#elif defined(_WIN32)

#endif

inline uint32_t writeFile_MeshBinary(const mesh_LeoHeader& headerMesh, std::vector<data_meshF>& data_meshes, const std::string dirOrigin, file_OP::writeFlags fileT)
{
  std::string nameMesh {headerMesh.nameMeshBin};

  uint32_t w_test {};

  nameMesh += data_meshCore::vefNum;
  std::string dir_create = dirOrigin + nameMesh; ///origin/name.leoa --- DIRECTION LEO MATERIAL

  log_System::meshCooker_logger.info("creating mesh binary | meshBin = " + nameMesh); 
 // std::cout << "CREATING MATERIAL BINARY::MATBIN ---> " << headerMesh.nameMeshBin;

  #if defined(__unix__) || defined(__unix) && defined(__MACH__)

  using typeWriteFlags = std::underlying_type_t<file_OP::writeFlags>;
   if(!static_cast<typeWriteFlags>(fileT & file_OP::writeFlags::OVERWRITE))
  {
    log_System::meshCooker_logger.info("overwriting meshBin | meshBin = " + nameMesh);
    w_test = pack_binMesh_posix_overwrite(headerMesh, data_meshes, dir_create, dirOrigin);
    return w_test;
  }

   if(!static_cast<typeWriteFlags>(fileT & file_OP::writeFlags::NEW_FILE))
  {
   log_System::meshCooker_logger.info("new file meshBin | meshBin = " + nameMesh);
   w_test = pack_binMesh_posix_newFile(headerMesh, data_meshes, dir_create, dirOrigin);
   return w_test;
  }

 #elif defined(_WIN32)

 #endif 

 return w_test;
}


#endif //MESH_BINFORMAT_H
