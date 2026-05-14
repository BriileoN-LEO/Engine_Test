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

  class entity_MateriaManager
  {
  private:
    material_LeoHeader dataMaterial{};
    std::string nameMaterial{};

  public:
    entity_MateriaManager();
    entity_MateriaManager(material_LeoHeader& dataMaterial);

    void insert_nameMaterial(std::string& nameMaterial);
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

namespace data_leoBinary
{
  extern std::vector<manager_AssimpData::entity_MateriaManager> materials_D;
  extern std::vector<manager_AssimpData::entity_ModelManager> models_D;
  extern Assimp::Importer assimpImporter;

 void loadModel(std::string nameModel, std::string path, unsigned int aiProcessFlags, uint32_t version);
 void processNode(manager_AssimpData::entity_ModelManager& model, aiNode* node, const aiScene* scene, int& meshesCounter, uint32_t& version);
 void processMesh_data(manager_AssimpData::entity_MeshManager& meshManager, aiMesh* mesh, std::array<float, 16>& meshTransMatrix, uint32_t& version);
 //void processMaterial(manager_AssimpData::entity_MeshManager& meshManager);

}




#endif //LEARNING_MANAGER_BRIICOOKER_H