
#include "ModelAssimp.h"


namespace Assimp
{
	unsigned int TextureFromFile(const char* path, std::string directory, bool gamma)
	{
		std::string pa= std::string(path);
		pa = directory + "/" + pa;
		SDL_Log(pa.c_str());

		int width;
		int height;
		int nrChannels;
		unsigned int texID;

		glGenTextures(1, &texID);

		unsigned char* DataT= stbi_load(pa.c_str(), &width, &height, &nrChannels, 0);

		stbi_set_flip_vertically_on_load(true);
	

		if (DataT)
		{

			GLenum format{};
			if (nrChannels == 1)
			{
				format = GL_RED;

			}

			if (nrChannels == 3)
			{
				format = GL_RGB;

			}

			if (nrChannels == 4)
			{
				format = GL_RGBA;

			}
		
			    glBindTexture(GL_TEXTURE_2D, texID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, DataT);
				glGenerateMipmap(GL_TEXTURE_2D);
			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(DataT);
		}
		
		else
		{
			SDL_Log("ERROR::LOAD::TEXTURE");
			stbi_image_free(DataT);
		}


		return texID;
	}

	Mesh::Mesh() {};
	Mesh::Mesh(std::vector<vertexD> ver, std::vector<unsigned int> ind, std::vector<textureD> texture)
	{ 
		vertices = ver;
		indices = ind;
		
	   //para setear las texturas
		for(int i = 0; i < static_cast<int>(texture.size()); i++)
		{
			textures.texU_Data.emplace_back(texture[i].id, texture[i].type, texture::textureUnits_Data[i]);
		}

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
		
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);



	}
	void Mesh::Draw(camera::camera1 cam1, light::light1 light, shading::shader shader)
	{
		shader.use();

		shader.transformMat("model", MeshCoord.model);
		shader.transformMat("view", cam1.cam);
		shader.transformMat("projection", cam1.camProjection);
		shader.setVec3("objectColor", shaderSet.objectColor);

		shader.setVec3("lightColor", light.Color);
		shader.setVec3("lightPos", light.Posicion);
	
		shader.setVec3("Mat.ambient", shaderSet.ambient);
		shader.setVec3("Mat.difusse", shaderSet.difusse);
		shader.setVec3("Mat.specular", shaderSet.specular);
		shader.setFloat("Mat.shiness", shaderSet.shiness);

		if (static_cast<int>(textures.texU_Data.size()) > 0)
		{
			textures.useTextures_PerMaterial(shader);
		}

		shader.setVec3("viewPos", cam1.posCam);
		shader.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);

	

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
	void Mesh::Draw_WithLights(camera::camera1 cam1, std::vector<light::light1>* pointLights, std::vector<light::DirectionalLight>* directionalLights, shading::shader shader)
	{
		shader.use();

		shader.transformMat("model", MeshCoord.model);
		shader.transformMat("view", cam1.cam);
		shader.transformMat("projection", cam1.camProjection);
		shader.setVec3("objectColor", shaderSet.objectColor);

		if (pointLights != nullptr)
		{
			int pointLight_pos{ 1 };
			
			for (auto& pL : *pointLights)
			{
				std::string pL_name{ "pointLight_" + std::to_string(pointLight_pos++) };
				shader.setVec3(pL_name, pL.Color);
				shader.setVec3(pL_name, pL.Posicion);

			}
		}
		
		////////////Corregir aqui y revisar si funciona nullptr
		if (directionalLights != nullptr)
		{
			int dirLight_pos{ 1 };

			for (auto& dL : *directionalLights)
			{
				std::string dL_name{ "directionalLight_" + std::to_string(dirLight_pos++) };
				std::string dL_color{ dL_name + '.' + "lightColor" };
				std::string dL_posicion{ dL_name + '.' + "lightDir" };
				shader.setVec3(dL_color, dL.Color);
				shader.setVec3(dL_posicion, dL.Posicion);

			}

		}


		shader.setVec3("Mat.ambient", shaderSet.ambient);
		shader.setVec3("Mat.difusse", shaderSet.difusse);
		shader.setVec3("Mat.specular", shaderSet.specular);
		shader.setFloat("Mat.shiness", shaderSet.shiness);

		if (static_cast<int>(textures.texU_Data.size()) > 0)
		{
			textures.useTextures_PerMaterial(shader);
		}

		shader.setVec3("viewPos", cam1.posCam);
		shader.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);



		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


	}
	
	void Mesh::setMeshCoord(glm::vec3 posicionMesh, glm::vec3 scaleMesh)
	{
		MeshCoord.translateModel(posicionMesh);
		MeshCoord.scaleModel(scaleMesh);
		MeshCoord.setInverseTransformsAll();
	}

	void Model::loadModel(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings)
	{
		//IMPORT SHADER
		shaders.shaderCreation(vertexPath, fragmentPath);

		//set transform Model General
		setModelCoord(modelCoords);

		//import all Assimp
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		 
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);

		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			meshes[i].setMeshCoord(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			meshes[i].MeshCoord.model = ModelCoord.model * meshes[i].MeshCoord.model;
			meshes[i].MeshCoord.setNormalModelMatrix();
			meshes[i].shaderSet = shaderSettings; ///seteo de texturas
			
		}
	
	}
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.emplace_back(processMesh(mesh, scene));
		}
		
		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);

		}

	}
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<vertexD> vertices{};
		std::vector<unsigned int> indices{};
		std::vector<textureD> textures{};
		
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			glm::vec3 vertex{};

			vertex.x = mesh->mVertices[i].x;
			vertex.y = mesh->mVertices[i].y;
			vertex.z = mesh->mVertices[i].z;

			glm::vec3 normals{};
			normals.x = mesh->mNormals[i].x;
			normals.y = mesh->mNormals[i].y;
			normals.z = mesh->mNormals[i].z;

			glm::vec2 texCoords{};

			if (mesh->mTextureCoords[0])
			{
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;

			}

			else
			{
				texCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.emplace_back(vertex, normals, texCoords);
			SDL_Log(std::to_string(texCoords.x).c_str());
		}
		
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace Face = mesh->mFaces[i];

			for (int f = 0; f < Face.mNumIndices; f++)
			{
				indices.emplace_back(Face.mIndices[f]);
			}			
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };
	
			std::vector<textureD> difusseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
			shaders.use();


			textures.insert(textures.end(), difusseMaps.begin(), difusseMaps.end());
			
			std::vector<textureD> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

