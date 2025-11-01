#ifndef ModelAssimp
#define ModelAssimp

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SHADER_H.h"
#include "stb_image.h"


namespace Assimp
{

	unsigned int TextureFromFile(const char* path, std::string directory = "", bool gamma = false);

	enum class renderSeq
	{
		renderNear = 0,
		renderFar = 1
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
		bool changeStateSelection{};
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

	class Mesh
	{
	private:

		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};
		void setupMesh();

	public:

		std::string nameMesh{}; ///Este nombre servira para linkear con las acciones del bounding box
		std::vector<vertexD> vertices{};
		std::vector<unsigned int> indices{};
		texture::textureBuild textures{};

		transformation_basics::basics_Model3D MeshCoord{};
		std::vector<glm::vec3> verticesPos{};
		shaderSettings shaderSet{};

		renderSeq renderP{};

		Mesh();
		Mesh(std::vector<vertexD> ver, std::vector<unsigned int> indi, std::vector<textureD> texture);

		void Draw(camera::camera1 cam1, light::light1 light, shading::shader shader);
		void Draw_WithLights(shading::shader& shader);
		void Draw_Alone();
		void Draw_WithoutModel(shading::shader& shader);
		void build_PreDraw(shading::shader& shader);

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

		void loadModel(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings, unsigned int processFlags);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<textureD> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	public:

		std::string nameModel{};	//Nombre global del modelo
		transformation_basics::basics_Model3D ModelCoord{};
		coordModel ModelGlobal_Coord{};

		Model();
		Model(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings, unsigned int processFlags);
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

		void SetTexture_Mesh(const char* pathTexture, std::string nameMesh, texture::typeTextures tex);
	
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
		std::vector<Assimp::vertexD> vertex{};
		texture::textureBuild texture{};
		shading::shader shader{};
		transformation_basics::basics_Model3D MeshCoord{};
		glm::vec3 centroidTriangle{};
		
		Assimp::shaderSettings shaderSet{};
		Assimp::structModelName name{};

		singleTriangle();
		singleTriangle(std::vector<Assimp::vertexD> vertex);
		void setTriangle(std::vector<Assimp::vertexD> vertex);
		void insertTriangle();
		void setShaderSettings(Assimp::shaderSettings shader);

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

		Assimp::structModelName name{};
		std::vector<Assimp::structMesh_Data> setDataMesh_Multi{};
		bool ActiveMesh{ false };

		Multiple_AssimpMesh();
		Multiple_AssimpMesh(Assimp::structModelName meshToCopy, std::vector<glm::vec3> quantityMesh);

		void setMultipleMesh(Assimp::structModelName meshToCopy, std::vector<glm::vec3> quantityMesh);
		void drawMultipleMesh();
		
		


	};


}




#endif ModelAssimp