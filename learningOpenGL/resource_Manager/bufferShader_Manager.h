#ifndef LAYOUTS_OPENGL_MANAGER_H
#define LAYOUTS_OPENGL_MANAGER_H

#include <iostream>
#include <memory>
#include <vector>

 namespace buffersShader_manager
 {
   enum class typeBuff : uint32_t 
  {
   UBO = 0,
   SSBO = 1
  };

   enum class buff_D : uint32_t
  {
   OBJECT = 0,
   MATERIAL = 1
  };

////CONTINUE HERE TO SEE HOW TO CONFIGURATE TO GET THE BUFFERS TO GET THE SIZE OF THEM AND INSERT IN THE MATERIAL.
///I think to delete the buffers entity from the mesh 

   class buffer_shader
  { 
   private:
    uint64_t ID{}; ///THE ASSIGNED ID BASED OF THE NAME OF THE BUFFER, you can search the name in str of the buffer in a container of str
    uint32_t buff_number{}; ///THE NUMBER OF THE BUFFER ASSIGNED IN THE MEMORY, CHANGE IF NOTING IS TOUCH THE BUFFER IN THE RENDER LOOP, OR IN VICEVERSA ADD
    typeBuff buff_T{}; //TYPE OF BUFFER, UBO or SSBO
    size_t buff_size{}; //size of the buffer in bytes
    
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
