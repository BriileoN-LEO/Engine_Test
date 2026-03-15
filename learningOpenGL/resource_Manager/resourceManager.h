#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

//#include "Model_Assimp/ModelAssimp.h"
#include "learningOpenGL.h"

namespace geo_2D
{
   class point_geo;
}

namespace Assimp_D
{
  namespace loadToCPU
  {
    struct ModelData_loadCPU;
  }

 class mesh;
 class model;

}

namespace light
{
  class light1;
}

using pointLight = std::unique_ptr<light::light1>;
using point_geo2D = std::unique_ptr<geo_2D::point_geo>;

using pLight_raw = light::light1*;
//using pGeo2D_raw = geo_2D::point_geo*;

namespace discard_objs  ///////////CONTINUE WITH DISCARD SCENARIOS
{
  class objs_Discard
  {
  private:
   std::vector<Assimp_D::excluded_Obj> e_obj{};
   uint32_t size_eObj{};

   std::unordered_map<uint32_t, uint32_t> indices_MeshDiscard{};
   std::vector<uint32_t> meshes_discard{};
   uint32_t size_meshes{};

   void update_meshes();
   void add_meshes_pos(uint32_t& pos);
   void delete_meshes_pos(uint32_t& pos);

  public:
   objs_Discard();
   objs_Discard(std::vector<Assimp_D::excluded_Obj>& e_obj);
   ~objs_Discard();

   void remplace_obj(uint32_t& pos, Assimp_D::excluded_Obj& obj_remplace);
   bool find_mesh(uint32_t& meshID);
  };

  class discard_objs_scenario
  {
  private:

     std::unordered_set<ControlScenarios::stateScenarios> research_discard{};
     std::unordered_map<ControlScenarios::stateScenarios, std::unique_ptr<objs_Discard>> obj_discard{};

  public:

    discard_objs_scenario();
    void insert_objs_Discard(ControlScenarios::stateScenarios scene, std::vector<Assimp_D::excluded_Obj> obj_to_discard);
    void remplace_excluded_Obj(ControlScenarios::stateScenarios scene, uint32_t pos, Assimp_D::excluded_Obj obj_remplace);


  };


}

namespace resourceManager
{
    class manager_Model
    {
    private:

        std::vector<uint32_t> ID_models{};
        std::unordered_map<std::string, uint32_t> models_find_ID {};
        std::unordered_map<uint32_t, std::unique_ptr<Assimp_D::Model>> models_D{};

    public:

        manager_Model();

        void reserve_size(int size_r);

        void insertModel (std::string nameStr, Assimp_D::loadToCPU::ModelData_loadCPU& model_info);
        Assimp_D::Model* model_by_ID(uint32_t ID);
        Assimp_D::Model* model_by_str(std::string str_v);
        Assimp_D::Model* model_by_num(int pos);
        const std::unordered_map<std::string, uint32_t>& out_ModelsID();

        std::string get_nameModel(uint32_t ID);/// check if the const throws error
        int size_models_D();

        void clean_data();
    };

    class manager_PointLights
    {
    private:

      std::vector<uint32_t> pL_find_pos{};
      std::unordered_map<std::string, uint32_t> pL_find_str{};
      std::unordered_map<uint32_t, pointLight> pointLight_D{}; //pointLight is unique_ptr

      uint32_t sizeContainer_PL{};

    public:

    manager_PointLights();

    void insert_PL(std::string nameStr, pointLight pL_D);
    pLight_raw pL_by_ID(uint32_t ID);
    pLight_raw pL_by_str(std::string str_ID);
    pLight_raw pL_by_num(uint32_t pos);

    const uint32_t& out_size();

    void clean_data();
    };
}

namespace utilities
{
  struct render_data_D
  {
    uint32_t meshID{};
    double dist{};

  };

  struct render_entity
  {
    uint32_t model_pos{};
    uint32_t meshes_start{};
    uint32_t meshes_end{};
    double model_dist{};
  };