//		meshes.emplace_back(vertices, indices, textures);
		
		return Mesh(vertices, indices, textures);
	}

	std::vector<textureD> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<textureD> tex{};

		for (int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str{};
			mat->GetTexture(type, i, &str);
			textureD texture{};
			bool skip{ false };

			for (int i = 0; i < static_cast<int>(textures_Loaded.size()); i++)
			{
				if (std::strcmp(textures_Loaded[i].type.c_str(), str.C_Str()) == 0)
				{
					tex.emplace_back(textures_Loaded[i]);
					skip = true;
					break;
				}

			}

			if (!skip)
			{
				textureD texture{};
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str;
				tex.emplace_back(texture);
				textures_Loaded.emplace_back(texture);
				SDL_Log(texture.type.c_str());
			
			}
		}
		
		return tex;

	}
	Model::Model(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings)
	{
		loadModel(path, vertexPath, fragmentPath, modelCoords, shaderSettings);
	}
	void Model::Draw(camera::camera1 cam1, light::light1 light)
	{

		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			if (ModelCoord.model != ModelCoord.lastModel)
			{
				meshes[i].MeshCoord.model = ModelCoord.model * meshes[i].MeshCoord.modelCurrent;
				//meshes[i].MeshCoord.model = ModelCoord.modelCurrent;
				meshes[i].MeshCoord.setNormalModelMatrix();
				meshes[i].MeshCoord.lastModel = meshes[i].MeshCoord.model;
			}

			meshes[i].Draw(cam1, light, shaders);

		}

			ModelCoord.lastModel = ModelCoord.model;

		
	}
	void Model::setModelCoord(coordModel modelCoords)
	{
		ModelCoord.translateModel(modelCoords.posicion);
		ModelCoord.scaleModel(modelCoords.scale);
		ModelCoord.setPivotRotModel(modelCoords.pivotRot);
		ModelCoord.setAngRotModel(modelCoords.angRot);
		ModelCoord.setTransformsAll();
		ModelCoord.setNormalModelMatrix();
	}
	
}