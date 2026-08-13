#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "SHADER_H.h"
#include <iostream>
#include <unordered_map>

namespace shader_Manager
{

 class shadersContainer
 {
  private:
   
  std::vector<uint64_t> shaders_ID{};
  std::unordered_map<uint64_t, uint64_t> shader_by_pos{};
  std::vector<std::unique_ptr<shading::shader>> shaders{};
  
  public: 
 
  shaderContainer();
  
  void insert_shader();
  void link_bufferWithShader();
  //When create shaders, link the buffers with each shader
  //in the shading::shader save all the ID of the buffers that are linked with the shader
  // -modified to insert in order each buffer ID
  
  ///WRITE HERE THE FUNCTION TO MANAGE ALL THE SHADERS and create them

 };
 

 struct OpenGL_PipelineState
 {
  shading::shader* shader{nullptr}; ///HERE THE MAIN SHADER
 
  bool depthTest_enable{true};
  bool depthWrite_enable{true};
  bool blend_enable{true};

  ////PUT ALL THE STUFF THAT THE RENDER REQUIERE for the SHADER*
 }; 

  class manager_ShaderPSO
 {
  private:

  std::vector<OpenGL_PipelineState> shadersPSO{};
  
  public:  
  
  manager_ShaderPSO();
 
  void insert_shaderPSO(OpenGL_PipelineState* pso_s);
  //HERE THE MATERIAL GETS THE SHADER_PSO

 };

///Include here the configuration of the shader and what it will have
  

 


}





#endif //SHADER_MANAGER_H
