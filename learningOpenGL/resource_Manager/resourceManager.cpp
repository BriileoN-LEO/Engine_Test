#include "Model_Assimp/ModelAssimp.h"
#include "resource_Manager/resourceManager.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "Render/RenderData.h"
#include "2D_geo/basic_geometry_D.h"
#include "LIGHTS_test.h"


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


  manager_PointLights::manager_PointLights(){};

  void manager_PointLights::insert_PL(std::string nameStr, pointLight pL_D)
  {
    uint32_t hashID{FNV::str_to_hash(nameStr)};

    pL_find_pos.emplace_back(hashID);
    pL_find_str.emplace(nameStr, hashID);
    pointLight_D.emplace(hashID, std::move(pL_D));

    ++sizeContainer_PL;
  };
  pLight_raw manager_PointLights::pL_by_ID(uint32_t ID)
  {
    auto find_ID {pointLight_D.find(ID)};

    if (find_ID != pointLight_D.end())
    {
      return find_ID->second.get();
    }

    return nullptr;
  };
  pLight_raw manager_PointLights::pL_by_str(std::string str_ID)
  {
    auto find_Str {pL_find_str.find(str_ID)};

    if (find_Str != pL_find_str.end())
    {
      return pointLight_D[find_Str->second].get();
    }

  };
  pLight_raw manager_PointLights::pL_by_num(uint32_t pos)
  {
    if (pos <= sizeContainer_PL - 1)
    {
     return pointLight_D[pL_find_pos[pos]].get();
    }

    return nullptr;
  };

  const uint32_t& manager_PointLights::out_size()
  {
    return sizeContainer_PL;
  };

  void manager_PointLights::clean_data()
  {
    pL_find_pos.clear();
    pL_find_str.clear();
    pointLight_D.clear();
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
      uint32_t& pos {pos_entities[models_pos[model_ID]]}
;      return &models_entities[pos];
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


  void scene::render_nearPos(std::vector<uint32_t>& meshes_to_discard)
  {
    std::vector<render_data_D> render_meshes{};
    std::vector<render_entity> renderBlocking{};

    uint32_t pos_start_meshes{}; //0
    uint32_t pos_end_meshes{}; // 0

    uint32_t current_posModel{};

    for (auto& model_entity : models_entities) ///this render per model
    {
      const int& num_meshes { model_entity.model_entity->out_numSec_meshes() };

      for (int i = 0; i < num_meshes; i++)
      {
        Assimp_D::Mesh& out_mesh {model_entity.model_entity->out_MeshByPos(i)};

        auto find_mesh {std::find(meshes_to_discard.begin(), meshes_to_discard.end(), out_mesh.ID)}; //CHANGES THIS TO MORE FASTER SEARCH

        if (find_mesh == meshes_to_discard.end())
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

    for (auto& model_render : renderBlocking)
    {
      for (uint32_t renderP = model_render.meshes_start; renderP <= model_render.meshes_end; renderP++)
      {
        models_entities[model_render.model_pos].model_entity->Draw_singleMesh_ID(render_meshes[renderP].meshID, 1);  ///test if this render the mesh
      }

    }

  }
  void scene::render_farPos(std::vector<uint32_t>& meshes_to_discard)
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

namespace utilities_pointLight
{


  entity_pL::entity_pL(){};
  entity_pL::entity_pL(pLight_raw pL_entity) : pL_entity(std::move(pL_entity)){};

  scene_pointLights::scene_pointLights()
  {
    point_geo2D p2D {std::make_unique<geo_2D::point_geo>()};
    point_geo = std::move(p2D);
  }

  void scene_pointLights::setPoint_geo(point_geo2D point_geo)
  {
   this->point_geo = nullptr;
   this->point_geo = std::move(point_geo);
  }

  void scene_pointLights::insert(pLight_raw pL_entity)
  {

    if (pL_entity != nullptr)
    {
      uint32_t& ID {pL_entity->ID};

      pL_pos.emplace(ID, current_size_M);
      pos_pL_entity.emplace_back(ID);
      pL_entities.emplace_back(std::move(pL_entity));
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

    return nullptr;
  }

  entity_pL* scene_pointLights::entity_by_Pos(uint32_t pos)
  {
    if (pos < current_size_M)
    {
      uint32_t& ID{ pos_pL_entity[pos] };
      return &pL_entities[ID];
    }

    return nullptr;
  }

  const uint32_t& scene_pointLights::num_pointLights()
  {
   return current_size_M;
  }

  void scene_pointLights::renderAll()
  {
    for (auto& pL : pL_entities)
    {
      point_geo->setPosicion(pL.pL_entity->Posicion);
      point_geo->setColor(pL.pL_entity->Color);
      point_geo->draw();
    }

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

