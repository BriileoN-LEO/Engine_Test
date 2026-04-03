//
// Created by brii on 02/04/26.
//

#include "Clustered_Shading_Render.h"
#include "config_window.h"
#include "log_Errors/log_error_General.h"

namespace Clustered_Shading
{
    void renderManager_CS::destroy_zPrepass()
    {
      glDeleteFramebuffers(1, &zPrepass->FBO);
      glDeleteTextures(1, &zPrepass->TCB);

    }
    void renderManager_CS::load_data(unsigned int res_width, unsigned int res_height)
    {
        unsigned int FBO{};
        glGenFramebuffers(1, &FBO);
        // glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        unsigned int TCB{};
        glGenTextures(1, &TCB);
        glBindTexture(GL_TEXTURE_2D, TCB);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, res_width, res_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_REPEAT  // GL_CLAMP_TO_BORDER
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_ARRAY, TCB, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, TCB, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            log_ErrorG::register_w("ERROR::FRAMEBUFFER CLUSTERED SHADING::NOT COMPLETE!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        std::unique_ptr<data_zPrepass> s_pL {std::make_unique<data_zPrepass>(FBO, TCB, res_width, res_height)};

        if (zPrepass != nullptr)
        {
            destroy_zPrepass();
            zPrepass.reset();
            zPrepass = nullptr;
        }

        zPrepass = std::move(s_pL);
    }

    renderManager_CS::renderManager_CS() = default;
    void renderManager_CS::create_zPrepass(unsigned int res_width = screenSettings::screen_w, unsigned int res_height = screenSettings::screen_h)
    {
      load_data(res_width, res_height);
    }
    void create_dataT_buffer()
    {

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

}