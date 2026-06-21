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
#include "files_CoreManager/files_Core.h"

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

   data_image::data_image(){};
   data_image data_image::operator<<(data_image& iR)
   {
      if (texture_KTX2 != nullptr)
      {
         texture_KTX2 = nullptr;
      }

      texture_KTX2 = std::move(iR.texture_KTX2);
      iR.texture_KTX2 = nullptr;

      if (texturePixels != nullptr)
      {
         texturePixels = nullptr;
      }

      texturePixels = std::move(iR.texturePixels);
      iR.texturePixels = nullptr;

      width = iR.width;
      height = iR.height;
      nrChannels = iR.nrChannels;

      nameTex = iR.nameTex;
     // hash_nameTex = iR.hash_nameTex;

      key_texture = iR.key_texture;
     // directoryTex_ktx2 = iR.directoryTex_ktx2;

      type_mem_image = iR.type_mem_image;

      status_tex = iR.status_tex;

      iR.clear();

      return *this;
   }

   void data_image::clear()
   {
      ktxTexture2_Destroy(texture_KTX2);
      texture_KTX2 = nullptr;
      freeMemoryImage(type_mem_image, texturePixels);
      width = 0;
      height = 0;
      nrChannels = 0;
      nameTex.clear();
      //hash_nameTex = 0;
      key_texture = 0;
    //  directoryTex_ktx2.clear();
      key_texture = 0;
      status_tex = texStatus::NOT_EXISTS;
   }


   packPBR_texData::packPBR_texData(){};
   packPBR_texData::packPBR_texData(data_image& albedo_D, data_image& normal_D, data_image& RMA_D, data_image& emissive_D, data_image& height_D)
   {
      ///transfer the data of each texture
      albedo_data << albedo_D;
      normal_data << normal_D;
      RMA_data << RMA_D;
      emissive_data << emissive_D;
      height_data << height_D;
   }

   packPBR_texData packPBR_texData::operator<<(packPBR_texData& dataP)
   {
      albedo_data << dataP.albedo_data;
      normal_data << dataP.normal_data;
      RMA_data << dataP.RMA_data;
      emissive_data << dataP.emissive_data;
      height_data << dataP.height_data;

      return *this;
   }

    std::unordered_map<aiTextureType, std::string> nameTextures
   {
     {aiTextureType_BASE_COLOR, "ALBEDO"},
     {aiTextureType_DIFFUSE, "DIFFUSE"}
   };

 //  std::unordered_map<uint64_t, uint64_t> textures_saved{};
   std::vector<uint64_t> textures_saved{};

   void statePixelColor(std::vector<unsigned char>& texData, int idxData, unsigned char* value, int idxValue)
   {
      texData[idxData] = value[idxValue];
   }
   void statePixelColor_NULL(std::vector<unsigned char>& texData, int idxData, unsigned char* value, int idxValue) {
      texData[idxData] = static_cast<unsigned char>(255.0f);
      unsigned char* val { value };
      int idxVal { idxValue };
   }

   void combine_to_rgba(std::vector<unsigned char> outAlbedoOpa, unsigned char* rgbPixels, unsigned char* aPixels, const int& totalPixels_notChannels)
   {
      pixel_color_state funcAlbedoPixels {statePixelColor};
      pixel_color_state funcOpaPixels {statePixelColor};

      if (rgbPixels == nullptr)
      {
        funcAlbedoPixels = nullptr;
        funcAlbedoPixels = statePixelColor_NULL;
      }

      if (aPixels == nullptr)
      {
         funcOpaPixels = nullptr;
         funcOpaPixels = statePixelColor_NULL;
      }

      auto combAll = [&]()
      {
         for (int i = 0; i < totalPixels_notChannels; ++i)
         {
            int index = i * 4;

            funcAlbedoPixels(outAlbedoOpa, index, rgbPixels, index);
            funcAlbedoPixels(outAlbedoOpa, index + 1, rgbPixels, index + 1);
            funcAlbedoPixels(outAlbedoOpa, index + 2, rgbPixels, index + 2);
            funcOpaPixels(outAlbedoOpa, index + 3, aPixels, i);

            /*
            outAlbedoOpa[index] = albedoPixels[index];
            outAlbedoOpa[index + 1] = albedoPixels[index + 1];
            outAlbedoOpa[index + 2] = albedoPixels[index + 2];
            outAlbedoOpa[index + 3] = opacityPixels[i];
*/
            /////CONTINUE HERE
            ////REVIEW THIS FUNCTION
         }
      };

      if (rgbPixels != nullptr || aPixels != nullptr)
      {
         outAlbedoOpa.resize(totalPixels_notChannels * 4);
         combAll();
      }
   }

   void convert_RMA(std::vector<unsigned char>& RMA, unsigned char* roughnessData, unsigned char* metallicData, unsigned char* ambientOclussionData, const int& totalPixels)
   {
      RMA.resize(totalPixels * 4);

      int index{};

      unsigned char roughness_pixel{};
      unsigned char metallic_pixel{};
      unsigned char ambientOclussion_pixel{};

     for (int i = 0; i < totalPixels; ++i)
     {
        index = i * 4;

        RMA[index] = roughnessData ? roughnessData[i] : static_cast<unsigned char>(255.0f);
        RMA[index + 1] = metallicData ? metallicData[i] : static_cast<unsigned char>(255.0f);
        RMA[index + 2] = ambientOclussionData ? ambientOclussionData[i] : static_cast<unsigned char>(255.0f);
        RMA[index + 4] = static_cast<unsigned char>(255.0f);
     }

   }
   void resizeTexture_stb(std::vector<unsigned char>& resize_Texture, unsigned char* texture, const int& width_old, const int& height_old, const int& width_new, const int& height_new, const int& numChannels, resizeType resT) {
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

   material_Status testMaterial_PBR(aiMaterial* material)
   {
     int shadingM {};

      if (material->Get(AI_MATKEY_SHADING_MODEL, shadingM) == AI_SUCCESS)
      {
        if (shadingM == aiShadingMode_PBR_BRDF)
        {
           return material_Status::MATERIAL_PBR;
        }
      }

      if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
      {
        return material_Status::MATERIAL_PBR;
      }

      if (material->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
      {
         return material_Status::MATERIAL_PBR;
      }

      if (material->GetTextureCount(aiTextureType_NORMAL_CAMERA) > 0)
      {
         return material_Status::MATERIAL_PBR;
      }

      if (material->GetTextureCount(aiTextureType_EMISSION_COLOR) > 0)
      {
         return material_Status::MATERIAL_PBR;
      }

      if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
      {
         return material_Status::MATERIAL_PBR;
      }

      if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
      {
         return material_Status::MATERIAL_PBR;
      }

      ///////////NOT TEXTURES PBR

      if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
      {
        return material_Status::MATERIAL_LEGACY;
      }

      if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
      {
         return material_Status::MATERIAL_LEGACY;
      }

      if (material->GetTextureCount(aiTextureType_SHININESS) > 0)
      {
         return material_Status::MATERIAL_LEGACY;
      }

      if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
      {
         return material_Status::MATERIAL_LEGACY;
      }

      if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
      {
         return material_Status::MATERIAL_LEGACY;
      }

      return material_Status::NOT_TEXTURES_MATERIAL;
   }


   namespace KTX2_manager
   {

      bool compressPixels_UASTC(data_image& texture, ktx_bool_t normalMap)
      {
        ktxBasisParams params{};
        params.structSize = sizeof(params);
        params.codec = 2;
        params.uastcFlags = KTX_PACK_UASTC_LEVEL_FASTEST;

        params.threadCount = 4;
        params.verbose = KTX_FALSE;

        params.normalMap = normalMap;

        KTX_error_code result {ktxTexture2_CompressBasisEx(texture.texture_KTX2, &params)};

        if (result == KTX_SUCCESS)
        {
          std::cout << "COMPRESS_UASTC KTX2 TEXTURE:: TEXTURE --->" + texture.nameTex << "\n";
          return true;
        }

        std::cerr << "ERROR::COMPRESS_UASTC KTX2 TEXTURE:: TEXTURE--->" + texture.nameTex << ktxErrorString(result) <<"\n";
        return false;
      }

      bool inyectPixels(data_image& texture)
      {
        size_t image_size = texture.width * texture.height * texture.nrChannels;

        KTX_error_code result {ktxTexture_SetImageFromMemory(ktxTexture(texture.texture_KTX2), 0, 0, 0, texture.texturePixels, image_size)};

        if (result == KTX_SUCCESS)
         {
           std::cout << "INYECTING KTX2 TEXTURE:: TEXTURE--->" + texture.nameTex << "\n";
           return true;
         }

        std::cerr << "ERROR::INYECT KTX2 TEXTURE:: TEXTURE--->" + texture.nameTex << ktxErrorString(result) <<"\n";
       return false;
      }

      void ktx2_convert(data_image& texture, ktx_bool_t normalMap)
      {
     //   std::string output_dir {pathDirectory_save + "/" + texture.nameTex + ".ktx2"};
        ktxTextureCreateInfo createInfo_KTX2{};

        createInfo_KTX2.vkFormat = VK_FORMAT_R8G8B8_UNORM;
        createInfo_KTX2.baseWidth = texture.width;
        createInfo_KTX2.baseHeight = texture.height;
        createInfo_KTX2.numDimensions = 2;
        createInfo_KTX2.baseDepth = 1;
        createInfo_KTX2.numLevels = 1;
        createInfo_KTX2.numLayers = 1;
        createInfo_KTX2.numFaces = 1;
        createInfo_KTX2.isArray = KTX_FALSE;
        createInfo_KTX2.generateMipmaps = KTX_FALSE;

        KTX_error_code result {ktxTexture2_Create(&createInfo_KTX2, KTX_TEXTURE_CREATE_ALLOC_STORAGE, &texture.texture_KTX2)};

        if (result == KTX_SUCCESS)
        {
         std::cout << "CREATING KTX2 TEXTURE:: TEXTURE--->" + texture.nameTex << "\n";
        }

        else if (result != KTX_SUCCESS)
        {
           std::cerr << "ERROR::NOT CREATE KTX2 TEXTURE:: TEXTURE--->" + texture.nameTex <<  ktxErrorString(result) << "\n";
           texture.clear();
           // ktxTexture2_Destroy(texture.texture_KTX2);
          // texture.texture_KTX2 = nullptr;
           return;
        }

         bool success_tex{};

         //FASE 1 --- INYECT PIXELS
         success_tex = inyectPixels(texture);
         if (success_tex == false)
         {
           texture.clear();
          // ktxTexture2_Destroy(texture.texture_KTX2);
          // texture.texture_KTX2 = nullptr;
           return;
         }

         //FASE 3 --- COMPRESS TO UASTC PIXELS
         success_tex = compressPixels_UASTC(texture, normalMap);
         if (success_tex == false)
         {
            texture.clear();
            return;
         }

         std::cout << "SUCCESS CONVERT KTX2 :: TEXTURE--->" + texture.nameTex << "\n";
      }

   }

   namespace convert_to_PBR {

      void conv_to_RMA(std::vector<unsigned char>& outRMA, std::vector<unsigned char>& specShininess, briT::br_4& spec, float& metallic_v, int& idx)
      {
         spec.r = static_cast<float>(specShininess[idx]);
         spec.g = static_cast<float>(specShininess[idx + 1]);
         spec.b = static_cast<float>(specShininess[idx + 2]);
         spec.a = static_cast<float>(specShininess[idx + 3]);

         float roughness_v { std::max(0.0f, std::min(1.0f, 1.0f - spec.a))};

         float specLuma { (0.2126f * spec.r) + (0.7152f * spec.g) + (0.0722f * spec.b) };

         if (specLuma > 0.2f)
         {
            metallic_v = (specLuma - 0.2f) / 0.8f;
            metallic_v = std::max(0.0f, std::min(1.0f, metallic_v));
         }

         else
         {
           metallic_v = 0;
         }

         outRMA[idx] = static_cast<unsigned char>(roughness_v * 255.0f);
         outRMA[idx + 1] = static_cast<unsigned char>(metallic_v * 255.0f);
         outRMA[idx + 2] = static_cast<unsigned char>(255.0f);
         outRMA[idx + 3] = static_cast<unsigned char>(255.0f);
      }

      void convDiff_to_albedo(std::vector<unsigned char>& outAlbedo, std::vector<unsigned char>& diffOpa, briT::br_4& spec, float& metallic_v, int& idx)
      {
         float albedo_R = static_cast<float>(diffOpa[idx]) * (1.0f - metallic_v) + spec.r * metallic_v;
         float albedo_G = static_cast<float>(diffOpa[idx + 1]) * (1.0f - metallic_v) + spec.g * metallic_v;
         float albedo_B = static_cast<float>(diffOpa[idx + 2]) * (1.0f - metallic_v) + spec.b * metallic_v;

         outAlbedo[idx] = static_cast<unsigned char>(albedo_R * 255.0f);
         outAlbedo[idx + 1] = static_cast<unsigned char>(albedo_G * 255.0f);
         outAlbedo[idx + 2] = static_cast<unsigned char>(albedo_B * 255.0f);
         outAlbedo[idx + 3] = diffOpa[idx + 3];
      }

      void conv_NULL_RMA(std::vector<unsigned char>& outNULL, std::vector<unsigned char>& inNUll, briT::br_4& spec, float& metallic_v, int& idx)
      {
         std::vector<unsigned char>& o = outNULL;
         std::vector<unsigned char>& i = inNUll;

         spec = briT::br_4(0.5f);
         float roughness_v { std::max(0.0f, std::min(1.0f, 1.0f - spec.a))};

         float specLuma { (0.2126f * spec.r) + (0.7152f * spec.g) + (0.0722f * spec.b) };

         if (specLuma > 0.2f)
         {
            metallic_v = (specLuma - 0.2f) / 0.8f;
            metallic_v = std::max(0.0f, std::min(1.0f, metallic_v));
         }

         else
         {
            metallic_v = 0;
         }
         int& id = idx;
      }

      void conv_NULL_Albedo(std::vector<unsigned char>& outNULL, std::vector<unsigned char>& inNULL, briT::br_4& spec, float& metallic_v, int& idx)
      {
         std::vector<unsigned char>& oN = outNULL;
         std::vector<unsigned char>& iN = inNULL;
         briT::br_4& s = spec;
         float& m = metallic_v;
         int& i = idx;
      }

      void convertTextures(std::vector<unsigned char>& pixelsDiffuseOpa, std::vector<unsigned char>& pixelsSpecShinness,
                  const int& totalPixels, std::vector<unsigned char>& outAlbedo, std::vector<unsigned char>& outRMA)
      {
         conv_func convertRMA {conv_NULL_RMA};
         conv_func convertAlbedo {conv_NULL_Albedo};

         if (!pixelsDiffuseOpa.empty())
         {
            outAlbedo.resize(totalPixels);
            convertAlbedo = convDiff_to_albedo;
         }

         if (!pixelsSpecShinness.empty())
         {
            outRMA.resize(totalPixels);
            convertRMA = conv_to_RMA;
         }

         briT::br_4 diff_rgb{};
         briT::br_4 spec_rgb{};
         float metallic_v{};

         //////////////CONTINUE HERE

         for (int i = 0; i < totalPixels; ++i)
         {
            int index = i * 4;

            convertRMA(outRMA, pixelsSpecShinness, spec_rgb, metallic_v, index);

            /////////////////////////////
            /*
            float spec_R { specularPixels ? (specularPixels[index] / 255.0f) : 0.5f };
            float spec_G { specularPixels ? (specularPixels[index + 1] / 255.0f) : 0.5f };
            float spec_B { specularPixels ? (specularPixels[index + 2] / 255.0f) : 0.5f };

            float shine_R { shininessPixels ? (shininessPixels[index] / 255.0f) : 0.5f };

            float roughness_v { std::max(0.0f, std::min(1.0f, 1.0f - shine_R))};

            float specLuma { (0.2126f * spec_R) + (0.7152f * spec_G) + (0.0722f * spec_B) };

            metallic_v {};

            if (specLuma > 0.2f)
            {
               metallic_v = (specLuma - 0.2f) / 0.8f;
               metallic_v = std::max(0.0f, std::min(1.0f, metallic_v));
            }

            outRMA[index] = static_cast<unsigned char>(roughness_v * 255.0f);
            outRMA[index + 1] = static_cast<unsigned char>(metallic_v * 255.0f);
            outRMA[index + 2] = static_cast<unsigned char>(255.0f);
            outRMA[index + 3] = static_cast<unsigned char>(255.0f);
*/
            //////////////
            convertAlbedo(outAlbedo, pixelsDiffuseOpa, spec_rgb, metallic_v, index);
            /*
            float diff_R { diffusePixels ? (diffusePixels[index] / 255.0f) : 1.0f };
            float diff_G { diffusePixels ? (diffusePixels[index + 1] / 255.0f) : 1.0f };
            float diff_B { diffusePixels ? (diffusePixels[index + 2] / 255.0f) : 1.0f };

            float albedo_R = diff_R * (1.0f - metallic_v) + spec_R * metallic_v;
            float albedo_G = diff_G * (1.0f - metallic_v) + spec_G * metallic_v;
            float albedo_B = diff_B * (1.0f - metallic_v) + spec_B * metallic_v;

            outAlbedo[index] = static_cast<unsigned char>(albedo_R * 255.0f);
            outAlbedo[index + 1] = static_cast<unsigned char>(albedo_G * 255.0f);
            outAlbedo[index + 2] = static_cast<unsigned char>(albedo_B * 255.0f);
            outAlbedo[index + 3] = diffusePixels ? diffusePixels[index + 3] : static_cast<unsigned char>(255.0f);
            */
         }

      }
   }

   texStatus process_EmbeddedTexture(const aiTexture* texture, data_image& data_Tex, const std::string& nameModel_Path, aiTextureType& matType, int numChannels_obj)
   {
     if (texture->mHeight == 0) ///THE TEXTURE IS COMPRESS
     {
      std::cout << "TEXTURE(" << nameTextures[matType] << ") EMBEDDED :: MODEL( " << nameModel_Path << "):: COMPRESS::" << data_Tex.nameTex << std::endl;

      unsigned char* texPixels {
         stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(texture->pcData),
        texture->mWidth,
        &data_Tex.width,
        &data_Tex.height,
        &data_Tex.nrChannels,
        numChannels_obj
       )};

        if (texPixels)
        {
          std::cout << "SUCCESS::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: COMPRESS::" << data_Tex.nameTex << std::endl;
          data_Tex.texturePixels = std::move(texPixels);
          texPixels = nullptr;  ////SEE IF I GETS ERROR HERE

          return texStatus::EXISTS;
        }

          std::cout << "ERROR::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: COMPRESS::" << data_Tex.nameTex << std::endl;
          std::cout << "REASON --> " << stbi_failure_reason() << std::endl;
          return texStatus::NOT_EXISTS;

     }

     else if (texture->mHeight > 0)  ///THE TEXTURE IS NOT COMPRESS
     {
        std::cout << "TEXTURE(" << nameTextures[matType] << ") EMBEDDED :: MODEL( " << nameModel_Path << "):: RAW::" << data_Tex.nameTex << std::endl;

        data_Tex.type_mem_image = manager_GD::memType::HEAP_ENGINE;

        data_Tex.width = texture->mWidth;
        data_Tex.height = texture->mHeight;

        size_t sizeBytes = data_Tex.width * data_Tex.height * numChannels_obj;

        unsigned char* texPixels {(unsigned char*)malloc(sizeBytes)};

        memcpy(texPixels, texture->pcData, sizeBytes);

        if (texPixels)
        {
           std::cout << "SUCCESS::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: RAW::" << data_Tex.nameTex << std::endl;
           data_Tex.texturePixels = std::move(texPixels);
           texPixels = nullptr;  ////SEE IF I GETS ERROR HERE

           return texStatus::EXISTS;
        }

           std::cout << "ERROR::LOAD_EMBEDDED_TEXTURE(" << nameTextures[matType] << ") :: MODEL( " << nameModel_Path << "):: RAW::" << data_Tex.nameTex << std::endl;
           std::cout << "REASON --> " << stbi_failure_reason() << std::endl;
           return texStatus::NOT_EXISTS;
     }

      return texStatus::NOT_EXISTS;
   }

   texStatus processTexture_pixels(aiMaterial* material, aiTextureType matType, const aiScene* scene, std::string prefix_nameModel, const std::string& directory, data_image& data_Tex,  int numChannels_obj)
   {
      if (material->GetTextureCount(matType) > 0)
     {
       aiString aiStr{};
       material->GetTexture(matType, 0, &aiStr);

       std::string pathTex {aiStr.C_Str()};

       size_t binPath {pathTex.find_first_of("*")};

       if (binPath == std::string::npos)
       {
          data_Tex.nameTex = std::filesystem::path(pathTex).stem().string(); ////ONLY NAME
          size_t pos_BC{};

          if (convert_str::find_badCharacters_filePath(data_Tex.nameTex, pos_BC) && !data_Tex.nameTex.empty())
           {
              data_Tex.nameTex = prefix_nameModel + data_Tex.nameTex.substr(0, pos_BC) + "_" + nameTextures[matType];
           }

           else
           {
              data_Tex.nameTex = prefix_nameModel + nameTextures[matType];
           }

          customFiles::clear_spaceKey(data_Tex.nameTex);

          std::string str_keyTexture {directory + prefix_nameModel + "_textures" + "/" + data_Tex.nameTex + ".ktx2"};
          data_Tex.key_texture = FNV::hash_1a(str_keyTexture);
          //auto find_TexStatus {textures_saved.find(data_Tex.key_texture)};

          if (std::binary_search(textures_saved.begin(), textures_saved.end(), data_Tex.key_texture))
          {
             std::cout << "PROCESS::DIRECTORY_TEXTURE:: NAME_MODEL(" << prefix_nameModel << ")" << " :: MATERIAL_TYPE(" << nameTextures[matType] << ")::" << std::filesystem::path(pathTex).stem().string() << std::endl;

             std::string pathTexture{directory + pathTex};
             data_Tex.texturePixels = stbi_load(pathTexture.c_str(), &data_Tex.width, &data_Tex.height, &data_Tex.nrChannels, numChannels_obj);

             return texStatus::NOT_LOADED;  ///THE TEXTURE EXISTS BUT IS NOT LOADED IN THE SYSTEM
          }

            return texStatus::LOADED; /////IN THIS CASE THAT THE TEXTURE EXISTS
       }

       else if (binPath != std::string::npos)
       {
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

            data_Tex.nameTex  = prefix_nameModel + data_Tex.nameTex + "_" + nameTextures[matType];
         }

         else
         {
            data_Tex.nameTex = prefix_nameModel + nameTextures[matType];
         }

          customFiles::clear_spaceKey(data_Tex.nameTex);

          std::string str_keyTexture {directory + prefix_nameModel + "_textures" + "/" + data_Tex.nameTex + ".ktx2"};

          data_Tex.key_texture = FNV::hash_1a(str_keyTexture);
         // auto find_TexStatus {textures_saved.find(data_Tex.key_texture)};

          if (std::binary_search(textures_saved.begin(), textures_saved.end(), data_Tex.key_texture))
          {
             std::cout << "PROCESS::EMBEDDED_TEXTURE:: NAME_MODEL(" << prefix_nameModel << ")" << " :: MATERIAL_TYPE(" << nameTextures[matType] << ")::" << std::filesystem::path(data_Tex.nameTex).stem().string() <<std::endl;
             texStatus status_emTex { process_EmbeddedTexture(embeddedTex, data_Tex, prefix_nameModel, matType, numChannels_obj) };

             return status_emTex == texStatus::EXISTS ? texStatus::NOT_LOADED : status_emTex;  ////FALSE IS --> texStatus::NOT_EXISTS
          }

          else
          {
             return texStatus::LOADED; /////IN THIS CASE THAT THE TEXTURE EXISTS
          }
       }
     }

      return texStatus::NOT_EXISTS;
   }

   void freeMemoryImage(const manager_GD::memType& memTexture, unsigned char* dataMem)
   {
      if (dataMem != nullptr)
      {
         switch (memTexture)
         {
            case manager_GD::memType::STBI_MEM :
               stbi_image_free(dataMem);
               break;
            case manager_GD::memType::HEAP_ENGINE :
               free(dataMem);
               break;
            case manager_GD::memType::STACK_HEAP_ENGINE :
               dataMem = nullptr;
               break;
         }

      //   if (memTexture == manager_GD::memType::STBI_MEM)
       //  {
      //      stbi_image_free(dataMem);
      //   }
       //  else if (memTexture == manager_GD::memType::HEAP_ENGINE)
       //  {
       //     free(dataMem);
      //   }
         dataMem = nullptr;
      }

   }

    ///CONVERT TO KTX2
   void convertTextures_KTX2(packPBR_texData& textures_Data)
   {
      if (textures_Data.albedo_data.status_tex == texStatus::NOT_LOADED)
       {
          KTX2_manager::ktx2_convert(textures_Data.albedo_data, KTX_FALSE);
       }

      if (textures_Data.normal_data.status_tex == texStatus::NOT_LOADED)
      {
         KTX2_manager::ktx2_convert(textures_Data.normal_data, KTX_TRUE);
      }

      if (textures_Data.RMA_data.status_tex == texStatus::NOT_LOADED)
      {
         KTX2_manager::ktx2_convert(textures_Data.RMA_data, KTX_FALSE);
      }

      if (textures_Data.emissive_data.status_tex == texStatus::NOT_LOADED)
      {
         KTX2_manager::ktx2_convert(textures_Data.emissive_data, KTX_FALSE);
      }

      if (textures_Data.height_data.status_tex == texStatus::NOT_LOADED)
      {
         KTX2_manager::ktx2_convert(textures_Data.height_data, KTX_FALSE);
      }
      ////////CONTINUE HERE
      ///implement the other functions to create directories and the other functions to save texture to ktx2 texture

   }

   void resizeTex(std::vector<unsigned char>& newDataImage, data_image& texData, texStatus& status, const int& maxHeight, const int& maxWidth, resizeType rT)
   {
      if (status == texStatus::NOT_LOADED)
      {
         if (texData.height != maxHeight || texData.width != maxWidth)
         {
            std::cout << "RESIZING_TEXTURE::" + texData.nameTex + ":: new_Height = " + std::to_string(maxHeight) + " | new_Width = " + std::to_string(maxWidth) << std::endl;
            resizeTexture_stb(newDataImage, texData.texturePixels, texData.width, texData.height, maxWidth, maxHeight, texData.nrChannels, rT);

            freeMemoryImage(texData.type_mem_image, texData.texturePixels);

            texData.texturePixels = newDataImage.data();
            return;
         }
      }

      if (!newDataImage.empty()) { newDataImage.clear(); };
   }

   packPBR_texData loadTextures_PBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb)
   {
      data_image dataOpacity{};
      texStatus opacity_tex{processTexture_pixels(material, aiTextureType_OPACITY, scene, "", directory, dataOpacity, 4)};

      //////HEIGHT MAP SECTION
      data_image dataHeight{};
      texStatus height_tex {processTexture_pixels(material, aiTextureType_HEIGHT, scene, prefixName, directory, dataHeight, 4)};

      if (height_tex == texStatus::NOT_EXISTS)
      {
         height_tex = processTexture_pixels(material, aiTextureType_DISPLACEMENT, scene, prefixName, directory, dataHeight, 4);
      }

      if (height_tex == texStatus::LOADED || height_tex == texStatus::NOT_LOADED)
      {
         saveTexture_ktx2(dataHeight, height_tex, str_textures.str_HeightPath);
      }

         data_image dataAlbedo{};
         texStatus albedo_tex {processTexture_pixels(material, aiTextureType_BASE_COLOR, scene, "", directory, dataAlbedo, 4)};

         data_image dataNormalCamera{};
         texStatus normalCamera_tex {processTexture_pixels(material, aiTextureType_NORMAL_CAMERA, scene, prefixName, directory, dataNormalCamera, 4)};
         saveTexture_ktx2(dataNormalCamera, normalCamera_tex, str_textures.str_NormalsPath);

         data_image dataEmission{};
         texStatus emission_tex {processTexture_pixels(material, aiTextureType_EMISSION_COLOR, scene, prefixName, directory, dataEmission, 4)};
         saveTexture_ktx2(dataEmission, emission_tex, str_textures.str_EmissionPath);

         data_image dataMetalness{};
         texStatus metalness_tex {processTexture_pixels(material, aiTextureType_METALNESS, scene, "", directory, dataMetalness, 4)};

         data_image dataDiffuseRoughness{};
         texStatus diffuseRoughness_tex {processTexture_pixels(material, aiTextureType_DIFFUSE_ROUGHNESS, scene, "", directory, dataDiffuseRoughness, 4)};

         data_image dataAO{};
         texStatus ao_tex {processTexture_pixels(material, aiTextureType_AMBIENT_OCCLUSION, scene, "", directory, dataAO, 4)};

         ////THIS TO IDENTIFY IF RMA EXISTS
         std::string nameRMA {"RMA_" + dataMetalness.nameTex + "_" + dataDiffuseRoughness.nameTex + "_" + dataAO.nameTex + "_" + nameModel_Path};
         customFiles::standard_textureNameKTX(nameRMA);
         uint32_t FNV_nameRMA{FNV::str_to_hash(nameRMA)};

         auto find_TexRMA {textures_saved.find(FNV_nameRMA)};

         if (find_TexRMA == textures_saved.end())
            {

            int maxSize_RMA_PBR {dataDiffuseRoughness.height >= dataMetalness.height ? dataDiffuseRoughness.height : dataMetalness.height};
            maxSize_RMA_PBR = maxSize_RMA_PBR >= dataAO.height ? maxSize_RMA_PBR : dataAO.height;

            int totalSize_RMA_PBR {maxSize_RMA_PBR * maxSize_RMA_PBR * 4};

            std::vector<unsigned char> newDataMetalness{};
            newDataMetalness.resize(totalSize_RMA_PBR);  ////////ALWAYS 4
            resizeTex(newDataMetalness, dataMetalness, metalness_tex, maxSize_RMA_PBR, maxSize_RMA_PBR, resizeType::LINEAR);

            std::vector<unsigned char> newDataRoughness{};
            newDataRoughness.resize(totalSize_RMA_PBR);
            resizeTex(newDataRoughness, dataDiffuseRoughness, diffuseRoughness_tex, maxSize_RMA_PBR, maxSize_RMA_PBR, resizeType::LINEAR);

            std::vector<unsigned char> newDataAO{};
            newDataAO.resize(totalSize_RMA_PBR);
            resizeTex(newDataAO, dataAO, ao_tex, maxSize_RMA_PBR, maxSize_RMA_PBR, resizeType::LINEAR);

            int totalPixels {maxSize_RMA_PBR * maxSize_RMA_PBR};

            std::vector<unsigned char> RMA_pbr{};
            data_image dataRMA{};
            convert_RMA(RMA_pbr, dataDiffuseRoughness.texturePixels, dataMetalness.texturePixels, dataAO.texturePixels, totalPixels);
            dataRMA.width = maxSize_RMA_PBR;
            dataRMA.height = maxSize_RMA_PBR;
            dataRMA.nrChannels = 4;
            dataRMA.nameTex = nameRMA;
            dataRMA.hash_nameTex = FNV_nameRMA;
            dataRMA.texturePixels = RMA_pbr.data();
            saveTexture_ktx2(dataRMA, texStatus::NOT_LOADED, str_textures.str_RMAPath);

            dataDiffuseRoughness.clear();
            dataMetalness.clear();
            dataAO.clear();
            /////LIBERATE MEMORY OF ALL TEXTURES RMA OF EACH SEPARATED, BECAUSE I CREATE THE KTX2 TEXTURE
         }

         else if (find_TexRMA != textures_saved.end())
         {
            str_textures.str_RMAPath = find_TexRMA->second;
         }


         std::string nameTex_AlbedoOpa{ "AlbOpa_" + dataAlbedo.nameTex + "_" + dataOpacity.nameTex + "_" + nameModel_Path};
         customFiles::standard_textureNameKTX(nameTex_AlbedoOpa);
         uint32_t FNV_texAlbOpa{FNV::str_to_hash(nameTex_AlbedoOpa)};
         auto find_TexAlbOpa{textures_saved.find(FNV_texAlbOpa)};

         if (find_TexAlbOpa == textures_saved.end())
         {

            if (opacity_tex != texStatus::NOT_EXISTS || albedo_tex != texStatus::NOT_EXISTS)
            {
               int totalPixels_AlbedoOpa{dataAlbedo.height * dataAlbedo.width};
               bool textureReSize {(totalPixels_AlbedoOpa > 0) && totalPixels_AlbedoOpa != (dataOpacity.width + dataOpacity.height) ? true : false};  ////CHECK IF FAILS

               if (textureReSize)
               {
                  std::vector<unsigned char> newOpaData {};
                  resizeTexture_stb(newOpaData, dataOpacity.texturePixels, dataOpacity.width, dataOpacity.height, dataAlbedo.width, dataAlbedo.height, dataOpacity.nrChannels, resizeType::LINEAR);

                  freeMemoryImage(dataOpacity.type_mem_image, dataOpacity.texturePixels);

                  dataOpacity.texturePixels = newOpaData.data();
               }

               std::vector<unsigned char> AlbedoOpa_pbr{};
               data_image dataAlbedoOpa{};
               combine_to_rgba(AlbedoOpa_pbr, dataAlbedo.texturePixels, dataOpacity.texturePixels, totalPixels_AlbedoOpa);

               /////CONTINUE HERE TO SAVE ALL THE DATA AND DELETE THE INFORMATION OF EACH TEXTURE

               dataAlbedoOpa.texturePixels = AlbedoOpa_pbr.data();
            }
         }
   }
   packPBR_texData loadTextures_notPBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb)
   {
      data_image dataOpacity{};
      texStatus opacity_tex{processTexture_pixels(material, aiTextureType_OPACITY, scene, "", directory, dataOpacity, 4)};

      //////HEIGHT MAP SECTION
      data_image dataHeight{};
      texStatus height_tex {processTexture_pixels(material, aiTextureType_HEIGHT, scene, prefixName, directory, dataHeight, 4)};

      if (height_tex == texStatus::NOT_EXISTS)
      {
         height_tex = processTexture_pixels(material, aiTextureType_DISPLACEMENT, scene, prefixName, directory, dataHeight, 4);
      }
      dataHeight.status_tex = height_tex;

     // if (height_tex == texStatus::LOADED || height_tex == texStatus::NOT_LOADED)
     // {
     //    saveTexture_ktx2(dataHeight, height_tex, str_textures.str_HeightPath);
     // }

      data_image dataDiffuse{};
      texStatus diffuse_tex = processTexture_pixels(material, aiTextureType_DIFFUSE, scene, nameModel_Path, directory, dataDiffuse, 4);

      data_image dataSpecular{};
      texStatus specular_tex = processTexture_pixels(material, aiTextureType_SPECULAR, scene, nameModel_Path, directory, dataSpecular, 4);

      data_image dataShininess{};
      texStatus shininess_tex = processTexture_pixels(material, aiTextureType_SHININESS, scene, nameModel_Path, directory, dataShininess, 4);

      data_image dataNormals{};
      texStatus normals_tex = processTexture_pixels(material, aiTextureType_NORMALS, scene, nameModel_Path, directory, dataNormals, 4);
      dataNormals.status_tex = normals_tex;

      data_image dataEmissive{};
      texStatus emissive_tex = processTexture_pixels(material, aiTextureType_EMISSIVE, scene, nameModel_Path, directory, dataEmissive, 4);
      dataEmissive.status_tex = emissive_tex;

      data_image dataAlbedo_convPBR{};
      data_image dataRMA_convPBR{};

      std::string nameRMA {"RMA_" + dataSpecular.nameTex + "_" + dataShininess.nameTex + "_AO_" + nameModel_Path};
      customFiles::standard_textureNameKTX(nameRMA);
      std::string str_keyTexture_RMA {directory + nameModel_Path + "_textures" + "/" + nameRMA + ".ktx2"};
      uint64_t FNV_nameRMA{ FNV::hash_1a(str_keyTexture_RMA)};

      std::string nameAlbedo{"Albedo_" + dataDiffuse.nameTex + "_" + dataOpacity.nameTex + "_" + nameModel_Path};
      customFiles::standard_textureNameKTX(nameAlbedo);
      std::string str_keyTexture_Albedo {directory + nameModel_Path + "_textures" + "/" + nameAlbedo + ".ktx2"};
      uint64_t FNV_nameAlbedo{FNV::hash_1a(str_keyTexture_Albedo)};

      //auto find_RMA {textures_saved.find(FNV_nameRMA)};
      //auto find_Albedo {textures_saved.find(FNV_nameAlbedo)};
      bool find_RMA {std::binary_search(textures_saved.begin(), textures_saved.end(), FNV_nameRMA)};
      bool find_Albedo {std::binary_search(textures_saved.begin(), textures_saved.end(), FNV_nameAlbedo)};

      if (find_RMA == false || find_Albedo == false)
      {
         int maxSize_pixels{};
         int totalPixels_RMA{};

         int maxHeigth_convPBR{};
         int maxWidth_convPBR{};

         if (diffuse_tex != texStatus::NOT_EXISTS)
         {
            maxHeigth_convPBR = dataDiffuse.height;
            maxWidth_convPBR = dataDiffuse.width;

            maxSize_pixels = dataDiffuse.width * dataDiffuse.height * 4;
            totalPixels_RMA = dataDiffuse.width * dataDiffuse.height;
         }

         else if (diffuse_tex == texStatus::NOT_EXISTS)  ///IF DIFFUSE NOT EXISTS
         {
            maxHeigth_convPBR = dataSpecular.height >= dataShininess.height ? dataSpecular.height : dataShininess.height;  ////THIS PREVENTS
            maxHeigth_convPBR = maxHeigth_convPBR >= dataOpacity.height ? maxHeigth_convPBR : dataOpacity.height;

            maxWidth_convPBR = dataSpecular.width >= dataShininess.width ? dataSpecular.width : dataShininess.width;  ////THIS PREVENTS
            maxWidth_convPBR = maxWidth_convPBR >= dataOpacity.width ? maxWidth_convPBR : dataOpacity.width;

            maxSize_pixels = maxWidth_convPBR * maxHeigth_convPBR * 4;
            totalPixels_RMA = maxWidth_convPBR * maxHeigth_convPBR;
         }

         //////////

         std::vector<unsigned char> texRS_albedoOpa{};
         std::vector<unsigned char> texRS_specShininess{};

         //////////IN THE FUTURE IMPLEMENT AND SEE IF ALL THE TEXTURES SIZE ARE LOWER THAN DIFFUSE AND RESIZE TO CREATE ANOTHER RMA


         std::vector<unsigned char> texRS_Spec{};
         texRS_Spec.resize(maxSize_pixels);
         resizeTex(texRS_Spec, dataSpecular, specular_tex, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

         std::vector<unsigned char> texRS_Shininess{};
         texRS_Shininess.resize(maxSize_pixels);
         resizeTex(texRS_Shininess, dataShininess, shininess_tex, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

         combine_to_rgba(texRS_specShininess, dataSpecular.texturePixels, dataShininess.texturePixels, totalPixels_RMA);

         //////////////

         std::vector<unsigned char> texRS_Opa{};
         texRS_Opa.resize(maxSize_pixels);
         resizeTex(texRS_Opa, dataOpacity, opacity_tex, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

         combine_to_rgba(texRS_albedoOpa, dataDiffuse.texturePixels, dataOpacity.texturePixels, totalPixels_RMA);

         //////////////

         //std::vector<unsigned char> dataAlbedo{};
         //std::vector<unsigned char> dataRMA{}; combine_textures_D& texturesComb
         convert_to_PBR::convertTextures(texRS_albedoOpa, texRS_specShininess, totalPixels_RMA, texturesComb.dataAlbedo, texturesComb.dataRMA);

         if (!texturesComb.dataAlbedo.empty() && find_Albedo == false)
         {
            dataAlbedo_convPBR.texturePixels = texturesComb.dataAlbedo.data();
            dataAlbedo_convPBR.width = maxWidth_convPBR;
            dataAlbedo_convPBR.height = maxHeigth_convPBR;
            dataAlbedo_convPBR.nrChannels = 4;
            dataAlbedo_convPBR.nameTex = nameAlbedo;
            dataAlbedo_convPBR.key_texture = FNV_nameAlbedo;
            dataAlbedo_convPBR.status_tex = texStatus::NOT_LOADED;
            dataAlbedo_convPBR.type_mem_image = manager_GD::memType::STACK_HEAP_ENGINE;
         }

         else if (!texturesComb.dataAlbedo.empty() && find_Albedo == true)
         {
       //     dataAlbedo_convPBR = textures_saved[FNV_nameAlbedo];
            dataAlbedo_convPBR.status_tex = texStatus::LOADED;
         }

         if (!texturesComb.dataRMA.empty() && find_RMA == false)
         {
            dataRMA_convPBR.texturePixels = texturesComb.dataRMA.data();
            dataRMA_convPBR.width = maxWidth_convPBR;
            dataRMA_convPBR.height = maxHeigth_convPBR;
            dataRMA_convPBR.nrChannels = 4;
            dataRMA_convPBR.nameTex = nameRMA;
            dataRMA_convPBR.key_texture = FNV_nameRMA;
            dataRMA_convPBR.status_tex = texStatus::NOT_LOADED;
            dataRMA_convPBR.type_mem_image = manager_GD::memType::STACK_HEAP_ENGINE;
         }

         else if (!texturesComb.dataRMA.empty() && find_RMA == true)
         {
        //    dataRMA_convPBR = textures_saved[FNV_nameRMA];
            dataRMA_convPBR.status_tex = texStatus::LOADED;
         }

      }

      else if (find_RMA == true && find_Albedo == true)
      {
      // dataAlbedo_convPBR = textures_saved[FNV_nameAlbedo];
       dataAlbedo_convPBR.status_tex = texStatus::LOADED;
       //dataRMA_convPBR = textures_saved[FNV_nameRMA];
       dataRMA_convPBR.status_tex = texStatus::LOADED;
      }

     // saveTextures_to_KTX2(str_textures, dataAlbedo_convPBR, )

      return packPBR_texData(dataAlbedo_convPBR, dataNormals, dataRMA_convPBR, dataEmissive, dataHeight);
   }
   void loadTextures(aiMaterial* material, manager_AssimpData::textures_MaterialManager& hash_textures, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene)
   {
      std::string prefix_name {nameModel_Path + "_"};

      material_Status matS {testMaterial_PBR(material)};

      packPBR_texData textures_Data{};

      ////VECTOR OF TEXTURE ALBEDO AND RMA
      combine_textures_D texturesComb{};

      switch (matS)
      {
         case material_Status::MATERIAL_PBR :
         {
           textures_Data = loadTextures_PBR(material, nameModel_Path, directory, scene, prefix_name, texturesComb);
         }

         case material_Status::MATERIAL_LEGACY :
         {
           textures_Data = loadTextures_notPBR(material, nameModel_Path, directory, scene, prefix_name, texturesComb);
         }

         case material_Status::NOT_TEXTURES_MATERIAL :
         {


            return;
           ///HERE TO SAVE ONLY THE TEXTURES OPACITY OR HEIGHT
         }
      }

      /////CREATE THE FILE OF BIN DIRECTORY OUT OF THE FUNCTION
      std::string textures_binDirectory{directory + "assets_engine/texturesModels/" + nameModel_Path + "_textures"}; ///CREATE A BINARY DIRECTORY FROM THIS

      ////directory for KTX2 textures  ---------- hash of the texture
      //const std::string directory_TexturesKTX2{directory + nameModel_Path + "_textures"};

      ////create directory
      //bool exist_file{};
     // filesystem_manager::create_DirectoryFile(directory_TexturesKTX2, exist_file);

      convertTextures_KTX2(textures_Data);



      ////IF THE MATERIAL ARE NOT PBR


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

      customFiles::clear_spaceKey(nameModel_path);

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
       else
       {
        nameMat = nameModel_path + "mat" + std::to_string(i);
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