  struct entity
  {
    Assimp_D::Model* model_entity{nullptr};

    entity();
    entity(Assimp_D::Model* model_entity);
  };

  class scene
  {
  private :
    std::unordered_map<uint32_t, uint32_t> models_pos{};
    std::vector<uint32_t> pos_entities{};
    std::vector<entity> models_entities{};
    uint32_t current_size_M{};

    std::vector<uint32_t> ordered_allMeshes{};  ////to find if i discard some meshes

    std::vector<render_data_D> render_meshes{}; ///TO SAVE ALL THE ID OF MESHES IN THE SCENE
    std::vector<render_entity> renderBlocking{}; //TO SAVE THE CORRECTO POSICION OF EACH MESH INSIDE OF THE MODEL ID

  public :

    scene();

    void insert_entity_model(Assimp_D::Model* model_entity);
    entity* out_entity_model(uint32_t model_ID);
    entity* out_entity_model_byPos(uint32_t pos);
    std::vector<entity>& out_entities();
    Assimp_D::Mesh* out_mesh_fromModel(uint32_t model_ID, uint32_t mesh_ID);
    Assimp_D::Mesh* out_mesh_fromID(uint32_t mesh_ID);
    Assimp_D::Mesh* out_mesh_fromNameMesh(std::string nameMesh);
    const Assimp_D::shader_SetType& out_shaderSet_fromModel(uint32_t model_ID, Assimp_D::shader_type shader_t);
    const uint32_t& size_VM();

    void renderAll();
    void render_singleModel(uint32_t model_ID);
    void render_singleMesh(uint32_t model_ID, uint32_t mesh_ID, int shaderOP);

    void order_MeshesID();

    void order_nearPosMeshes(); ///TO ORDER THE POSICION ONCE TO DRAW AFTER
    void order_farPosMeshes();

    void render_nearPos(std::vector<uint32_t>& meshes_to_discard);
    void render_farPos(std::vector<uint32_t>& meshes_to_discard);

    void render_nearPos_shadows(std::vector<uint32_t>& meshes_to_discard, shading::shader* shaderShadow); ///TO RENDER NEAR OBJECTS TO SHADOW MAP
    void render_farPos_shadows(std::vector<uint32_t>& meshes_to_discard,  shading::shader* shaderShadow); ///TO RENDER FAR OBJECTS TO SHADOW MAP

    void cleanAll_scene();
  };

}

namespace utilities_pointLight
{
  struct entity_pL
  {
   pLight_raw pL_entity{nullptr};

    entity_pL();
    entity_pL(pLight_raw pL_entity);
    ~entity_pL();
  };

  class scene_pointLights
  {
  private:

    std::unordered_map<uint32_t, uint32_t> pL_pos{};
    std::vector<uint32_t> pos_pL_entity{};
    std::vector<entity_pL> pL_entities{};
    uint32_t current_size_M{};

    point_geo2D point_geo{ nullptr };

  public:

   scene_pointLights();

   void setPoint_geo(point_geo2D point_geo); //INSERTAR EL PUNTO
   void insert(pLight_raw pL_entity);

   entity_pL* entity_by_ID(uint32_t ID);
   entity_pL* entity_by_Pos(uint32_t pos);
   const uint32_t& num_pointLights();

   void renderAll();
   void clean_data();
  };


}

namespace register_error_RM
{
    void register_inexistence_Model_(const char* file, int line, uint32_t model_ID);
#define register_inexistence_Model(id) register_inexistence_Model_(__FILE__, __LINE__, id)

    void register_inexistence_Mesh_(const char* file, int line, uint32_t model_ID, uint32_t mesh_ID);
#define register_inexistence_Mesh(model_id, mesh_id) register_inexistence_Mesh_(__FILE__, __LINE__, model_id, mesh_id)

    void register_error_withSentence_(const char* file, int line, const char* sentence);
#define register_error_withSentence(sen) register_error_withSentence_(__FILE__, __LINE__, sen)

}


#endif //RESOURCE_MANAGER