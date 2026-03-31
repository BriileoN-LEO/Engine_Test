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

  std::array<float, 5> distances_CSM{};
  std::array<glm::vec4, 8> frustrumCorners_preLoad{};
  std::array<glm::mat4, 4> lightSpaceMatrices {};
  int size_LSM{4};
  float intensity_shadow{};

  glm::mat4 lightSpaceMat{};
 // glm::mat4 lightSpaceMatrix{};

  std::string shaderID{};

  void load_ShadowMap(unsigned int resolution_width, unsigned int resolution_height);

 public:

  directional_shadowMap_dL();
  directional_shadowMap_dL(unsigned int resolution_width, unsigned int resolution_height);
  void loadShadow_Data(unsigned int resolution_width, unsigned int resolution_height);
  void setShader(std::string shaderID);
  void set_FrustrumCorners();
  void insert_directionalLight(uint32_t ID);

  glm::mat4 calc_LightSpaceMatrix_CSM(const float& nearSplit, const float& farSplit);

  void update_Lights();  ////THIS IS THE FIRST VERSION
  void update_CascadeShadowMapping(); ////THIS IS THE NEW VERSION
  void update_shadowDistances();
  void update_intensity();

  void blind_FrameBuffer();
  void render_FrameBuffer();

  shading::shader* out_shader();
  const unsigned int& out_texture();
  const std::string* out_shaderID();
  inline const glm::mat4& out_lightSpaceMatrix()
  {
   return lightSpaceMat;
  }
  inline std::vector<float*> out_distancesCSM()
  {
   std::vector<float*> LightSpaceMatrix_Array{};

   for (auto& LSM : distances_CSM)
   {
    LightSpaceMatrix_Array.emplace_back(&LSM);
   }

   return std::move(LightSpaceMatrix_Array);
  }
  inline const size_t size_LightSpaceMatrix()
  {
   return lightSpaceMatrices.size();
  }
  inline const unsigned int& out_widthViewShadow()
  {
   return dataBuffer->width_Shadow;
  }
  inline const unsigned int& out_heightViewShadow()
  {
   return dataBuffer->height_Shadow;
  }
  inline const glm::mat4* out_LightSpaceMatrix(uint8_t pos)
  {
     const size_t sizeArray_LSM {size_LightSpaceMatrix()};

     //int mask_bit {-(pos < sizeArray_LSM)};
     //nt pos_bit {pos & mask_bit};
     if (static_cast<size_t>(pos) < sizeArray_LSM)
     {
       return &lightSpaceMatrices[pos];
     }

   // register_error_RM::register_error_withSentence("ERROR::VALUE OUT OF SIZE::OVERSIZE LIGHT SPACE MATRIX ARRAY");
    SDL_Log("ERROR::VALUE OUT OF SIZE::OVERSIZE LIGHT SPACE MATRIX ARRAY");
    return nullptr;
  }
  inline std::vector<glm::mat4*> out_LightSpaceMatrix_array()
  {
    std::vector<glm::mat4*> LightSpaceMatrix_Array{};

    for (auto& LSM : lightSpaceMatrices)
    {
     LightSpaceMatrix_Array.emplace_back(&LSM);
    }

   return std::move(LightSpaceMatrix_Array);
  }

  inline float& out_intensityShadow()
  {
   return intensity_shadow;
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