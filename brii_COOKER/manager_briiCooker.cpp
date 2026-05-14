//
// Created by brii on 12/05/26.
//

#include "manager_briiCooker.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "dataManager/convertion_DataManager.h"

namespace manager_GD
{
   std::array<std::array<char, 4>, 4> verifiedNumbers_D
   {
    {
       {'L', 'E', 'O', 'R'},
       {'L', 'E', 'O', 'M'},
       {'L', 'E', 'O', 'A'},
       {'L', 'E', 'O', 'T'}
    }
   };

   const std::string pathMaterials {"assets_engine/binMaterials"};
   const std::string pathTextures {"assets_engine/KTX_Textures"};
   const std::string pathModels {"assets_engine/bin_Models_Meshes"};
}

namespace manager_AssimpData
{
   entity_MeshManager::entity_MeshManager() = default;
   entity_MeshManager::entity_MeshManager(mesh_LeoHeader& headerMesh, std::vector<vertex_D>& dataVertex) : headerMesh(headerMesh), dataVertex(dataVertex){};
   entity_MeshManager::entity_MeshManager(entity_MeshManager&& mesh_D) noexcept
   {
      headerMesh = mesh_D.get_headerMesh();
      dataVertex = mesh_D.get_dataVertex();
      dataIndices = mesh_D.get_dataIndices();
   }
   entity_MeshManager::entity_MeshManager(entity_MeshManager& mesh_D)
   {
      headerMesh = mesh_D.get_headerMesh();
      dataVertex = mesh_D.get_dataVertex();
      dataIndices = mesh_D.get_dataIndices();

   }
   entity_MeshManager::~entity_MeshManager() { destroy(); };

   void entity_MeshManager::insert_nameMesh(std::string& nameMesh_D)
   {
      headerMesh.mesh_nameLenght = sizeof(nameMesh);
      nameMesh = nameMesh_D;
   }

   void entity_MeshManager::insert_verifiedNumber(std::array<char, 4>& verifiedNum)
   {
       for (unsigned int i = 0; i < 4; ++i)
       {
          headerMesh.verifiedNumber[i] = verifiedNum[i];
       }
   }
   void entity_MeshManager::insert_version(uint32_t& version)
   {
      headerMesh.version = version;
   }

   void entity_MeshManager::insert_vertexD(float position[3], float normal[3], float uv[2])
   {
      dataVertex.emplace_back(position, normal, uv);
   }
   void entity_MeshManager::update_size_vertexD()
   {
      headerMesh.vertexCount = dataVertex.size();
   };

   void entity_MeshManager::insert_Indice(unsigned int indice)
   {
      dataIndices.emplace_back(indice);
   }
   void entity_MeshManager::update_size_indices()
   {
     headerMesh.indexCount = dataIndices.size();
   }

   void entity_MeshManager::insert_mat4Transformation(std::array<float, 16>& mat4_array)
   {
     for (unsigned int i = 0; i < 16; ++i)
     {
        headerMesh.mesh_transformation[i] = mat4_array[i];
     }
   }

   void entity_MeshManager::destroy()
   {
      dataVertex.clear();
      dataIndices.clear();
   }

   mesh_LeoHeader& entity_MeshManager::get_headerMesh()
   {
      return headerMesh;
   }
   std::vector<vertex_D>& entity_MeshManager::get_dataVertex()
   {
     return dataVertex;
   }
   std::vector<unsigned int>& entity_MeshManager::get_dataIndices()
   {
     return dataIndices;
   }

/*
   entity_MeshManager entity_MeshManager::operator=(entity_MeshManager&& mesh_D) noexcept
   {
      headerMesh = mesh_D.get_headerMesh();
      dataVertex = mesh_D.get_dataVertex();
      dataIndices = mesh_D.get_dataIndices();

      return *this;
   }

   entity_MeshManager entity_MeshManager::operator=(entity_MeshManager& mesh_D)
   {
      headerMesh = mesh_D.get_headerMesh();
      dataVertex = mesh_D.get_dataVertex();
      dataIndices = mesh_D.get_dataIndices();

      return *this;
   }
*/
   entity_MateriaManager::entity_MateriaManager() = default;
   entity_MateriaManager::entity_MateriaManager(material_LeoHeader& dataMaterial) : dataMaterial(dataMaterial){};

   entity_ModelManager::entity_ModelManager() = default;

   /*
   entity_ModelManager::entity_ModelManager(entity_ModelManager&& entity_modelM) noexcept
   {
      std::vector<entity_MeshManager>& meshes {entity_modelM.outMeshes()};

      meshes_D.assign(meshes.begin(), meshes.end());
      entity_modelM.destroy();
   }
   entity_ModelManager::entity_ModelManager(entity_ModelManager& entity_modelM)
   {
      meshes_D = entity_modelM.outMeshes();
      entity_modelM.destroy();
   }
*/
   void entity_ModelManager::insertMesh(entity_MeshManager& mesh)
   {
      meshes_D.emplace_back(mesh);
   }

