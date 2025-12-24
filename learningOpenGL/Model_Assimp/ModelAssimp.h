#ifndef ModelAssimp
#define ModelAssimp

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SHADER_H.h"
#include "stb_image.h"
#include "textureData/textureManager.h"
#include <thread>
#include <stdexcept>
#include <functional>
#include <atomic>
#include <queue>
#include <mutex>


namespace sky
{
	struct transformation_Skybox
	{
		glm::vec3 pivot_rot{0.0f, 1.0f, 0.0f};
		float rad{};

		glm::mat3 rotationVec{};
	};

	class cubeMap_Skybox
	{
	private:

		std::string name{};
		std::string nameShader{};

		unsigned int VAO{};
		unsigned int VBO{};

		unsigned int textureID{};


	public:

		transformation_Skybox transform_SkyBox{};
		std::vector<float> skyBox_Vertices{};

		cubeMap_Skybox();
		cubeMap_Skybox(std::string name, std::string directory_Tex, std::vector<std::string> nameFiles, std::string nameShader);
		
		void loadTexture_Skybox_Seq(std::string directory_Tex, std::vector<std::string> nameFiles);
		void loadCube();
		void draw_Skybox();
		void bind_Texture(shading::shader& shader, std::string nameToBind, texture::textureUnits activeTexturePos);

	};


}


namespace Assimp_D
{
	////////////

	unsigned int TextureFromFile(const char* path, std::string directory = "", bool gamma = false);
	unsigned int TextureFromData(unsigned char* dataTexture, int width, int height, int nrChannels);

	enum class renderSeq
	{
		renderNear = 0,
		renderFar = 1
	};
	enum class excludedOP
	{
		exclude_complete_model = 0,
		exclude_only_meshes = 1,

	};

	struct excluded_Obj
	{
		excludedOP exclude_Type{};
		std::string nameModel{};
		std::vector<std::string> nameMeshes{};
	
	};

	struct structMesh_Data
	{
		std::string subNameMesh{};
		glm::vec3 posicion{};
		std::vector<glm::vec3> verticesPos{};
		glm::mat4 model{};
	};

	struct structModelName
	{
		std::string nameModel{};
		std::string nameMesh{};
		[[maybe_unused]] bool changeStateSelection{};

		void reset();
	};

	struct shaderSettings
	{
		glm::vec3 objectColor{};
		/// MATERIALS 
		glm::vec3 ambient{};
		glm::vec3 difusse{};
		glm::vec3 specular{};
		float shiness{};
	};

	struct coordModel
	{
		glm::vec3 posicion{};
		glm::vec3 scale{};
		glm::vec3 pivotRot{};
		GLfloat angRot{};
	};

	struct vertexD
	{
		glm::vec3 posicion{};
		glm::vec3 Normal{};
		glm::vec2 TexCoord{};
	};

	struct textureD
	{
		unsigned int id{};
		std::string type{};
		aiString path{};
	};


	namespace loadToCPU
	{
		//NUEVA FORMA DE CARGAR MODELOS DESDE LA CPU ANTES DE QUE SE INICIE EL THREAD DE OPENGL
		////////////////

		struct MeshData_loadCPU
		{
			std::vector<vertexD> vertices{};
			std::vector<unsigned int> indices{};
			std::vector<texDataManager::TextureData_File> textures{};
		};

		struct ModelData_loadCPU
		{
			std::string nameModel{};
			std::string nameShader{};
			std::string directory{};
			std::vector<MeshData_loadCPU> Meshes_LoadCPU{};
		};

		struct insertProcessModel
		{
			std::string nameModel{};
			std::string path{};
			std::string nameShader{};
			unsigned int flagsProcessModel{};

		};

		extern std::queue<ModelData_loadCPU> modelsData;
		extern std::atomic<int> atomic_CounterModel;
		extern std::atomic<int> atomic_sizeModels;
		extern std::atomic<bool> flagsAtomic;
		extern std::atomic<bool> finishLoadModels;
		extern std::mutex mutexModel;

		extern std::condition_variable manageImporter;
		extern std::mutex mutexImporter;
		extern Assimp::Importer importer;


		texDataManager::TextureData_File LoadTextureFromFile(const char* path, std::string directory = "", std::string typeTexture = "", bool gamma = false);

		std::vector<texDataManager::TextureData_File> loadMatTextures(aiMaterial* mat, aiTextureType matType, std::string typeName, std::string directory); ///FUNCTION LEGACY::LOAD TEXTURES
		std::vector<texDataManager::TextureData_File> loadMatTextures_Cache(aiMaterial* mat, aiTextureType matType, std::string typeName, std::string directory);

		void loadModelsThread(std::queue<insertProcessModel> models);

		ModelData_loadCPU processModel(insertProcessModel dataModel);
		void processNode(aiNode* node, const aiScene* scene, std::vector<MeshData_loadCPU>& meshes, std::string directory);
		MeshData_loadCPU processMesh(aiMesh* mesh, const aiScene* scene, std::string directory);


	}

