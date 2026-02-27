#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

#include "Model_Assimp/ModelAssimp.h"

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
    void render_nearPos(std::vector<uint32_t>& meshes_to_discard);
    void render_farPos(std::vector<uint32_t>& meshes_to_discard);

    void cleanAll_scene();
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