   std::vector<entity_MeshManager>& entity_ModelManager::outMeshes()
   {
     return meshes_D;
   }

   void entity_ModelManager::destroy()
   {
      meshes_D.clear();
   }
}

namespace data_leoBinary
{
   std::vector<manager_AssimpData::entity_MateriaManager> materials_D{};
   std::vector<manager_AssimpData::entity_ModelManager> models_D{};
   Assimp::Importer assimpImporter{};

   void loadModel(std::string nameModel, std::string path, unsigned int aiProcessFlags, uint32_t version)
   {
      std::filesystem::path modelPath {path};
      const aiScene* scene = assimpImporter.ReadFile(modelPath.string(), aiProcessFlags);

      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
      {
         std::cout << "ERROR::ASSIMP::" << assimpImporter.GetErrorString() << '\n';
         return;
      }

      manager_AssimpData::entity_ModelManager model{};

      model.insertModelName(nameModel);

      model_LeoHeader header{};

      uint8_t signVerf {static_cast<uint8_t>(manager_GD::signBin::MODEL)};
      std::array<char, 4>& verfNum {manager_GD::verifiedNumbers_D[signVerf]};

      header.verifiedNumber[0] = verfNum[0];
      header.verifiedNumber[1] = verfNum[1];
      header.verifiedNumber[2] = verfNum[2];
      header.verifiedNumber[3] = verfNum[3];

      uint32_t modelID { FNV::str_to_hash(nameModel) };  /////CONTINUE HERE
      header.modelID = modelID;

      header.version = version;

      int meshesCounter{};
      processNode(model, scene->mRootNode, scene, meshesCounter, version);

   }

   void processNode(manager_AssimpData::entity_ModelManager& model, aiNode* node, const aiScene* scene, int& meshesCounter,uint32_t& version)
   {

      aiMatrix4x4 aiModelMat {node->mTransformation}; ///TRANSFORM THIS MODEL MATRIX TO NORMAL GLM::MATRIX

      std::array<float, 16> array_ModelMat4 {convert_dataTypes::aiMat4_to_arrayFloat16(aiModelMat)}; ///GET THE MODEL MATRIX FROM THE NODE

      for (unsigned int i = 0; i < node->mNumMeshes; i++)
      {
         manager_AssimpData::entity_MeshManager mesh_D{};
         aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

         std::string meshName { mesh->mName.C_Str() };

         constexpr std::string_view badCharacters{ "./"};
         size_t find_BC {meshName.find_first_of(badCharacters)};

         if (meshName.empty() || find_BC != std::string::npos)
         {
            meshName = scene->mRootNode->mName.C_Str();
            meshName += "_" + std::to_string(meshesCounter);
         }

         mesh_D.insert_nameMesh(meshName);   ////INSERT THE NAME OF THE MESH

         processMesh_data(mesh_D, mesh, array_ModelMat4, version);
         processMaterial(mesh_D);

         model.insertMesh(mesh_D);
         meshesCounter++;
         //SDL_Log(nameMesh.c_str());
      }

      for (unsigned int i = 0; i < node->mNumChildren; i++)
      {
         processNode(model, node->mChildren[i], scene, meshesCounter, version);
      }
   }
   void processMesh_data(manager_AssimpData::entity_MeshManager& meshManager, aiMesh* mesh, std::array<float, 16>& meshTransMatrix, uint32_t& version)
   {
      uint8_t signVerf {static_cast<uint8_t>(manager_GD::signBin::MESH)};
      std::array<char, 4>& verfNum {manager_GD::verifiedNumbers_D[signVerf]};
      meshManager.insert_verifiedNumber(verfNum);
      meshManager.insert_version(version);

      meshManager.insert_mat4Transformation(meshTransMatrix);

      for (unsigned int i = 0; i < mesh->mNumVertices; i++)
      {
         float position[3]
         {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
         };

         float normal[3]
         {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
         };

         float uv[2]{};
         if (mesh->mTextureCoords[0])
         {
            uv[0] = mesh->mTextureCoords[0][i].x;
            uv[1] = mesh->mTextureCoords[0][i].y;
         }

         else
         {
            uv[0] = 0.0f;
            uv[1] = 0.0f;
            std::cout << "ERROR::NOT::TEXCOORDS";
         }

         meshManager.insert_vertexD(position, normal, uv);
      }

      meshManager.update_size_vertexD();

      for (unsigned int i = 0; i < mesh->mNumFaces; i++)
      {
         aiFace Face = mesh->mFaces[i];

         for (unsigned int f = 0; f < Face.mNumIndices; f++)
         {
           meshManager.insert_Indice(Face.mIndices[f]);
         }
      }

      meshManager.update_size_indices();

   }


   void processMaterial(manager_AssimpData::entity_MeshManager& meshManager)
   {
          /////CONTINUAR CON LOS MATERIALES
   }

}
