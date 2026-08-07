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
#include "texturesPack_binFormat.h"
#include "dataManager/dataTypes_brii.h"
#include "dataManager/containerTypes_manager.h"
#include <ktx.h> ///NUEVA LIBRERIA PARA CARGAR IMAGENES
#include <KHR/khr_df.h>
#include <vulkan/vulkan.h>
#include <ktxvulkan.h>
//#include "cmake-build-debug/_deps/ktx_software-src/lib/vkformat_enum.h" /////CHANGE THIS FOR A WAY TO REMPLACE THIS
#include <iostream>
#include <string>
#include <cstring> 
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
   STACK_HEAP_ENGINE = 2,
   NOT_MEM = 3
  };

  enum class signBin : uint32_t
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
  extern const std::string pathMeshes; 

}

namespace manager_AssimpData
{

//==================    MESH SET BINARY    =======================

  class mesh_D
  {
  private:
    std::vector<vertex_D> dataVertex{};
    std::vector<unsigned int> dataIndices{};
    meshPack_Register mesh_info{}; //////CHANGE NAME

  public:
    mesh_D();
    mesh_D(meshPack_Register& mesh_info, std::vector<vertex_D>& dataVertex, std::vector<unsigned int>& dataIndices);
    mesh_D(mesh_D&& mesh_D) noexcept;
    mesh_D(mesh_D& mesh_D);
    mesh_D(const mesh_D& mesh_D);

    ~mesh_D();
    
    mesh_D operator=(mesh_D&& mD) noexcept;  ///REMEMBER noexcept is use to test in functions that not throw any exception, std::terminate throw (0) ant rfinal return (1)
    mesh_D operator=(mesh_D& mD);
    mesh_D operator=(const mesh_D& mD);

    void insert_headerMesh(meshPack_Register& hM);
    void insert_nameMesh(std::string& nameMesh);
    void insert_meshID(uint64_t& meshID);
    void insert_materialID(uint64_t& material_ID);
  
    void insert_vertexD(float position[3], float normal[3], float uv[2]);
    void setSize_VertexContainer(int size);
    void update_size_vertexD();

    void insert_Indice(unsigned int indice);
    void setSize_IndicesContainer(int size);
    void update_size_indices();

    void insert_mat4Transformation(float (&mat4_array)[16]);

    const uint64_t& get_meshID();
    meshPack_Register& get_meshPackRegister();
    std::vector<vertex_D>& get_dataVertex();
    std::vector<unsigned int>& get_dataIndices();

    void destroy();
  };

    class meshesBin_D
  { 
   private:
   std::vector<uint64_t> ID_mesh{}; ///IN ORDER TO MADE BINARY SEARCH
   std::unordered_map<uint64_t, uint64_t> pos_by_ID{}; ///ID'S FOR SEARCH BY ID THE POSICION
   std::vector<mesh_D> meshes_data{};
   mesh_LeoHeader headerMesh{};

   public:
   meshesBin_D();
   meshesBin_D(const meshesBin_D&& mBD) noexcept;
   meshesBin_D(const meshesBin_D& mBD);

   void insert_nameMeshBin(std::string& nameMesh);  ///COMPLETE
   void insert_version(uint32_t version);
   void update_meshesCount();
   float (&get_generalMatrixTrans()) [16];

   void insert_Mesh(mesh_D& mD); 
   void upload_Meshes_bin(const std::string& directory, file_OP::writeFlags& fileT);
   //void upload_SelectedMesh_byID(uint64_t ID_mesh);   ///DON'T WORRIED IF THE HEADER IS THE SAME, IS THE PURPOSE 
  };

  class entity_MeshManager
  { 
    private:
    std::vector<meshesBin_D> meshesBin_data{};
    
    public:
    entity_MeshManager();
    void insert_MeshBinD_ref(meshesBin_D& meshBinD);
    void upload_allMeshesBin(const std::string& directory, file_OP::writeFlags& fileT);
    
  };  

 //==================================================================================================================
 //==================================================================================================================

  class model_D
  {
  private:
    model_LeoHeader headerModel{};
    std::vector<uint64_t> meshesRegister_LeoMesh{};//BINARY ORDER 
   // std::vector<entity_MeshManager> meshes_D{};

