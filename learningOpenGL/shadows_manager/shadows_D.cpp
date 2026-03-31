//
// Created by brii on 08/03/26.
//

#include "shadows_D.h"
#include "Render/frameBuffers.h"
#include "Render/RenderData.h"
#include "SHADER_H.h"
#include "Render/Render.h"

namespace shadowsManager {

  void directional_shadowMap_dL::load_ShadowMap(unsigned int resolution_width = 1024, unsigned int resolution_height = 1024)   /////////AQUI IMPLEMENTAR CASCADE SHADOW MAPPING
  {

    distances_CSM[0] = cameras::cameras_D[cameras::name_CurrentCamera].nearCut;
    distances_CSM[1] = 5.0f;
    distances_CSM[2] = 15.0f;
    distances_CSM[3] = 45.0f;
    distances_CSM[4] = 85.0f;

    set_FrustrumCorners();

    unsigned int FBO{};
    glGenFramebuffers(1, &FBO);
    // glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int TCB{};
    glGenTextures(1, &TCB);
    glBindTexture(GL_TEXTURE_2D_ARRAY, TCB);

    //    glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, resolution_width, resolution_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, resolution_width, resolution_height, 4, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_REPEAT  // GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_ARRAY, TCB, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, TCB, 0);
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

  void directional_shadowMap_dL::set_FrustrumCorners()
  {
    frustrumCorners_preLoad[0] = glm::vec4{-1.0f, -1.0f, -1.0f, 1.0f};
    frustrumCorners_preLoad[1] = glm::vec4{ 1.0f, -1.0f, -1.0f, 1.0f};
    frustrumCorners_preLoad[2] = glm::vec4{-1.0f,  1.0f, -1.0f, 1.0f};
    frustrumCorners_preLoad[3] = glm::vec4{ 1.0f,  1.0f, -1.0f, 1.0f};
    frustrumCorners_preLoad[4] = glm::vec4{-1.0f, -1.0f,  1.0f, 1.0f};
    frustrumCorners_preLoad[5] = glm::vec4{ 1.0f, -1.0f,  1.0f, 1.0f};
    frustrumCorners_preLoad[6] = glm::vec4{-1.0f,  1.0f,  1.0f, 1.0f};
    frustrumCorners_preLoad[7] = glm::vec4{ 1.0f,  1.0f,  1.0f, 1.0f};
  }

  void directional_shadowMap_dL::insert_directionalLight(uint32_t ID)
  {
   ID_dL.emplace_back(ID);
  }


  glm::mat4 directional_shadowMap_dL::calc_LightSpaceMatrix_CSM(const float& nearSplit, const float& farSplit)
  {
    light::DirectionalLight& dirLight { RenderData_Set::directionalLights_D[ID_dL[0]] };
    camera::camera1& cam_L { cameras::cameras_D[cameras::name_CurrentCamera]};

    glm::mat4 projectionCam {glm::perspective(glm::radians(cam_L.fovCam), cam_L.aspectRatio, nearSplit, farSplit)};


 //////CALCULAR ESTE LIGHT SPACE MATRIX DEL

    glm::mat4 camView {glm::lookAt(cam_L.posCam, cam_L.directionView - cam_L.posCam, glm::vec3(0.0f, 1.0f, 0.0f))};
  //  glm::mat4 camView {glm::lookAt(cam_L.posCam, dirLight.Direction - cam_L.posCam, glm::vec3(0.0f, 1.0f, 0.0f))};

    glm::mat4 cam_invViewProj {glm::inverse(projectionCam * cam_L.cam)};
   // glm::mat4 cam_invViewProj {glm::inverse(projectionCam * camView)};

    glm::vec3 center {0.0f};
    std::array<glm::vec4, 8> current_FrustumCorners;

    uint8_t n{};
    for (auto& fC : frustrumCorners_preLoad)
    {
      glm::vec4 current_FC = cam_invViewProj * fC;
      current_FrustumCorners[n] = current_FC / current_FC.w;

      center += glm::vec3(current_FrustumCorners[n]);
      ++n;
    }

    center /= 8.0f; ///8 is the size of frustrumCorners

    glm::vec3 up_LV {glm::vec3(0.0f, 1.0f, 0.0f)};

    if (abs(dirLight.Direction.y) > 0.999f)
    {
      up_LV = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glm::mat4 lightView {glm::lookAt(center - dirLight.Direction, center, up_LV)};

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (const auto& cfC : current_FrustumCorners)
    {
      glm::vec4 trf {lightView * cfC};
      minX = glm::min(minX, trf.x);
      maxX = glm::max(maxX, trf.x);
      minY = glm::min(minY, trf.y);
      maxY = glm::max(maxY, trf.y);
      minZ = glm::min(minZ, trf.z);
      maxZ = glm::max(maxZ, trf.z);

    }

/*
    float unitTexelX {(maxX - minX) / static_cast<float>(dataBuffer->width_Shadow)};
    float unitTexelY {(maxY - minY) / static_cast<float>(dataBuffer->height_Shadow)};

    minX = std::floor(minX / unitTexelX) * unitTexelX;
    maxX = std::floor(maxX / unitTexelX) * unitTexelX;
    minY = std::floor(minY / unitTexelY) * unitTexelY;
    maxY = std::floor(maxY / unitTexelY) * unitTexelY;
*/
    float pM {1.5f};

    minX *= pM;
    maxX *= pM;
    minY *= pM;
    maxY *= pM;

    minZ -= 100.0f;
    maxZ += 100.0f;

/*
    const float zMulti {10.0f};

    minZ = (minZ < 0) ? minZ * zMulti : minZ / zMulti;
    maxZ = (maxZ < 0) ? maxZ / zMulti : maxZ * zMulti;
*/
    glm::mat4 lightProjection {glm::ortho(minX, maxX, minY, maxY, minZ, maxZ)};

    return lightProjection * lightView;
  }

  void directional_shadowMap_dL::update_Lights()
  {
    light::DirectionalLight& dirLight { RenderData_Set::directionalLights_D[ID_dL[0]] }; ///IN THE FUTURE CHANGE THIS FOR AN ID

    float orthoCalc { 10.0f };
    glm::mat4 lightProjection {glm::ortho(-orthoCalc, orthoCalc, -orthoCalc, orthoCalc, 0.5f, 18.0f)};
    glm::mat4 lightView {glm::lookAt(dirLight.Posicion, dirLight.Direction, glm::vec3(0.0f, 1.0f, 0.0f))};
    lightSpaceMat = lightProjection * lightView;
  }

  void directional_shadowMap_dL::update_CascadeShadowMapping()
  {
    for (uint8_t n = 0; n < static_cast<uint8_t>(distances_CSM.size()) - 1; ++n)
    {
       const float& nearSplit {distances_CSM[n]};
       const float& farSplit {distances_CSM[n + 1]};

      lightSpaceMatrices[n] = calc_LightSpaceMatrix_CSM(nearSplit, farSplit);
      //lightSpaceMatrices[n] = lightSpaceMat;
    }

  }

  void directional_shadowMap_dL::update_shadowDistances()
  {
   // float& nearCutCam {cameras::cameras_D[cameras::name_CurrentCamera].nearCut};

    float maxCutCam {cameras::cameras_D[cameras::name_CurrentCamera].maxCut * (1.0f / (static_cast<float>(distances_CSM.size()) * 3.0f))};

    distances_CSM[0] = cameras::cameras_D[cameras::name_CurrentCamera].nearCut;

    float sum{};

    for (uint8_t i = 1; i < distances_CSM.size()-1; ++i)
    {
      sum += maxCutCam;
    //  SDL_Log(std::to_string(sum).c_str());
      distances_CSM[i] = sum;

    }

    distances_CSM[distances_CSM.size()-1] = cameras::cameras_D[cameras::name_CurrentCamera].maxCut;

  }

  void directional_shadowMap_dL::update_intensity()
  {
    glm::vec3 view_dirCam { normalize(cameras::cameras_D[cameras::name_CurrentCamera].directionView)};
    glm::vec3 angle_calc {view_dirCam.x, 0.0f, view_dirCam.z};

    float dot_angle {glm::dot(view_dirCam, angle_calc)};

    dot_angle = BGL::fit_map(dot_angle, 0.0f, 90.0f, 0.0f, 1.0f);
    intensity_shadow = 1.0f + dot_angle;
  }

  void directional_shadowMap_dL::blind_FrameBuffer()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, dataBuffer->FBO);
  };
  void directional_shadowMap_dL::render_FrameBuffer()
  {
    RenderData_Set::frameBuffers_D["screen"].useFrameBuffer_textureShadow(dataBuffer->TCB, 0, intensity_shadow);
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
    glEnable(GL_DEPTH_TEST);

    update_Lights();
    update_CascadeShadowMapping();
    update_shadowDistances();


    // shading::shader* shaderShadowDL {directional_shadowMap_dL::out_shader()};
     std::string* shader_shadowMap { &shaderID };

    shading::shader* shaderShadowDL {directional_shadowMap_dL::out_shader()};

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dataBuffer->TCB, 0);

