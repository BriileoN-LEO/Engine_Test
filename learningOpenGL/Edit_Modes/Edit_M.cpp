#include "Edit_M.h"
#include "Render/RenderData.h"

namespace edit_visualize {

    std::vector<std::string> nameSelect_Model{};
    ////MAKING A VISUALIZER OF EXPLOTED OBJECTS
    ///MAKING A VISUALIZER OF NORMAL OBJECTSr

}

namespace control_EditMode
{

    void detectSelectionMode()
    {
      if (controlMouse::mouseEvents[SDL_BUTTON_LEFT].active == true)
      {


      }


    }

    void controlAll_EditMode()
    {
      if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
      {
          detectSelectionMode();

      }

    }

}