#ifndef LAYOUTS_OPENGL_MANAGER_H
#define LAYOUTS_OPENGL_MANAGER_H

#include <iostream>
#include <memory>

 namespace buffersShader_manager
 {
   enum class typeBuff : uint32_t 
  {
   UBO = 0,
   SSBO = 1
  };

   class buffer_shader
  { 
   private:
    uint64_t ID{};
    std::string nameBuffer{};
    typeBuff buff_T{};
    uint32_t buff_size{};
    
   public:

  };

   class buffers_shader_Container
  {
   private:
   std::vector<std::unique_ptr<buffer_shader>> buffers_shader{};
   
   public:
 

  };
  
   struct entity_BufferShader
  {
    buffer_shader* buffShader{};
    entity_BufferShader();
 
  };
 }



#endif //LAYOUTS_OPENGL_MANAGER_H
