#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

//#include "Model_Assimp/ModelAssimp.h"
#include "learningOpenGL.h"
#include "2D_geo/basic_geometry_D.h"

namespace data_Manager
{
  uint32_t find_and_remplace_str(const std::vector<uint32_t>& dataContainer, std::string& str_data); ////IT GETS A NEW HASHID IF REQUIERES AND MODIFIES THE STRING

};

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

namespace lights_T
{
  using directionalLight_PBR = std::unique_ptr<light::directionalLight_PBR>;
  using pointLight = std::unique_ptr<light::light1>;
  using pointLight_PBR= std::unique_ptr<light::pointLight_PBR>;
  using spotLight_PBR = std::unique_ptr<light::SpotLight_PBR>;

  using point_geo2D = geo_2D::point_geo;

  using dLight_PBR_raw = light::directionalLight_PBR*;
  using pLight_raw = light::light1*;
  using pLight_PBR_raw = light::pointLight_PBR*;
  using sLight_PBR_raw = light::SpotLight_PBR*;

}
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
   void delete_obj(uint32_t& pos);
   bool find_mesh(uint32_t& meshID);
   uint32_t& out_size_eObjs();
   const std::vector<Assimp_D::excluded_Obj>& out_eObj_vec();

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
    void delete_excluded_Obj(ControlScenarios::stateScenarios scene, uint32_t pos);

    bool find_existence_mesh(ControlScenarios::stateScenarios scene, uint32_t& meshID);

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

    class manager_DirectionalLights
    {
    private:

      std::vector<uint32_t> dL_find_pos{};
      std::unordered_map<std::string, uint32_t> dL_find_str{};
      std::unordered_map<uint32_t, lights_T::directionalLight_PBR> directionalLight_PBR_D{};

      uint32_t sizeContainer_dL{};

    public:

       manager_DirectionalLights();

       void insert_DL(std::string nameStr, lights_T::directionalLight_PBR dL_D);

       lights_T::dLight_PBR_raw pL_by_ID(uint32_t ID);
       lights_T::dLight_PBR_raw pL_by_str(std::string str_ID);
       lights_T::dLight_PBR_raw pL_by_num(uint32_t pos);

       const uint32_t& out_size();

       void clean_data();
      //////////CONTINUE HERE
    };

    class manager_PointLights
    {
    private:

      std::vector<uint32_t> pL_find_pos{};
      std::unordered_map<std::string, uint32_t> pL_find_str{};

      std::unordered_map<uint32_t, lights_T::pointLight> pointLight_D{}; //pointLight is unique_ptr
      std::unordered_map<uint32_t, lights_T::pointLight_PBR> pointLight_PBR_D{}; //pointLight_PBR is unique_ptr, HERE IMPLEMENTE OTHER POINT LIGHT DATA TO PBR

      uint32_t sizeContainer_PL{};

    public:

    manager_PointLights();

    void insert_PL(std::string nameStr, lights_T::pointLight pL_D, lights_T::pointLight_PBR pL_PBR_D);
    lights_T::pLight_raw pL_by_ID(uint32_t ID);
    lights_T::pLight_raw pL_by_str(std::string str_ID);
    lights_T::pLight_raw pL_by_num(uint32_t pos);

    lights_T::pLight_PBR_raw pL_PBR_by_ID(uint32_t ID);
    lights_T::pLight_PBR_raw pL_PBR_by_str(std::string str_ID);
    lights_T::pLight_PBR_raw pL_PBR_by_num(uint32_t pos);

    const uint32_t& out_size();

    void clean_data();
    };

    class manager_SpotLights
    {
    private:
      std::vector<uint32_t> sL_find_pos{};     ////ITS SORTED THE ORDER
      std::unordered_map<std::string, uint32_t> sL_find_str{};
      std::unordered_map<uint32_t, lights_T::spotLight_PBR> spotLight_PBR_D{};

      uint32_t sizeContainer_sL{};

    public:

     manager_SpotLights();

     void insert_sL(std::string nameStr, lights_T::spotLight_PBR sL_D);

     lights_T::sLight_PBR_raw pL_by_ID(uint32_t ID);  ////FIND BY BYNARY SEARCH
     lights_T::sLight_PBR_raw pL_by_str(std::string str_ID);
     lights_T::sLight_PBR_raw pL_by_num(uint32_t pos);

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

    std::vector<uint32_t> ordered_allMeshes{};  ////to find if I discard some meshes

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

    void render_nearPos_depthMapShadow(std::string* shader_ID);
    void render_nearPos();
    void render_farPos();

    void render_nearPos_shadows(std::vector<uint32_t>& meshes_to_discard, shading::shader* shaderShadow); ///TO RENDER NEAR OBJECTS TO SHADOW MAP
    void render_farPos_shadows(std::vector<uint32_t>& meshes_to_discard,  shading::shader* shaderShadow); ///TO RENDER FAR OBJECTS TO SHADOW MAP

    void cleanAll_scene();
  };

}

