#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <iostream>
#include <memory>
#include <vector>
#include "shader_Manager.h"
#include "bufferShader_Manager.h"

namespace material_Manager
{
/*  struct material_ShaderPipeline
 {
  uint64_t psoID{}; ///LINKER WITH THE SHADER OF THE MATERIAL, OpenGL_PipelineState
  uint64_t uniformBufferMaterial_size{}; ///CALL WHEN link_Shader(), call the OpenGL_PipelineState::shader::entityBuffers::search the buffer Material if exists
 };*/
 
 class material_Instance
{
  private: 
  shader_Manager::PipelineStateObject* m_shaderPipeline{};
  uint64_t ID{}; ///THIS GET ACCESS TO THE MATERIAL AND THE MATERIAL NAME OF THE FUTURE CONTAINER OF STR

  public:
  material_Instance();
  void link_ShaderPSO(shader_Manager::PipelineStateObject* shaderPSO); ///GET THE SIZE OF THE BUFFER MATERIAL 

 };


  class materials_InstanceContainer
 {
  private:
 // std::unordered_map<uint64_t, uint64_t> IDmat_ByPos_orderRender{}; //<posRender, IDmat> == this is to render in order of materials
  std::unordered_map<uint64_t, uint64_t> mat_by_pos{};
  std::vector<uint64_t> material_ID{};
  std::vector<std::unique_ptr<material_Instance>> materials{};
  size_t materials_Count{};
  
  public: 
  materials_InstanceContainer(); 
  void order_Materials(); //Order all the materials in order depending of the order to render 
 
   //CONTINUE HERE 12/08/2026
   ///-Remember that the material only will update the UBO(buffer) of the shader
   ///-The object buffer object(UBO) will be update when it is requiered by the render_manager to update, calls the mesh for update the transformation
  
 };


}





#endif //MATERIAL_MANAGER
