#ifndef ModelAssimp
#define ModelAssimp

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SHADER_H.h"
#include "stb_image.h"


namespace Assimp
{

	unsigned int TextureFromFile(const char* path, std::string directory, bool gamma = false);

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

		std::vector<vertexD> vertices{};
		std::vector<unsigned int> indices{};
		texture::textureBuild textures{};
		transformation_basics::basics_Model3D MeshCoord{};
		shaderSettings shaderSet{};

		Mesh();
		Mesh(std::vector<vertexD> ver, std::vector<unsigned int> indi, std::vector<textureD> texture);

		void Draw(camera::camera1 cam1, light::light1 light, shading::shader shader);
		void Draw_WithLights(camera::camera1 cam1, std::vector<light::light1>& pointLights, std::vector<light::DirectionalLight>& directionalLights, std::map<std::string, light::SpotLight>& spotLights, shading::shader shader);
		void Draw_Alone();
		void setMeshCoord(glm::vec3 posicionMesh, glm::vec3 scaleMesh);
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
		transformation_basics::basics_Model3D ModelCoord{};
		coordModel ModelGlobal_Coord{};

		Model();
		Model(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings, unsigned int processFlags);
		void Draw(camera::camera1 cam1, light::light1 light);
		void Draw_WL(camera::camera1 cam1, std::vector<light::light1>& pointLights, std::vector<light::DirectionalLight>& directionalLights, std::map<std::string, light::SpotLight>& spotLights);
		void destroyModel();

		void setModelCoord(coordModel modelCoords);
		void SetShinessTex_Mesh(int numMesh, float valueShiness);
		std::vector<Mesh>& outMeshes();
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
		std::unique_ptr<texture::textureBuild> textures{ nullptr };
		std::unique_ptr<shading::shader> shaders{ nullptr };
		std::unique_ptr<transformation_basics::basics_Model3D> MeshCoord{ nullptr };
		Assimp::shaderSettings shaderSet{};

		singleTriangle();
		singleTriangle(std::vector<Assimp::vertexD> vertex, texture::textureBuild& texture, shading::shader& shader);
		void setTriangle(std::vector<Assimp::vertexD> vertex, texture::textureBuild& texture, shading::shader& shader);
		void setMeshCoord(transformation_basics::basics_Model3D& Coord);
		void setShaderSettings(Assimp::shaderSettings shader);
		void draw();
		void destroy();
    };


}



#endif ModelAssimp