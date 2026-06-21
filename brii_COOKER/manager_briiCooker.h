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
#include "dataManager/dataTypes_brii.h"
#include <ktx.h> ///NUEVA LIBRERIA PARA CARGAR IMAGENES
#include <KHR/khr_df.h>
#include "cmake-build-debug/_deps/ktx_software-src/lib/vkformat_enum.h" /////CHANGE THIS FOR A WAY TO REMPLACE THIS
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
  enum class memType : uint8_t
  {
   STBI_MEM = 0,
   HEAP_ENGINE = 1,
   STACK_HEAP_ENGINE = 2
  };

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
    uint64_t Albedo_hash{};
    uint64_t Normals_hash{};
    uint64_t RMA_hash{};
    uint64_t Height_hash{};
    uint64_t Emission_hash{};

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
  enum class resizeType : uint8_t
  {
    LINEAR = 0,
    SRGB = 1,
  };

  enum class texStatus : uint8_t
  {
    NOT_LOADED = 0, ////IF TEXTURE NOT LOADED IN THE UNORDERED_MAP textures_saved
    LOADED = 1,  ////IF TEXTURE NOT LOADED IN THE UNORDERED_MAP textures_saved

    NOT_EXISTS = 2, ////TO INDICATE IF THE TEXTURE EXISTS
    EXISTS = 3 /////TO INDICATE IF THE TEXTURE EXISTS
  };

  enum class material_Status : uint8_t
  {
    MATERIAL_PBR = 0,
    MATERIAL_LEGACY = 1,
    NOT_TEXTURES_MATERIAL = 2
  };

  struct data_image
  {
    ktxTexture2* texture_KTX2{ nullptr };

    unsigned char* texturePixels{ nullptr };  ////not loaded != nullptr  ||||  loaded == nullptr
    int width{};
    int height{};
    int nrChannels{};

    std::string nameTex{};
    uint64_t key_texture{};
    //uint32_t hash_nameTex{};

   // std::string directoryTex_ktx2{};
    //uint64_t key_texture{};

    manager_GD::memType type_mem_image{ manager_GD::memType::STBI_MEM };

    texStatus status_tex{ texStatus::NOT_EXISTS };

    data_image();
    data_image operator<<(data_image& iR);

    void clear();
  };

  struct packPBR_texData
  {
   data_image albedo_data{};
   data_image normal_data{};
   data_image RMA_data{};
   data_image emissive_data{};
   data_image height_data{};

   packPBR_texData();
   packPBR_texData(data_image& albedo_D, data_image& normal_D, data_image& RMA_D, data_image& emissive_D, data_image& height_D);

   packPBR_texData operator<<(packPBR_texData& dataP);
  };

  struct combine_textures_D
  {
    std::vector<unsigned char> dataAlbedo{};
    std::vector<unsigned char> dataRMA{};
  };

   extern std::unordered_map<aiTextureType, std::string> nameTextures;
   //extern std::unordered_map<uint64_t, uint64_t> textures_saved; ///THIS IS A CONTAINER THAT SAVES ALL THE TEXTURES THAT ARE UPLOADED ----> [NAME TEXTURE ID FNV], [PATH OF TEXTURE KTX CONVERTER
   extern std::vector<uint64_t> textures_saved;

   using pixel_color_state = void(*)(std::vector<unsigned char>&, int, unsigned char*, int);
   void statePixelColor(std::vector<unsigned char>& texData, int idxData, unsigned char* value, int idxValue);
   void statePixelColor_NULL(std::vector<unsigned char>& texData, int idxData, unsigned char* value, int idxValue);

   void combine_to_rgba(std::vector<unsigned char> outAlbedoOpa, unsigned char* albedoPixels, unsigned char* opacityPixels, const int& totalPixels_notChannels);
   void convert_RMA(std::vector<unsigned char>& RMA, unsigned char* roughnessData, unsigned char* metallicData, unsigned char* ambientOclussionData, const int& totalPixels);
   void resizeTexture_stb(std::vector<unsigned char>& resize_Texture, unsigned char* texture, const int& width_old, const int& height_old, const int& width_new, const int& height_new, const int& numChannels, resizeType resT);

  using pixel_colorArray = void(*)(const unsigned char*, float[4], int);

  material_Status testMaterial_PBR(aiMaterial* material);

  namespace KTX2_manager
  {
    bool compressPixels_UASTC(data_image& texture, ktx_bool_t normalMap);
    bool inyectPixels(data_image& texture);
    void ktx2_convert(data_image& texture, ktx_bool_t normalMap);

  }

  namespace convert_to_PBR
  {
    using conv_func = void(*)(std::vector<unsigned char>&, std::vector<unsigned char>&, briT::br_4&, float&, int&);
    void conv_to_RMA(std::vector<unsigned char>& outRMA, std::vector<unsigned char>& specShininess, briT::br_4& spec, float& metallic_v, int& idx);
    void convDiff_to_albedo(std::vector<unsigned char>& outAlbedo, std::vector<unsigned char>& diffOpa, briT::br_4& spec, float& metallic_v, int& idx);

    void conv_NULL_RMA(std::vector<unsigned char>& outNULL, std::vector<unsigned char>& inNULL, briT::br_4& spec, float& metallic_v, int& idx);
    void conv_NULL_Albedo(std::vector<unsigned char>& outNULL, std::vector<unsigned char>& inNUll, briT::br_4& spec, float& metallic_v, int& idx);

    void convertTextures(std::vector<unsigned char>& pixelsDiffuseOpa, std::vector<unsigned char>& pixelsSpecShinness,
                    const int& totalPixels, std::vector<unsigned char>& outAlbedo, std::vector<unsigned char>& outRMA);
  }

   texStatus process_EmbeddedTexture(const aiTexture* texture, data_image& data_Tex, const std::string& nameModel_Path, aiTextureType& matType,  int numChannels_obj);
   texStatus processTexture_pixels(aiMaterial* material, aiTextureType matType, const aiScene* scene, std::string prefix_nameModel, const std::string& directory, data_image& data_Tex, int numChannels_obj);

   void freeMemoryImage(const manager_GD::memType& memTexture, unsigned char* dataMem);

   void convertTextures_KTX2(packPBR_texData& textures_Data);
   void packTextures_KTX2_bin();

   void resizeTex(std::vector<unsigned char>& newDataImage, data_image& texData, texStatus& status, const int& maxHeight, const int& maxWidth, resizeType rT);

   packPBR_texData loadTextures_PBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb);
   packPBR_texData loadTextures_notPBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb);
   void loadTextures(aiMaterial* material, manager_AssimpData::textures_MaterialManager& hash_textures, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene);

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