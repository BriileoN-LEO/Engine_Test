#include "Model_Assimp/ModelAssimp.h"
#include "resource_Manager/resourceManager.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "Render/RenderData.h"
#include "2D_geo/basic_geometry_D.h"
#include "LIGHTS_test.h"

namespace data_Manager
{
  uint32_t find_and_remplace_str(const std::vector<uint32_t>& dataContainer, std::string& str_data) ////IT GETS A NEW HASHID IF REQUIERES AND MODIFIES THE STRING
  {
    uint32_t hashID {FNV::str_to_hash(str_data)};

    if (std::binary_search(dataContainer.begin(), dataContainer.end(), hashID))
    {
      uint32_t num{ 1 };

      while (true)
      {
        std::string str_data_n {str_data + "_" + std::to_string(num)};
        hashID = FNV::str_to_hash(str_data_n);

        if (std::binary_search(dataContainer.begin(), dataContainer.end(), hashID))
        {
          ++num;
        }

        else
        {
          str_data = str_data_n;
          break;
        }

      }
     }

    return hashID;
  }
};

namespace discard_objs
{
  void objs_Discard::update_meshes()
  {

    indices_MeshDiscard.clear();
    meshes_discard.clear();
    size_meshes = 0;

    for (auto& data_Eobj : e_obj)
    {
      Assimp_D::excludedOP& excludedT {data_Eobj.exclude_Type };

      switch (excludedT)
      {
        case Assimp_D::excludedOP::exclude_complete_model :
        {
          utilities::entity* entity_model {RenderData_Set::ModelsScene_D->out_entity_model(data_Eobj.model_ID)};

          if (entity_model != nullptr)
          {
            const int& numMeshes {entity_model->model_entity->out_numSec_meshes()};
            for (uint32_t i = 0; i < numMeshes; i++)
            {
              uint32_t* meshID{entity_model->model_entity->out_MeshID_ByPos(i)};
              meshes_discard.emplace_back(*meshID);  /////
              indices_MeshDiscard.emplace(*meshID, size_meshes);
             data_Eobj.meshes_ID.emplace_back(*meshID); ////THIS to remplace all the data

              ++size_meshes;

              meshID = nullptr;
            }
            entity_model = nullptr;
          }

          else
          {
            register_error_RM::register_error_withSentence("ERROR FIND MODEL TO DISCARD");
          }

          break;
        }
        case Assimp_D::excludedOP::exclude_only_meshes :
        {
          uint32_t numMesh {static_cast<uint32_t>(data_Eobj.meshes_ID.size())};

          for (uint32_t i = 0; i < numMesh; i++)
          {
            meshes_discard.emplace_back(data_Eobj.meshes_ID[i]);  /////
            indices_MeshDiscard.emplace(data_Eobj.meshes_ID[i], size_meshes);
            ++size_meshes;
          }

          break;
        }
      }

    }

  }
  void objs_Discard::add_meshes_pos(uint32_t& pos)
  {

    Assimp_D::excludedOP& excludedT {e_obj[pos].exclude_Type };

    switch (excludedT)
    {
      case Assimp_D::excludedOP::exclude_complete_model :
      {
        utilities::entity* entity_model {RenderData_Set::ModelsScene_D->out_entity_model(e_obj[pos].model_ID)};
        const int& numMeshes {entity_model->model_entity->out_numSec_meshes()};

        if (entity_model != nullptr)
        {
          for (uint32_t i = 0; i < numMeshes; i++)
          {
            uint32_t* meshID{entity_model->model_entity->out_MeshID_ByPos(i)};
            meshes_discard.emplace_back(*meshID);  /////
            indices_MeshDiscard.emplace(*meshID, size_meshes);
            e_obj[pos].meshes_ID.emplace_back(*meshID); ////THIS to remplace all the data

            ++size_meshes;

            meshID = nullptr;
          }
          entity_model = nullptr;
        }
        break;
      }
      case Assimp_D::excludedOP::exclude_only_meshes :
      {
        uint32_t numMesh {static_cast<uint32_t>(e_obj[pos].meshes_ID.size())};

        for (uint32_t i = 0; i < numMesh; i++)
        {
          meshes_discard.emplace_back(e_obj[pos].meshes_ID[i]);  /////
          indices_MeshDiscard.emplace(e_obj[pos].meshes_ID[i], size_meshes);
          ++size_meshes;
        }

        break;
      }
    }
  }
  void objs_Discard::delete_meshes_pos(uint32_t& pos)
  {
    uint32_t size_m {static_cast<uint32_t>(e_obj[pos].meshes_ID.size())};
    for (uint32_t i = 0; i < size_m; i++)
    {
      /////continuar aqui // TO REMPLACE MESHES AND DELETE INSIDE THE VECTOR;
      auto find_mesh {indices_MeshDiscard.find(e_obj[pos].meshes_ID[i])};

      if (find_mesh != indices_MeshDiscard.end())
      {
        uint32_t pos {find_mesh->second};

        std::swap(meshes_discard[pos], meshes_discard[size_meshes-1]);
        indices_MeshDiscard[meshes_discard[pos]] = pos;

        indices_MeshDiscard.erase(find_mesh);
        meshes_discard.pop_back();

        --size_meshes;
      }

      else
      {
       SDL_Log("ERROR_FIND_MESH");
      }
    }
  }

  objs_Discard::objs_Discard() = default;
  objs_Discard::objs_Discard(std::vector<Assimp_D::excluded_Obj>& e_obj)
  {
    size_eObj = static_cast<uint32_t>(e_obj.size());
    this->e_obj.clear();
    this->e_obj = e_obj;
 //   std::swap(this->e_obj, e_obj);
    SDL_Log("UPDATE_MESHES_DISCARD");
    update_meshes();

  }
  objs_Discard::~objs_Discard()
  {
    e_obj.clear();
    indices_MeshDiscard.clear();
    meshes_discard.clear();
    size_meshes = 0;
  }
  void objs_Discard::remplace_obj(uint32_t& pos, Assimp_D::excluded_Obj& obj_remplace)
  {
    if (pos < size_eObj)
    {
      SDL_Log("DELETE_MESHES");
      delete_meshes_pos(pos);

      e_obj.emplace_back(obj_remplace);
      std::swap(e_obj[pos], e_obj[size_eObj-1]);
      e_obj.pop_back();

      SDL_Log("ADD_MESHES");
      add_meshes_pos(pos);
      return;
    }

    else
    {
      e_obj.emplace_back(obj_remplace);
      add_meshes_pos(size_eObj);
      ++size_eObj;
    }

    std::string error_log {"ERROR::POS IS MUCH BIGGER THAN THE ARRAY TO REMPLACE:: discard_objs_scenario"};
    register_error_RM::register_error_withSentence(error_log.c_str());
  }