  public:
    model_D();
    model_D(const model_D&& mD) noexcept;
    model_D(const model_D& mD);
    //entity_ModelManager(entity_ModelManager&& entity_modelM) noexcept;
    //entity_ModelManager(entity_ModelManager& entity_modelM);
    void insert_headerModel(model_LeoHeader& headerModel);
    void insert_nameHeader(std::string& nameHeader);
    void insert_version(uint32_t version);
    void insert_MeshID(uint64_t& mesh_ID);
    void update_MeshCounter();
    void upload_ModelBin(const std::string& directory, file_OP::writeFlags& fileT);

    const size_t& get_meshesCount();
    std::string get_nameModel_str();
    void destroy();
  };

  class entity_ModelManager
  {
   private: 
   std::vector<model_D> modelsBin_data{};

   public: 
   entity_ModelManager();
   
   void insert_ModelBinD_ref(model_D& model);
   void upload_allModelBin(const std::string& directory, file_OP::writeFlags& fileT);
  };

  struct textures_MaterialManager
  {
    uint64_t Albedo_hash{};
    uint64_t Normals_hash{};
    uint64_t RMA_hash{};
    uint64_t Height_hash{};
    uint64_t Emission_hash{};

  };


}

//==========HERE SAVE ALL THE CONTAINERS==============  //////CONTINUE HERE namespace data_utilities

namespace manage_texturesCooker
{
  enum class resizeType : uint8_t
  {
    LINEAR = 0,
    SRGB = 1,
  };

  enum class texStatus : uint32_t
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
    texKTX2_ptr texture_KTX2{ nullptr };

    unsigned char* texturePixels{ nullptr };  ////not loaded != nullptr  ||||  loaded == nullptr
    int width{};
    int height{};
    int nrChannels{};

    std::string nameTex{};
    uint64_t key_texture{};
    //uint32_t hash_nameTex{};

   // std::string directoryTex_ktx2{};
    //uint64_t key_texture{};

    manager_GD::memType type_mem_image{ manager_GD::memType::NOT_MEM };

    texStatus status_tex{ texStatus::NOT_EXISTS };

    data_image();
    data_image(data_image&& iR) noexcept;
    data_image(data_image& iR);
    data_image operator<<(data_image& iR);
    ~data_image();

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
   packPBR_texData(packPBR_texData&& pack_T) noexcept;
   packPBR_texData(packPBR_texData& pack_T);
   packPBR_texData(data_image& albedo_D, data_image& normal_D, data_image& RMA_D, data_image& emissive_D, data_image& height_D);

   packPBR_texData operator<<(packPBR_texData& dataP);
   //packPBR_texData operator=(packPBR_texData& dataP);
  };

  struct combine_textures_D
  {
    std::vector<unsigned char> dataAlbedo{};
    std::vector<unsigned char> dataRMA{};
  };

   extern std::unordered_map<aiTextureType, std::string> nameTextures;
   const std::string& get_nameTextureType(aiTextureType texType);
   //extern std::unordered_map<uint64_t, uint64_t> textures_saved; ///THIS IS A CONTAINER THAT SAVES ALL THE TEXTURES THAT ARE UPLOADED ----> [NAME TEXTURE ID FNV], [PATH OF TEXTURE KTX CONVERTER
   extern std::vector<uint64_t> textures_saved;

   using pixel_color_state = void(*)(std::vector<unsigned char>&, int, unsigned char*, int);
   void statePixelColor(std::vector<unsigned char>& texData, int idxData, unsigned char* value, int idxValue);
   void statePixelColor_NULL(std::vector<unsigned char>& texData, int idxData, unsigned char* value, int idxValue);

   void combine_to_rgba(std::vector<unsigned char> outAlbedoOpa, unsigned char* albedoPixels, unsigned char* opacityPixels, const int& totalPixels_notChannels);
   void convert_RMA(std::vector<unsigned char>& RMA, unsigned char* roughnessData, unsigned char* metallicData, unsigned char* ambientOclussionData, const int& totalPixels);

   void resizeTexture_stb(std::vector<unsigned char>& resize_Texture, unsigned char* texture, const int& width_old, const int& height_old, const int& width_new,const int& height_new, const int& numChannels, resizeType resT);

  using pixel_colorArray = void(*)(const unsigned char*, float[4], int);

  material_Status testMaterial_PBR(aiMaterial* material);

  namespace KTX2_manager
  {
    bool compressPixels_UASTC(data_image& texture, ktxTexture2* texKTX2, ktx_bool_t normalMap);
    bool inyectPixels(data_image& texture, ktxTexture2* texKTX2);
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
   loadTexture_Memory packTexKTX2_memory(data_image& tex_data);
   void packTextures_KTX2_bin(packPBR_texData& textures_Data, const std::string& directory);

   void resizeTex(std::vector<unsigned char>& newDataImage, data_image& texData, const int& maxHeight, const int& maxWidth, resizeType rT);
   void assign_texturesID(manager_AssimpData::textures_MaterialManager& hash_textures, packPBR_texData& textures_Data);

   packPBR_texData loadTextures_PBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb);

   packPBR_texData loadTextures_notPBR(aiMaterial* material, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene, std::string& prefixName, combine_textures_D& texturesComb);

   void loadTextures(aiMaterial* material, manager_AssimpData::textures_MaterialManager& hash_textures, const std::string& nameModel_Path, const std::string& directory, const aiScene* scene);
}

