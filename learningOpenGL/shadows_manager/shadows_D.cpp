//
// Created by brii on 08/03/26.
//

#include "shadows_D.h"
#include "Render/frameBuffers.h"
#include "Render/RenderData.h"
#include "SHADER_H.h"
#include "Render/Render.h"

namespace shadowsManager {

  void directional_shadowMap_dL::load_ShadowMap(unsigned int resolution_width = 1024, unsigned int resolution_height = 1024)
  {
    unsigned int FBO{};
    glGenFramebuffers(1, &FBO);
   // glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int TCB{};
    glGenTextures(1, &TCB);
    glBindTexture(GL_TEXTURE_2D, TCB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution_width, resolution_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_REPEAT  // GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TCB, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      register_error_RM::register_error_withSentence("ERROR::FRAMEBUFFER::NOT COMPLETE!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    std::unique_ptr<shadow_FB_data> s_pL {std::make_unique<shadow_FB_data>(std::move(FBO), std::move(TCB), resolution_width, resolution_height)};

    dataBuffer = std::move(s_pL);
  }

  directional_shadowMap_dL::directional_shadowMap_dL() = default;
  directional_shadowMap_dL::directional_shadowMap_dL(unsigned int resolution_width, unsigned int resolution_height)
  {
    load_ShadowMap(resolution_width, resolution_height);
  }

  void directional_shadowMap_dL::loadShadow_Data(unsigned int resolution_width = 1024, unsigned int resolution_height = 1024)
  {
    load_ShadowMap(resolution_width, resolution_height);
  }

  void directional_shadowMap_dL::setShader(std::string shaderID)
  {
    this->shaderID = shaderID;
  }

  void directional_shadowMap_dL::insert_directionalLight(uint32_t ID)
  {
   ID_dL.emplace_back(ID);
  }

  void directional_shadowMap_dL::update_Lights()
  {
    light::DirectionalLight& dirLight { RenderData_Set::directionalLights_D[ID_dL[0]] }; ///IN THE FUTURE CHANGE THIS FOR AN ID

    float orthoCalc { 10.0f };
    glm::mat4 lightProjection {glm::ortho(-orthoCalc, orthoCalc, -orthoCalc, orthoCalc, 0.5f, 18.0f)};
    glm::mat4 lightView {glm::lookAt(dirLight.Posicion, dirLight.Direction, glm::vec3(0.0f, 1.0f, 0.0f))};
    lightSpaceMatrix = lightProjection * lightView;

  }

  void directional_shadowMap_dL::blind_FrameBuffer()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, dataBuffer->FBO);
  };
  void directional_shadowMap_dL::render_FrameBuffer()
  {
    RenderData_Set::frameBuffers_D["screen"].useFrameBuffer_texture(dataBuffer->TCB);
  };

  shading::shader* directional_shadowMap_dL::out_shader()
  {
    auto findShader {RenderData_Set::shader_D.find(shaderID)};

   if (findShader != RenderData_Set::shader_D.end())
     {
       return &RenderData_Set::shader_D[shaderID];
     }

    else
    {
      const std::string log_error_find {"ERROR FIND SHADER::" + shaderID};

      register_error_RM::register_error_withSentence(log_error_find.c_str());

    }

    return nullptr;
  }

   const unsigned int& directional_shadowMap_dL::out_texture()
  {
    return dataBuffer->TCB;
  }

   const std::string* directional_shadowMap_dL::out_shaderID()
  {
    auto findShader {RenderData_Set::shader_D.find(shaderID)};

    if (findShader != RenderData_Set::shader_D.end())
    {
      return &shaderID;
    }

     const std::string log_error_find {"ERROR FIND SHADER::" + shaderID};

     register_error_RM::register_error_withSentence(log_error_find.c_str());


    return nullptr;
  }

  void directional_shadowMap_dL::set_ShadowMap(shadow_render shadow_set)
  {
    openGL_render::viewportSet(0, 0, dataBuffer->width_Shadow, dataBuffer->height_Shadow);
    glBindFramebuffer(GL_FRAMEBUFFER, dataBuffer->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    update_Lights();

   shading::shader* shaderShadowDL {directional_shadowMap_dL::out_shader()};

    shaderShadowDL->use();
    shaderShadowDL->transformMat("lightSpaceMatrix", lightSpaceMatrix);
    std::string* shader_shadowMap { &shaderID };

    shaderShadowDL = nullptr;
    glCullFace(GL_FRONT);
    RenderData_Set::ModelsScene_D->render_nearPos_depthMapShadow(shader_shadowMap);
    glCullFace(GL_BACK);
    shader_shadowMap = nullptr;


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    openGL_render::viewportSet(0, 0, screenSettings::screen_w, screenSettings::screen_h);
    if (shadow_set == shadow_render::RENDER_SAVE_TEXTURE_SHADOW)
    {
    //  openGL_render::viewportSet(0, 0, dataBuffer->width_Shadow, dataBuffer->height_Shadow);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      render_FrameBuffer();
    }
    ///delete shaderShadowDL;

  }


}