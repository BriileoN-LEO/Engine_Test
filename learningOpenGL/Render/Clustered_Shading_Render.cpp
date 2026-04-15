//
// Created by brii on 02/04/26.
//

#include "Clustered_Shading_Render.h"
#include "config_window.h"
#include "log_Errors/log_error_General.h"
#include "Render/RenderData.h"
#include "textureData/textureManager.h"

namespace Clustered_Shading
{
    void renderManager_CS::destroy_zPrepass()
    {
      glDeleteFramebuffers(1, &Prepass_D.FBO);
      glDeleteTextures(1, &Prepass_D.TCB_zBuffer);
    }

    void renderManager_CS::load_data(unsigned int res_width, unsigned int res_height)
    {
        unsigned int FBO{};
        glGenFramebuffers(1, &FBO);
        // glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);


        unsigned int TCB_zBuffer{texLoad_Data::load_Texture_zBuffer(res_width, res_height)};

        unsigned int TCB_position{};
        unsigned int TCB_normal{};
        unsigned int TCB_FragPosViewSpace_and_shiness{};
        unsigned int TCB_diffuse_and_specular{};
        unsigned int* texture_ptr{nullptr};

        texture_ptr = texLoad_Data::load_Texture_nBuffer(res_width, res_height, GL_RGB, GL_COLOR_ATTACHMENT0);

        if (texture_ptr != nullptr)
        {
            TCB_position = *texture_ptr;
            delete texture_ptr;
            texture_ptr = nullptr;
        }

        else
        {
            log_ErrorG::register_w("ERROR::TEXTURE_POSITION_CLUSTER_RENDER::LOAD_TEXTURE_TO_BUFFER");
        }

        texture_ptr = texLoad_Data::load_Texture_nBuffer(res_width, res_height, GL_RGB, GL_COLOR_ATTACHMENT1);

        if (texture_ptr != nullptr)
        {
            TCB_normal = *texture_ptr;
            delete texture_ptr;
            texture_ptr = nullptr;
        }

        else
        {
            log_ErrorG::register_w("ERROR::TEXTURE_NORMAL_CLUSTER_RENDER::LOAD_TEXTURE_TO_BUFFER");
        }

        texture_ptr = texLoad_Data::load_Texture_nBuffer(res_width, res_height, GL_RGBA, GL_COLOR_ATTACHMENT2);

        if (texture_ptr != nullptr)
        {
            TCB_FragPosViewSpace_and_shiness = *texture_ptr;
            delete texture_ptr;
            texture_ptr = nullptr;
        }

        else
        {
            log_ErrorG::register_w("ERROR::TEXTURE_FRAGPOSVIEWSPACE_CLUSTER_RENDER::LOAD_TEXTURE_TO_BUFFER");
        }

        texture_ptr = texLoad_Data::load_Texture_nBuffer(res_width, res_height, GL_RGBA, GL_COLOR_ATTACHMENT3);

        if (texture_ptr != nullptr)
        {
          TCB_diffuse_and_specular = *texture_ptr;
          delete texture_ptr;
          texture_ptr = nullptr;
        }

        else
        {
            log_ErrorG::register_w("ERROR::TEXTURE_DIFFUSE_AND_SPECULAR_CLUSTER_RENDER::LOAD_TEXTURE_TO_BUFFER");
        }

        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_ARRAY, TCB, 0);
        unsigned int attachment_color[4] {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(4, attachment_color);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            log_ErrorG::register_w("ERROR::FRAMEBUFFER CLUSTERED SHADING::NOT COMPLETE!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);



     //   std::unique_ptr<data_Prepass> s_pL {std::make_unique<data_Prepass>(FBO, TCB_zBuffer, TCB_position, TCB_normal, TCB_FragPosViewSpace_and_shiness, TCB_diffuse_and_specular, res_width, res_height)};

      //  Prepass_D = std::move(s_pL);
        Prepass_D.FBO = FBO; ///SAVE ALL THE TEXTURES INSIDE OF THIS FRAMEBUFFER

        Prepass_D.TCB_zBuffer = TCB_zBuffer;
        Prepass_D.TCB_Position = TCB_position;
        Prepass_D.TCB_Normal = TCB_normal;
        Prepass_D.TCB_FragPosViewSpace_and_shiness = TCB_FragPosViewSpace_and_shiness;
        Prepass_D.TCB_Diffuse_and_specular = TCB_diffuse_and_specular;

        Prepass_D.res_width = res_width;
        Prepass_D.res_height = res_height;
    }

    void renderManager_CS::load_tilesBound_SSBO_03()
    {
            unsigned int tilesBound{};
            glGenBuffers(1, &tilesBound);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, tilesBound);

            size_t tilesContentSize { sizeof(glm::vec4) * 45900 }; /////////MAX SIZE OF THE TILES PER SCREEN == 45900

            /// 3840 / 16 = 240
            /// 2160 / 16 = 135
            /// 135 * 140 = 45900

            glBufferData(GL_SHADER_STORAGE_BUFFER, tilesContentSize, nullptr, GL_DYNAMIC_DRAW);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, tilesBound);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, tilesBound);

            dataSSBO_CS.tilesBound_SSBO = tilesBound;
    }
    void renderManager_CS::update_tilesBound_SSBO_03(const int& numTiles)
    {

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.tilesBound_SSBO);

            size_t numTiles_t {sizeof(glm::vec4) * numTiles};
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, numTiles_t, nullptr);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    }


    void renderManager_CS::load_clusterAABB_SSBO_04()
    {
            unsigned int clusterAABB{};
            glGenBuffers(1, &clusterAABB);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, clusterAABB);

            size_t tilesContentSize { sizeof(glm::vec4) * 2 };
            tilesContentSize *= 45900;
            /// 3840 / 16 = 240
            /// 2160 / 16 = 135
            /// 135 * 140 = 45900

            glBufferData(GL_SHADER_STORAGE_BUFFER, clusterAABB, nullptr, GL_DYNAMIC_DRAW);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, clusterAABB);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, clusterAABB);

            dataSSBO_CS.clusterAABB_SSBO = clusterAABB;
    }

    void renderManager_CS::update_clusterAABB_SSBO_04(const int& numTiles)
    {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.tilesBound_SSBO);

            size_t numTiles_t {(sizeof(glm::vec4) * 2) * numTiles};
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, numTiles_t, nullptr);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void renderManager_CS::load_Lights_SSBO_05(const int& max_Lights)
    {

           unsigned int lights{};
           glGenBuffers(1, &lights);

           glBindBuffer(GL_SHADER_STORAGE_BUFFER, lights);

           size_t tilesContentSize { sizeof(glm::vec4) * 3 };
           tilesContentSize *= max_Lights;

           glBufferData(GL_SHADER_STORAGE_BUFFER, tilesContentSize, nullptr, GL_DYNAMIC_DRAW);

           glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, lights);

           glBindBuffer(GL_SHADER_STORAGE_BUFFER, lights);

           dataSSBO_CS.Lights_SSBO = lights;

    }

    void renderManager_CS::update_Lights_SSBO_05()
    {

    }

    renderManager_CS::renderManager_CS() = default;
    void renderManager_CS::create_zPrepass(unsigned int res_width = screenSettings::screen_w, unsigned int res_height = screenSettings::screen_h)
    {
      load_data(res_width, res_height);

    }
    void renderManager_CS::create_dataSSBO()
    {

        load_tilesBound_SSBO_03();
        load_clusterAABB_SSBO_04();

        unsigned int Lights{};   ///BINDING 5
        unsigned int Lights_Grid{};  ///BINDING 6
        unsigned int global_ID_Lights{};  ///BINDING 7
        unsigned int globalCounter_Atomic{};  ///BINDING 8

    }
    void renderManager_CS::update_allSSBO()
    {
        int size_Y {static_cast<int>(glm::ceil(screenSettings::screen_h / max_tileSize_Y))};
        int size_X {static_cast<int>(glm::ceil(screenSettings::screen_h / max_tileSize_X))};

        int numTiles {size_Y * size_X};

        update_tilesBound_SSBO_03(numTiles);
        update_clusterAABB_SSBO_04(numTiles);
    }
    void renderManager_CS::insert_Shader(typeShader shaderT, std::string shaderID)
    {
       auto find_s {shaders_ID.find(shaderT)};

        if (find_s != shaders_ID.end())
        {
          find_s->second = shaderID;
        }

        else
        {
         shaders_ID.emplace(shaderT, shaderID);
        }

    }

    void renderManager_CS::update_lightsSSBO()
    {

    }

}