  void objs_Discard::delete_obj(uint32_t& pos)
  {
    if (pos < size_eObj)
    {
      delete_meshes_pos(pos);
      std::swap(e_obj[pos], e_obj[--size_eObj]);  ///CHECK THIS
      e_obj.pop_back();

    }
  }

  bool objs_Discard::find_mesh(uint32_t& meshID)
  {
    return indices_MeshDiscard.contains(meshID);
  }

  uint32_t& objs_Discard::out_size_eObjs()
  {
    return size_eObj;
  }
  const std::vector<Assimp_D::excluded_Obj>& objs_Discard::out_eObj_vec()
  {
   return e_obj;
  }

  discard_objs_scenario::discard_objs_scenario(){};
  void discard_objs_scenario::insert_objs_Discard(ControlScenarios::stateScenarios scene, std::vector<Assimp_D::excluded_Obj> obj_to_discard)
  {
    std::unique_ptr<objs_Discard> obj_D {std::make_unique<objs_Discard>(obj_to_discard)};

      if (research_discard.contains(scene))
      {
        obj_discard[scene].reset();
        obj_discard[scene] = nullptr;
        obj_discard[scene] = std::move(obj_D);
      }

      else
      {
        research_discard.emplace(scene);
        obj_discard.emplace(scene, std::move(obj_D));
      }
  }
  void discard_objs_scenario::remplace_excluded_Obj(ControlScenarios::stateScenarios scene, uint32_t pos, Assimp_D::excluded_Obj obj_remplace)
  {
    if (research_discard.contains(scene))
    {
     obj_discard[scene]->remplace_obj(pos, obj_remplace);
     return;
    }

    std::string error_log {"ERROR::NOT FIND THE SCENE TO REMPLACE:: discard_objs_scenario"};
    register_error_RM::register_error_withSentence(error_log.c_str());

  }
  void discard_objs_scenario::delete_excluded_Obj(ControlScenarios::stateScenarios scene, uint32_t pos)
  {
    if (research_discard.contains(scene))
    {
      obj_discard[scene]->delete_obj(pos);
    }

  }

  bool discard_objs_scenario::find_existence_mesh(ControlScenarios::stateScenarios scene, uint32_t& meshID)
  {
    if (research_discard.contains(scene))
    {
      return obj_discard[scene]->find_mesh(meshID);
    }

    return false;
  }

}


namespace resourceManager
{
  manager_Model::manager_Model() = default;

  void manager_Model::reserve_size(int size_r)
  {
    models_D.reserve(size_r);
    models_find_ID.reserve(size_r);
  }

  void manager_Model::insertModel (std::string nameStr, Assimp_D::loadToCPU::ModelData_loadCPU& model_info)
  {
    uint32_t hashID {FNV::str_to_hash(nameStr)};

    ID_models.emplace_back(hashID);

    std::unique_ptr<Assimp_D::Model> MD {std::make_unique<Assimp_D::Model>(model_info)};
    models_D.emplace(hashID, std::move(MD));

    models_find_ID.emplace(nameStr, hashID);
  }
  Assimp_D::Model* manager_Model::model_by_ID(uint32_t ID)
  {
   auto find_M {models_D.find(ID)};

      if (find_M != models_D.end())
      {
       return find_M->second.get();
      }

     return nullptr;
  }
  Assimp_D::Model* manager_Model::model_by_str(std::string str_v)
  {
    auto find_str {models_find_ID.find(str_v)};

      if (find_str != models_find_ID.end())
      {
        return models_D[find_str->second].get();

      }

    return nullptr;
  }

  const std::unordered_map<std::string, uint32_t>& manager_Model::out_ModelsID()
  {
    return models_find_ID;
  }

  Assimp_D::Model* manager_Model::model_by_num(int pos)
  {
     if (static_cast<int>(ID_models.size() - 1) >= pos)
     {
      return std::move(models_D[ID_models[pos]].get());
     }

    else
    {
     const char* error {"ERROR_FIND_MODEL::YOUR POS EXCED THE CONTAINER"};

     register_error_RM::register_error_withSentence(error);
    }

     return nullptr;
  }

  std::string manager_Model::get_nameModel(uint32_t ID)
  {
    if (models_D.contains(ID))
    {
     return models_D[ID]->nameModel;
    }

    else
    {
     return "";
    }

  }

  int manager_Model::size_models_D()
  {
    return static_cast<int>(models_D.size());
  }

  void manager_Model::clean_data()
  {
    ID_models.clear();
    models_D.clear();
    models_find_ID.clear();
  }


  manager_DirectionalLights::manager_DirectionalLights() {};

  void manager_DirectionalLights::insert_DL(std::string nameStr, lights_T::directionalLight_PBR dL_D)
  {
//    uint32_t hashID{FNV::str_to_hash(nameStr)};

    uint32_t hashID { data_Manager::find_and_remplace_str(dL_find_pos, nameStr) };
    uint32_t currentID {dL_D->get_ID_ref()};

    if (hashID != currentID)
    {
      dL_D->setID_by_int(hashID);
    }


    auto find_pos_in {std::lower_bound(dL_find_pos.begin(), dL_find_pos.end(), hashID)};

    dL_find_pos.insert(find_pos_in, hashID);
    dL_find_str.emplace(nameStr, hashID);

    directionalLight_PBR_D.emplace(hashID, std::move(dL_D));

    ++sizeContainer_dL;
  }

