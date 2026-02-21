//
// Created by brii on 16/02/26.
//

#include "resourceManager.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
#include "Render/RenderData.h"

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
    models_D.emplace(hashID, std::make_unique<Assimp_D::Model>(Assimp_D::Model(model_info)));
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
  Assimp_D::Model* manager_Model::model_by_str(std::string_view str_v)
  {
    auto find_str {models_find_ID.find(std::string(str_v))};

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
     if (static_cast<int>(ID_models.size() - 1) < pos)
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



}

namespace utilities
{

    entity::entity() = default;
    entity::entity(Assimp_D::Model* model_entity) : model_entity(std::move(model_entity)){};

  scene::scene() = default;

  void scene::insert_entity_model(Assimp_D::Model* model_entity)
  {
    if (model_entity != nullptr)
    {
      models_entities.emplace_back(std::move(model_entity));
    }

  };
  entity* scene::out_entity_model(uint32_t model_ID)
  {
    auto find_model = std::ranges::find_if(models_entities,
      [&](const entity& model) {
        return model.model_entity->ID == model_ID;
      });

    if (find_model != std::ranges::end(models_entities))
    {
      return std::to_address(find_model);  ///this probably get an error, so detects
    }

    else if (find_model == std::ranges::end(models_entities))
    {
      SDL_Log("ERROR::NOT FIND MODEL | resourceManager.cpp ---> line 95");
    }

    return nullptr;
  }
  Assimp_D::Mesh* scene::out_mesh_fromModel(uint32_t model_ID, uint32_t mesh_ID)
  {
    auto find_model = std::ranges::find_if(models_entities,
      [&](entity& entity_m)
      {return entity_m.model_entity->ID == model_ID;}
      );

    if (find_model != std::ranges::end(models_entities))
    {
       Assimp_D::Mesh* find_mesh {new Assimp_D::Mesh(find_model->model_entity->outSpecificMesh(mesh_ID))};  ///TENER CUIDADO SI FALLA ESTA LINEA

      if (find_mesh->ID != 0)
      {
        return std::move(find_mesh);
      }

      else if (find_mesh->ID == 0)
      {
        SDL_Log("POINTER_ERROR::NOT FIND MESH | resourceManager.cpp ---> scene::out_mesh_fromModel");
      }

    }

    else if (find_model == std::ranges::end(models_entities))
    {
      SDL_Log("POINTER_ERROR::NOT FIND MODEL | resourceManager.cpp ---> scene::out_mesh_fromModel");
    }

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