	class Mesh
	{
	private:

		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};
		void setupMesh();
		void setupMesh_subBuffer();

	public:

		std::string nameMesh{}; ///Este nombre servira para linkear con las acciones del bounding box
		std::vector<vertexD> vertices{};
		std::vector<unsigned int> indices{};
		//texture::textureBuild textures{};
		textureCache::texture_Data textures{};

		transformation_basics::basics_Model3D MeshCoord{};
		std::vector<glm::vec3> verticesPos{};
		std::vector<glm::vec3> normalsPos{}; ///ESTA ES LA POSICION GLOBAL DE LAS NORMALES
		shaderSettings shaderSet{};
		shading::config::shaderConfig settingsShader{};

		renderSeq renderP{};

		Mesh();
		Mesh(std::vector<vertexD> ver, std::vector<unsigned int> indi, std::vector<textureD> texture);

		Mesh(Assimp_D::loadToCPU::MeshData_loadCPU loadData);

		void Draw(camera::camera1 cam1, light::light1 light, shading::shader shader);
		void Draw_WithLights(shading::shader& shader);
		void Draw_Alone();
		void Draw_WithoutModel(shading::shader& shader);
		void build_PreDraw(shading::shader& shader);
		unsigned int& outVAO();

		void setMeshCoord(glm::vec3 posicionMesh, glm::vec3 scaleMesh);
		void updateVerticesPos();
		void destroyMesh();

	};

	class Model
	{
	private:

		std::vector<Mesh> meshes{};
		std::string directory{};
		std::vector<textureD> textures_Loaded{};
		shading::shader shaders{};

		void loadModel_CPU(Assimp_D::loadToCPU::ModelData_loadCPU model);
		void loadModel(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings, unsigned int processFlags);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<textureD> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	public:

		std::string nameModel{};
		std::string nameShader{};   //Nombre global del modelo
		transformation_basics::basics_Model3D ModelCoord{};
		coordModel ModelGlobal_Coord{};


		Model();
		Model(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings, unsigned int processFlags);
		Model(Assimp_D::loadToCPU::ModelData_loadCPU model);
		
		void setModelSettings(coordModel modelCoords, shaderSettings shaderSettings);
		void Draw(camera::camera1 cam1, light::light1 light);
		void Draw_WL();
		void DrawSingleMesh(std::string nameMesh, int shaderOp);
		void DrawExcludeMesh(std::string nameMesh);
		void destroyModel();

		void updateModel();
		void setNameModel(const std::string name);
		void setModelCoord(coordModel modelCoords);
		void SetShinessTex_Mesh(int numMesh, float valueShiness);
		void SetOrderRender_Mesh(const std::string nameMesh, renderSeq renderOrder);
		void BlendModeTexture_Mesh(const std::string nameMesh, bool op);

		void SetTexture_Mesh(const char* pathTexture, std::string nameMesh, texDataManager::typeTexture tex);
		void loadTemporalShaders(const char* vertexPath, const char* fragmentPath);
	
		std::vector<Mesh>& outMeshes();
		shading::shader& outShader();
		int numMeshes();
		void refresh_ModelCoord();
	};

};

namespace individualComp
{
 
	class singleTriangle
	{
	public:

		unsigned int VAO{};
		unsigned int VBO{};
		std::vector<Assimp_D::vertexD> vertex{};
		//texture::textureBuild texture{}; //OLD WAY TO LOAD TEXTURES TO TRIANGLE
		textureCache::texture_Data texture{};
		shading::shader shader{};
		transformation_basics::basics_Model3D MeshCoord{};
		glm::vec3 centroidTriangle{};
		
		Assimp_D::shaderSettings shaderSet{};
		Assimp_D::structModelName name{};

		singleTriangle();
		singleTriangle(std::vector<Assimp_D::vertexD> vertex);
		 
		void setTriangle(std::vector<Assimp_D::vertexD> vertex);
		void insertTriangle();
		void setShaderSettings(Assimp_D::shaderSettings shader);

		void draw();
		void drawTest_2();
		void drawSelection();

		void updateTexture();
		void updateModel();
		void destroy();
    };

	class Multiple_AssimpMesh
	{
	public:

		Assimp_D::structModelName name{};
		std::vector<Assimp_D::structMesh_Data> setDataMesh_Multi{};
		bool ActiveMesh{ false };

		Multiple_AssimpMesh();
		Multiple_AssimpMesh(Assimp_D::structModelName meshToCopy, std::vector<glm::vec3> quantityMesh);

		void setMultipleMesh(Assimp_D::structModelName meshToCopy, std::vector<glm::vec3> quantityMesh);
		void drawMultipleMesh();
		
		


	};


}




#endif ModelAssimp