//
// Created by brii on 02/04/26.
//

#include "Clustered_Shading_Render.h"
#include "config_window.h"
#include "log_Errors/log_error_General.h"
#include "Render/RenderData.h"
#include "textureData/textureManager.h"

namespace Clustered_Shading {
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



        num_tiles_Y = static_cast<int>(glm::ceil(screenSettings::screen_h / max_tileSize_Y));
        num_tiles_X = static_cast<int>(glm::ceil(screenSettings::screen_w / max_tileSize_X));

        num_tiles_all = (num_tiles_Y * num_tiles_X) + 1000;
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

        size_t tilesContentSize { sizeof(glm::vec4) * num_tiles_all }; /////////MAX SIZE OF THE TILES PER SCREEN == 45900

        /// 3840 / 16 = 240
        /// 2160 / 16 = 135
        /// 135 * 140 = 45900

        glBufferData(GL_SHADER_STORAGE_BUFFER, tilesContentSize, nullptr, GL_DYNAMIC_DRAW);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, tilesBound);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        dataSSBO_CS.tilesBound_SSBO = tilesBound;
    }
    void renderManager_CS::update_tilesBound_SSBO_03()
    {

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.tilesBound_SSBO);

        size_t numTiles_t {sizeof(glm::vec4) * num_tiles_all};
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, numTiles_t, nullptr);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    }


    void renderManager_CS::load_clusterAABB_SSBO_04()
    {
        unsigned int clusterAABB{};
        glGenBuffers(1, &clusterAABB);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, clusterAABB);

        size_t tilesContentSize { sizeof(glm::vec4) * 2 };
        tilesContentSize *= num_tiles_all;
        /// 3840 / 16 = 240
        /// 2160 / 16 = 135
        /// 135 * 140 = 45900

        glBufferData(GL_SHADER_STORAGE_BUFFER, clusterAABB, nullptr, GL_DYNAMIC_DRAW);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, clusterAABB);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        dataSSBO_CS.clusterAABB_SSBO = clusterAABB;
    }

    void renderManager_CS::update_clusterAABB_SSBO_04()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.tilesBound_SSBO);

        size_t numTiles_t {(sizeof(glm::vec4) * 2) * num_tiles_all};
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, numTiles_t, nullptr);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void renderManager_CS::load_Lights_SSBO_05(const int& max_Lights)
    {

        unsigned int lights{};
        glGenBuffers(1, &lights);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lights);

        size_t tilesContentSize { sizeof(glm::vec4) * 4 };
        tilesContentSize *= max_Lights;

        glBufferData(GL_SHADER_STORAGE_BUFFER, tilesContentSize, nullptr, GL_DYNAMIC_DRAW);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, lights);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        dataSSBO_CS.Lights_SSBO = lights;

    }

    void renderManager_CS::update_Lights_SSBO_05()
    {
        std::vector<data_SSBO_CS::lights_SSBO> lights_Data{};

        const uint32_t& num_dL {RenderData_Set::lightsScene_D->num_Lights(light::typeLight::DIRECTIONAL_LIGHT) };
        for (uint32_t i = 0; i < num_dL; ++i)
        {
            utilities_Lights::entity_dL dL_entity_D { RenderData_Set::lightsScene_D->entity_dL_by_Pos(i) };
            if (dL_entity_D.dL_PBR_entity != nullptr) {
                const light::data_directionalLightPBR& dL_PBR { dL_entity_D.dL_PBR_entity->out_Data() };

                lights_Data.emplace_back
                  (
                    glm::vec4(0.0f),
                    glm::vec4(glm::vec3(dL_PBR.Color), dL_PBR.Intensity),
                    glm::vec4(dL_PBR.Direction, 0.0f),
                    dL_PBR.stateLight,
                    0, ///THIS MEAN DIRECTIONAL LIGHT TYPE
                    0.0f,
                    0.0f
                  );
            }

            else
            {
                std::string log_Error {"ERROR::FAIL TO UPDATE (DIRECTIONAL LIGHT) IN SSBO CLUSTERED RENDERING::DATA == NULLPTR::NUM -->" + std::to_string(i)};
                log_ErrorG::register_w(log_Error.c_str());
            }

        }

        const uint32_t& num_pL {RenderData_Set::lightsScene_D->num_Lights(light::typeLight::POINT_LIGHT)};
        for (uint32_t i = 0; i < num_pL; ++i)
        {
            utilities_Lights::entity_pL pL_entity_D { RenderData_Set::lightsScene_D->entity_pL_by_Pos(i) };
            if (pL_entity_D.pL_PBR_entity != nullptr)
            {
                const light::data_pointLightPBR& pL_PBR { pL_entity_D.pL_PBR_entity->out_Data() };

                lights_Data.emplace_back
                (
                 glm::vec4(pL_PBR.Position, pL_PBR.Radio),
                 glm::vec4(pL_PBR.Color, pL_PBR.Intensity),
                 glm::vec4(0.0f),
                 pL_PBR.stateLight,
                 1,
                 0.0f,
                 0.0f
                );
            }

            else
            {
                std::string log_Error {"ERROR::FAIL TO UPDATE (POINT LIGHT) IN SSBO CLUSTERED RENDERING::DATA == NULLPTR::NUM -->" + std::to_string(i)};
                log_ErrorG::register_w(log_Error.c_str());
            }

        }

        const uint32_t& num_sL {RenderData_Set::lightsScene_D->num_Lights(light::typeLight::SPOT_LIGHT)};
        for (uint32_t i = 0; i < num_sL; ++i)
        {
            utilities_Lights::entity_sL sL_entity_D { RenderData_Set::lightsScene_D->entity_sL_by_Pos(i) };
            if (sL_entity_D.sL_PBR_entity != nullptr)
            {
                const light::data_SpotLightPBR& sL_PBR { sL_entity_D.sL_PBR_entity->out_Data() };

                lights_Data.emplace_back
                (
                 glm::vec4(sL_PBR.Position, sL_PBR.Radio),
                 glm::vec4(sL_PBR.Color, sL_PBR.Intensity),
                 glm::vec4(sL_PBR.Direction, 0.0f),
                 sL_PBR.stateLight,
                 2,
                 sL_PBR.cutOff,
                 sL_PBR.outerCutOff
                );
            }
            else
            {
                std::string log_Error {"ERROR::FAIL TO UPDATE (SPOT LIGHT) IN SSBO CLUSTERED RENDERING::DATA == NULLPTR::NUM -->" + std::to_string(i)};
                log_ErrorG::register_w(log_Error.c_str());
            }

        }

        /////CONTINUE HERE TO LOAD ALL THE LIGHTS

        size_t size_vec { static_cast<size_t>(sizeof(data_SSBO_CS::lights_SSBO) * lights_Data.size())};

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.Lights_SSBO);

        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size_vec, lights_Data.data());

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);



    }

    void renderManager_CS::load_LightGridBuff_SSBO_06()
    {
        unsigned int lightGridBuf{};
        glGenBuffers(1, &lightGridBuf);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightGridBuf);

        size_t tilesContentSize { sizeof(glm::vec4) * num_tiles_all };

        glBufferData(GL_SHADER_STORAGE_BUFFER, tilesContentSize, nullptr, GL_DYNAMIC_DRAW);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, lightGridBuf);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        dataSSBO_CS.Lights_Grid_SSBO = lightGridBuf;

    }
    void renderManager_CS::update_LightGridBuff_SSBO_06()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.Lights_Grid_SSBO);

        size_t tilesContentSize { sizeof(glm::vec4) * num_tiles_all };
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, tilesContentSize, nullptr);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    }

    void renderManager_CS::load_globalIDLights_SSBO_07()
    {
        unsigned int globalIDLights{};
        glGenBuffers(1, &globalIDLights);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, globalIDLights);

        size_t tilesContentSize { static_cast<size_t>( sizeof(uint32_t) * (num_tiles_all * RenderData_Set::lightsScene_D->num_all_Lights())) };

        glBufferData(GL_SHADER_STORAGE_BUFFER, tilesContentSize, nullptr, GL_DYNAMIC_DRAW);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, globalIDLights);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        dataSSBO_CS.global_ID_Lights_SSBO = globalIDLights;
    }
    void renderManager_CS::update_globalIDLights_SSBO_07()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.global_ID_Lights_SSBO);

        size_t tilesContentSize { static_cast<size_t>( sizeof(uint32_t) * (num_tiles_all * RenderData_Set::lightsScene_D->num_all_Lights())) };
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, tilesContentSize, nullptr);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    }

    void renderManager_CS::load_globalCounterAtomic_SSBO_08()
    {
        unsigned int globalIDLights{};
        glGenBuffers(1, &globalIDLights);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, globalIDLights);

        size_t tilesContentSize { sizeof(uint32_t) };

        glBufferData(GL_SHADER_STORAGE_BUFFER, tilesContentSize, nullptr, GL_DYNAMIC_DRAW);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, globalIDLights);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        dataSSBO_CS.globalCounter_Atomic_SSBO = globalIDLights;
    }
    void renderManager_CS::update_globalCounterAtomic_SSBO_08()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataSSBO_CS.globalCounter_Atomic_SSBO);

        size_t tilesContentSize { sizeof(uint32_t) };
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, tilesContentSize, nullptr);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    renderManager_CS::renderManager_CS() = default;
    void renderManager_CS::create_Data_CS(unsigned int res_width = screenSettings::screen_w, unsigned int res_height = screenSettings::screen_h)
    {
        load_data(res_width, res_height);

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

    void renderManager_CS::create_dataSSBO()
    {
        load_tilesBound_SSBO_03();
        load_clusterAABB_SSBO_04();
        load_Lights_SSBO_05(RenderData_Set::lightsScene_D->num_all_Lights());
        load_LightGridBuff_SSBO_06();
        load_globalIDLights_SSBO_07();
        load_globalCounterAtomic_SSBO_08();

    }
    void renderManager_CS::update_allSSBO()
    {
        update_tilesBound_SSBO_03();
        update_clusterAABB_SSBO_04();
        update_LightGridBuff_SSBO_06();
        update_globalIDLights_SSBO_07();
        update_globalCounterAtomic_SSBO_08();
    }
    void renderManager_CS::update_lightsSSBO()
    {
        update_Lights_SSBO_05();
    }

    void renderManager_CS::load_beautyPass()
    {
        beautyPass_D.zNear = cameras::cameras_D[cameras::name_CurrentCamera].nearCut;
        beautyPass_D.zFar = cameras::cameras_D[cameras::name_CurrentCamera].maxCut;
        beautyPass_D.Nz = 24.0f;

        float logZFarNear {std::log2(beautyPass_D.zFar) - std::log2(beautyPass_D.zNear)};
        beautyPass_D.scaleZ = beautyPass_D.Nz / logZFarNear;
        beautyPass_D.biasZ = -(beautyPass_D.Nz * std::log2(beautyPass_D.zNear)) / logZFarNear;
        beautyPass_D.gridDimension_X = static_cast<uint32_t>(std::ceil(screenSettings::screen_w / 16.0f));
        beautyPass_D.gridDimension_Y = static_cast<uint32_t>(std::ceil(screenSettings::screen_h / 16.0f));
    }
    void renderManager_CS::update_beautyPass()
    {
        float& zNear_T {cameras::cameras_D[cameras::name_CurrentCamera].nearCut};
        float& zNearLast_T {cameras::cameras_D[cameras::name_CurrentCamera].nearCut_last};
        float& zFar_T {cameras::cameras_D[cameras::name_CurrentCamera].maxCut};
        float& zFarLast_T {cameras::cameras_D[cameras::name_CurrentCamera].maxCut_last};

        if (zNear_T != zNearLast_T || zFar_T != zFarLast_T)
        {
            beautyPass_D.zNear = cameras::cameras_D[cameras::name_CurrentCamera].nearCut;
            beautyPass_D.zFar = cameras::cameras_D[cameras::name_CurrentCamera].maxCut;

            float logZFarNear {std::log2(beautyPass_D.zFar) - std::log2(beautyPass_D.zNear)};
            beautyPass_D.scaleZ = beautyPass_D.Nz / logZFarNear;
            beautyPass_D.biasZ = -(beautyPass_D.Nz * std::log2(beautyPass_D.zNear)) / logZFarNear;
        }

        ////// UPDATE THE GRID WHEN I CAN CHOSE THE DIMENSIONS OF SCREEN

    }

    void renderManager_CS::Render_passCS_rendering()
    {
        auto find_shaderPass { shaders_ID.find(typeShader::prepassCS_rendering) };

        if (find_shaderPass != shaders_ID.end())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, Prepass_D.FBO);
            glEnable(GL_DEPTH_TEST);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Prepass_D.TCB_zBuffer, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Prepass_D.TCB_Position, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, Prepass_D.TCB_Normal, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, Prepass_D.TCB_FragPosViewSpace_and_shiness, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, Prepass_D.TCB_Diffuse_and_specular, 0);

            glCullFace(GL_FRONT);
            RenderData_Set::ModelsScene_D->render_nearPos_shaderSet(&find_shaderPass->second);
            glCullFace(GL_BACK);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        else
        {
            std::string error_Log {"ERROR::CLUSTERED SHADING RENDERING::NOT FIND SHADER FOR PRE_PASS"};
            log_ErrorG::register_w(error_Log.c_str());

        }

    }

    void renderManager_CS::Render_computeCS()
    {
        auto find_shaderTiles {shaders_ID.find(typeShader::computeS_zPassTiles_01)};

        if (find_shaderTiles != shaders_ID.end())
        {
            RenderData_Set::shader_D[find_shaderTiles->second].use_computeShader(beautyPass_D.gridDimension_X, beautyPass_D.gridDimension_Y);
        }
        else
        {
            log_ErrorG::register_w("ERROR::COMPUTE SHADER (zPassTiles) NOT LINK IN CLUSTERED SHADING");
        }

        auto find_shaderClusterAABB {shaders_ID.find(typeShader::computeS_ClusterAABB_02)};

        if (find_shaderClusterAABB != shaders_ID.end())
        {
            RenderData_Set::shader_D[find_shaderClusterAABB->second].use_computeShader(beautyPass_D.gridDimension_X, beautyPass_D.gridDimension_Y);
        }

        else
        {
            log_ErrorG::register_w("ERROR::COMPUTE SHADER (ClusterAABB) NOT LINK IN CLUSTERED SHADING");
        }

        auto find_shaderLightCulling {shaders_ID.find(typeShader::computeS_LightCulling_03)};

        if (find_shaderLightCulling != shaders_ID.end())
        {
            RenderData_Set::shader_D[find_shaderLightCulling->second].use_computeShader(beautyPass_D.gridDimension_X, beautyPass_D.gridDimension_Y);
        }

        else
        {
            log_ErrorG::register_w("ERROR::COMPUTE SHADER (LightCulling) NOT LINK IN CLUSTERED SHADING");
        }


    }

    void renderManager_CS::Render_beautyPassCS()  /////CONTINUE HERE
    {
        auto find_shaderBeautyPass {shaders_ID.find(typeShader::computeS_LightCulling_03)};

       // if (find_shaderLightCulling != shaders_ID.end())
        //{
         //   RenderData_Set::shader_D[find_shaderLightCulling->second].use_computeShader(beautyPass_D.gridDimension_X, beautyPass_D.gridDimension_Y);
        //}

    }
}