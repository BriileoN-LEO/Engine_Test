#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "SHADER_H.h"
#include "bufferShader_Manager.h"
#include <iostream>
#include <unordered_map>

namespace shader_Manager
{ 
  namespace material_config
 {
   enum class render_BlendMode : uint8_t
  {
   OPAQUE = 0,
   TRANSPARENT = 1,
   MASKED = 2,
   NONE = 3
  };

  struct material_RenderState
  {
 //DEPTH STATE
   bool depthTest_enable{true};
   bool depthWrite_enable{true};
   uint32_t depthCompareOP {0};

 //BLEND STATE
   bool blend_enable{false};
   uint32_t src_ColorBlendFactor{0};
   uint32_t dst_ColorBlendFactor{0};
   uint32_t colorBlendOp{0};

 //RASTERIZER STATE
   uint32_t cullMode {0};
   uint32_t frontFace {0};
   bool isTwoSized {false};
   float alphaCutOff {0.5f};

 //STENCIL STATE
   
////////
  render_BlendMode renderMode{render_BlendMode::NONE};
  };

  namespace presets_MatRS //CONTINUE HERE TO MAKE THE PRESETS OF THE RENDER STATE 
  {
   constexpr material_RenderState materialOpaque = 
  {
   .depthTest_enable = true,
   .depthWrite_enable = true,
   .depthCompareOP = 0, ///REMPLACE HERE WITH THE RHI DATA
   .blend_enable = false,
   .renderMode = render_BlendMode::OPAQUE
  };
 
   constexpr material_RenderState materialTransparent = 
  { 
   .depthTest_enable = true,
   .depthWrite_enable = false,
   
   .blend_enable = true, 
   .renderMode = render_BlendMode::TRANSPARENT
  };
 
   constexpr material_RenderState materialMasked = 
  {

   .renderMode = render_BlendMode::MASKED
  };

  } 

 }

 

 class shadersContainer
 {
  private:
   
  std::vector<uint64_t> shaders_ID{};
  std::unordered_map<uint64_t, uint64_t> shader_by_pos{};
  std::vector<std::unique_ptr<shading::shader>> shaders{};
  
  public: 
 
  shadersContainer();
  
  void insert_shader();
  void link_bufferWithShader(buffersShader_manager::entity_BufferShader& buffer);
  //When create shaders, link the buffers with each shader
  //in the shading::shader save all the ID of the buffers that are linked with the shader
  // -modified to insert in order each buffer ID
  
  ///WRITE HERE THE FUNCTION TO MANAGE ALL THE SHADERS and create them

  };

 


 struct PipelineStateObject
 {
  shading::shader* shaderPSO{nullptr}; ///HERE THE MAIN SHADER
  //uint32_t shaderPSO_ID{}; ///ID for the configuration of the shader PSO.
  
  material_config::material_RenderState MatRenderState{}; 
 
  ////PUT ALL THE STUFF THAT THE RENDER REQUIERE for the SHADER*
 }; 

  class manager_ShaderPSO
 {
  private:

  std::unordered_map<uint64_t, uint64_t>  shaderPSO_by_id{};
  std::vector<PipelineStateObject> shadersPSO{};
  
  public:  
  
  manager_ShaderPSO();
 
  void insert_shaderPSO(PipelineStateObject* pso_s);
  void activate_shaderPSO();
  
  
  //HERE THE MATERIAL GETS THE SHADER_PSO

 };

///Include here the configuration of the shader and what it will have
  

 


}





#endif //SHADER_MANAGER_H
