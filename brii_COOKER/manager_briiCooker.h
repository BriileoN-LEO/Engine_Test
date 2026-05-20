//
// Created by brii on 12/05/26.
//

#ifndef LEARNING_MANAGER_BRIICOOKER_H
#define LEARNING_MANAGER_BRIICOOKER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "model_binFormat.h"
#include "mesh_binFormat.h"
#include "material_binFormat.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <filesystem>
#include <queue>
#include <unordered_map>

namespace manager_GD
{
  enum class signBin : uint8_t
  {
    MODEL = 0,
    MESH = 1,
    MATERIAL = 2,
    TEXTURE = 3
  };

  extern std::array<std::array<char, 4>, 4> verifiedNumbers_D;
  extern const std::string pathMaterials;
  extern const std::string pathTextures;
  extern const std::string pathModels;
}

namespace manager_AssimpData
{
  class entity_MeshManager
  {
  private:
    mesh_LeoHeader headerMesh{};
    std::string nameMesh{};
    std::vector<vertex_D> dataVertex{};
    std::vector<unsigned int> dataIndices{};

  public:
    entity_MeshManager();
    entity_MeshManager(mesh_LeoHeader& headerMesh, std::vector<vertex_D>& dataVertex);
    entity_MeshManager(entity_MeshManager&& mesh_D) noexcept;
    entity_MeshManager(entity_MeshManager& mesh_D);
    ~entity_MeshManager();

    void insert_nameMesh(std::string& nameMesh_D);
    void insert_verifiedNumber(std::array<char, 4>& verifiedNum);
    void insert_version(uint32_t& version);

    void insert_vertexD(float position[3], float normal[3], float uv[2]);
    void update_size_vertexD();

    void insert_Indice(unsigned int indice);
    void update_size_indices();

    void insert_mat4Transformation(std::array<float, 16>& mat4_array);


    mesh_LeoHeader& get_headerMesh();
    std::vector<vertex_D>& get_dataVertex();
    std::vector<unsigned int>& get_dataIndices();

    //entity_MeshManager operator=(entity_MeshManager&& mesh_D) noexcept;
    //entity_MeshManager operator=(entity_MeshManager& mesh_D);

    void destroy();
  };

  struct textures_MaterialManager
  {
    std::string str_AlbedoPath{};
    std::string str_NormalsPath{};
    std::string str_RMAPath{};
    std::string str_HeightPath{};
    std::string str_EmissionPath{};
  };

  class entity_MateriaManager
  {
  private:
    material_LeoHeader dataMaterial{};
    textures_MaterialManager str_pathTextures{};
    std::string nameMaterial{};

  public:
    entity_MateriaManager();
    entity_MateriaManager(material_LeoHeader& dataMaterial, textures_MaterialManager& str_pathTextures);


  };

  class entity_ModelManager
  {
  private:
    model_LeoHeader headerModel{};
    std::string modelName{};
    std::string directory{};  ///directory of the binary file
    std::vector<meshAsset_register> meshesRegister_LeoMesh{};
    std::vector<entity_MeshManager> meshes_D{};

  public:
    entity_ModelManager();
    //entity_ModelManager(entity_ModelManager&& entity_modelM) noexcept;
    //entity_ModelManager(entity_ModelManager& entity_modelM);

    void insertMesh(entity_MeshManager& mesh);
    void insertHeader(model_LeoHeader& header);
    void insertModelName(std::string modelName);
    void insertDirectory(std::string directory);
    std::vector<entity_MeshManager>& outMeshes();

    void destroy();
  };

}

namespace manage_texturesCooker
{
  struct data_image
  {
    int width{};
    int height{};
    int nrChannels{};
    std::string nameTex{};

  };

   extern std::unordered_map<aiTextureType, std::string> nameTextures;
   extern std::unordered_map<uint32_t, std::string> textures_saved; ///THIS IS A CONTAINER THAT SAVES ALL THE TEXTURES THAT ARE UPLOADED ----> [NAME TEXTURE ID FNV], [PATH OF TEXTURE KTX CONVERTER

   unsigned char* resizeTexture(unsigned char* texture, const int& width_old, const int& height_old, const int& width_new, const int& height_new, const int& numChannels);

   bool testMaterial_PBR(aiMaterial* material);
   unsigned char* convertTex_to_PBR(const unsigned char* diffusePixels, const unsigned char* specularPixels, const unsigned char* glossinesPixels);

   unsigned char* process_EmbeddedTexture(const aiTexture* texture, int& width, int& height, int& channels, const std::string& nameModel_Path, aiTextureType& matType);
   unsigned char* processTexture_pixels(aiMaterial* material, aiTextureType matType, const aiScene* scene, const std::string& nameModel_Path, const std::string& directory, data_image& data_Tex);

   void loadTextures(aiMaterial* material, manager_AssimpData::textures_MaterialManager& str_textures, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene);

}

namespace data_leoBinary
{

  extern std::vector<manager_AssimpData::entity_MateriaManager> materials_D;
  extern std::vector<manager_AssimpData::entity_ModelManager> models_D;
  extern Assimp::Importer assimpImporter;
  extern uint32_t ID_defaultMaterial;

 void load_Settings_Cooker();

 uint32_t proccess_nameMaterial(std::string& nameMat, const std::string& nameModel_path, const unsigned int& index);

 void loadModel(std::string nameModel, std::string path, unsigned int aiProcessFlags, uint32_t version);
 void processNode(manager_AssimpData::entity_ModelManager& model, aiNode* node, const aiScene* scene, int& meshesCounter, uint32_t& version);
 void processMesh_data(manager_AssimpData::entity_MeshManager& meshManager, aiMesh* mesh, std::array<float, 16>& meshTransMatrix, uint32_t& version);

 void processMaterials(const aiScene* scene, const std::string& nameModel_path, const std::string& directory);

}




#endif //LEARNING_MANAGER_BRIICOOKER_H