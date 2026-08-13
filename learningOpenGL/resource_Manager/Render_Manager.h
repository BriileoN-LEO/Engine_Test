#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "resourceManager.h"
#include "material_Manager.h"

namespace render_engine
{
//create a struct to save al the meshes from the scene
 struct renderCommand
 {
  Assimp_D::Mesh* mesh_entity{nullptr};
  material_Manager::material_Instance* material{};
  transformation_basics::basics_Model3D entityMesh_transformation{};
 };

  class render_queue
 {
  private:

  std::vector<renderCommand> renderQueue_pile{};
 //HERE CREATE class render_queue, all the vector of renderCommands(pile of meshes orderd by Material
 //-only update one time, if some meshes change the material reorder again 
 //-get the meshes entity with the scene and the models inside
 };


  class render_Manager
 {
  private:
  ///manage the order of the materials

  public: 

 }


}




#endif ///RENDER_MANAGER