  lights_T::dLight_PBR_raw manager_DirectionalLights::pL_by_ID(uint32_t ID)
  {
    if (std::binary_search(dL_find_pos.begin(), dL_find_pos.end(), ID))
    {
      return directionalLight_PBR_D[ID].get();
    }

    std::string error_log {"NOT FIND DIRECTIONAL LIGHT WITH ID:: ID = " + std::to_string(ID)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }
  lights_T::dLight_PBR_raw manager_DirectionalLights::pL_by_str(std::string str_ID)
  {
    auto find_dL {dL_find_str.find(str_ID)};

    if (find_dL != dL_find_str.end())
    {
      return directionalLight_PBR_D[find_dL->second].get();
    }

    std::string error_log {"NOT FIND DIRECTIONAL LIGHT WITH (NAME):: NAME = " + str_ID};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }

  lights_T::dLight_PBR_raw manager_DirectionalLights::pL_by_num(uint32_t pos)
  {
     if (pos < sizeContainer_dL)
     {
        return directionalLight_PBR_D[dL_find_pos[pos]].get();
     }

    std::string error_log {"NOT FIND POINT LIGHT IN POSICION:: POSICION = " + std::to_string(pos)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }

  const uint32_t& manager_DirectionalLights::out_size()
  {
   return sizeContainer_dL;
  }

  void manager_DirectionalLights::clean_data()
  {
    dL_find_pos.clear();
    dL_find_str.clear();
    directionalLight_PBR_D.clear();

    sizeContainer_dL = 0;
  }

  manager_PointLights::manager_PointLights(){};

  void manager_PointLights::insert_PL(std::string nameStr, lights_T::pointLight pL_D, lights_T::pointLight_PBR pL_PBR_D)
  {
   // uint32_t hashID{FNV::str_to_hash(nameStr)};

    uint32_t hashID { data_Manager::find_and_remplace_str(pL_find_pos, nameStr) };
    uint32_t currentID {pL_PBR_D->get_ID_ref()};

    if (hashID != currentID)
    {
      pL_PBR_D->setID_by_int(hashID);
    //  pL_D->ID = hashID;
    }

    pL_D->ID = hashID;

    auto find_pos_in {std::lower_bound(pL_find_pos.begin(), pL_find_pos.end(), hashID)};

    pL_find_pos.insert(find_pos_in, hashID);
    pL_find_str.emplace(nameStr, hashID);

    pointLight_D.emplace(hashID, std::move(pL_D));
    pointLight_PBR_D.emplace(hashID, std::move(pL_PBR_D));

    ++sizeContainer_PL;
  };
  lights_T::pLight_raw manager_PointLights::pL_by_ID(uint32_t ID)
  {
    auto find_ID {pointLight_D.find(ID)};

    if (find_ID != pointLight_D.end())
    {
      return find_ID->second.get();
    }

    std::string error_log {"NOT FIND POINT LIGHT WITH ID:: ID = " + std::to_string(ID)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  };
  lights_T::pLight_raw manager_PointLights::pL_by_str(std::string str_ID)
  {
    auto find_Str {pL_find_str.find(str_ID)};

    if (find_Str != pL_find_str.end())
    {
      return pointLight_D[find_Str->second].get();
    }

    std::string error_log {"NOT FIND POINT LIGHT WITH (NAME):: NAME = " + str_ID};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  };
  lights_T::pLight_raw manager_PointLights::pL_by_num(uint32_t pos)
  {
    if (pos < sizeContainer_PL)
    {
     return pointLight_D[pL_find_pos[pos]].get();
    }

    std::string error_log {"NOT FIND POINT LIGHT IN POSICION:: POSICION = " + std::to_string(pos)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  };

  lights_T::pLight_PBR_raw manager_PointLights::pL_PBR_by_ID(uint32_t ID)
  {
    if (std::binary_search(pL_find_pos.begin(), pL_find_pos.end(), ID))
    {
     return pointLight_PBR_D[pL_find_pos[ID]].get();
    }

    std::string error_log {"NOT FIND POINT LIGHT PBR WITH ID:: ID = " + std::to_string(ID)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }
  lights_T::pLight_PBR_raw manager_PointLights::pL_PBR_by_str(std::string str_ID)
  {
    auto find_pL_PBR = pL_find_str.find(str_ID);

    if (find_pL_PBR != pL_find_str.end())
    {
      return pointLight_PBR_D[find_pL_PBR->second].get();
    }

    std::string error_log {"NOT FIND POINT LIGHT PBR WITH NAME:: NAME = " + str_ID};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }
  lights_T::pLight_PBR_raw manager_PointLights::pL_PBR_by_num(uint32_t pos)
  {
    if (pos < sizeContainer_PL)
    {
      return pointLight_PBR_D[pL_find_pos[pos]].get();
    }

    std::string error_log {"NOT FIND POINT LIGHT IN POSICION:: POSICION = " + std::to_string(pos)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    nullptr;
  }

  const uint32_t& manager_PointLights::out_size()
  {
    return sizeContainer_PL;
  };

  void manager_PointLights::clean_data()
  {
    pL_find_pos.clear();
    pL_find_str.clear();
    pointLight_D.clear();

    sizeContainer_PL = 0;
  }

  manager_SpotLights::manager_SpotLights() = default;

  void manager_SpotLights::insert_sL(std::string nameStr, lights_T::spotLight_PBR sL_D)
  {
    uint32_t hashID { data_Manager::find_and_remplace_str(sL_find_pos, nameStr) };
    uint32_t currentID {sL_D->get_ID_ref()};

    if (hashID != currentID)
    {
      sL_D->setID_by_int(hashID);
    }

    auto find_pos_in {std::lower_bound(sL_find_pos.begin(), sL_find_pos.end(), hashID)};

    sL_find_pos.insert(find_pos_in, hashID);
    sL_find_str.emplace(nameStr, hashID);
    spotLight_PBR_D.emplace(hashID, std::move(sL_D));

    ++sizeContainer_sL;

  }

  lights_T::sLight_PBR_raw manager_SpotLights::pL_by_ID(uint32_t ID)
  {
    if (std::binary_search(sL_find_pos.begin(), sL_find_pos.end(), ID))
    {
      return spotLight_PBR_D[ID].get();
    }

    std::string error_log {"NOT FIND SPOT LIGHT PBR WITH ID:: ID = " + std::to_string(ID)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }

  lights_T::sLight_PBR_raw manager_SpotLights::pL_by_str(std::string str_ID)
  {
    auto find_str {sL_find_str.find(str_ID)};

    if (find_str != sL_find_str.end())
    {
     return  spotLight_PBR_D[sL_find_str[str_ID]].get();
    }

    std::string error_log {"NOT FIND POINT LIGHT PBR WITH NAME:: NAME = " + str_ID};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }

  lights_T::sLight_PBR_raw manager_SpotLights::pL_by_num(uint32_t pos)
  {
      if (pos < sizeContainer_sL)
      {
        return  spotLight_PBR_D[sL_find_pos[pos]].get();
      }

    std::string error_log {"NOT FIND POINT LIGHT IN POSICION:: POSICION = " + std::to_string(pos)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    nullptr;
  }

  const uint32_t& manager_SpotLights::out_size()
  {
    return sizeContainer_sL;
  }

  void manager_SpotLights::clean_data()
  {
    sL_find_pos.clear();
    sL_find_str.clear();
    spotLight_PBR_D.clear();

    sizeContainer_sL = 0;
  }

}

namespace utilities {
  entity::entity() = default;
  entity::entity(Assimp_D::Model* model_entity) : model_entity(std::move(model_entity)){};

  scene::scene() = default;

  void scene::insert_entity_model(Assimp_D::Model* model_entity)
  {
    if (model_entity != nullptr)
    {
      std::vector<Assimp_D::Mesh>& meshes_data { model_entity->outMeshes()};

      for (auto& mesh : meshes_data)
      {
        ordered_allMeshes.emplace_back(mesh.ID);
      }

      uint32_t ID_model = model_entity->ID;
      ///SDL_Log(std::to_string(ID_model).c_str()); ///SEE THE ID

      models_pos.emplace(ID_model, current_size_M);
      pos_entities.emplace_back(current_size_M);
      models_entities.emplace_back(std::move(model_entity));

      ++current_size_M;
    }

  };
  entity* scene::out_entity_model(uint32_t model_ID)
  {
 //   auto find_model = std::ranges::find_if(models_entities,
 ///    [&](const entity& model) {
//        return model.model_entity->ID == model_ID;
//      });

    auto find_m {models_pos.find(model_ID)};

    if (find_m != models_pos.end())
    {
     // return std::to_address(find_model);  ///this probably get an error, so detects
      uint32_t& pos {pos_entities[models_pos[model_ID]]};
      return &models_entities[pos];
    }

    else if (find_m == models_pos.end())
    {
      std::string error_log { "ERROR::NOT FIND MODEL"};
      register_error_RM::register_error_withSentence(error_log.c_str());
    }

    return nullptr;
  }
  entity* scene::out_entity_model_byPos(uint32_t pos)
  {
     if (pos <= static_cast<uint32_t>(pos_entities.size()) - 1)
     {
       uint32_t& pos_t {pos_entities[pos]};
       return &models_entities[pos_t];

     }

     else
     {
      // SDL_Log("ERROR::NOT FIND MODEL | resourceManager.cpp ---> line 95");

       std::string error_log { "ERROR::NOT FIND MODEL"};

       register_error_RM::register_error_withSentence(error_log.c_str());
     }

    return nullptr;
  }

  std::vector<entity>& scene::out_entities()
  {
   return models_entities;
  }

  Assimp_D::Mesh* scene::out_mesh_fromModel(uint32_t model_ID, uint32_t mesh_ID)
  {

    auto find_model {models_pos.find(model_ID)};


    if (find_model != models_pos.end())
    {
      uint32_t& pos {pos_entities[find_model->second]};

      entity& entity_model {models_entities[pos]};

      Assimp_D::Mesh* find_mesh {&entity_model.model_entity->outSpecificMesh(mesh_ID)};  ///TENER CUIDADO SI FALLA ESTA LINEA

      if (find_mesh->ID != 0)
      {
        return std::move(find_mesh);
      }

      else if (find_mesh->ID == 0)
      {
        SDL_Log("POINTER_ERROR::NOT FIND MESH | resourceManager.cpp ---> scene::out_mesh_fromModel");
      }

    }

    else if (find_model == models_pos.end())
    {
     // SDL_Log("POINTER_ERROR::NOT FIND MODEL | resourceManager.cpp ---> scene::out_mesh_fromModel");  ////FOR A MOMENT
      std::string log_error {"POINTER_ERROR::NOT FIND MODEL"};
      register_error_RM::register_error_withSentence(log_error.c_str());

    }

    return nullptr;
  }
  Assimp_D::Mesh* scene::out_mesh_fromID(uint32_t mesh_ID)
  {
     for (auto& model_entity : models_entities)
     {
       Assimp_D::Mesh& mesh{model_entity.model_entity->outSpecificMesh(mesh_ID)};

       if (!mesh.nameMesh.empty())
       {
         return  std::move(new Assimp_D::Mesh(mesh));
       }
     }

    std::string error_log { "ERROR::NOT FIND MESH::DOESNT EXIST IN ANY MODEL---> nullptr"};

    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }

  Assimp_D::Mesh* scene::out_mesh_fromNameMesh(std::string nameMesh) ///SEE IF ITS WORKS
  {
    Assimp_D::Mesh* mesh_out { nullptr };
    for (auto& entity : models_entities)
    {
      mesh_out = new Assimp_D::Mesh(entity.model_entity->outSpecificMesh(FNV::str_to_hash(nameMesh)));
      if (!mesh_out->nameMesh.empty() == true)
      {
        break;
      }
      else if (!mesh_out->nameMesh.empty() == false)
      {
        mesh_out = nullptr;
      }

    }

    if (mesh_out != nullptr)
    {
     return std::move(mesh_out);  /// TO MOVE THE POINTER

    }

    if (mesh_out == nullptr)
    {
      std::string error_log { "ERROR::NOT FIND " + nameMesh + "::DOESNT EXIST IN ANY MODEL---> nullptr"};

      register_error_RM::register_error_withSentence(error_log.c_str());
    }

  //  delete nameMesh;
   // nameMesh = nullptr;

    return nullptr;
  }

  const Assimp_D::shader_SetType& scene::out_shaderSet_fromModel(uint32_t model_ID, Assimp_D::shader_type shader_t)
  {
    auto find_model = std::ranges::find_if(models_entities,
    [&](const entity& model) {
      return model.model_entity->ID == model_ID;
    });

    if (find_model != std::ranges::end(models_entities))
    {
      return find_model->model_entity->shaders_set[static_cast<uint32_t>(shader_t)];

    }

  }
  const uint32_t& scene::size_VM()
  {
    return current_size_M;
  }

  void scene::renderAll()
  {
    for (auto& model : models_entities)
    {
      model.model_entity->Draw_WL();
    }

  }
  void scene::render_singleModel(uint32_t model_ID)
  {
    auto find_model {models_pos.find(model_ID)};

    if (find_model != models_pos.end())
    {
      entity& entity_model {models_entities[find_model->second]};
      entity_model.model_entity->Draw_WL();
    }

  }
  void scene::render_singleMesh(uint32_t model_ID, uint32_t mesh_ID, int shaderOP)
  {
    auto find_model {models_pos.find(model_ID)};

    if (find_model != models_pos.end())
    {
      entity& entity_model {models_entities[find_model->second]};

      entity_model.model_entity->Draw_singleMesh_ID(mesh_ID, shaderOP);
    }

  }

  void scene::order_MeshesID()
  {
   std::ranges::sort(ordered_allMeshes);
  }

  void scene::order_nearPosMeshes()
  {
    render_meshes.clear();
    renderBlocking.clear();

    uint32_t pos_start_meshes{}; //0
    uint32_t pos_end_meshes{}; // 0

    uint32_t current_posModel{};

    for (auto& model_entity : models_entities) ///this render per model
    {
      const int& num_meshes { model_entity.model_entity->out_numSec_meshes() };

      for (int i = 0; i < num_meshes; i++)
      {
        Assimp_D::Mesh& out_mesh {model_entity.model_entity->out_MeshByPos(i)};

       //auto find_mesh {std::find(meshes_to_discard.begin(), meshes_to_discard.end(), out_mesh.ID)}; //CHANGES THIS TO MORE FASTER SEARCH
        bool find_mesh {RenderData_Set::discardObj_D->find_existence_mesh(ControlScenarios::scene, out_mesh.ID)};

       // if (!find_mesh)
         if (!find_mesh)
        {
          double dist_mesh {glm::distance2(out_mesh.MeshCoord.posModel, cameras::cameras_D[cameras::name_CurrentCamera].posCam)};
          render_meshes.emplace_back(out_mesh.ID, dist_mesh);

          for (uint32_t p = pos_start_meshes; p < pos_end_meshes; p++)
          {
           if (render_meshes[pos_end_meshes].dist < render_meshes[p].dist )
            {
             std::swap(render_meshes[p], render_meshes[pos_end_meshes]);  //// SWAP THE MESHES TO CALCULATE WHAT MESH RENDER FIRST
            }

          }

          ++pos_end_meshes;  //+1 increment for the size of the meshes
        }
      }

      if (pos_start_meshes != pos_end_meshes)
        {

        double dist {glm::distance2(model_entity.model_entity->ModelCoord.posModel, cameras::cameras_D[cameras::name_CurrentCamera].posCam)}; ///DISTANCE2 is more effioeient
        renderBlocking.emplace_back
        (
        current_posModel,
        pos_start_meshes,
        pos_end_meshes - 1,
        dist
        );

        uint32_t current_size_blocking {static_cast<uint32_t>(renderBlocking.size())};
        for (uint32_t lM = 0; lM < current_size_blocking; lM++)
        {
          if (lM != current_size_blocking - 1) {

            if (renderBlocking[current_size_blocking - 1].model_dist < renderBlocking[lM].model_dist)
            {
              std::swap(renderBlocking[lM], renderBlocking[current_size_blocking - 1]);
            }
          }
        }

        pos_start_meshes = pos_end_meshes;
      }

        ++current_posModel;
    }

   // std::vector<render_data_D> render_meshes{};
   // std::vector<render_entity> renderBlocking{};
  }

  void scene::order_farPosMeshes()
  {

  }

  void scene::render_nearPos_shaderSet(std::string* shader_ID)
  {
    for (auto& model_render : renderBlocking)
    {
      for (uint32_t renderP = model_render.meshes_start; renderP <= model_render.meshes_end; renderP++)
      {
        models_entities[model_render.model_pos].model_entity->Draw_DepthMapShadow(render_meshes[renderP].meshID, shader_ID);  ///test if this render the mesh
      }

    }

    shader_ID = nullptr;

  }
  void scene::render_nearPos_prePass_CS(std::string* shader_ID)
  {
    for (auto& model_render : renderBlocking)
    {
      for (uint32_t renderP = model_render.meshes_start; renderP <= model_render.meshes_end; renderP++)
      {
        models_entities[model_render.model_pos].model_entity->Draw_PrePassCS(render_meshes[renderP].meshID, shader_ID);  ///test if this render the mesh
      }

    }

    shader_ID = nullptr;
  }

  void scene::render_nearPos()
  {

    for (auto& model_render : renderBlocking)
    {
      for (uint32_t renderP = model_render.meshes_start; renderP <= model_render.meshes_end; renderP++)
      {
        models_entities[model_render.model_pos].model_entity->Draw_singleMesh_ID(render_meshes[renderP].meshID, 1);  ///test if this render the mesh
      }

    }

  }
  void scene::render_farPos()
  {

  }

  void scene::render_nearPos_shadows(std::vector<uint32_t>& meshes_to_discard, shading::shader* shaderShadow)
  {

  }

  void scene::render_farPos_shadows(std::vector<uint32_t>& meshes_to_discard,  shading::shader* shaderShadow)
  {

  }

  void scene::cleanAll_scene()
  {
    for (int i = 0; i < static_cast<uint32_t>(models_entities.size()); i++)
    {
      models_entities[i].model_entity = nullptr;

    }

    models_entities.clear();
    models_pos.clear();
    pos_entities.clear();
    current_size_M = 0;
  }

}

namespace utilities_Lights
{
   uint32_t num_empty{};

  entity_dL::entity_dL(){}
  entity_dL::entity_dL(lights_T::dLight_PBR_raw dL_PBR_entity)
  {
    this->dL_PBR_entity = nullptr;
    this->dL_PBR_entity = std::move(dL_PBR_entity);
    dL_PBR_entity = nullptr;
  }
  entity_dL::~entity_dL()
  {
    dL_PBR_entity = nullptr;
    delete dL_PBR_entity;
  }

  entity_dL::entity_dL(const entity_dL&& pL_entity_d) noexcept
  {
    dL_PBR_entity = pL_entity_d.dL_PBR_entity;
  }
  entity_dL::entity_dL(const entity_dL& pL_entity_d)
  {
    dL_PBR_entity = pL_entity_d.dL_PBR_entity;
  }

  entity_pL::entity_pL(){}
  entity_pL::entity_pL(lights_T::pLight_raw pL_entity, lights_T::pLight_PBR_raw pL_PBR_entity)
  {
    this->pL_entity = nullptr;
    this->pL_PBR_entity = nullptr;

    this->pL_entity = std::move(pL_entity);
    this->pL_PBR_entity = std::move(pL_PBR_entity);

    pL_entity = nullptr;
    pL_PBR_entity = nullptr;

  };
  entity_pL::entity_pL(lights_T::pLight_PBR_raw pL_PBR_entity)
  {
    this->pL_PBR_entity = nullptr;
    this->pL_PBR_entity = std::move(pL_PBR_entity);
    pL_PBR_entity = nullptr;
  }

  entity_pL::~entity_pL()
  {
    pL_entity = nullptr;
    pL_PBR_entity = nullptr;
    delete pL_PBR_entity;
    delete pL_entity;
  }

  entity_pL::entity_pL(const entity_pL&& pL_entity_d) noexcept
  {
    pL_entity = pL_entity_d.pL_entity;
    pL_PBR_entity = pL_entity_d.pL_PBR_entity;
  }
  entity_pL::entity_pL(const entity_pL& pL_entity_d)
  {
    pL_entity = pL_entity_d.pL_entity;
    pL_PBR_entity = pL_entity_d.pL_PBR_entity;
  }

  entity_sL::entity_sL(){};
  entity_sL::entity_sL(lights_T::sLight_PBR_raw sL_PBR_entity)
  {
    this->sL_PBR_entity = nullptr;
    this->sL_PBR_entity = std::move(sL_PBR_entity);
    sL_PBR_entity = nullptr;
  };
  entity_sL::~entity_sL()
  {
    sL_PBR_entity = nullptr;
    delete sL_PBR_entity;
  }
  entity_sL::entity_sL(const entity_sL&& pL_entity_d) noexcept
  {
    sL_PBR_entity = pL_entity_d.sL_PBR_entity;
  }
  entity_sL::entity_sL(const entity_sL& pL_entity_d)
  {
    sL_PBR_entity = pL_entity_d.sL_PBR_entity;
  }

  entity_dL empty_entity_dL{};
  entity_pL empty_entity_pL{};
  entity_sL empty_entity_sL{};


  scene_LightsManager::scene_LightsManager(){}
  scene_LightsManager::~scene_LightsManager()
  {
    clear_data_dL();
    clear_data_pL();
    clear_data_sL();
  }

  void scene_LightsManager::setPoint_geo(lights_T::point_geo2D point_geo)
  {
    this->point_geo = point_geo;
  }
  void scene_LightsManager::buildPoint_geo(std::string shaderID)
  {
   point_geo.build_pointGeo(shaderID);
  }

  void scene_LightsManager::insert_dL(lights_T::dLight_PBR_raw dL_PBR)
  {
     if (dL_PBR != nullptr)
     {
       directionalLights.L_pos.emplace(dL_PBR->get_ID_c(), directionalLights.current_size_L);
       directionalLights.pos_L_entity.emplace_back(directionalLights.current_size_L);
       directionalLights.L_entities.emplace_back(std::move(dL_PBR));

       dL_PBR = nullptr;

       ++directionalLights.current_size_L;
       return;
     }

    std::string error_Log{"ERROR INSERT DIRECTIONAL LIGHT IN SCENE::NULLPTR"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
  }
  void scene_LightsManager::insert_pL(lights_T::pLight_PBR_raw pL_PBR)
  {
    if (pL_PBR != nullptr)
    {
      pointLights.L_pos.emplace(pL_PBR->get_ID_c(), pointLights.current_size_L);
      pointLights.pos_L_entity.emplace_back(pointLights.current_size_L);
      pointLights.L_entities.emplace_back(std::move(pL_PBR));

      pL_PBR = nullptr;

      ++pointLights.current_size_L;
      return;
    }
    std::string error_Log{"ERROR INSERT POINT LIGHT IN SCENE::NULLPTR"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
  }
  void scene_LightsManager::insert_pLights_two(lights_T::pLight_raw pL, lights_T::pLight_PBR_raw pL_PBR)
  {
    if (pL_PBR != nullptr && pL != nullptr)
      {
      pointLights.L_pos.emplace(pL_PBR->get_ID_c(), pointLights.current_size_L);
      pointLights.pos_L_entity.emplace_back(pointLights.current_size_L);
      pointLights.L_entities.emplace_back(std::move(pL), std::move(pL_PBR));

      pL = nullptr;
      pL_PBR = nullptr;

      ++pointLights.current_size_L;

      return;
    }

    std::string error_Log{"ERROR INSERT POINT LIGHT DOUBLE IN SCENE::NULLPTR"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
  }

  void scene_LightsManager::insert_sL(lights_T::sLight_PBR_raw sL_PBR)
  {
    if (sL_PBR != nullptr)
    {
      spotLights.L_pos.emplace(sL_PBR->get_ID_c(), spotLights.current_size_L);
      spotLights.pos_L_entity.emplace_back(spotLights.current_size_L);
      spotLights.L_entities.emplace_back(std::move(sL_PBR));

      sL_PBR = nullptr;

      ++spotLights.current_size_L;
      return;
    }
    std::string error_Log{"ERROR INSERT SPOT LIGHT IN SCENE::NULLPTR"};
    register_error_RM::register_error_withSentence(error_Log.c_str());

  }

  entity_dL& scene_LightsManager::entity_dL_by_ID(uint32_t ID)
  {
    auto find_entity {directionalLights.L_pos.find(ID)};

    if (find_entity != directionalLights.L_pos.end())
    {
      return directionalLights.L_entities[find_entity->second];
    }

    std::string error_Log{"ERROR::DIRECTIONAL LIGHT NOT FOUND WITH THE (ID)"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
    return empty_entity_dL;
  }
  entity_pL& scene_LightsManager::entity_pL_by_ID(uint32_t ID)
  {
    auto find_entity {pointLights.L_pos.find(ID)};

    if (find_entity != pointLights.L_pos.end())
    {
      return pointLights.L_entities[find_entity->second];
    }

    std::string error_Log{"ERROR::POINT LIGHT NOT FOUND WITH THE (ID)"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
    return empty_entity_pL;
  }

  entity_sL& scene_LightsManager::entity_sL_by_ID(uint32_t ID)
  {
    auto find_entity {spotLights.L_pos.find(ID)};

    if (find_entity != spotLights.L_pos.end())
    {
      return spotLights.L_entities[find_entity->second];
    }

    std::string error_Log{"ERROR::SPOT LIGHT NOT FOUND WITH THE (ID)"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
    return empty_entity_sL;
  }

  entity_dL& scene_LightsManager::entity_dL_by_Pos(uint32_t pos)
  {
    if (pos < directionalLights.current_size_L)
    {
      return directionalLights.L_entities[pos];
    }

    std::string error_Log{ "ERROR::(POS) IS OUT OF SCOUPE TO FIND DIRECTIONAL LIGHT" };
    register_error_RM::register_error_withSentence(error_Log.c_str());
    return empty_entity_dL;
  }
  entity_pL& scene_LightsManager::entity_pL_by_Pos(uint32_t pos)
  {
    if (pos < pointLights.current_size_L)
    {
      return pointLights.L_entities[pos];
    }

    std::string error_Log{"ERROR::(POS) IS OUT OF SCOUPE TO FIND POINT LIGHT"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
    return empty_entity_pL;
  }
  entity_sL& scene_LightsManager::entity_sL_by_Pos(uint32_t pos)
  {
    if (pos < spotLights.current_size_L)
    {
      return spotLights.L_entities[pos];
    }

    std::string error_Log{"ERROR::(POS) IS OUT OF SCOUPE TO FIND SPOT LIGHT"};
    register_error_RM::register_error_withSentence(error_Log.c_str());
    return empty_entity_sL;
  }

  uint32_t& scene_LightsManager::num_Lights(light::typeLight light_T)
  {
    switch (light_T)
    {
      case light::typeLight::DIRECTIONAL_LIGHT :
      {
        return directionalLights.current_size_L;
        break;
      }

      case light::typeLight::POINT_LIGHT :
      {
        return pointLights.current_size_L;
        break;
      }

      case light::typeLight::SPOT_LIGHT :
      {
        return spotLights.current_size_L;
        break;
      }
    }

    return num_empty;
  }

  uint32_t scene_LightsManager::num_all_Lights()
  {
    return directionalLights.current_size_L + pointLights.current_size_L + spotLights.current_size_L;
  }

  const std::vector<entity_dL>& scene_LightsManager::out_entities_dL()
  {
    return directionalLights.L_entities;
  }
  const std::vector<entity_pL>& scene_LightsManager::out_entities_pL()
  {
    return pointLights.L_entities;
  }
  const std::vector<entity_sL>& scene_LightsManager::out_entities_sL()
  {
    return spotLights.L_entities;
  }

  void scene_LightsManager::render_point_dL()
  {
    if (point_geo.bufferBuild())
    {
      for (auto dL_e : directionalLights.L_entities)
      {
        const light::data_directionalLightPBR& data_dL {dL_e.dL_PBR_entity->out_Data()};

        glm::vec3 pos_dL {glm::normalize(-data_dL.Direction) * 5.0f};

        point_geo.setPosicion(pos_dL);
        point_geo.setColor(data_dL.Color);
        point_geo.draw();
      }
    }
  }

  void scene_LightsManager::render_point_pL()
  {
    for (auto& pL : pointLights.L_entities)
    {
      const light::data_pointLightPBR& data_pL {pL.pL_PBR_entity->out_Data()};

      point_geo.setPosicion(data_pL.Position);
      point_geo.setColor(data_pL.Color);
      point_geo.setSize(data_pL.size);
      point_geo.draw();
    }
  }
  void scene_LightsManager::render_point_sL()
  {
    for (auto& sL : spotLights.L_entities)
    {
      const light::data_SpotLightPBR& data_sL {sL.sL_PBR_entity->out_Data()};

      point_geo.setPosicion(data_sL.Position);
      point_geo.setColor(data_sL.Color);
      point_geo.draw();
    }

  }

  void scene_LightsManager::renderAll()
  {
     render_point_dL();
     render_point_pL();
     render_point_sL();
  }

  void scene_LightsManager::renderPrepassCS_point_dL(shading::shader& shader)
  {
    if (point_geo.bufferBuild())
    {
      for (auto dL_e : directionalLights.L_entities)
      {
        const light::data_directionalLightPBR& data_dL {dL_e.dL_PBR_entity->out_Data()};

        glm::vec3 pos_dL {glm::normalize(-data_dL.Direction) * 5.0f};

        point_geo.setPosicion(pos_dL);
        point_geo.setColor(data_dL.Color);
        point_geo.draw_PrePass_editMode(shader);
      }
    }
  }
  void scene_LightsManager::renderPrepassCS_point_pL(shading::shader& shader)
  {
    for (auto& pL : pointLights.L_entities)
    {
      const light::data_pointLightPBR& data_pL {pL.pL_PBR_entity->out_Data()};

      point_geo.setPosicion(data_pL.Position);
      point_geo.setColor(data_pL.Color);
      point_geo.setSize(data_pL.size);
      point_geo.draw_PrePass_editMode(shader);
    }
  }
  void scene_LightsManager::renderPrepassCS_point_sL(shading::shader& shader)
  {
    for (auto& sL : spotLights.L_entities)
    {
      const light::data_SpotLightPBR& data_sL {sL.sL_PBR_entity->out_Data()};

      point_geo.setPosicion(data_sL.Position);
      point_geo.setColor(data_sL.Color);
      point_geo.draw_PrePass_editMode(shader);
    }
  }
  void scene_LightsManager::renderPrepassCS_All(shading::shader& shader)
  {
    renderPrepassCS_point_dL(shader);
    renderPrepassCS_point_pL(shader);
    renderPrepassCS_point_sL(shader);
  }

  void scene_LightsManager::clear_data_dL()
  {
    directionalLights.L_pos.clear();
    directionalLights.pos_L_entity.clear();
    directionalLights.L_entities.clear();
    directionalLights.current_size_L = 0;
  }
  void scene_LightsManager::clear_data_pL()
  {
    pointLights.L_pos.clear();
    pointLights.pos_L_entity.clear();
    pointLights.L_entities.clear();
    pointLights.current_size_L = 0;
  }
  void scene_LightsManager::clear_data_sL()
  {
    spotLights.L_pos.clear();
    spotLights.pos_L_entity.clear();
    spotLights.L_entities.clear();
    spotLights.current_size_L = 0;
  }

  void scene_LightsManager::clearAll_data()
  {
    clear_data_dL();
    clear_data_pL();
    clear_data_sL();

    point_geo.clean_data();
  }

  scene_pointLights::scene_pointLights()
  {
    //lights_T::point_geo2D p2D {std::make_unique<geo_2D::point_geo>()};
    //point_geo = std::move(p2D);
  }
  scene_pointLights::~scene_pointLights()
  {
    clean_data();
  }
  void scene_pointLights::setPoint_geo(lights_T::point_geo2D point_geo)
  {
//   this->point_geo = nullptr;
   this->point_geo = point_geo;
  }

  void scene_pointLights::insert(lights_T::pLight_raw pL_entity, lights_T::pLight_PBR_raw pL_PBR_entity)
  {
    if (pL_entity != nullptr)
    {
      uint32_t& ID { pL_entity->ID };

      pL_pos.emplace(ID, current_size_M);
      pos_pL_entity.emplace_back(current_size_M);
      pL_entities.emplace_back(std::move(pL_entity), std::move(pL_PBR_entity));
      ++current_size_M;
    }
  }

  entity_pL* scene_pointLights::entity_by_ID(uint32_t ID)
  {
    auto find_pL {pL_pos.find(ID)};

    if (find_pL != pL_pos.end())
    {
      return &pL_entities[find_pL->second];
    }


    std::string error_log {"NOT FIND ENTITY WITH THE ID:: ID = " + std::to_string(ID)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }

  entity_pL* scene_pointLights::entity_by_Pos(uint32_t pos)
  {
    if (pos < current_size_M)
    {
      uint32_t& ID{ pos_pL_entity[pos] };
      return &pL_entities[ID];
    }

    std::string error_log {"NOT FIND ENTITY WITH THE POSICION:: POSICION = " + std::to_string(pos)};
    register_error_RM::register_error_withSentence(error_log.c_str());

    return nullptr;
  }

  const uint32_t& scene_pointLights::num_pointLights()
  {
   return current_size_M;
  }
  const std::vector<entity_pL>& scene_pointLights::out_entities()
  {
    return pL_entities;
  }

  void scene_pointLights::renderAll()
  {
    for (auto& pL : pL_entities)
    {
      point_geo.setPosicion(pL.pL_entity->Posicion);
      point_geo.setColor(pL.pL_entity->Color);
      point_geo.draw();
    }

  }

  void scene_pointLights::clean_data()
  {
   //for (uint32_t i = current_size_M; i >= 0; --i)
    // {
       //uint32_t p {i};
      // p = p & ~(current_size_M >> 31);

     /*
        if (i < current_size_M)
        {
          std::swap(pL_entities[0], pL_entities[i]);
          pL_entities[i].pL_entity = nullptr;
          pL_entities.pop_back();
        }
     }
     */
    pL_entities.clear();
    pL_pos.clear();
    pos_pL_entity.clear();
    current_size_M = 0;

    point_geo.clean_data();
  }

}



namespace register_error_RM
{
  void register_inexistence_Model_(const char* file, int line, uint32_t model_ID)
  {
    utilities::entity* entity_Model {RenderData_Set::ModelsScene_D->out_entity_model(model_ID)};

    std::string error_log{};

    if (entity_Model == nullptr)
    {
     error_log = "ERROR::INEXISTENCE MODEL::NOT FIND THE MODEL IN THE SCENE | " + std::string(file) + "---> line: " + std::to_string(line);
    }

    else if (entity_Model != nullptr)
    {
      error_log = "ERROR::INEXISTENCE MODEL IN FUNCTION | nameModel = " + entity_Model->model_entity->nameModel + " | " + std::string(file) + "---> line: " + std::to_string(line);
    }

    SDL_Log(error_log.c_str());
  }

  void register_inexistence_Mesh_(const char* file, int line, uint32_t model_ID, uint32_t mesh_ID)
  {
    utilities::entity* entity_Model {RenderData_Set::ModelsScene_D->out_entity_model(model_ID)};

    std::string error_log{};

    if (entity_Model == nullptr)
    {
      error_log = "ERROR::INEXISTENCE MODEL::NOT FIND THE MODEL IN THE SCENE | " + std::string(file) + "---> line: " + std::to_string(line);
    }

    else if (entity_Model != nullptr)
    {
      Assimp_D::Mesh& entity_Mesh {entity_Model->model_entity->outSpecificMesh(mesh_ID)};

      if (!entity_Mesh.nameMesh.empty() == true)
      {
        error_log = "ERROR::INEXISTENCE MESH IN FUNCTION | nameMesh = " + entity_Mesh.nameMesh + " | " + std::string(file) + "---> line: " + std::to_string(line);
      }

      else if (!entity_Mesh.nameMesh.empty() == false)
      {
        error_log = "ERROR::INEXISTENCE MESH::NOT FIND THE MESH IN THE SCENE | " + std::string(file) + "---> line: " + std::to_string(line);
      }

    }

    SDL_Log(error_log.c_str());
  }

  void register_error_withSentence_(const char* file, int line, const char* sentence)
  {
     std::string str_error {sentence};
     str_error += " | ";
     str_error += + file;
     str_error += "---> line: " + std::to_string(line);


    SDL_Log(str_error.c_str());


  }

}

