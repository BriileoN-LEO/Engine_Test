#include "Edit_M.h"
#include "Render/RenderData.h"

namespace edit_visualize {

    edit_MV::edit_MV(){};
    edit_MV::edit_MV(edit_MV&& e_MV) noexcept : names(e_MV.names){};
    edit_MV::edit_MV(edit_MV& e_MV) : names(e_MV.names){};
    edit_MV::edit_MV(Assimp_D::structModelName&& names) noexcept : names(names){};
    edit_MV::edit_MV(Assimp_D::structModelName& names) : names(names){};
   // edit_MV::~edit_MV(){};

    edit_MV edit_MV::operator= (edit_MV&& e_MV) noexcept
    {
         names = e_MV.names;
         active_Edit_MV = e_MV.active_Edit_MV;
         explode = e_MV.explode;
      return *this;
    }
    edit_MV edit_MV::operator= (edit_MV& e_MV)
    {
        names = e_MV.names;
        active_Edit_MV = e_MV.active_Edit_MV;
        explode = e_MV.explode;
        return *this;
    }

    //std::vector<Assimp_D::structModelName> nameSelect_Model{};
    std::vector<edit_MV> nameSelect_Model{};
    std::vector<Assimp_D::excluded_Obj> exclude_EditMeshes{};

    bool editMode_active{false};

    bool selectionMode{false};
    bool select_one_time{false};

    bool exploded_objs_Active{false};
    float time_EOS{0.001f};
    float max_EOS {0.4f};

    bool show_normals_active{false};

