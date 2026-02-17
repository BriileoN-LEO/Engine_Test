
#ifndef Edit_M
#define Edit_M

//#include "../Render/RenderData.h"
#include "learningOpenGL.h"
#include "Model_Assimp/ModelAssimp.h"

namespace edit_visualize
{
 struct explode_OBJ
 {
   bool active{};
   float dist_explode{};
 };

 class edit_MV
 {
 public:
  Assimp_D::structModelName names{};
  bool active_Edit_MV{};
  explode_OBJ explode{};
  //bool active_show_normals{};//

  bool start_erase{};

  edit_MV();
  edit_MV(edit_MV&& e_MV) noexcept;
  edit_MV(edit_MV& e_MV);
  edit_MV(Assimp_D::structModelName&& names) noexcept;
  edit_MV(Assimp_D::structModelName& names);
//  ~edit_MV();

  edit_MV operator= (edit_MV&& e_MV) noexcept;
  edit_MV operator= (edit_MV& e_MV);

 };


 //extern std::vector<Assimp_D::structModelName> nameSelect_Model;
 extern std::vector<edit_MV> nameSelect_Model;
 extern std::vector<Assimp_D::excluded_Obj> exclude_EditMeshes;

 extern bool editMode_active;

 extern bool selectionMode;
 extern bool select_one_time;

 extern bool exploded_objs_Active;
 extern float time_EOS;
 extern float max_EOS;

 extern bool show_normals_active;
 ////MAKING A VISUALIZER OF EXPLOTED OBJECTS
 ///MAKING A VISUALIZER OF NORMAL OBJECTSr
}

namespace control_EditMode
{

 void detectSelectionMode();

 void deleteMesh_exploded_deselect();
 //void control_showNormals_OBJs();
 void control_Exploded_OBJs();
 void update_status_OBJs();

 void controlAll_EditMode();


}







#endif //LEARNING_EDIT_M_H