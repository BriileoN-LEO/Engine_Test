
#ifndef Edit_M
#define Edit_M

//#include "../Render/RenderData.h"
#include "learningOpenGL.h"
#include "Model_Assimp/ModelAssimp.h"

namespace edit_visualize
{
  extern std::vector<Assimp_D::structModelName> nameSelect_Model;
  extern std::vector<Assimp_D::excluded_Obj> exclude_EditMeshes;
  extern bool selectionMode;
  extern bool select_one_time;
 ////MAKING A VISUALIZER OF EXPLOTED OBJECTS
 ///MAKING A VISUALIZER OF NORMAL OBJECTSr

}

namespace control_EditMode
{

 void detectSelectionMode();
 void controlAll_EditMode();


}







#endif //LEARNING_EDIT_M_H