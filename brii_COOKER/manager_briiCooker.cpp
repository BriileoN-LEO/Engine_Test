//
// Created by brii on 12/05/26.
//

#include "manager_briiCooker.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "dataManager/convertion_DataManager.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "ThirdPartyLibs/stb_image.h"
#include <stdlib.h>

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
   entity_MateriaManager::entity_MateriaManager(material_LeoHeader& dataMaterial, textures_MaterialManager& str_pathTextures) :  dataMaterial(dataMaterial), str_pathTextures(str_pathTextures) {};



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

namespace manage_texturesCooker
{
    std::unordered_map<aiTextureType, std::string> nameTextures
   {
     {aiTextureType_BASE_COLOR, "ALBEDO"},
     {aiTextureType_DIFFUSE, "DIFFUSE"}
   };

   std::unordered_map<uint32_t, std::string> textures_saved;

   unsigned char* process_EmbeddedTexture(const aiTexture* texture, int& width, int& height, int& channels, const std::string& nameModel_Path, aiTextureType& matType)
   {
     if (texture->mHeight == 0) ///THE TEXTURE IS COMPRESS
     {
      std::cout << "TEXTURE(" << nameTextures[matType] << ") EMBEDDED :: MODEL( " << nameModel_Path << "):: COMPRESS" << std::endl;

      unsigned char* texPixels {
         stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(texture->pcData),
        texture->mWidth,
        &width,
        &height,
        &channels,
        4
       )};

        if (texPixels)
        {
          std::cout << "SUCCESS::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: COMPRESS" << std::endl;
        }

        else
        {
          std::cout << "ERROR::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: COMPRESS" << std::endl;
          std::cout << "REASON --> " << stbi_failure_reason() << std::endl;
        }

        return std::move(texPixels);
     }

     else if (texture->mHeight > 0)  ///THE TEXTURE IS NOT COMPRESS
     {
        std::cout << "TEXTURE(" << nameTextures[matType] << ") EMBEDDED :: MODEL( " << nameModel_Path << "):: RAW" << std::endl;

        width = texture->mWidth;
        height = texture->mHeight;

        size_t sizeBytes = width * height * 4;

        unsigned char* texPixels {(unsigned char*)malloc(sizeBytes)};

        memcpy(texPixels, texture->pcData, sizeBytes);

        if (texPixels)
        {
           std::cout << "SUCCESS::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: RAW" << std::endl;
        }

        else
        {
           std::cout << "ERROR::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: RAW" << std::endl;
           std::cout << "REASON --> " << stbi_failure_reason() << std::endl;
        }

        return texPixels;
     }


      return nullptr;
   }

   unsigned char* processTexture_pixels(aiMaterial* material, aiTextureType matType, const aiScene* scene, const std::string& nameModel_Path, const std::string& directory, int& width, int& height, int& nrChannels, std::string& nameTexture)
   {
      if (material->GetTextureCount(matType) > 0)
     {

       aiString aiStr{};
       material->GetTexture(matType, 0, &aiStr);

       std::string pathTex {aiStr.C_Str()};

       size_t binPath {pathTex.find_first_of("*")};

       unsigned char* pixelTex{ nullptr };

       if (binPath == std::string::npos)
       {
          std::cout << "PROCESS::DIRECTORY_TEXTURE:: NAME_MODEL(" << nameModel_Path << ")" << " :: MATERIAL_TYPE(" << nameTextures[matType] << ")" <<std::endl;

          nameTexture = std::filesystem::path(pathTex).stem().string(); ////ONLY NAME
          size_t pos_BC{};
          if (convert_str::find_badCharacters_filePath(nameTexture, pos_BC))
          {
            nameTexture = nameTexture.substr(0, pos_BC);
          }

        //  nameTexture = nameModel_Path + "_" + nameTexture + "_" + nameTextures[matType];
         // uint32_t ID_nameTexture { FNV::str_to_hash(nameTexture) };

       //   auto findTexture {textures_saved.find(ID_nameTexture)};

        //  if (findTexture != textures_saved.end())
         // {
         //   return findTexture->second;
         // }

          /////////////// IF THE TEXTURE IS NOT FIND  /////////////////

         std::string pathTexture{directory + pathTex};

         pixelTex = stbi_load(pathTexture.c_str(), &width, &height, &nrChannels, 0);

       }

       else if (binPath != std::string::npos)
       {
         std::cout << "PROCESS::EMBEDDED_TEXTURE:: NAME_MODEL(" << nameModel_Path << ")" << " :: MATERIAL_TYPE(" << nameTextures[matType] << ")" <<std::endl;

         pathTex = pathTex.substr(1);
         const aiTexture* embeddedTex { scene->mTextures[std::stoi(pathTex)]};

         nameTexture = embeddedTex->mFilename.C_Str();

         if (!nameTexture.empty())
         {
            nameTexture = std::filesystem::path(nameTexture).stem().string();
            size_t pos_BC{};
            if (convert_str::find_badCharacters_filePath(nameTexture, pos_BC))
            {
               nameTexture = nameTexture.substr(0, pos_BC);
            }

           // nameTexture = nameModel_Path + "_" + nameTexture + "_" + nameTextures[matType];
         }

         else
         {
            nameTexture = nameModel_Path + "_" + nameTextures[matType];
         }

         pixelTex = process_EmbeddedTexture(embeddedTex, width, height, nrChannels, nameModel_Path, matType);
       }


      return std::move(pixelTex);
     }

   }

   void loadTextures(aiMaterial* material, manager_AssimpData::textures_MaterialManager& str_textures, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene)
   {
      std::string textures_binDirectory{"assets_engine/texturesModels/" + nameModel_Path + "_textures"};
      ////CONTINUE HERE TO LOAD ALL TEXTURES

      int width_albedo{};
      int height_albedo{};
      int nrChannels_albedo{};
      std::string nameTex_albedo{};

      unsigned char* albedo_tex {processTexture_pixels(material, aiTextureType_BASE_COLOR, scene, nameModel_Path, directory, width_albedo, height_albedo, nrChannels_albedo, nameTex_albedo)};
      if (albedo_tex == nullptr)
      {
        albedo_tex = processTexture_pixels(material, aiTextureType_DIFFUSE, scene, nameModel_Path, directory, width_albedo, height_albedo, nrChannels_albedo, nameTex_albedo);
      }

      int width_opacity{};
      int height_opacity{};
      int nrChannels_opacity{};
      std::string nameTex_opacity{};

      unsigned char* opacity_tex{processTexture_pixels(material, aiTextureType_OPACITY, scene, nameModel_Path, directory, width_opacity, height_opacity, nrChannels_opacity, nameTex_opacity)};

      ///THINGS TO DO
      ///
      ///- CONVERT THE TEXTURES TO PBR IF THE DONT ARE PBR
      ///- COMBINE THE TEXTURES ALBEDO WITH OPACITY, AND THE TEXTURE RMA [ROUGHNESS] [METALLIC] [AMBIENT OCLUSION]


   }

}

