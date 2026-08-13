#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <iostream>
#include <memory>
#include <vector>
#include "shader_Manager.h"
#include "bufferShader_Manager.h"

namespace material_Manager
{

  struct material_ShaderPipeline
 {
  uint64_t psoID{}; ///LINKER WITH THE SHADER OF THE MATERIALR, OpenGL_PIpelineState
  uint64_t uniformBufferMaterial_size{}; ///CALL WHEN link_Shader(), call the OpenGL_PipelineState::shader::entityBuffers::search the buffer Material if exists 
  //shader_Manager::OpenGL_PipelineState* shader_P{};
  
  ////---THIS IS THE LINK TO THE BUFFER OF THE MATERIAL SHADER 

  //IMPLEMENT LINK OR CONECTION THE SHADER WITH THE PSO in shader_Manager.h 
        
 };

 class material_Instance
{
  private: 
  material_ShaderPipeline m_shaderPipeline{};
  uint64_t ID_nameMaterial{}; 
  uint32_t materialOrder{};

  public:
  material_Instance();
  void link_ShaderPSO(shader_Manager::OpenGL_PipelineState& shaderPSO); ///GET THE SIZE OF THE BUFFER MATERIAL 
  

 };


  class materials_InstanceContainer
 {
  private:
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
