//
// Created by brii on 02/04/26.
//

#ifndef LEARNING_CLUSTERED_SHADING_RENDER_H
#define LEARNING_CLUSTERED_SHADING_RENDER_H

#include "learningOpenGL.h"

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
      std::string ID_camera{};
      float zNear{};
      float zFar{};
      float scaleZ{};  ///CALCULATE = Nz / log(zFar/zNear)
      float biasZ{};  ///CALCULATE = -(Nz * log(zNear) / log(zFar/zNear))
      float gridDimension_X{}; // QUANTITY OF GRIDS IN X, 1920 / 16 ---- 16 = SIZE OF TILE IN X
      float gridDimension_Y{}; // QUANTITY OF GRIDS IN Y, 1080 / 16 ---- 16 = SIZE OF TILE IN Y

    };


    enum class typeShader : uint8_t
    {
      zPrepass = 0,
      computeS_zPassTiles_01 = 1,
      computeS_ClusterAABB_02 = 2,
      computeS_LightCulling_03 = 3,
      beauty_Pass = 4
    };

    class renderManager_CS
    {
    private:

        dataSSBO_ClusteredShading dataSSBO_CS{};
        data_Prepass Prepass_D{};
        data_BeautyPass beautyPass_D{};
        std::unordered_map<typeShader, std::string> shaders_ID{};
        int max_tileSize_Y{16};
        int max_tileSize_X{16};


    void destroy_zPrepass();

    void load_data(unsigned int res_width, unsigned int res_height);

    void load_tilesBound_SSBO_03();
    void update_tilesBound_SSBO_03(const int& numTiles);

    void load_clusterAABB_SSBO_04();
    void update_clusterAABB_SSBO_04(const int& numTiles);  ////////CONTINUE HEREEE

    void load_Lights_SSBO_05(const int& max_Lights);
    void update_Lights_SSBO_05();


    public:

    renderManager_CS();
    void create_zPrepass(unsigned int res_width, unsigned int res_height);
    void create_dataSSBO(); ///TO CREATE THE DATA OF THE SSBO
    void update_allSSBO();
    void insert_Shader(typeShader shaderT, std::string shaderID);

    void update_lightsSSBO();

    };
}


#endif //LEARNING_CLUSTERED_SHADING_RENDER_H