namespace utilities_Lights
{
  extern uint32_t num_empty;

  struct entity_dL
  {
    lights_T::dLight_PBR_raw dL_PBR_entity { nullptr };

    entity_dL();
    entity_dL(lights_T::dLight_PBR_raw dL_PBR_entity);
    ~entity_dL();

    entity_dL(const entity_dL&& pL_entity_d) noexcept;
    entity_dL(const entity_dL& pL_entity_d);

  };

  struct entity_pL
  {
    lights_T::pLight_raw pL_entity{ nullptr };
    lights_T::pLight_PBR_raw pL_PBR_entity{ nullptr };

    entity_pL();
    entity_pL(lights_T::pLight_raw pL_entity, lights_T::pLight_PBR_raw pL_PBR_entity);
    entity_pL(lights_T::pLight_PBR_raw pL_PBR_entity);
    ~entity_pL();

    entity_pL(const entity_pL&& pL_entity_d) noexcept;
    entity_pL(const entity_pL& pL_entity_d);

  };

  struct entity_sL
  {
    lights_T::sLight_PBR_raw sL_PBR_entity { nullptr };

    entity_sL();
    entity_sL(lights_T::sLight_PBR_raw sL_PBR_entity);
    ~entity_sL();

    entity_sL(const entity_sL&& pL_entity_d) noexcept;
    entity_sL(const entity_sL& pL_entity_d);
  };

  extern entity_dL empty_entity_dL;
  extern entity_pL empty_entity_pL;
  extern entity_sL empty_entity_sL;

  template<typename entity_L>
  struct lights_data
  {
    std::unordered_map<uint32_t, uint32_t> L_pos{};
    std::vector<uint32_t> pos_L_entity{};
    std::vector<entity_L> L_entities{};
    uint32_t current_size_L{};
  };

  class scene_LightsManager {
  private :

    lights_data<entity_dL> directionalLights{};
    lights_data<entity_pL> pointLights{};
    lights_data<entity_sL> spotLights{};

    lights_T::point_geo2D point_geo{};

  public:

    scene_LightsManager();
    ~scene_LightsManager();

    void setPoint_geo(lights_T::point_geo2D point_geo);   ////THIS COULD BE DISCARTED
    void buildPoint_geo(std::string shaderID);

    void insert_dL(lights_T::dLight_PBR_raw dL_PBR);
    void insert_pL(lights_T::pLight_PBR_raw pL_PBR);
    void insert_pLights_two(lights_T::pLight_raw pL, lights_T::pLight_PBR_raw pL_PBR);
    void insert_sL(lights_T::sLight_PBR_raw sL_PBR);

    entity_dL& entity_dL_by_ID(uint32_t ID);
    entity_pL& entity_pL_by_ID(uint32_t ID);
    entity_sL& entity_sL_by_ID(uint32_t ID);

    entity_dL& entity_dL_by_Pos(uint32_t pos);
    entity_pL& entity_pL_by_Pos(uint32_t pos);
    entity_sL& entity_sL_by_Pos(uint32_t pos);

    uint32_t& num_Lights(light::typeLight light_T);

    const std::vector<entity_dL>& out_entities_dL();
    const std::vector<entity_pL>& out_entities_pL();
    const std::vector<entity_sL>& out_entities_sL();

    void render_point_dL();
    void render_point_pL();
    void render_point_sL();
    void renderAll();

    void clear_data_dL();
    void clear_data_pL();
    void clear_data_sL();
    void clearAll_data();

  };

  class scene_pointLights
  {
  private:

    std::unordered_map<uint32_t, uint32_t> pL_pos{};
    std::vector<uint32_t> pos_pL_entity{};
    std::vector<entity_pL> pL_entities{};
    uint32_t current_size_M{};

    lights_T::point_geo2D point_geo{};

  public:

   scene_pointLights();
   ~scene_pointLights();

   void setPoint_geo(lights_T::point_geo2D point_geo); //INSERTAR EL PUNTO
   void insert(lights_T::pLight_raw pL_entity, lights_T::pLight_PBR_raw pL_PBR_entity);

   entity_pL* entity_by_ID(uint32_t ID);
   entity_pL* entity_by_Pos(uint32_t pos);
   const uint32_t& num_pointLights();
   const std::vector<entity_pL>& out_entities();

   void renderAll();
   void clean_data();
  };


  extern bool stateLights_data;  ////THIS CONTROLS THE STATE IF ONE OF ALL LIGHTS CHANGE ONE PARAMETER TO RESTORE THE PARAMETERS IN THE SSBO OF THE CLUSTER RENDERING.
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