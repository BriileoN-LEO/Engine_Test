//
// Created by brii on 12/05/26.
//

#include "manager_briiCooker.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "dataManager/convertion_DataManager.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "dataManager/algorithms_brii.h"
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
   const std::string pathTextures {"assets_engine/KTX_binTextures"};
   const std::string pathModels {"assets_engine/bin_Models_Meshes"};

   const std::string texture_binSign {".leotex"};
}

namespace manager_AssimpData
{
   mesh_D::mesh_D() = default;
   mesh_D::mesh_D(meshPack_Register& mesh_info, std::vector<vertex_D>& dataVertex, std::vector<unsigned int>& dataIndices) :  dataVertex(dataVertex), dataIndices(dataIndices), mesh_info(mesh_info){};
   mesh_D::mesh_D(mesh_D&& mesh_D) noexcept
   {
      mesh_info = mesh_D.get_meshPackRegister();
      dataVertex = mesh_D.get_dataVertex();
      dataIndices = mesh_D.get_dataIndices();
   } 
   mesh_D::mesh_D(mesh_D& mesh_D)
   {
      mesh_info = mesh_D.get_meshPackRegister();
      dataVertex = mesh_D.get_dataVertex();
      dataIndices = mesh_D.get_dataIndices();
      
   }
    mesh_D::mesh_D(const mesh_D& mesh_D)
   {
     mesh_info = mesh_D.mesh_info;
     dataVertex = mesh_D.dataVertex;
     dataIndices = mesh_D.dataIndices;
      
   }


   mesh_D::~mesh_D() { destroy(); };


   mesh_D mesh_D::operator=(mesh_D&& mD) noexcept
   {
     mesh_info = mD.get_meshPackRegister();
     dataVertex = mD.get_dataVertex();
     dataIndices = mD.get_dataIndices();

    return *this;
   }
   mesh_D mesh_D::operator=(mesh_D& mD)
   {
     mesh_info = mD.get_meshPackRegister();
     dataVertex = mD.get_dataVertex();
     dataIndices = mD.get_dataIndices();
    return *this;
   }
   mesh_D mesh_D::operator=(const mesh_D& mD)
   {
     mesh_info = mD.mesh_info;
     dataVertex = mD.dataVertex;
     dataIndices = mD.dataIndices;
     return *this;
   }

   void mesh_D::insert_headerMesh(meshPack_Register& hM)
   { 
    mesh_info = hM;
   }
   
   void mesh_D::insert_nameMesh(std::string& nameMesh)
   {
     constexpr size_t size_nameMesh{data_meshCore::get_maxSize_nameMeshArray()};
     char (&array_nameMesh)[size_nameMesh] = mesh_info.get_nameMesh();
     convert_str::conv_str_to_rawArray<size_nameMesh>(nameMesh, array_nameMesh);
   }
 
   void mesh_D::insert_meshID(uint64_t& meshID)
   {
    mesh_info.meshID = meshID;
   }
   void mesh_D::insert_materialID(uint64_t& material_ID)
   {
     mesh_info.material_ID = material_ID;
   }

   void mesh_D::insert_vertexD(float position[3], float normal[3], float uv[2])
   {
     dataVertex.emplace_back(position, normal, uv);
     mesh_info.vertexCount++;
   }
   void mesh_D::setSize_VertexContainer(int size)
   {
    dataVertex.resize(size);
   }

   void mesh_D::update_size_vertexD()
   {
      mesh_info.vertexCount = dataVertex.size();
   };

   void mesh_D::insert_Indice(unsigned int indice)
   {
      dataIndices.emplace_back(indice);
      mesh_info.indexCount++;
   }
   void mesh_D::setSize_IndicesContainer(int size)
   {
     dataIndices.resize(size);
   }
   void mesh_D::update_size_indices()
   {
     mesh_info.indexCount = dataIndices.size();
   }

   void mesh_D::insert_mat4Transformation(float (&mat4_array)[16])
   {
      convert_dataTypes::copy_rawArrayData<float, 16>(mat4_array, mesh_info.mesh_transformation);
   }

  const uint64_t& mesh_D::get_meshID()
  {
   return mesh_info.meshID;
  }

  meshPack_Register& mesh_D::get_meshPackRegister()   
  {
      return mesh_info;
  }
  std::vector<vertex_D>& mesh_D::get_dataVertex()
   {
     return dataVertex;
   }
  std::vector<unsigned int>& mesh_D::get_dataIndices()
   {
     return dataIndices;
   }

   void mesh_D::destroy()
   {
      dataVertex.clear();
      dataIndices.clear();
   }

   meshesBin_D::meshesBin_D() = default;
   meshesBin_D::meshesBin_D(const meshesBin_D&& mBD) noexcept
   {
    headerMesh = mBD.headerMesh;
    ID_mesh = mBD.ID_mesh;
    pos_by_ID = mBD.pos_by_ID;
    meshes_data = mBD.meshes_data; 
   }

   meshesBin_D::meshesBin_D(const meshesBin_D& mBD)
   {
    headerMesh = mBD.headerMesh;
    ID_mesh = mBD.ID_mesh;
    pos_by_ID = mBD.pos_by_ID;
    meshes_data = mBD.meshes_data; 
   } 

   void meshesBin_D::insert_nameMeshBin(std::string& nameMesh)
   {
     ////implement here conv_str_to_rawArray
    constexpr size_t size_nameHeaderMesh { data_meshCore::get_maxSize_meshHeaderArray() };
    char (&mesh_nameHeader)[size_nameHeaderMesh] = headerMesh.get_nameMeshBin();
    convert_str::conv_str_to_rawArray<size_nameHeaderMesh>(nameMesh, mesh_nameHeader); 
   }
   void meshesBin_D::insert_version(uint32_t version)
   {
    headerMesh.version = version; 
   }
   void meshesBin_D::update_meshesCount()
   {
    headerMesh.meshesCount = meshes_data.size();
   }

   float (&meshesBin_D::get_generalMatrixTrans()) [16]
   {   
    return headerMesh.get_generalMatTrans();
   }

