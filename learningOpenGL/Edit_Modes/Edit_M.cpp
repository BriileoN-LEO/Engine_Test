#include "Edit_M.h"
#include "Render/RenderData.h"

namespace edit_visualize {

    std::vector<Assimp_D::structModelName> nameSelect_Model{};
    std::vector<Assimp_D::excluded_Obj> exclude_EditMeshes{};
    bool selectionMode{false};
    bool select_one_time{false};
    ////MAKING A VISUALIZER OF EXPLOTED OBJECTS
    ///MAKING A VISUALIZER OF NORMAL OBJECTS

}

namespace control_EditMode
{

    void detectSelectionMode()
    {

      if (controlMouse::mouseEvents[SDL_BUTTON_LEFT].active == true) {

          if (edit_visualize::select_one_time == false)
          {
              if (data_HitAABB::renderSelection == true)
              {
                  int dS {};
                  auto findSelect_M = std::ranges::find_if(edit_visualize::nameSelect_Model,
                  [&](Assimp_D::structModelName& vF)
                  {
                      ++dS;
                      return data_HitAABB::selectedObj.first == vF;
                  });

                  if (!edit_visualize::nameSelect_Model.empty())
                  {
                      --dS;
                  }

                  if (findSelect_M == std::ranges::end(edit_visualize::nameSelect_Model)) {

                      edit_visualize::nameSelect_Model.emplace_back(data_HitAABB::selectedObj.first);

                      int pos{};
                      auto find_eM = std::ranges::find_if(edit_visualize::exclude_EditMeshes,
                          [&](Assimp_D::excluded_Obj& eM)
                          {
                            pos++;
                            return eM.nameModel == data_HitAABB::selectedObj.first.nameModel;
                          });

                      if (!edit_visualize::exclude_EditMeshes.empty())
                      {
                          --pos;
                      }

                      if (find_eM != std::ranges::end(edit_visualize::exclude_EditMeshes))
                      {
                          auto find_Mesh = std::ranges::find_if(edit_visualize::exclude_EditMeshes[pos].nameMeshes,
                              [&](std::string& nameMesh)
                              {
                                return nameMesh == data_HitAABB::selectedObj.first.nameMesh;
                              });

                          if (find_Mesh == std::ranges::end(edit_visualize::exclude_EditMeshes[pos].nameMeshes))
                          {
                              edit_visualize::exclude_EditMeshes[pos].nameMeshes.emplace_back(data_HitAABB::selectedObj.first.nameMesh);
                          }
                      }

                      else if (find_eM == std::ranges::end(edit_visualize::exclude_EditMeshes)) {
                          edit_visualize::exclude_EditMeshes.emplace_back
                       (
                          Assimp_D::excludedOP::exclude_only_meshes,
                          data_HitAABB::selectedObj.first.nameModel,
                          std::vector<std::string>{data_HitAABB::selectedObj.first.nameMesh}
                       );
                      }

                      // edit_visualize::exclude_EditMeshes

                  }

                  else if (findSelect_M != std::ranges::end(edit_visualize::nameSelect_Model))
                  {
                      edit_visualize::nameSelect_Model.erase(edit_visualize::nameSelect_Model.begin() + dS);

                      int pos{};
                      int pos_meshes{};

                      for (int eM = 0; eM < static_cast<int>(edit_visualize::exclude_EditMeshes.size()); eM++)
                      {
                         // ++pos;
                          pos_meshes = 0;
                          bool exist_delete{};
                          for (auto& mesh : edit_visualize::exclude_EditMeshes[eM].nameMeshes)
                          {

                              if (mesh == data_HitAABB::selectedObj.first.nameMesh)
                              {
                          //        --pos_meshes;
                          //        --pos;
                                  exist_delete = true;
                                  break;
                              }
                         //     ++pos;
                              ++pos_meshes;
                          }

                          if (exist_delete == true)
                          {
                            break;
                          }

                          ++pos;
                      }
                      ////RESOLVE THE PROBLEM OF STATIC SELECTION AND DYNAMIC SELECTION WHEN I PRESS THE BUTTOM
                      /*
                      auto find_eM_D = std::ranges::find_if(edit_visualize::exclude_EditMeshes,
                          [&](Assimp_D::excluded_Obj& eM)
                          {
                            ++pos;
                            pos_meshes = 0;
                            bool exist_delete{};
                            for (auto& mesh : eM.nameMeshes)
                            {
                               ++pos_meshes;
                               if (mesh == findSelect_M->nameMesh)
                               {
                                 --pos_meshes;
                                 --pos;
                                 exist_delete = true;
                                 break;
                               }
                            }
                            return exist_delete;
                          });
                      */
             //         if (!edit_visualize::exclude_EditMeshes.empty())
             //         {
             //          --pos;
             //         }

                      edit_visualize::exclude_EditMeshes[pos].nameMeshes.erase( edit_visualize::exclude_EditMeshes[pos].nameMeshes.begin() + pos_meshes);

                      if (edit_visualize::exclude_EditMeshes[pos].nameMeshes.empty())  ///this delete the vector if any meshes exist inside the model selected
                      {
                         edit_visualize::exclude_EditMeshes.erase(edit_visualize::exclude_EditMeshes.begin() + pos);
                      }

                  }

              }

              edit_visualize::select_one_time = true;

        }

      }

      if (controlMouse::mouseEvents[SDL_BUTTON_LEFT].active == false)
      {
          if (edit_visualize::select_one_time == true)
          {
              edit_visualize::select_one_time = false;
          }

      }

    }

    void controlAll_EditMode()
    {
       if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
      {
          if (edit_visualize::selectionMode == true)
          {
              detectSelectionMode();  ///CONTINUE
          }
      }

    }

}