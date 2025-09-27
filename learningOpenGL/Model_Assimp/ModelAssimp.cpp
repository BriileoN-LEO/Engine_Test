
#include "ModelAssimp.h"


namespace Assimp
{

	Mesh::Mesh() {};
	Mesh::Mesh(std::vector<vertexD> vertices, std::vector<unsigned int> indices, std::vector<textureD> textures)
	{ 

	   //para setear las texturas
		for(int i = 0; i < static_cast<int>(textures.size()); i++)
		{
			textures.texU_Data.emplace_back(textures[i].id, textures[i].type, texture::textureUnits_Data[i]);

		}

	}
	Mesh::Mesh(std::vector<vertexD> vertices, std::vector<unsigned int> indices, texture::textureBuild textures, shading::shader shaders)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->shaders = shaders;

		setupMesh();
	}
	void Mesh::setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexD), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexD), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexD), (void*)offsetof(vertexD, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexD), (void*)offsetof(vertexD,TexCoord));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}
	void Mesh::Draw(camera::camera1* cam1, light::light1* light)
	{
		shaders.use();
		s
		shaders.transformMat("view", cam1->cam);
		shaders.transformMat("projection", cam1->camProjection);

		textures.useTextures_PerMaterial(&shaders);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

	/////

	void Model::loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << '\n';
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene.mRootNode, scene);
		////SEGUIR CON EL LOAD MODEL AQUI
	}
	void Model::processNode(aiNode* node, const aiScene* scene)
	{


	}
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<texture::textureBuild> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Model::Model(std::string path)
	{
		loadModel(path);
	}
	void Model::Draw(camera::camera1* cam1, light::light1* light)
	{
		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			meshes[i].Draw(*cam1, *light);
		}
		
	}
	
}