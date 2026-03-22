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

 enum class shadow_render : uint8_t
 {
   RENDER_SAVE_TEXTURE_SHADOW = 0,
   SAVE_TEXTURE_SHADOW = 1
 };

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

  void load_ShadowMap(unsigned int resolution_width, unsigned int resolution_height);

 public:

  directional_shadowMap_dL();
  directional_shadowMap_dL(unsigned int resolution_width, unsigned int resolution_height);
  void loadShadow_Data(unsigned int resolution_width, unsigned int resolution_height);
  void setShader(std::string shaderID);
  void insert_directionalLight(uint32_t ID);
  void update_Lights();

  void blind_FrameBuffer();
  void render_FrameBuffer();

  shading::shader* out_shader();
  const unsigned int& out_texture();
  const std::string* out_shaderID();
  inline const glm::mat4& out_lightSpaceMatrix()
  {
   return lightSpaceMatrix;
  }
  inline const unsigned int& out_widthViewShadow()
  {
   return dataBuffer->width_Shadow;
  }
  inline const unsigned int& out_heightViewShadow()
  {
   return dataBuffer->height_Shadow;
  }


  void set_ShadowMap(shadow_render shadow_set);

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