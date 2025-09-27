#ifndef ModelAssimp
#define ModelAssimp

#include "SHADER.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Assimp
{

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
		shading::shader shaders{};
		transformation_basics::basics_Model3D modelCoord{};

		Mesh();
		Mesh(std::vector<vertexD> vertices, std::vector<unsigned int> indices, std::vector<textureD> textures);
		Mesh(std::vector<vertexD> vertices, std::vector<unsigned int> indices, texture::textureBuild textures, shading::shader shaders);

		void Draw(camera::camera1* cam1, light::light1* light);

	};
	
	class Model
	{
	private:

		std::vector<Mesh> meshes{};
		std::string directory{};

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<texture::textureBuild> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	public:
		Model(std::string path);
		void Draw(camera::camera1* cam1, light::light1* light);
	};


}



#endif ModelAssimp