//
// Created by brii on 08/03/26.
//

#ifndef SHADOWS_D
#define SHADOWS_D

#include "learningOpenGL.h"
//#include "Render/frameBuffers.h"



namespace frameBuff
{
 struct data_frameBuffer;

}

namespace shading
{
  class shader;
}

namespace shadowsManager {


 struct shadow_FB_data
 {
  unsigned int FBO; //FRAMEBUFFER
  unsigned int TCB; //TEXTURE
  unsigned int width_Shadow{};
  unsigned int height_Shadow{};
 };

 class directional_shadowMap_dL
 {
 private:

  std::unique_ptr<shadow_FB_data> dataBuffer{nullptr};
  std::vector<uint32_t> ID_dL{};

  glm::mat4 model{};
  glm::mat4 lightSpaceMatrix{};

  std::string shaderID{};

  void load_ShadowMap();

 public:

  directional_shadowMap_dL();
  void loadShadow_Data();
  void setShader(std::string shaderID);
  void insert_directionalLight(uint32_t ID);
  void update_Lights();
  shading::shader* out_shader();

  void draw_ShadowMap();



 };

 class omnidirectional_ShadowMap_PL
 {

 private:

  std::unique_ptr<shadow_FB_data> dataBuffer{nullptr};
  std::vector<uint32_t> ID_pL{}; ///ID OF THE POINT LIGHTS

  void load_ShadowMap();

 public:

  omnidirectional_ShadowMap_PL();


 };

 class shadowsMap
 {
 private:


 };

}

#endif //SHADOWS_D