   void meshesBin_D::insert_Mesh(mesh_D& mD)
   { 
     uint64_t meshID {mD.get_meshID()};
    
     std_vectorManager::insert_sorted_order<uint64_t>(ID_mesh, mD.get_meshID());
     pos_by_ID.emplace(meshID, meshes_data.size());
     meshes_data.emplace_back(mD); 
     headerMesh.meshesCount++;
     ////

   }
   void meshesBin_D::upload_Meshes_bin(const std::string& directory, file_OP::writeFlags& fileT)
   {
    std::vector<data_meshF> mD{};
    mD.resize(headerMesh.meshesCount);
    
    for(uint64_t i = 0; i < headerMesh.meshesCount; i++)
    {
     mD[i].insert_NewData_ptr
      (
      meshes_data[i].get_dataVertex(),
      meshes_data[i].get_dataIndices(),
      meshes_data[i].get_meshPackRegister()
      );
    } 

    writeFile_MeshMaterial(headerMesh, mD, directory, fileT); ///CHECK FOR IMPLEMENTATION 

    for(auto& meshD : mD)
    {
     meshD.destroy();
    }
    mD.clear();  ///////REVISIT THIS if not cause anything 
    
   }

   //void meshesBin_D::upload_SelectedMesh_byID(uint64_t ID_mesh);   ///DON'T WORRIED IF THE HEADER IS THE SAME, IS THE PURPOSE 

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

   entity_MeshManager::entity_MeshManager(){};
   void entity_MeshManager::insert_MeshBinD_ref(meshesBin_D& meshBinD)
   {
    meshesBin_D newMesh(meshBinD);
    meshesBin_data.emplace_back(newMesh);
   }
   void entity_MeshManager::upload_allMeshesBin(const std::string& directory, file_OP::writeFlags& fileT)
   {
    for(int i = 0; i < static_cast<int>(meshesBin_data.size()); i++)
    {
     meshesBin_data[i].upload_Meshes_bin(directory, fileT); 
    }
   }

   model_D::model_D() = default;
   model_D::model_D(const model_D&& mD) noexcept
   {
    headerModel = mD.headerModel;
    meshesRegister_LeoMesh = mD.meshesRegister_LeoMesh;
   }
   model_D::model_D(const model_D& mD)
   {
    headerModel = mD.headerModel;
    meshesRegister_LeoMesh = mD.meshesRegister_LeoMesh;
   }

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
   void model_D::insert_headerModel(model_LeoHeader& headerModel)
   {
     this->headerModel = headerModel;
   }

   void model_D::insert_nameHeader(std::string& nameHeader) ///THE FIRST SPACE [0], DOESNT HAVE '\0' AN EMPTY SPACE
   {
    constexpr size_t size_nameModel { data_modelCore::get_maxSize_nameModelArray() };
    char (&name_HeaderModel)[size_nameModel] = headerModel.get_modelName();
    convert_str::conv_str_to_rawArray<size_nameModel>(nameHeader, name_HeaderModel);
   }
   void model_D::insert_version(uint32_t version)
   {
    headerModel.version = version;
   }
   void model_D::insert_MeshID(uint64_t& mesh_ID)
   {
     meshesRegister_LeoMesh.emplace_back(mesh_ID);
   }
   void model_D::update_MeshCounter()
   {
    headerModel.meshesCount = meshesRegister_LeoMesh.size();
   }

   std::string model_D::get_nameModel_str()
   {
    std::string name(headerModel.get_modelName());
    return name; 
   }
   void model_D::destroy()
   {
     meshesRegister_LeoMesh.clear();
   }

   entity_ModelManager::entity_ModelManager(){};
   void entity_ModelManager::insert_ModelBinD_ref(model_D& model)
   {
    modelsBin_data.emplace_back(model);
   }
   void entity_ModelManager::upload_allModelBin()
   {
     
   }

}

namespace manage_texturesCooker
{