namespace data_leoBinary
{
   std::vector<manager_AssimpData::entity_MateriaManager> materials_D{};
   std::vector<manager_AssimpData::entity_ModelManager> models_D{};
   Assimp::Importer assimpImporter{};
   uint32_t ID_defaultMaterial{};

   void load_Settings_Cooker()
   {
     ID_defaultMaterial = FNV::str_to_hash("DefaultMaterial");
   }

   uint32_t proccess_nameMaterial(std::string& nameMat, const std::string& nameModel_path, const unsigned int& index)
   {

      uint32_t ID_material_FNV { FNV::str_to_hash(nameMat) };

      if (nameMat.empty() || ID_material_FNV == ID_defaultMaterial)
      {
         nameMat = nameModel_path + "_" + std::to_string(index); ////this is the index of the material.
         ID_material_FNV = FNV::str_to_hash(nameMat);
      }

      else
      {
         nameMat = nameModel_path + "_" + nameMat;
         ID_material_FNV = FNV::str_to_hash(nameMat);
      }

      return ID_material_FNV;
   }

   void loadModel(std::string nameModel, std::string path, unsigned int aiProcessFlags, uint32_t version)
   {
      std::filesystem::path modelPath {path};
      const aiScene* scene = assimpImporter.ReadFile(modelPath.string(), aiProcessFlags);

      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
      {
         std::cout << "ERROR::ASSIMP::" << assimpImporter.GetErrorString() << '\n';
         return;
      }

      std::string directory {path};
      size_t pos_d = directory.find_last_of("/");
      directory = directory.substr(0, pos_d);

      ////////LOAD MATERIALS PROCESS/////////
      std::string nameModel_path {std::filesystem::path(path).stem().string()};
      size_t pos_BC{};
      if (convert_str::find_badCharacters_filePath(nameModel_path, pos_BC))
      {
        nameModel_path = nameModel_path.substr(0, pos_BC);
      }
      processMaterials(scene, nameModel_path, directory);


     /////////LOAD MODEL AND MESHES PROCESS//////////// THIS SAVE MATERIALS ID WITH THE NAME
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
         size_t pos_BC{};
         if (convert_str::find_badCharacters_filePath(meshName, pos_BC))
         {
            meshName = scene->mRootNode->mName.C_Str();
            meshName += "_" + std::to_string(meshesCounter);
         }

         mesh_D.insert_nameMesh(meshName);   ////INSERT THE NAME OF THE MESH

         processMesh_data(mesh_D, mesh, array_ModelMat4, version);

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

   void processMaterials(const aiScene* scene, const std::string& nameModel_path, const std::string& directory)
   {
     for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
     {
       material_LeoHeader headerMat{};

       aiMaterial* material { scene->mMaterials[i] };

       aiString matName {};
       std::string nameMat {};

       if (material->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS)
       {
         nameMat = matName.C_Str();
       }

       uint32_t ID_material_FNV { proccess_nameMaterial(nameMat, nameModel_path, i) };
       headerMat.materialID = ID_material_FNV;

       uint8_t signVerf {static_cast<uint8_t>(manager_GD::signBin::MATERIAL)};
       std::array<char, 4>& verfNum {manager_GD::verifiedNumbers_D[signVerf]};

       headerMat.verifiedNumber[0] = verfNum[0];
       headerMat.verifiedNumber[1] = verfNum[1];
       headerMat.verifiedNumber[2] = verfNum[2];
       headerMat.verifiedNumber[3] = verfNum[3];

       std::cout << "LOADING::MATERIAL:: NAME -> " << nameMat << std::endl;

       manager_AssimpData::textures_MaterialManager str_tex{};
       manage_texturesCooker::loadTextures(material, str_tex, nameModel_path, directory, scene);

       //

     }

          /////CONTINUAR CON LOS MATERIALES
   }

}