    bool shading_Blinn_Phong_Active{false};
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
                  [&](edit_visualize::edit_MV& vF)
                  {
                      ++dS;
                      return data_HitAABB::selectedObj.first == vF.names;
                  });

                  if (!edit_visualize::nameSelect_Model.empty())
                  {
                      --dS;
                  }

                  if (findSelect_M == std::ranges::end(edit_visualize::nameSelect_Model))
                      {

                      edit_visualize::nameSelect_Model.emplace_back(edit_visualize::edit_MV(data_HitAABB::selectedObj.first));

                      int pos{};
                      auto find_eM = std::ranges::find_if(edit_visualize::exclude_EditMeshes,
                          [&](Assimp_D::excluded_Obj& eM)
                          {
                            pos++;
                            return eM.model_ID == data_HitAABB::selectedObj.first.model_ID;
                          });

                      if (!edit_visualize::exclude_EditMeshes.empty())
                      {
                          --pos;
                      }

                      if (find_eM != std::ranges::end(edit_visualize::exclude_EditMeshes))
                      {
                          auto find_Mesh = std::ranges::find_if(edit_visualize::exclude_EditMeshes[pos].meshes_ID,
                              [&](uint32_t& mesh_ID)
                              {
                                return mesh_ID == data_HitAABB::selectedObj.first.mesh_ID;
                              });

                          if (find_Mesh == std::ranges::end(edit_visualize::exclude_EditMeshes[pos].meshes_ID))
                          {
                              edit_visualize::exclude_EditMeshes[pos].meshes_ID.emplace_back(data_HitAABB::selectedObj.first.mesh_ID);
                          }
                      }

                      else if (find_eM == std::ranges::end(edit_visualize::exclude_EditMeshes)) {
                          edit_visualize::exclude_EditMeshes.emplace_back
                       (
                          Assimp_D::excludedOP::exclude_only_meshes,
                          data_HitAABB::selectedObj.first.model_ID,
                          std::vector<uint32_t>{data_HitAABB::selectedObj.first.mesh_ID}
                       );
                      }

                      // edit_visualize::exclude_EditMeshes

                  }

                  else if (findSelect_M != std::ranges::end(edit_visualize::nameSelect_Model)) {

                      if (edit_visualize::exploded_objs_Active == false)
                      {
                          edit_visualize::nameSelect_Model.erase(edit_visualize::nameSelect_Model.begin() + dS);

                          int pos{};
                          int pos_meshes{};

                          for (int eM = 0; eM < static_cast<int>(edit_visualize::exclude_EditMeshes.size()); eM++)
                          {
                              // ++pos;
                              pos_meshes = 0;
                              bool exist_delete{};
                              for (auto& mesh : edit_visualize::exclude_EditMeshes[eM].meshes_ID)
                              {

                                  if (mesh == data_HitAABB::selectedObj.first.mesh_ID)
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

                          edit_visualize::exclude_EditMeshes[pos].meshes_ID.erase( edit_visualize::exclude_EditMeshes[pos].meshes_ID.begin() + pos_meshes);

                          if (edit_visualize::exclude_EditMeshes[pos].meshes_ID.empty())  ///this delete the vector if any meshes exist inside the model selected
                          {
                              edit_visualize::exclude_EditMeshes.erase(edit_visualize::exclude_EditMeshes.begin() + pos);
                          }

                      }

                      else if (edit_visualize::exploded_objs_Active == false)
                      {
                          if (findSelect_M->start_erase == false)
                          {
                              findSelect_M->start_erase = true;

                          }

                          else if (findSelect_M->start_erase == true)
                          {
                              findSelect_M->start_erase = false;

                          }
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

    void deleteMesh_exploded_deselect()
    {
     std::vector<std::pair<int, uint32_t>> meshes_to_delete{};

     int pos2{};
     for (auto& mesh_select :  edit_visualize::nameSelect_Model)
     {
      if (mesh_select.start_erase == true && mesh_select.explode.dist_explode <= 0)
      {
         meshes_to_delete.emplace_back(std::pair<int, uint32_t>(pos2, mesh_select.names.mesh_ID));
      }
         ++pos2;
     }

     ///HERE I DELETE
     if (static_cast<int>(meshes_to_delete.size()) > 0)
     {
       for (auto& mesh_delete : meshes_to_delete)
       {
                          edit_visualize::nameSelect_Model.erase(edit_visualize::nameSelect_Model.begin() + mesh_delete.first);

                          int pos{};
                          int pos_meshes{};

                          for (int eM = 0; eM < static_cast<int>(edit_visualize::exclude_EditMeshes.size()); eM++)
                          {
                              // ++pos;
                              pos_meshes = 0;
                              bool exist_delete{};
                              for (auto& mesh : edit_visualize::exclude_EditMeshes[eM].meshes_ID)
                              {

                                  if (mesh == mesh_delete.second)
                                  {
                                      exist_delete = true;
                                      break;
                                  }

                                  ++pos_meshes;
                              }

                              if (exist_delete == true)
                              {
                                  break;
                              }

                              ++pos;
                          }


                          edit_visualize::exclude_EditMeshes[pos].meshes_ID.erase( edit_visualize::exclude_EditMeshes[pos].meshes_ID.begin() + pos_meshes);

                          if (edit_visualize::exclude_EditMeshes[pos].meshes_ID.empty())  ///this delete the vector if any meshes exist inside the model selected
                          {
                              edit_visualize::exclude_EditMeshes.erase(edit_visualize::exclude_EditMeshes.begin() + pos);
                          }
          }
     }

    }
/*
    void control_showNormals_OBJs()
    {
     if (edit_visualize::show_normals_active == true)
     {
      //   for (auto& select_Obj : edit_visualize::nameSelect_Model)
     }

    }
*/
    void control_Exploded_OBJs()
    {
      if (edit_visualize::exploded_objs_Active == false || edit_visualize::editMode_active == false)  ///FINISH IF ALL THE EXPLODED OBJS ARE FALSE OR IF THE EDIT MODE IS NOT ACTIVE
      {
        for (auto& select_Obj : edit_visualize::nameSelect_Model)
        {
                if (select_Obj.explode.active == true)
                {
                    select_Obj.explode.dist_explode -= edit_visualize::time_EOS;

                    if (select_Obj.explode.dist_explode <= 0.0f)
                    {
                        select_Obj.explode.active = false;
                    }
                }
        }

      }
      else if (edit_visualize::exploded_objs_Active == true)
      {
          for (auto& select_Obj : edit_visualize::nameSelect_Model)
          {
              if (select_Obj.start_erase == false)
              {
                  if (select_Obj.explode.active == false)
                  {
                      select_Obj.explode.active = true;
                  }

                  if (select_Obj.explode.dist_explode < edit_visualize::max_EOS)
                  {
                      select_Obj.explode.dist_explode += edit_visualize::time_EOS;
                  }
              }

              else if (select_Obj.start_erase == true)
              {
                  select_Obj.explode.dist_explode -= edit_visualize::time_EOS;

                  if (select_Obj.explode.dist_explode <= 0.0f)
                  {
                      select_Obj.explode.active = false;
                  }
              }
          }
      }

        deleteMesh_exploded_deselect();

    }
    void update_status_OBJs()
    {
      std::vector<utilities::entity>& all_Entities {RenderData_Set::ModelsScene_D->out_entities()};  /// SEE IF ITS WORKS AND UPDATE THE ESTATUS

     for (auto& model : all_Entities)
     {
       std::vector<Assimp_D::Mesh>& meshes_D { model.model_entity->outMeshes() };
       for (auto& mesh : meshes_D)
       {
         mesh.update_editMode();
       }
     }

    }

    void controlAll_EditMode()
    {
       if (cameras::cameras_D[cameras::name_CurrentCamera].type == camera::typeCam::editMode)
      {
           if (edit_visualize::editMode_active == false)
           {
               edit_visualize::editMode_active = true;
           }

          if (edit_visualize::selectionMode == true)
          {
              detectSelectionMode();  ///CONTINUE
          }
      }
      else
      {
          if (edit_visualize::editMode_active == true)
          {
              edit_visualize::editMode_active = false;
          }
      }

        control_Exploded_OBJs();
        update_status_OBJs();
    }

}