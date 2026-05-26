//
// Created by brii on 12/05/26.
//

#include "manager_briiCooker.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "dataManager/convertion_DataManager.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_resize2.h"


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

   void combine_Albedo_Opacity(std::vector<unsigned char> outAlbedoOpa, unsigned char* albedoPixels, unsigned char* opacityPixels, const int& totalPixels_notChannels)
   {
      outAlbedoOpa.resize(totalPixels_notChannels * 4);

     for (int i = 0; i < totalPixels_notChannels; ++i)
     {
       int index = i * 4;

       outAlbedoOpa[index] = albedoPixels[index];
       outAlbedoOpa[index + 1] = albedoPixels[index + 1];
       outAlbedoOpa[index + 2] = albedoPixels[index + 2];
       outAlbedoOpa[index + 3] = opacityPixels[i];

        /////CONTINUE HERE
        ////REVIEW THIS FUNCTION
     }
   }

   void convert_RMA(std::vector<unsigned char>& RMA, unsigned char* roughnessPixels, unsigned char* metallicPixels, unsigned char* ambientOclussionPixels, const int& totalPixels)
   {
      RMA.resize(totalPixels * 4);

      int index{};

      unsigned char roughness_pixel{};
      unsigned char metallic_pixel{};
      unsigned char ambientOclussion_pixel{};

     for (int i = 0; i < totalPixels; ++i)
     {
        index = i * 4;

        if (roughnessPixels != nullptr)
        {
           roughness_pixel = roughnessPixels[i];
        }
        else
        {
           roughness_pixel = static_cast<unsigned char>(255.0f);
        }

        if (metallicPixels != nullptr)
        {
           metallic_pixel = metallicPixels[i];
        }
        else
        {
           metallic_pixel = static_cast<unsigned char>(255.0f);
        }

        if (ambientOclussionPixels != nullptr)
        {
           ambientOclussion_pixel = ambientOclussionPixels[i];
        }
        else
        {
           ambientOclussion_pixel = static_cast<unsigned char>(255.0f);
        }

        RMA[index] = roughness_pixel;
        RMA[index + 1] = metallic_pixel;
        RMA[index + 2] = ambientOclussion_pixel;
        RMA[index + 4] = static_cast<unsigned char>(255.0f);
     }
   }
   void resizeTexture_stb(std::vector<unsigned char>& resize_Texture, unsigned char* texture, const int& width_old, const int& height_old, const int& width_new, const int& height_new, const int& numChannels, resizeType resT)
   {

      int total_pixel {width_new * height_new * numChannels};

      resize_Texture.resize(total_pixel);

      stbir_pixel_layout layout = STBIR_RGBA;

      switch (numChannels)
      {
         case 1 :
            layout = STBIR_1CHANNEL;
            break;
         case 2 :
            layout = STBIR_2CHANNEL;
            break;
         case 3 :
            layout = STBIR_RGB;
            break;
      }

      switch (resT)
      {
         case resizeType::LINEAR :
         {
           stbir_resize_uint8_linear
           (
            texture, width_old, height_old, 0,
            resize_Texture.data(), width_new, height_new, 0,
            layout
           );
           break;
         }

         case resizeType::SRGB :
            {
            stbir_resize_uint8_srgb
            (
             texture, width_old, height_old, 0,
             resize_Texture.data(), width_new, height_new, 0,
             layout
            );
            break;
            }
      }
   }

   bool testMaterial_PBR(aiMaterial* material)
   {
     int shadingM {};

      if (material->Get(AI_MATKEY_SHADING_MODEL, shadingM) == AI_SUCCESS)
      {
        if (shadingM == aiShadingMode_PBR_BRDF)
        {
           return true;
        }
      }

      if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
      {
        return true;
      }

      if (material->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
      {
         return true;
      }

      if (material->GetTextureCount(aiTextureType_NORMAL_CAMERA) > 0)
      {
         return true;
      }

      if (material->GetTextureCount(aiTextureType_EMISSION_COLOR) > 0)
      {
         return true;
      }

      if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
      {
         return true;
      }

      if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
      {
         return true;
      }

      return false;
   }

   void convertTex_to_PBR(const unsigned char* diffusePixels, const unsigned char* specularPixels, const unsigned char* shininessPixels,
               const int& totalPixels, std::vector<unsigned char>& outAlbedo, std::vector<unsigned char>& outRMA)
   {
      outAlbedo.resize(totalPixels);
      outRMA.resize(totalPixels);

      for (int i = 0; i < totalPixels; ++i)
      {
         int index = i * 4;

         float diff_R { diffusePixels ? (diffusePixels[index] / 255.0f) : 1.0f };
         float diff_G { diffusePixels ? (diffusePixels[index + 1] / 255.0f) : 1.0f };
         float diff_B { diffusePixels ? (diffusePixels[index + 2] / 255.0f) : 1.0f };

         float spec_R { specularPixels ? (specularPixels[index] / 255.0f) : 0.5f };
         float spec_G { specularPixels ? (specularPixels[index + 1] / 255.0f) : 0.5f };
         float spec_B { specularPixels ? (specularPixels[index + 2] / 255.0f) : 0.5f };

         float shine_R { shininessPixels ? (shininessPixels[index] / 255.0f) : 0.5f };


         float roughness_v { std::max(0.0f, std::min(1.0f, 1.0f - shine_R))};

         float specLuma { (0.2126f * spec_R) + (0.7152f * spec_G) + (0.0722f * spec_B) };

         float metallic_v {};

         if (specLuma > 0.2f)
         {
          metallic_v = (specLuma - 0.2f) / 0.8f;
          metallic_v = std::max(0.0f, std::min(1.0f, metallic_v));
         }

         float albedo_R = diff_R * (1.0f - metallic_v) + spec_R * metallic_v;
         float albedo_G = diff_G * (1.0f - metallic_v) + spec_G * metallic_v;
         float albedo_B = diff_B * (1.0f - metallic_v) + spec_B * metallic_v;


         outAlbedo[index] = static_cast<unsigned char>(albedo_R * 255.0f);
         outAlbedo[index + 1] = static_cast<unsigned char>(albedo_G * 255.0f);
         outAlbedo[index + 2] = static_cast<unsigned char>(albedo_B * 255.0f);
         outAlbedo[index + 3] = diffusePixels[index + 3];

         outRMA[index] = static_cast<unsigned char>(roughness_v * 255.0f);
         outRMA[index + 1] = static_cast<unsigned char>(metallic_v * 255.0f);
         outRMA[index + 2] = static_cast<unsigned char>(255.0f);
         outRMA[index + 3] = static_cast<unsigned char>(255.0f);
      }

   }

   unsigned char* process_EmbeddedTexture(const aiTexture* texture, int& width, int& height, int& channels, const std::string& nameModel_Path, aiTextureType& matType, int numChannels_obj)
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
        numChannels_obj
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

        size_t sizeBytes = width * height * numChannels_obj;

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

   unsigned char* processTexture_pixels(aiMaterial* material, aiTextureType matType, const aiScene* scene, const std::string& nameModel_Path, const std::string& directory, data_image& data_Tex,  int numChannels_obj)
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

          data_Tex.nameTex = std::filesystem::path(pathTex).stem().string(); ////ONLY NAME
          size_t pos_BC{};
          if (convert_str::find_badCharacters_filePath(data_Tex.nameTex, pos_BC))
          {
            data_Tex.nameTex = data_Tex.nameTex.substr(0, pos_BC);
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

         pixelTex = stbi_load(pathTexture.c_str(), &data_Tex.width, &data_Tex.height, &data_Tex.nrChannels, numChannels_obj);

       }

       else if (binPath != std::string::npos)
       {
         std::cout << "PROCESS::EMBEDDED_TEXTURE:: NAME_MODEL(" << nameModel_Path << ")" << " :: MATERIAL_TYPE(" << nameTextures[matType] << ")" <<std::endl;

         pathTex = pathTex.substr(1);
         const aiTexture* embeddedTex { scene->mTextures[std::stoi(pathTex)]};

         data_Tex.nameTex = embeddedTex->mFilename.C_Str();

         if (!data_Tex.nameTex.empty())
         {
            data_Tex.nameTex = std::filesystem::path(data_Tex.nameTex).stem().string();
            size_t pos_BC{};
            if (convert_str::find_badCharacters_filePath(data_Tex.nameTex, pos_BC))
            {
               data_Tex.nameTex = data_Tex.nameTex.substr(0, pos_BC);
            }

           // nameTexture = nameModel_Path + "_" + nameTexture + "_" + nameTextures[matType];
         }

         else
         {
            data_Tex.nameTex = nameModel_Path + "_" + nameTextures[matType];
         }

         pixelTex = process_EmbeddedTexture(embeddedTex, data_Tex.width, data_Tex.height, data_Tex.nrChannels, nameModel_Path, matType, numChannels_obj);
       }


      return std::move(pixelTex);
     }

      return nullptr;
   }

   void loadTextures(aiMaterial* material, manager_AssimpData::textures_MaterialManager& str_textures, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene)
   {
      std::string textures_binDirectory{"assets_engine/texturesModels/" + nameModel_Path + "_textures"};

      data_image dataOpacity{};
      unsigned char* opacity_tex{processTexture_pixels(material, aiTextureType_OPACITY, scene, nameModel_Path, directory, dataOpacity, 4)};

      //////HEIGHT MAP SECTION
      data_image dataHeight{};
      unsigned char* height_tex {processTexture_pixels(material, aiTextureType_HEIGHT, scene, nameModel_Path, directory, dataHeight, 4)};

      if (height_tex == nullptr)
      {
       height_tex = processTexture_pixels(material, aiTextureType_DISPLACEMENT, scene, nameModel_Path, directory, dataHeight, 4);
      }

      if (testMaterial_PBR(material))
      {
         data_image dataAlbedo{};
         unsigned char* albedo_tex {processTexture_pixels(material, aiTextureType_BASE_COLOR, scene, nameModel_Path, directory, dataAlbedo, 4)};

         data_image dataNormalCamera{};
         unsigned char* normalCamera_tex {processTexture_pixels(material, aiTextureType_NORMAL_CAMERA, scene, nameModel_Path, directory, dataNormalCamera, 4)};

         data_image dataEmission{};
         unsigned char* emission_tex {processTexture_pixels(material, aiTextureType_EMISSION_COLOR, scene, nameModel_Path, directory, dataEmission, 4)};

         data_image dataMetalness{};
         unsigned char* metalness_tex {processTexture_pixels(material, aiTextureType_METALNESS, scene, nameModel_Path, directory, dataMetalness, 4)};

         data_image dataDiffuseRoughness{};
         unsigned char* diffuseRoughness_tex {processTexture_pixels(material, aiTextureType_DIFFUSE_ROUGHNESS, scene, nameModel_Path, directory, dataDiffuseRoughness, 4)};

         data_image dataAO{};
         unsigned char* ao_tex {processTexture_pixels(material, aiTextureType_AMBIENT_OCCLUSION, scene, nameModel_Path, directory, dataAO, 4)};


         int maxHeight_RMA {dataDiffuseRoughness.height > dataMetalness.height ? dataDiffuseRoughness.height : dataMetalness.height};
         maxHeight_RMA = maxHeight_RMA > dataAO.height ? maxHeight_RMA : dataAO.height;

         int maxWidth_RMA {dataDiffuseRoughness.width > dataMetalness.width ? dataDiffuseRoughness.width : dataMetalness.width};
         maxWidth_RMA = maxWidth_RMA > dataAO.width ? maxWidth_RMA : dataAO.width;

         unsigned char* metalness_tex_F { metalness_tex };
         unsigned char* roughness_tex_F { diffuseRoughness_tex };
         unsigned char* ao_tex_F { ao_tex };

         if (metalness_tex != nullptr)
         {
            if (dataMetalness.height != maxHeight_RMA || dataMetalness.width != maxWidth_RMA)
            {
               std::vector<unsigned char> newMetalnessData {};
               resizeTexture_stb(newMetalnessData, metalness_tex, dataMetalness.width, dataMetalness.height, maxWidth_RMA, maxHeight_RMA, dataMetalness.nrChannels, resizeType::LINEAR);
               metalness_tex_F = nullptr;
               metalness_tex_F = std::move(newMetalnessData.data());
            }
         }

         if (diffuseRoughness_tex != nullptr)
         {
            if (dataDiffuseRoughness.height != maxHeight_RMA || dataDiffuseRoughness.width != maxWidth_RMA)
            {
               std::vector<unsigned char> newRoughnessData {};
               resizeTexture_stb(newRoughnessData, diffuseRoughness_tex, dataDiffuseRoughness.width, dataDiffuseRoughness.height, maxWidth_RMA, maxHeight_RMA, dataDiffuseRoughness.nrChannels, resizeType::LINEAR);
               roughness_tex_F = nullptr;
               roughness_tex_F = std::move(newRoughnessData.data());
            }
         }


         if (ao_tex != nullptr)
         {
            if (dataAO.height != maxHeight_RMA || dataAO.width != maxWidth_RMA)
            {
               std::vector<unsigned char> newAOData {};
               resizeTexture_stb(newAOData, ao_tex, dataAO.width, dataAO.height, maxWidth_RMA, maxHeight_RMA, dataAO.nrChannels, resizeType::LINEAR);
               ao_tex_F = nullptr;
               ao_tex_F = std::move(newAOData.data());
            }
         }

         int totalPixels {maxHeight_RMA * maxWidth_RMA};

         std::vector<unsigned char> RMA_pbr{};
         convert_RMA(RMA_pbr, roughness_tex_F, metalness_tex_F, ao_tex_F, totalPixels);

         unsigned char* RMA_tex { RMA_pbr.data() };       ///////////////////////RMA TEXTURE

         unsigned char* opa_tex_F {opacity_tex};
         if (opacity_tex != nullptr)
         {
            if (dataOpacity.height != dataAlbedo.height || dataOpacity.width != dataAlbedo.width)
            {
               std::vector<unsigned char> newOpaData {};
               resizeTexture_stb(newOpaData, opacity_tex, dataOpacity.width, dataOpacity.height, dataAlbedo.width, dataAlbedo.height, dataOpacity.nrChannels, resizeType::LINEAR);
               opa_tex_F = nullptr;
               opa_tex_F = std::move(newOpaData.data());
            }
         }

         int totalPixels_AlbedoOpa{dataAlbedo.height * dataAlbedo.width};

         std::vector<unsigned char> AlbedoOpa_pbr{};
         combine_Albedo_Opacity(AlbedoOpa_pbr, albedo_tex, opa_tex_F, totalPixels_AlbedoOpa);
         unsigned char* AlbedoOpa_tex { AlbedoOpa_pbr.data() };


         /////CONTINUE HERE
         //- REVIEW ALL THE CODE TO SEE ISSUES
         //- MAKE ALL THE TEXTURES TO KTX2

       return;
      }

      ////IF THE MATERIAL ARE NOT PBR

      data_image dataDiffuse{};
      unsigned char* diffuse_tex = processTexture_pixels(material, aiTextureType_DIFFUSE, scene, nameModel_Path, directory, dataDiffuse, 4);

      data_image dataSpecular{};
      unsigned char* specular_tex = processTexture_pixels(material, aiTextureType_SPECULAR, scene, nameModel_Path, directory, dataSpecular, 4);

      data_image dataShininess{};
      unsigned char* shininess_tex = processTexture_pixels(material, aiTextureType_SHININESS, scene, nameModel_Path, directory, dataShininess, 4);

      data_image dataNormals{};
      unsigned char* normals_tex = processTexture_pixels(material, aiTextureType_NORMALS, scene, nameModel_Path, directory, dataNormals, 4);

      data_image dataEmissive{};
      unsigned char* emissive_tex = processTexture_pixels(material, aiTextureType_EMISSIVE, scene, nameModel_Path, directory, dataEmissive, 4);

      ///COMPARATION OF SIZE OF TEXTURES
      int maxWidth {dataDiffuse.width >= dataSpecular.width ? dataDiffuse.width : dataSpecular.width};
      maxWidth = maxWidth >= dataShininess.width ? maxWidth : dataShininess.width;

      int maxHeight {dataDiffuse.height >= dataSpecular.height ? dataDiffuse.height : dataSpecular.height};
      maxHeight = maxHeight >= dataShininess.height ? maxHeight : dataShininess.height;

      int totalPixels {maxWidth * maxHeight};

      ///////////TEXTURES TO REMPLACE IF CHANGE THE SIZE
      unsigned char* diff_tex_F{diffuse_tex};
      unsigned char* spec_tex_F{specular_tex};
      unsigned char* shini_tex_F{shininess_tex};
      unsigned char* opa_tex_F{opacity_tex};

      if (diffuse_tex != nullptr) //////////TO ALBEDO
      {
         if (dataDiffuse.width != maxWidth || dataDiffuse.height != maxHeight)
         {
           std::vector<unsigned char> texRS_Diff{};
           resizeTexture_stb(texRS_Diff, diffuse_tex, dataDiffuse.width, dataDiffuse.height, maxWidth, maxHeight, dataDiffuse.nrChannels, resizeType::SRGB);
           diff_tex_F = nullptr;
           diff_tex_F = texRS_Diff.data();
         }
      }

      if (specular_tex != nullptr) //////////TO METALLIC
      {
         if (dataSpecular.width != maxWidth || dataSpecular.height != maxHeight)
         {
           std::vector<unsigned char> texRS_Spec{};
           resizeTexture_stb(texRS_Spec, specular_tex, dataSpecular.width, dataSpecular.height, maxWidth, maxHeight, dataSpecular.nrChannels, resizeType::LINEAR);
           spec_tex_F = nullptr;
           spec_tex_F = texRS_Spec.data();
         }
      }

      if (shininess_tex != nullptr)   //////////TO ROUGHNESS
      {
         if (dataShininess.width != maxWidth || dataShininess.height != maxHeight)
         {
            std::vector<unsigned char> texRS_Shini{};
            resizeTexture_stb(texRS_Shini, shininess_tex, dataShininess.width, dataShininess.height, maxWidth, maxHeight, dataSpecular.nrChannels, resizeType::LINEAR);
            shini_tex_F = nullptr;
            shini_tex_F = texRS_Shini.data();
         }
      }

      if (opacity_tex != nullptr)
      {
         if (dataOpacity.width != maxWidth || dataOpacity.height != maxHeight)
         {
            std::vector<unsigned char> texRS_Opa{};
            resizeTexture_stb(texRS_Opa, opacity_tex, dataOpacity.width, dataOpacity.height, maxWidth, maxHeight, dataOpacity.nrChannels, resizeType::LINEAR);
            opa_tex_F = nullptr;
            opa_tex_F = texRS_Opa.data();
         }
      }

      std::vector<unsigned char> texRS_albedoOpa{};
      combine_Albedo_Opacity(texRS_albedoOpa, diff_tex_F, opa_tex_F, totalPixels);
      unsigned char* albedoOpa_tex {texRS_albedoOpa.data()};

      std::vector<unsigned char> dataAlbedo{};
      std::vector<unsigned char> dataRMA{};
      convertTex_to_PBR(albedoOpa_tex, spec_tex_F, shini_tex_F, totalPixels, dataAlbedo, dataRMA);

      unsigned char* albedo{dataAlbedo.data()};
      unsigned char* RMA {dataRMA.data()};


      //////CONTINUE HERE TO MAKE THE CONVERTION TO PBR

      /////////MAKE A COMPARATION OF EACH SIZE OF THE TEXTURE, THEN CALCULATE WHAT IS THE MAX SIZE OF EACH TEXTURE

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