namespace manage_materialCooker
{
  class material_D
  {
  private:
    material_LeoHeader dataHeader_Mat{};
   
    std::vector<uint64_t> ID_materials{}; ///IN ORDER TO MADE BINARY SEARCH
    std::unordered_map<uint64_t, uint64_t> pos_by_ID{};
    std::vector<matPack_data_register> materials{};
      
  public:
    material_D();
    material_D(material_LeoHeader& dataMaterial, std::vector<matPack_data_register>& str_pathTextures);
    material_D(material_D&& matC) noexcept;
    material_D(material_D& matC);
   
    const uint64_t& get_matBin_ID();
    uint32_t mat_contains(uint64_t ID); ///BINARY_SEARCH
    matPack_data_register& get_mat_directly(uint64_t ID); //KNOW'S EXACTLY THAT THE MATERIAL EXISTS WITH THE ID
    const material_LeoHeader& get_const_header();

    uint32_t packBinary(const std::string& outDir, file_OP::writeFlags& fileT);

  };

 class entity_MaterialManager
  {
   private:
   std::vector<uint64_t> ID_matBin{}; ///IN ORDER TO MADE BINARY SEARCH
   std::unordered_map<uint64_t, uint64_t> pos_by_ID{}; ///ID'S for matBin
   std::vector<material_D> mat_data{};

   public: 
   entity_MaterialManager();
   
   void insertMat(material_D& mat);///INSERT IN ORDER
   data_MatCore::matPack_ptr get_mat_by_ID(uint64_t ID);
   uint32_t contain_mat_by_ID(uint64_t& ID);
   void upload_AllMat_bin(const std::string& directory, file_OP::writeFlags& fileT);
   void upload_selectMat_bin(const std::string& directory, uint64_t matBin_ID, file_OP::writeFlags& fileT);
   
   };

  uint64_t proccess_nameMaterial(std::string& nameMat, const std::string& nameModel_path, const unsigned int& index);

}

namespace data_utilities
{
  extern std::optional<manager_AssimpData::entity_ModelManager> model_D;
  extern std::optional<manager_AssimpData::entity_MeshManager> mesh_D;
  extern std::optional<manage_materialCooker::entity_MaterialManager> mat_D;
}


namespace data_leoBinary
{
//  extern std::vector<manager_AssimpData::entity_MateriaManager> materials_D;
 extern std::vector<manager_AssimpData::entity_ModelManager> models_D;
 extern Assimp::Importer assimpImporter;

 void init_dataUtilities_Cooker();

 void loadModel(std::string path, unsigned int aiProcessFlags, uint32_t version);
 void processNode(manager_AssimpData::model_D& model, manager_AssimpData::meshesBin_D& meshes_Bin, aiNode* node, const aiScene* scene, uint32_t& version);
 void processMesh_data(manager_AssimpData::mesh_D& meshManager, aiMesh* mesh, const aiScene* scene, const std::string& nameModel, float (&meshTransMatrix)[16]);
 
 void insert_TexturesID(matPack_data_register& matP, manager_AssimpData::textures_MaterialManager& textures_ID);
 void processMaterials(const aiScene* scene, const std::string& nameModel_path, const std::string& directory, uint32_t version);

 void create_ModelBinaries(const std::string& directory, file_OP::writeFlags& fileT);
 void create_MeshBinaries(const std::string& directory, file_OP::writeFlags& fileT);
 void create_MaterialBinaries(const std::string& directory, file_OP::writeFlags& fileT);
 
}


/////////TESTING IF THE BINARIES WORKS////////////////
namespace testBin_Laboratory
{
 void test_readModelBin();
 void test_readMeshBin();
 
}


#endif //LEARNING_MANAGER_BRIICOOKER_H