   data_image::data_image(){};
   data_image::data_image(data_image&& iR) noexcept
   {
      if (texture_KTX2 != nullptr)
      {
         texture_KTX2.reset();
         texture_KTX2 = nullptr;
      }

      texture_KTX2 = std::move(iR.texture_KTX2);
      iR.texture_KTX2 = nullptr;

      if (texturePixels != nullptr)
      {
         freeMemoryImage(type_mem_image, texturePixels);
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
   }
   data_image::data_image(data_image& iR)
   {
      if (texture_KTX2 != nullptr)
      {
         texture_KTX2.reset();
         texture_KTX2 = nullptr;
      }

      texture_KTX2 = std::move(iR.texture_KTX2);
      iR.texture_KTX2 = nullptr;

      if (texturePixels != nullptr)
      {
         freeMemoryImage(type_mem_image, texturePixels);
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
   }
   data_image data_image::operator<<(data_image& iR)
   {
      if (texture_KTX2 != nullptr)
      {
         texture_KTX2.reset();
         texture_KTX2 = nullptr;
      }

      texture_KTX2 = std::move(iR.texture_KTX2);
      iR.texture_KTX2 = nullptr;

      if (texturePixels != nullptr)
      {
         freeMemoryImage(type_mem_image, texturePixels);
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
   data_image::~data_image()
   {
      clear();
   }


   void data_image::clear()
   {
      //ktxTexture2_Destroy(texture_KTX2);
      texture_KTX2.reset();
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
   packPBR_texData::packPBR_texData(packPBR_texData&& pack_T) noexcept
   {
      albedo_data << pack_T.albedo_data;
      normal_data << pack_T.normal_data;
      RMA_data << pack_T.RMA_data;
      emissive_data << pack_T.emissive_data;
      height_data << pack_T.height_data;
   }
   packPBR_texData::packPBR_texData(packPBR_texData& pack_T)
   {
      albedo_data << pack_T.albedo_data;
      normal_data << pack_T.normal_data;
      RMA_data << pack_T.RMA_data;
      emissive_data << pack_T.emissive_data;
      height_data << pack_T.height_data;
   }
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
      unsigned char* val = value;
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
    
     pixel_color_state func_roughness{statePixelColor_NULL};
     pixel_color_state func_metallic{statePixelColor_NULL};
     pixel_color_state func_AO{statePixelColor_NULL};
   
     if(roughnessData != nullptr)
     {
       func_roughness = statePixelColor;
     }
     
     if(metallicData != nullptr)
     { 
       func_metallic = statePixelColor;
     }
    
     if(ambientOclussionData != nullptr)
     { 
       func_AO = statePixelColor;
     }

     for (int i = 0; i < totalPixels; ++i)
     {
        index = i * 4;

        func_roughness(RMA, index, roughnessData, i);
        func_metallic(RMA, index + 1, metallicData, i);
        func_AO(RMA, index + 2, ambientOclussionData, i);
        RMA[index + 3] = static_cast<unsigned char>(255.0f);

        //RMA[index] = roughnessData ? roughnessData[i] : static_cast<unsigned char>(255.0f);
        //RMA[index + 1] = metallicData ? metallicData[i] : static_cast<unsigned char>(255.0f);
        //RMA[index + 2] = ambientOclussionData ? ambientOclussionData[i] : static_cast<unsigned char>(255.0f);
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

      bool compressPixels_UASTC(data_image& texture, ktxTexture2* texKTX2, ktx_bool_t normalMap)
      {
        ktxBasisParams params{};
        params.structSize = sizeof(params);
        params.codec = 2;
        params.uastcFlags = KTX_PACK_UASTC_LEVEL_FASTEST;

        params.threadCount = 4;
        params.verbose = KTX_FALSE;

        params.normalMap = normalMap;

        KTX_error_code result {ktxTexture2_CompressBasisEx(texKTX2, &params)};

        if (result == KTX_SUCCESS)
        {
          std::cout << "COMPRESS_UASTC KTX2 TEXTURE:: TEXTURE --->" + texture.nameTex << "\n";
          return true;
        }

        std::cerr << "ERROR::COMPRESS_UASTC KTX2 TEXTURE:: TEXTURE--->" + texture.nameTex << ktxErrorString(result) <<"\n";
        return false;
      }

      bool inyectPixels(data_image& texture, ktxTexture2* texKTX2)
      {
        size_t image_size = texture.width * texture.height * texture.nrChannels;

        KTX_error_code result {ktxTexture_SetImageFromMemory(ktxTexture(texKTX2), 0, 0, 0, texture.texturePixels, image_size)};

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

        ktxTexture2* tex{ nullptr };

        KTX_error_code result {ktxTexture2_Create(&createInfo_KTX2, KTX_TEXTURE_CREATE_ALLOC_STORAGE, &tex)};

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
         success_tex = inyectPixels(texture, tex);
         if (success_tex == false)
         {
           texture.clear();
          // ktxTexture2_Destroy(texture.texture_KTX2);
          // texture.texture_KTX2 = nullptr;
           return;
         }

         //FASE 3 --- COMPRESS TO UASTC PIXELS
         success_tex = compressPixels_UASTC(texture, tex, normalMap);
         if (success_tex == false)
         {
            texture.clear();
            return;
         }

         ///TRANSFERE THE POINTER
         texture.texture_KTX2.reset(tex);
         tex = nullptr;

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
          data_Tex.type_mem_image = manager_GD::memType::STBI_MEM;

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

          if (!std::binary_search(textures_saved.begin(), textures_saved.end(), data_Tex.key_texture))
          {
             std::cout << "PROCESS::DIRECTORY_TEXTURE:: NAME_MODEL(" << prefix_nameModel << ")" << " :: MATERIAL_TYPE(" << nameTextures[matType] << ")::" << std::filesystem::path(pathTex).stem().string() << std::endl;

             std::string pathTexture{directory + pathTex};
             data_Tex.texturePixels = stbi_load(pathTexture.c_str(), &data_Tex.width, &data_Tex.height, &data_Tex.nrChannels, numChannels_obj);

             data_Tex.type_mem_image = manager_GD::memType::STBI_MEM;

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

          if (!std::binary_search(textures_saved.begin(), textures_saved.end(), data_Tex.key_texture))
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
               dataMem = nullptr;
               break;
            case manager_GD::memType::HEAP_ENGINE :
               free(dataMem);
               dataMem = nullptr;
               break;
            case manager_GD::memType::STACK_HEAP_ENGINE :
               dataMem = nullptr;
               break;
            case manager_GD::memType::NOT_MEM :
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

   loadTexture_Memory packTexKTX2_memory(data_image& tex_data)
   {

      loadTexture_Memory packT{};

      packT.name = tex_data.nameTex;
      packT.texData = std::move(tex_data.texture_KTX2);
      tex_data.texture_KTX2 = nullptr;
      std::strncpy(packT.dataTex.textureName, tex_data.nameTex.c_str(), MAX_SIZE_STR_BIN_TEXTURE - 1);
      packT.dataTex.textureName[MAX_SIZE_STR_BIN_TEXTURE - 1] = '\0';
      packT.dataTex.textureID = tex_data.key_texture;
      packT.dataTex.width = tex_data.width;
      packT.dataTex.height = tex_data.height;
      packT.dataTex.nrChannels = tex_data.nrChannels;
         /////////CONTINUE HERE TO PACK ALL THE TEXTURES AND THE DATA FROM DATA_IMAGE TO LOADTEXTURE_MEMORY
         ///SEE IF I HAVE AN ERROR WHEN I LOAD A TEXTURES, I COULD FIND A WAY THAT MARK THE texStatus::NOT_EXIST or make texStatus::ERROR to not upload
         ///the texture in the material
     return packT;
   }

   void packTextures_KTX2_bin(packPBR_texData& textures_Data, const std::string& directory)
   {
      //////
      std::vector<loadTexture_Memory> textures_pack{};
      std::vector<data_image*> textures_D_ptr{};
      textures_D_ptr.resize(5);

      /////pack all the textures
      if (textures_Data.albedo_data.status_tex == texStatus::NOT_LOADED)
      {
        textures_pack.emplace_back(packTexKTX2_memory(textures_Data.albedo_data));  ////REVIEW
        textures_D_ptr.emplace_back(&textures_Data.albedo_data);
      }

      if (textures_Data.normal_data.status_tex == texStatus::NOT_LOADED)
      {
         textures_pack.emplace_back(packTexKTX2_memory(textures_Data.normal_data));  ////REVIEW
         textures_D_ptr.emplace_back(&textures_Data.normal_data);
      }

      if (textures_Data.RMA_data.status_tex == texStatus::NOT_LOADED)
      {
         textures_pack.emplace_back(packTexKTX2_memory(textures_Data.RMA_data));  ////REVIEW
         textures_D_ptr.emplace_back(&textures_Data.RMA_data);
      }

      if (textures_Data.emissive_data.status_tex == texStatus::NOT_LOADED)
      {
         textures_pack.emplace_back(packTexKTX2_memory(textures_Data.emissive_data));  ////REVIEW
         textures_D_ptr.emplace_back(&textures_Data.emissive_data);
      }

      if (textures_Data.height_data.status_tex == texStatus::NOT_LOADED)
      {
         textures_pack.emplace_back(packTexKTX2_memory(textures_Data.height_data));  ////REVIEW
         textures_D_ptr.emplace_back(&textures_Data.height_data);
      }

      std::vector<int> p_texNotLoaded{};

      uint32_t packB {pack_binTextures_KTX2(textures_pack, directory, 1, p_texNotLoaded)};

      if (packB == 1)
      {
         for (auto& tex_P : p_texNotLoaded)
         {
            textures_D_ptr[tex_P]->status_tex = texStatus::LOADED;

            //////INSERT THE TEXTURE THAT WAS SAVED IN THE BINARY
            auto posI = std::lower_bound(textures_saved.begin(), textures_saved.end(), textures_D_ptr[tex_P]->key_texture);
            textures_saved.insert(posI, textures_D_ptr[tex_P]->key_texture);
         }
      }

      for (int i = 0; i < static_cast<int>(textures_D_ptr.size()); i++)
      {
         textures_D_ptr[i] = nullptr;
         delete textures_D_ptr[i];
      }

      textures_D_ptr.clear();

      ////the result of texStatus needs to reflected after save textures in binary, if its loaded or not

   }

   void resizeTex(std::vector<unsigned char>& newDataImage, data_image& texData, const int& maxHeight, const int& maxWidth, resizeType rT)
   {
      if (texData.status_tex == texStatus::NOT_LOADED)
      {
        int pass_tex { (maxHeight != 0) && (maxWidth != 0) ? 1 : 0 }; //// 0 == ANY EQUAL TO ZERO
        pass_tex = (pass_tex ^ (!(texData.height ^ maxHeight) & !(texData.width ^ maxWidth))); /// 0 == NOT EQUAL || 1 == EQUALS
        
         if (pass_tex == 0)
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

   void assign_texturesID(manager_AssimpData::textures_MaterialManager& hash_textures, packPBR_texData& textures_Data)
   {
      auto func_TextTex = [](uint64_t& id_take, uint64_t& id_share)
      {
       id_take = id_share;
      };
      auto empty_T = [](uint64_t& id_take, uint64_t& id_share){id_take = 0; uint64_t& var = id_share; var = id_share;};

      std::vector<funcPtr::funcDouble_uint64_t> func_ID{};
      func_ID.resize(2);
      func_ID[0] = empty_T;
      func_ID[1] = func_TextTex;

      /// 0 == NOT SAME  -------- 1 == SAME
      int exist_albedo {!(static_cast<uint32_t>(textures_Data.albedo_data.status_tex) ^ static_cast<uint32_t>(texStatus::LOADED))};
      func_ID[exist_albedo](hash_textures.Albedo_hash, textures_Data.albedo_data.key_texture);
      textures_Data.albedo_data.clear();

      int exist_normal {!(static_cast<uint32_t>(textures_Data.normal_data.status_tex) ^ static_cast<uint32_t>(texStatus::LOADED))};
      func_ID[exist_normal](hash_textures.Normals_hash, textures_Data.normal_data.key_texture);
      textures_Data.albedo_data.clear();

      int exist_RMA {!(static_cast<uint32_t>(textures_Data.RMA_data.status_tex) ^ static_cast<uint32_t>(texStatus::LOADED))};
      func_ID[exist_RMA](hash_textures.RMA_hash, textures_Data.RMA_data.key_texture);
      textures_Data.albedo_data.clear();

      int exist_height {!(static_cast<uint32_t>(textures_Data.height_data.status_tex) ^ static_cast<uint32_t>(texStatus::LOADED))};
      func_ID[exist_height](hash_textures.Height_hash, textures_Data.height_data.key_texture);
      textures_Data.albedo_data.clear();

      int exist_emission {!(static_cast<uint32_t>(textures_Data.emissive_data.status_tex) ^ static_cast<uint32_t>(texStatus::LOADED))};
      func_ID[exist_emission](hash_textures.Emission_hash, textures_Data.emissive_data.key_texture);
      textures_Data.albedo_data.clear();
   }

   packPBR_texData loadTextures_PBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb)
   {
      data_image dataOpacity{};
      texStatus opacity_tex{processTexture_pixels(material, aiTextureType_OPACITY, scene, prefixName, directory, dataOpacity, 4)};
      dataOpacity.status_tex = opacity_tex;
    
      //////HEIGHT MAP SECTION
      data_image dataHeight{};
      texStatus height_tex {processTexture_pixels(material, aiTextureType_HEIGHT, scene, prefixName, directory, dataHeight, 4)};

      if (height_tex == texStatus::NOT_EXISTS)
      {
         height_tex = processTexture_pixels(material, aiTextureType_DISPLACEMENT, scene, prefixName, directory, dataHeight, 4);
      }
      dataHeight.status_tex = height_tex;

      data_image dataAlbedo{};
      texStatus albedo_tex {processTexture_pixels(material, aiTextureType_BASE_COLOR, scene, prefixName, directory, dataAlbedo, 4)};
      dataAlbedo.status_tex = albedo_tex;

      data_image dataNormalCamera{};
      texStatus normalCamera_tex {processTexture_pixels(material, aiTextureType_NORMAL_CAMERA, scene, prefixName, directory, dataNormalCamera, 4)};
      dataNormalCamera.status_tex = normalCamera_tex;

      data_image dataEmission{};
      texStatus emission_tex {processTexture_pixels(material, aiTextureType_EMISSION_COLOR, scene, prefixName, directory, dataEmission, 4)};
      dataEmission.status_tex = emission_tex;

      data_image dataMetalness{};
      texStatus metalness_tex {processTexture_pixels(material, aiTextureType_METALNESS, scene, prefixName, directory, dataMetalness, 4)};
      dataMetalness.status_tex = metalness_tex;

      data_image dataDiffuseRoughness{};
      texStatus diffuseRoughness_tex {processTexture_pixels(material, aiTextureType_DIFFUSE_ROUGHNESS, scene, prefixName, directory, dataDiffuseRoughness, 4)};
      dataDiffuseRoughness.status_tex = diffuseRoughness_tex;

      data_image dataAO{};
      texStatus ao_tex {processTexture_pixels(material, aiTextureType_AMBIENT_OCCLUSION, scene, prefixName, directory, dataAO, 4)};
      dataAO.status_tex = ao_tex;


      data_image dataRMA{};
      data_image dataAlbedoOpa{};

     int exist_RMA { 
         (dataMetalness.status_tex != texStatus::NOT_EXISTS ||
         dataDiffuseRoughness.status_tex != texStatus::NOT_EXISTS ||
         dataAO.status_tex != texStatus::NOT_EXISTS) 
         ? 1 : 0 
     };

     int exist_AlbedoOpa {
         (dataAlbedo.status_tex != texStatus::NOT_EXISTS || 
          dataOpacity.status_tex != texStatus::NOT_EXISTS) 
         ? 1 : 0 
     };

     
      int maxSize_pixels{};
      int totalPixels{};

      int maxHeigth_convPBR{};
      int maxWidth_convPBR{};

      if (albedo_tex != texStatus::NOT_EXISTS)
      {
         maxHeigth_convPBR = dataAlbedo.height;
         maxWidth_convPBR = dataAlbedo.width;

         maxSize_pixels = dataAlbedo.width * dataAlbedo.height * 4;

         totalPixels = dataAlbedo.width * dataAlbedo.height;
      }

      else if (albedo_tex == texStatus::NOT_EXISTS || exist_RMA == 1 || exist_AlbedoOpa == 1)  ///IF DIFFUSE NOT EXISTS
      {
         maxHeigth_convPBR = dataDiffuseRoughness.height >= dataMetalness.height ? dataDiffuseRoughness.height : dataMetalness.height;  ////THIS PREVENTS
         maxHeigth_convPBR = maxHeigth_convPBR >= dataOpacity.height ? maxHeigth_convPBR : dataOpacity.height;

         maxWidth_convPBR = dataDiffuseRoughness.width >= dataMetalness.width ? dataDiffuseRoughness.width : dataMetalness.width;  ////THIS PREVENTS
         maxWidth_convPBR = maxWidth_convPBR >= dataOpacity.width ? maxWidth_convPBR : dataOpacity.width;

         maxSize_pixels = maxWidth_convPBR * maxHeigth_convPBR * 4;
         totalPixels = maxWidth_convPBR * maxHeigth_convPBR;
      }

      //auto find_TexRMA {textures_saved.find(FNV_nameRMA)};
      if(exist_RMA == 1)
   {
          ////THIS TO IDENTIFY IF RMA EXISTS
      std::string nameRMA {"RMA_" + dataMetalness.nameTex + "_" + dataDiffuseRoughness.nameTex + "_" + dataAO.nameTex + "_" + nameModel_Path};
      customFiles::standard_textureNameKTX(nameRMA);
      // uint32_t FNV_nameRMA{FNV::str_to_hash(nameRMA)};
      std::string str_keyTexture_RMA {directory + nameModel_Path + "_textures" + "/" + nameRMA + ".ktx2"};
      uint64_t FNV_nameRMA{ FNV::hash_1a(str_keyTexture_RMA)};

      auto find_RMA {search_algorithms::binary_search_OP_int<uint64_t>(textures_saved, FNV_nameRMA)};

      if (find_RMA == -1)
         {

            std::vector<unsigned char> newDataMetalness{};
            newDataMetalness.resize(maxSize_pixels);  ////////ALWAYS 4
            resizeTex(newDataMetalness, dataMetalness, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

            std::vector<unsigned char> newDataRoughness{};
            newDataRoughness.resize(maxSize_pixels);
            resizeTex(newDataRoughness, dataDiffuseRoughness, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

            std::vector<unsigned char> newDataAO{};
            newDataAO.resize(maxSize_pixels);
            resizeTex(newDataAO, dataAO, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

            convert_RMA(texturesComb.dataRMA, dataDiffuseRoughness.texturePixels, dataMetalness.texturePixels, dataAO.texturePixels, totalPixels);
            dataRMA.width = maxWidth_convPBR;
            dataRMA.height = maxHeigth_convPBR;
            dataRMA.nrChannels = 4;
            dataRMA.nameTex = nameRMA;
            dataRMA.key_texture = FNV_nameRMA;
            dataRMA.texturePixels = texturesComb.dataRMA.data();
            dataRMA.status_tex = texStatus::NOT_LOADED;   
            dataRMA.type_mem_image = manager_GD::memType::STACK_HEAP_ENGINE;

            dataDiffuseRoughness.clear();
            dataMetalness.clear();
            dataAO.clear();
            /////LIBERATE MEMORY OF ALL TEXTURES RMA OF EACH SEPARATED, BECAUSE I CREATE THE KTX2 TEXTURE
          }

          else
          {
            dataRMA.status_tex = texStatus::LOADED;
            dataRMA.key_texture = FNV_nameRMA;
          }
    }

   if(exist_AlbedoOpa == 1)
   {
      std::string nameAlbedo{"Albedo_" + dataAlbedo.nameTex + "_" + dataOpacity.nameTex + "_" + prefixName};
      customFiles::standard_textureNameKTX(nameAlbedo);
      std::string str_keyTexture_Albedo {directory + nameModel_Path + "_textures" + "/" + nameAlbedo + ".ktx2"};
      uint64_t FNV_nameAlbedo{FNV::hash_1a(str_keyTexture_Albedo)};

      auto find_Albedo {search_algorithms::binary_search_OP_int<uint64_t>(textures_saved, FNV_nameAlbedo)};

        if (find_Albedo == -1)
         {
            if (opacity_tex != texStatus::NOT_EXISTS || albedo_tex != texStatus::NOT_EXISTS)
            {
             
             std::vector<unsigned char> newDataOpacity{};
             newDataOpacity.resize(maxSize_pixels);
             resizeTex(texturesComb.dataAlbedo, dataAlbedoOpa, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);
            
             combine_to_rgba(texturesComb.dataAlbedo, dataAlbedo.texturePixels, dataOpacity.texturePixels, totalPixels); 
             dataAlbedoOpa.texturePixels = texturesComb.dataAlbedo.data();
             dataAlbedoOpa.width = maxWidth_convPBR;
             dataAlbedoOpa.height = maxHeigth_convPBR;
             dataAlbedoOpa.nrChannels = 4;
             dataAlbedoOpa.nameTex = nameAlbedo;
             dataAlbedoOpa.key_texture = FNV_nameAlbedo;
             dataAlbedoOpa.status_tex = texStatus::NOT_LOADED;
             dataAlbedoOpa.type_mem_image = manager_GD::memType::STACK_HEAP_ENGINE;
             
            dataAlbedo.clear();
            dataOpacity.clear();
            }
         }
       else
         {
          dataAlbedoOpa.status_tex = texStatus::LOADED;
          dataAlbedoOpa.key_texture = FNV_nameAlbedo;
         }
    }

     return packPBR_texData(dataAlbedoOpa, dataNormalCamera, dataRMA, dataEmission, dataHeight);
   }

   packPBR_texData loadTextures_notPBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb)
   {
      data_image dataOpacity{};
      texStatus opacity_tex{processTexture_pixels(material, aiTextureType_OPACITY, scene, prefixName, directory, dataOpacity, 4)};
      dataOpacity.status_tex = opacity_tex;
    
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
      texStatus diffuse_tex = processTexture_pixels(material, aiTextureType_DIFFUSE, scene, prefixName, directory, dataDiffuse, 4);
      dataDiffuse.status_tex = diffuse_tex;
    
      data_image dataSpecular{};
      texStatus specular_tex = processTexture_pixels(material, aiTextureType_SPECULAR, scene, prefixName, directory, dataSpecular, 4);
      dataSpecular.status_tex = specular_tex;

      data_image dataShininess{};
      texStatus shininess_tex = processTexture_pixels(material, aiTextureType_SHININESS, scene, prefixName, directory, dataShininess, 4);
      dataShininess.status_tex = shininess_tex;

      data_image dataNormals{};
      texStatus normals_tex = processTexture_pixels(material, aiTextureType_NORMALS, scene, prefixName, directory, dataNormals, 4);
      dataNormals.status_tex = normals_tex;

      data_image dataEmissive{};
      texStatus emissive_tex = processTexture_pixels(material, aiTextureType_EMISSIVE, scene, prefixName, directory, dataEmissive, 4);
      dataEmissive.status_tex = emissive_tex;

      data_image dataAlbedo_convPBR{};
      data_image dataRMA_convPBR{};

      std::string nameRMA {"RMA_" + dataSpecular.nameTex + "_" + dataShininess.nameTex + "_AO_" + prefixName};
      customFiles::standard_textureNameKTX(nameRMA);
      std::string str_keyTexture_RMA {directory + nameModel_Path + "_textures" + "/" + nameRMA + ".ktx2"};
      uint64_t FNV_nameRMA{ FNV::hash_1a(str_keyTexture_RMA)};

      std::string nameAlbedo{"Albedo_" + dataDiffuse.nameTex + "_" + dataOpacity.nameTex + "_" + prefixName};
      customFiles::standard_textureNameKTX(nameAlbedo);
      std::string str_keyTexture_Albedo {directory + nameModel_Path + "_textures" + "/" + nameAlbedo + ".ktx2"};
      uint64_t FNV_nameAlbedo{FNV::hash_1a(str_keyTexture_Albedo)};


      //auto find_RMA {textures_saved.find(FNV_nameRMA)};
      //auto find_Albedo {textures_saved.find(FNV_nameAlbedo)};
      bool find_RMA {std::binary_search(textures_saved.begin(), textures_saved.end(), FNV_nameRMA)};
      bool find_Albedo {std::binary_search(textures_saved.begin(), textures_saved.end(), FNV_nameAlbedo)};  ////CHANGE THIS BINARY SEARCH

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
         resizeTex(texRS_Spec, dataSpecular, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

         std::vector<unsigned char> texRS_Shininess{};
         texRS_Shininess.resize(maxSize_pixels);
         resizeTex(texRS_Shininess, dataShininess, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

         combine_to_rgba(texRS_specShininess, dataSpecular.texturePixels, dataShininess.texturePixels, totalPixels_RMA);

         //////////////

         std::vector<unsigned char> texRS_Opa{};
         texRS_Opa.resize(maxSize_pixels);
         resizeTex(texRS_Opa, dataOpacity, maxHeigth_convPBR, maxWidth_convPBR, resizeType::LINEAR);

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
            dataAlbedo_convPBR.key_texture = FNV_nameAlbedo;
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
            dataRMA_convPBR.key_texture = FNV_nameRMA;
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
           packPBR_texData pT {loadTextures_PBR(material, nameModel_Path, directory, scene, prefix_name, texturesComb)};
           textures_Data << pT;
         }

         case material_Status::MATERIAL_LEGACY :
         {
           packPBR_texData pT_L {loadTextures_notPBR(material, nameModel_Path, directory, scene, prefix_name, texturesComb)};
           textures_Data << pT_L;
         }

         case material_Status::NOT_TEXTURES_MATERIAL :
         {
            return;
           ///HERE TO SAVE ONLY THE TEXTURES OPACITY OR HEIGHT
         }
      }

      /////CREATE THE FILE OF BIN DIRECTORY OUT OF THE FUNCTION
      std::string textures_binDirectory{ manager_GD::pathTextures + "/" + nameModel_Path + "_textures" + manager_GD::texture_binSign }; ///CREATE A BINARY DIRECTORY FROM THIS

      ////directory for KTX2 textures  ---------- hash of the texture
      //const std::string directory_TexturesKTX2{directory + nameModel_Path + "_textures"};

      ////create directory
      //bool exist_file{};
     // filesystem_manager::create_DirectoryFile(directory_TexturesKTX2, exist_file);

      convertTextures_KTX2(textures_Data);

      packTextures_KTX2_bin(textures_Data, textures_binDirectory);

      assign_texturesID(hash_textures, textures_Data);

      ////CONTINUE HERE, MAKE A FUNCTION TO SAVE ALL THE ID TEXTURES FROM textures_Data to hash_textures


      ////IF THE MATERIAL ARE NOT PBR


      //////CONTINUE HERE TO MAKE THE CONVERTION TO PBR

      /////////MAKE A COMPARATION OF EACH SIZE OF THE TEXTURE, THEN CALCULATE WHAT IS THE MAX SIZE OF EACH TEXTURE

      ///THINGS TO DO
      ///
      ///- CONVERT THE TEXTURES TO PBR IF THE DONT ARE PBR
      ///- COMBINE THE TEXTURES ALBEDO WITH OPACITY, AND THE TEXTURE RMA [ROUGHNESS] [METALLIC] [AMBIENT OCLUSION]


   }

}

namespace manage_materialCooker
{
   material_D::material_D() = default;
   material_D::material_D(material_LeoHeader& dataHeader_Mat, std::vector<matPack_data_register>& materials) :  dataHeader_Mat(dataHeader_Mat), materials(materials) 
   {

     this->dataHeader_Mat = dataHeader_Mat;
     for(int i = 0; i < static_cast<int>(materials.size()); i++)
    {
     pos_by_ID.emplace(materials[i].materialID, i);

     std_vectorManager::insert_sorted_order<uint64_t>(ID_materials, materials[i].materialID);
     //auto find_p {std::lower_bound(ID_materials.begin(), ID_materials.end(), materials[i].materialID)};
     //ID_materials.emplace(find_p, materials[i].materialID); 
    }
     
   };


   material_D::material_D(material_D&& matC) noexcept
   {  
    dataHeader_Mat = matC.dataHeader_Mat;
    materials = matC.materials;
    pos_by_ID = matC.pos_by_ID;
    ID_materials = matC.ID_materials;
      ///COMPLETE HERE WITH VECTOR ID'S
   }
   material_D::material_D(material_D& matC)
   {
    dataHeader_Mat = matC.dataHeader_Mat;
    materials = matC.materials;
    pos_by_ID = matC.pos_by_ID;  
    ID_materials = matC.ID_materials; 
     ///COMPLETE HERE WITH VECTOR ID'S
   }

   const uint64_t& material_D::get_matBin_ID()
   { 
     return dataHeader_Mat.material_bin_ID;
   }

   uint32_t material_D::mat_contains(uint64_t ID)
   {
     return search_algorithms::binary_search_contains<uint64_t>(ID_materials, ID); 
   }

   matPack_data_register& material_D::get_mat_directly(uint64_t ID)
   {
    return materials[pos_by_ID[ID]];
   }
   
   const material_LeoHeader& material_D::get_const_header()
   {
    return dataHeader_Mat;
   }

   uint32_t material_D::packBinary(const std::string& outDir, file_OP::writeFlags& fileT)
   {
    return writeFile_binMaterial(dataHeader_Mat, materials, outDir, fileT) ;
   }

   entity_MaterialManager::entity_MaterialManager() = default;
   
   void entity_MaterialManager::insertMat(material_D& mat)
   {  
      pos_by_ID.emplace(mat.get_matBin_ID(), static_cast<uint64_t>(mat_data.size()));
      mat_data.emplace_back(mat);
      std_vectorManager::insert_sorted_order<uint64_t>(ID_matBin, mat.get_matBin_ID()); 
   }

   data_MatCore::matPack_ptr entity_MaterialManager::get_mat_by_ID(uint64_t ID)
   {
     data_MatCore::matPack_ptr ptr_matPack{nullptr};
     uint32_t mContain{};
     
     for(auto& matBin : mat_data)
      { 
        mContain = matBin.mat_contains(ID);
        
        if(mContain == 1)
        {
           ptr_matPack = &matBin.get_mat_directly(ID);
        }
      }

     return ptr_matPack;
   }

   uint32_t entity_MaterialManager::contain_mat_by_ID(uint64_t& ID)
   {  
     uint32_t mContain{};
     size_t size_MaterialHeader {ID_matBin.size()};
     size_t counter{};
     while(mContain != 1 || counter < size_MaterialHeader)
     { 
        mContain = mat_data[counter].mat_contains(ID);
        counter++;
     }

    return mContain;
   }

   void entity_MaterialManager::upload_AllMat_bin(const std::string& directory, file_OP::writeFlags& fileT)
   { 

    ///HERE FIND THE MESHES THAT HAVE THIS MATERIAL OF ID
    ///SUBSTITUDE THE ID FOR A THE ID OF STANDARD MATERIAL
    ///IF THE STANDARD MATERIAL NOT EXISTS, PUT THAT IT NOT HAVE MATERIAL TO UPLOAD ONLY THE COLORES NEEDED FOR TEXTURES
      std::cout << "=====UPLOAD ALL MATERIALS TO BINARY=====\n";
      for(auto& matBin : mat_data)
     { 
       matBin.packBinary(directory, fileT);
     }
   }

   void entity_MaterialManager::upload_selectMat_bin(const std::string& directory, uint64_t matBin_ID, file_OP::writeFlags& fileT)
   {
     uint32_t find_matBin {search_algorithms::binary_search_contains<uint64_t>(ID_matBin, matBin_ID)}; //FIND IF EXISTS MAT
     
     if(find_matBin == 1)
     {
      mat_data[pos_by_ID[matBin_ID]].packBinary(directory, fileT);
     }
     
   }

   uint64_t proccess_nameMaterial(std::string& nameMat, const std::string& nameModel_path, const unsigned int& index)
   { 

    auto nameExists = [](std::string& nMat, const std::string& nameModelPath, const unsigned int& idx) -> void
      {
         nMat = nameModelPath + "_" + nMat + "_" + std::to_string(idx);
      };
    auto nameNotExists = [](std::string& nMat, const std::string& nameModelPath, const unsigned int& idx)  -> void
      {
         nMat = nameModelPath + "_mat_" + std::to_string(idx); ////this is the index of the material.
      };
        
   std::vector<funcPtr::func_Double_str_constUint> func_NM{};
   func_NM.resize(2);
   func_NM[0] = nameExists;
   func_NM[1] = nameNotExists;

   ////CHANGE THE FNV_MATERIAL<C-F6>
   int selectFunc {nameMat.empty() == true ? 1 : 0 };

   func_NM[selectFunc](nameMat, nameModel_path, index);

   if(nameMat.size() > MAX_SIZE_STR_BIN_MATERIAL)
   {
     nameMat = nameMat.substr(0, MAX_SIZE_STR_BIN_MATERIAL-1);
   }

    uint64_t ID_material_FNV {FNV::hash_1a(nameMat)};
    return ID_material_FNV;
   }
  
 

}

namespace data_utilities
{
  std::optional<manager_AssimpData::entity_ModelManager> model_D;
  std::optional<manager_AssimpData::entity_MeshManager> mesh_D;
  std::optional<manage_materialCooker::entity_MaterialManager> mat_D;
}

namespace data_leoBinary
{
//   std::vector<manager_AssimpData::entity_MateriaManager> materials_D{};
   std::vector<manager_AssimpData::entity_ModelManager> models_D{};
   Assimp::Importer assimpImporter{};

   void init_dataUtilities_Cooker()
   {
    data_utilities::mesh_D.emplace();
    data_utilities::mat_D.emplace();
     //ID_defaultMaterial = FNV::str_to_hash("DefaultMaterial");
   }


   void loadModel(std::string path, unsigned int aiProcessFlags, uint32_t version)
   {
     log_System::modelCooker_logger.info("loading model... | path = " + path); 

      std::filesystem::path modelPath {path};
      const aiScene* scene = assimpImporter.ReadFile(modelPath.string(), aiProcessFlags);

      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
      {
        std::string error_assimp {assimpImporter.GetErrorString()};
        log_System::assimp_logger.error(error_assimp + "\n");
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

    //////////////===PROCESSING MATERIALS===//////////////////////
      log_System::modelCooker_logger.info("importing materials from model | path = " + path);
      processMaterials(scene, nameModel_path, directory, 1);

     /////////LOAD MODEL AND MESHES PROCESS//////////// THIS SAVE MATERIALS ID WITH THE NAME
      manager_AssimpData::model_D modelD{};
      modelD.insert_nameHeader(nameModel_path);
      modelD.insert_version(version);
   
      int meshesCounter{};
      manager_AssimpData::meshesBin_D meshes_Bin{};
  
      log_System::modelCooker_logger.info("importing meshes from model | path = " + path);
      processNode(modelD, meshes_Bin, scene->mRootNode, scene, meshesCounter, version);
     
      ///UPDATE AFTER PROCESS ALL THE MESHES
      modelD.update_MeshCounter();
       
      data_utilities::mesh_D->insert_MeshBinD_ref(meshes_Bin);
      data_utilities::model_D->insert_ModelBinD_ref(modelD);
   }

   void processNode(manager_AssimpData::model_D& model, manager_AssimpData::meshesBin_D& meshes_Bin, aiNode* node, const aiScene* scene, int& meshesCounter,uint32_t& version)
   {
      aiMatrix4x4 aiModelMat {node->mTransformation}; ///TRANSFORM THIS MODEL MATRIX TO NORMAL GLM::MATRIX
      float (&mat_GeneralMeshes)[16] {meshes_Bin.get_generalMatrixTrans()};   
      convert_dataTypes::aiMat4_to_rawArrayFloat16(aiModelMat, mat_GeneralMeshes);
      meshes_Bin.insert_version(version);

      std::string nameModel {model.get_nameModel_str()};
      /// std::array<float, 16> array_ModelMat4 {convert_dataTypes::aiMat4_to_arrayFloat16(aiModelMat)}; ///GET THE MODEL MATRIX FROM THE NODE                 

      for (unsigned int i = 0; i < node->mNumMeshes; i++)
      {
         manager_AssimpData::mesh_D mesh_data{};
         aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

         std::string meshName { mesh->mName.C_Str() };
         size_t pos_BC{}; 
         if (convert_str::find_badCharacters_filePath(meshName, pos_BC))
         {
            meshName = scene->mRootNode->mName.C_Str();
            meshName += "_" + std::to_string(meshesCounter);
         }
         meshName = nameModel + "_" + meshName; ///THIS IF EXISTS OTHER MESH WITH THE SAME NAME

         log_System::meshCooker_logger.info("loading mesh... | mesh name = " + meshName);
         ////1. INSERT THE NAME OF THE MESH
         mesh_data.insert_nameMesh(meshName);

         ////2. INSERT THE ID OF MESH 
         uint64_t meshID {FNV::hash_1a(meshName)};
         mesh_data.insert_meshID(meshID);
        
         processMesh_data(mesh_data, mesh, scene, nameModel, mat_GeneralMeshes);

         meshes_Bin.insert_Mesh(mesh_data);
         model.insert_MeshID(meshID);
         //model.insertMesh(mesh_D);
         meshesCounter++;
         //SDL_Log(nameMesh.c_str());
      }

      for (unsigned int i = 0; i < node->mNumChildren; i++)
      {
         processNode(model, meshes_Bin, node->mChildren[i], scene, meshesCounter, version);
      }
   }
   void processMesh_data(manager_AssimpData::mesh_D& meshManager, aiMesh* mesh, const aiScene* scene,  const std::string& nameModel, float (&meshTransMatrix)[16])
   {
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
          //  std::cout << "ERROR::NOT::TEXCOORDS";
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

      if(mesh->mMaterialIndex >= 0)
     { 
       aiMaterial* material { scene->mMaterials[mesh->mMaterialIndex] };

       aiString matName {};
       std::string nameMat {};

       if (material->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS)
       {
        nameMat = matName.C_Str();
       }
   
       uint64_t matID {manage_materialCooker::proccess_nameMaterial(nameMat, nameModel, mesh->mMaterialIndex)}; ///CONTINUE HERE 
       
       if(data_utilities::mat_D->contain_mat_by_ID(matID) == 1)  ////FIND IF MATERIAL ID EXISTS
       {
        meshManager.insert_materialID(matID);
       }
       
       else
       {
         log_System::meshCooker_logger.error("not find material with the ID created");
       }

     }

   }

   void insert_TexturesID(matPack_data_register& matP, manager_AssimpData::textures_MaterialManager& textures_ID)
   {
     matP.albedo_hash = textures_ID.Albedo_hash;
     matP.normals_hash = textures_ID.Normals_hash;
     matP.RMA_hash = textures_ID.RMA_hash;
     matP.height_hash = textures_ID.Height_hash;
     matP.emission_hash = textures_ID.Emission_hash;
   }

   void processMaterials(const aiScene* scene, const std::string& nameModel_path, const std::string& directory, uint32_t version)
   {
     material_LeoHeader headerMat{};
         
     headerMat.version = version;

     std::string name_matBin {data_MatCore::prefix_headerNameMat + nameModel_path};
     customFiles::quit_double_underscore_txt(name_matBin);

     if(name_matBin.size() > MAX_SIZE_STR_BIN_MATERIAL)
     {
      name_matBin = name_matBin.substr(0, MAX_SIZE_STR_BIN_MATERIAL-1);
     }

     std::strcpy(headerMat.nameMaterial_Header, name_matBin.c_str());
     headerMat.material_bin_ID = FNV::hash_1a(name_matBin);
     headerMat.materialCount = scene->mNumMaterials; 

     std::vector<matPack_data_register> materials_packing{};

     for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
     {
       matPack_data_register matP{};

       aiMaterial* material { scene->mMaterials[i] };

       aiString matName {};
       std::string nameMat {};

       if (material->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS)
       {
        nameMat = matName.C_Str();
       }
    //   else
    //   {
   //     nameMat = nameModel_path + "Mat" + std::to_string(i);
      /// }

       matP.materialID = manage_materialCooker::proccess_nameMaterial(nameMat, nameModel_path, i);  ////ASSING MATERIAL ID
       std::strcpy(matP.nameMaterial, nameMat.c_str()); ////ASSIGN NAME MATERIAL

       log_System::materialCooker_logger.info("loading material... | mat name = " + nameMat);
       //std::cout << "LOADING::MATERIAL:: NAME -> " << nameMat << std::endl;

       ///LOAD ALL TEXTURES OF MATERIAL
       manager_AssimpData::textures_MaterialManager str_tex{};
       manage_texturesCooker::loadTextures(material, str_tex, nameModel_path, directory, scene); //GET THE ID OF ALL TEXTURES LOADED
       insert_TexturesID(matP, str_tex); ////ASSING TEXTURES ID
      
       materials_packing.emplace_back(matP);
     }

    manage_materialCooker::material_D mat_data{headerMat, materials_packing}; 
    data_utilities::mat_D->insertMat(mat_data);

          /////CONTINUAR CON LOS MATERIALES
   }

}