    glClear(GL_DEPTH_BUFFER_BIT);

    shaderShadowDL->use();

    const std::string nameSLM{"lightSpaceMatrices"};
    std::vector<glm::mat4> lightSpaceMatrices_array
    {
      lightSpaceMatrices[0],
      lightSpaceMatrices[1],
      lightSpaceMatrices[2],
      lightSpaceMatrices[3]
    };

    shaderShadowDL->setMat4_array(nameSLM, lightSpaceMatrices_array);  ///FOR BETTER OPTIMIZATION

    shaderShadowDL->setInt("size_LSM", size_LSM);

    shaderShadowDL = nullptr;
    glCullFace(GL_FRONT);
    RenderData_Set::ModelsScene_D->render_nearPos_depthMapShadow(shader_shadowMap);
    glCullFace(GL_BACK);
    shader_shadowMap = nullptr;

/*
    uint8_t layer_p {};
    for (const auto& lightSpaceMatrix : lightSpaceMatrices)
    {

      shading::shader* shaderShadowDL {directional_shadowMap_dL::out_shader()};

      shaderShadowDL->use();

      glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dataBuffer->TCB, 0, layer_p++);
      glClear(GL_DEPTH_BUFFER_BIT); /// SEE IF THIS AFFECTS AFTER RENDER WITHOUT CALLING SHADOW->use();

      shaderShadowDL->transformMat("lightSpaceMatrix", lightSpaceMatrix);

      shaderShadowDL = nullptr;
      glCullFace(GL_FRONT);
      RenderData_Set::ModelsScene_D->render_nearPos_depthMapShadow(shader_shadowMap);
      glCullFace(GL_BACK);
      //shader_shadowMap = nullptr;

    }
*/

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