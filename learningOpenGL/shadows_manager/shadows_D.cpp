//
// Created by brii on 08/03/26.
//

#include "shadows_D.h"
#include "Render/frameBuffers.h"
#include "Render/RenderData.h"
#include "SHADER_H.h"
#include "Render/Render.h"

namespace shadowsManager {

  void directional_shadowMap_dL::load_ShadowMap()
  {

    const unsigned int width_Shadow { 1024 };
    const unsigned int height_Shadow { 1024 };

    unsigned int FBO{};
    glGenFramebuffers(1, &FBO);

    unsigned int TCB{};
    glGenTextures(1, &TCB);
    glBindTexture(GL_TEXTURE_2D, TCB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width_Shadow, height_Shadow, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //  glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TCB, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::unique_ptr<shadow_FB_data> s_pL {std::make_unique<shadow_FB_data>(FBO, TCB, width_Shadow, height_Shadow)};

    dataBuffer = std::move(s_pL);
  }

  directional_shadowMap_dL::directional_shadowMap_dL() = default;

  void directional_shadowMap_dL::loadShadow_Data()
  {
    load_ShadowMap();
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

    glm::mat4 lightProjection {glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f)};
    glm::mat4 lightView {glm::lookAt(dirLight.Posicion, dirLight.Direction, glm::vec3(0.0f, 1.0f, 0.0f))};
    lightSpaceMatrix = lightProjection * lightView;

  }

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

  void directional_shadowMap_dL::draw_ShadowMap()
  {
    shading::shader* shaderShadowDL {directional_shadowMap_dL::out_shader()};

    shaderShadowDL->use();
    shaderShadowDL->transformMat("lightSpaceMatrix", lightSpaceMatrix);

    ///glViewport(0, 0, dataBuffer->width_Shadow, dataBuffer->height_Shadow);
    openGL_render::viewportSet(0, 0, dataBuffer->width_Shadow, dataBuffer->height_Shadow);

    glBindFramebuffer(GL_FRAMEBUFFER, dataBuffer->FBO);
    update_Lights();

    shaderShadowDL = nullptr;
    ///delete shaderShadowDL;

  }


}

