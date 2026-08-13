//
// Created by brii on 02/04/26.
//

#ifndef LEARNING_CLUSTERED_SHADING_RENDER_H
#define LEARNING_CLUSTERED_SHADING_RENDER_H

#include "learningOpenGL.h"

namespace data_SSBO_CS
{
   struct lights_SSBO
   {
       glm::vec4 lightPos_radius; // 0-12  ---> .w == radius
       glm::vec4 Color_And_Intensity; // 76-80
       glm::vec4 Direction;
       int lightState; // state of the light
       int light_Type; /// 0 == DIRECTIONAL LIGHT /// 1 == POINT LIGHT /// 2 == SPOT LIGHT
       float cutOff;
       float outerCutOff;
   };

}

namespace Clustered_Shading
{
    struct data_Prepass
    {
      unsigned int FBO{}; ///SAVE ALL THE TEXTURES INSIDE OF THIS FRAMEBUFFER

      unsigned int TCB_zBuffer{};
      unsigned int TCB_Position{};
      unsigned int TCB_Normal{};
      unsigned int TCB_FragPosViewSpace_and_shiness{};
      unsigned int TCB_Diffuse_and_specular{};

      unsigned int FBO_editMode{};
      unsigned int TCB_zBufferEditMode{};
      unsigned int TCB_colors_EditMode{};

      unsigned int res_width{};
      unsigned int res_height{};
    };

    struct dataSSBO_ClusteredShading
    {
        unsigned int tilesBound_SSBO{}; //TILES SHADER STORAGE BUFFER  ///BINDING 3
        unsigned int clusterAABB_SSBO{}; //CLUSTER OF MINPOS AND MAX POS OF EACH CLUSTER PER TILE ON SCREEN  ///BINDING 4
        unsigned int Lights_SSBO{};   ///BINDING 5
        unsigned int Lights_Grid_SSBO{};  ///BINDING 6
        unsigned int global_ID_Lights_SSBO{};  ///BINDING 7
        unsigned int globalCounter_Atomic_SSBO{};  ///BINDING 8
    };

    struct data_BeautyPass
    {
      //std::string ID_camera{};
      float zNear{};
      float zFar{};
      float scaleZ{};  ///CALCULATE = Nz / log(zFar/zNear)
      float biasZ{};  ///CALCULATE = -(Nz * log(zNear) / log(zFar/zNear))
      uint32_t gridDimension_X{}; // QUANTITY OF GRIDS IN X, 1920 / 16 ---- 16 = SIZE OF TILE IN X
      uint32_t gridDimension_Y{}; // QUANTITY OF GRIDS IN Y, 1080 / 16 ---- 16 = SIZE OF TILE IN Y

      float Nz{};
    };


    enum class typeShader : uint8_t
    {
      prepassCS_rendering = 0,
      prepassCS_editMode = 1,
      computeS_zPassTiles_01 = 2,
      computeS_ClusterAABB_02 = 3,
      computeS_LightCulling_03 = 4,
      beauty_Pass = 5,
      complete_shaders = 6
    };

    class renderManager_CS
    {
    private:

    unsigned int empty_VAO{};

    dataSSBO_ClusteredShading dataSSBO_CS{};
    data_Prepass Prepass_D{};
    data_BeautyPass beautyPass_D{};
    std::unordered_map<typeShader, std::string> shaders_ID{};

    int max_tileSize_Y{16};
    int max_tileSize_X{9};
    int max_tileSize_Z{24};

    int num_tiles_Y{};
    int num_tiles_X{};

    int num_tiles_all{};


    void destroy_zPrepass();

    void load_data(unsigned int res_width, unsigned int res_height);
    void load_data_editMode(unsigned int res_width, unsigned int res_height);

    void load_tilesBound_SSBO_03();
    void update_tilesBound_SSBO_03();

    void load_clusterAABB_SSBO_04();
    void update_clusterAABB_SSBO_04();

    void load_Lights_SSBO_05(const int& max_Lights);  ///MAX OF LIGHTS TO SAVE SPACE IN MEMORY AND LOAD LIGHTS
    void update_Lights_SSBO_05();

    void load_LightGridBuff_SSBO_06();
    void update_LightGridBuff_SSBO_06();

    void load_globalIDLights_SSBO_07();
    void update_globalIDLights_SSBO_07();

    void load_globalCounterAtomic_SSBO_08();
    void update_globalCounterAtomic_SSBO_08();

    public:

    renderManager_CS();
    void create_Data_CS(unsigned int res_width, unsigned int res_height);
    void insert_Shader(typeShader shaderT, std::string shaderID);
    void create_dataSSBO(); ///TO CREATE THE DATA OF THE SSBO
    void update_allSSBO();
    void update_lightsSSBO();
    void load_beautyPass();
    void update_beautyPass();

    void Render_passCS_rendering_F01();
    void Render_passCS_editMode_F01();

    void Render_computeCS();
    void Render_compute_ClusteredAABB_F02();
    void Render_compute_LightCulling_F03();
    void Render_beautyPassCS_F04();

    };
}


#endif //LEARNING_CLUSTERED_SHADING_RENDER_H
