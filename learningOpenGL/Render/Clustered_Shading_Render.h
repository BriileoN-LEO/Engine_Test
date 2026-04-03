//
// Created by brii on 02/04/26.
//

#ifndef LEARNING_CLUSTERED_SHADING_RENDER_H
#define LEARNING_CLUSTERED_SHADING_RENDER_H

#include "learningOpenGL.h"

namespace Clustered_Shading
{
    struct data_zPrepass
    {
      unsigned int FBO{};
      unsigned int TCB{};
      unsigned int res_width{};
      unsigned int res_height{};
    };

    struct data_tileBounds
    {
      unsigned int SSBO{}; //SHADER STORAGE BUFFER
    };

    enum class typeShader : uint8_t
    {
      zPrepass = 0,
      computeS_Tiles_01 = 1,
      computeS_LightCulling_02 = 2
    };

    class renderManager_CS
    {
    private:

        std::unique_ptr<data_tileBounds> dataTiles_buffer{ nullptr };
        std::unique_ptr<data_zPrepass> zPrepass{nullptr};
        std::unordered_map<typeShader, std::string> shaders_ID{};

    void destroy_zPrepass();
    void load_data(unsigned int res_width, unsigned int res_height);

    public:

    renderManager_CS();
    void create_zPrepass(unsigned int res_width, unsigned int res_height);
    void create_dataT_buffer();
    void insert_Shader(typeShader shaderT, std::string shaderID);


    };
}


#endif //LEARNING_CLUSTERED_SHADING_RENDER_H