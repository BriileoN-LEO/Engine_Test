
#include "ModelAssimp.h"
#include "Render/RenderData.h"
#include "textureData/textureManager.h"
#include "Edit_Modes/Edit_M.h"
#include "Render/Render.h"
#include "optimize_Algorithmics/optimizeAlgorithmics.h"
//#include "resource_Manager/resourceManager.h"

namespace sky 
{
	cubeMap_Skybox::cubeMap_Skybox() {};
	cubeMap_Skybox::cubeMap_Skybox(std::string name, std::string directory_Tex, std::vector<std::string> nameFiles, std::string nameShader)
	{
		this->name = name;
		this->nameShader = nameShader;
		loadTexture_Skybox_Seq(directory_Tex, nameFiles);
		loadCube();

	}

	void cubeMap_Skybox::loadTexture_Skybox_Seq(std::string directory_Tex, std::vector<std::string> nameFiles)
	{
		std::map<std::string, int> posicionNames
		{
			{"right", 0},
			{"left", 1},
			{"top", 2},
			{"bottom", 3},
			{"back", 4},
			{"front", 5}
		};

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;

		for (auto imageFile : nameFiles)
		{
			std::string nameSimple{ imageFile.substr(0, imageFile.find_last_of('.')) };

			std::cout << nameSimple << '\n';

			if (posicionNames.find(nameSimple) != posicionNames.end())
			{
				std::string sum_DirImage{ directory_Tex + imageFile };
				unsigned char* data = stbi_load(sum_DirImage.c_str(), &width, &height, &nrChannels, 0);

				if (data)
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

					//size_t sizeImage{ sizeof(data) };
					//glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + posicionNames[nameSimple], 0, format, width, height, 0, sizeImage, data);

					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + posicionNames[nameSimple],
					 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

					stbi_image_free(data);
				}

			}
			
			else
			{
				std::string errorMessage{ "ERROR::NOT FIND TEXTURE::CUBEMAP ----->" + imageFile };
				SDL_Log(errorMessage.c_str());
			}

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	void cubeMap_Skybox::loadCube()
	{
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,////
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		std::vector<float> skyBoxVertex
		{
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,////
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		skyBox_Vertices = skyBoxVertex;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, skyBoxVertex.size() * sizeof(float), &skyBoxVertex[0], GL_STATIC_DRAW);
	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);

	}
	void cubeMap_Skybox::draw_Skybox()
	{
	

		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

	    glDepthMask(GL_FALSE);


		shading::shader& shaderSkybox{ RenderData_Set::shader_D[nameShader] };
		shaderSkybox.use();

		glm::mat4 model{ glm::mat4(1.0) };
		model = glm::rotate(model, glm::radians(transform_SkyBox.rad), transform_SkyBox.pivot_rot);
		transform_SkyBox.rotationVec = glm::toMat3(glm::quat(glm::angleAxis(glm::radians(transform_SkyBox.rad), transform_SkyBox.pivot_rot)));

		shaderSkybox.transformMat("model", model);

		glm::mat4 cameraView{ glm::mat4(glm::mat3(cameras::cameras_D[cameras::name_CurrentCamera].cam)) };
		shaderSkybox.transformMat("view", cameraView);

		shaderSkybox.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
		
		shaderSkybox.setInt("skybox", 0);
	//	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

	}

	void cubeMap_Skybox::bind_Texture(shading::shader& shader, std::string nameToBind, texture::textureUnits activeTexturePos)
	{
		shader.setInt(nameToBind, static_cast<int>(activeTexturePos ));
		glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTexturePos));
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	}

}


namespace Assimp_D {
	unsigned int TextureFromFile(const char* path, std::string directory, bool gamma)
	{
	
		std::string pa = std::string(path);

		if (directory != "")
		{
			pa = directory + "/" + pa;
			//SDL_Log(pa.c_str());
		}


		int width{};
		int height{};
		int nrChannels{};
		unsigned int texID{};

		glGenTextures(1, &texID);

		unsigned char* DataT= stbi_load(pa.c_str(), &width, &height, &nrChannels, 0);

		stbi_set_flip_vertically_on_load(true);
	

		if (DataT)
		{
			GLenum format{};
			GLenum formatSecond{};
			if (nrChannels == 1)
			{
				format = GL_RED;

			}

			if (nrChannels == 3)
			{
				format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				formatSecond = GL_RGB;

			}

			if (nrChannels == 4)
			{
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				formatSecond = GL_RGBA;

			}
		
			glBindTexture(GL_TEXTURE_2D, texID);

			//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, DataT);
			//	glGenerateMipmap(GL_TEXTURE_2D);


			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, formatSecond, GL_UNSIGNED_BYTE, DataT);
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
	unsigned int TextureFromData(unsigned char* dataTexture, int width, int height, int nrChannels)
	{
		unsigned int id{};
		if (dataTexture)
		{

			GLenum format{};
			GLenum sec_Format{};
			if (nrChannels == 1)
			{
				format = GL_RED;

			}

			if (nrChannels == 3)
			{
				//format = GL_RGB;
				format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				sec_Format = GL_RGB;
			}

			if (nrChannels == 4)
			{
				//format = GL_RGBA;
				//format = GL_RGBA8;
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				sec_Format = GL_RGBA;
			}

			glGenTextures(1, &id);

			//	glBindTexture(GL_TEXTURE_2D, id);
			//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dataTexture);
			//	glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, id);
			//size_t sizeImage{ sizeof(dataTexture) };
			//glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, sizeImage, dataTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, sec_Format, GL_UNSIGNED_BYTE, dataTexture);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(dataTexture);
		}

		else
		{
			SDL_Log("ERROR::NOT LOAD::TEXTURE");
			stbi_image_free(dataTexture);
		}

		return id;
	}

	Uint8 size_shaderType{2};

	structModelName::structModelName() = default;

	structModelName::structModelName(std::string nameModel, std::string nameMesh)
	:
	model_ID{FNV::str_to_hash(nameModel)},
	mesh_ID{FNV::str_to_hash(nameMesh)}
	{}
	structModelName::structModelName(std::string nameModel, std::string nameMesh , bool changeStateSelection)
		:
	model_ID{FNV::str_to_hash(nameModel)},
    mesh_ID{FNV::str_to_hash(nameMesh)},
	changeStateSelection{changeStateSelection}
	{};
	structModelName::structModelName(uint32_t model_ID, uint32_t mesh_ID, bool changeStateSelection) :
	mesh_ID(mesh_ID),
	model_ID(model_ID),
	changeStateSelection(changeStateSelection)
	{};
	structModelName::structModelName(structModelName&& insertNew) noexcept :
	model_ID{insertNew.model_ID},
	mesh_ID{insertNew.mesh_ID},
	changeStateSelection{insertNew.changeStateSelection}
	{}
	structModelName::structModelName(structModelName& insertNew):
	model_ID{insertNew.model_ID},
    mesh_ID{insertNew.mesh_ID},
	changeStateSelection{insertNew.changeStateSelection}
	{}
	structModelName::structModelName(structModelName* insertNew) :
	 model_ID{insertNew->model_ID},
	 mesh_ID{insertNew->mesh_ID},
	 changeStateSelection{insertNew->changeStateSelection}
	{}

	bool structModelName::operator!=(structModelName&& sModelName) noexcept
	{
		if (model_ID != sModelName.model_ID &&
			mesh_ID != sModelName.mesh_ID)
		{
			return true;
		}

		return false;
	}

	bool structModelName::operator!=(structModelName& sModelName)
	{
		if (model_ID != sModelName.model_ID &&
			mesh_ID != sModelName.mesh_ID)
		{
			return true;
		}

		return false;
	}

	bool structModelName::operator==(structModelName&& sModelName) noexcept
	{
		if (model_ID == sModelName.model_ID &&
			mesh_ID == sModelName.mesh_ID)
		{
			return true;
		}

		return false;
	}
	bool structModelName::operator ==(structModelName& sModelName)
	{
		if (model_ID == sModelName.model_ID &&
				mesh_ID == sModelName.mesh_ID)
		{
			return true;
		}

		return false;
	}

	structModelName structModelName::operator=(structModelName&& sModelName) noexcept
	{
		model_ID = sModelName.model_ID;
		mesh_ID = sModelName.mesh_ID;
		changeStateSelection = sModelName.changeStateSelection;

		return *this;
	}
	structModelName structModelName::operator=(structModelName& sModelName)
	{
		model_ID = sModelName.model_ID;
		mesh_ID = sModelName.mesh_ID;
		changeStateSelection = sModelName.changeStateSelection;

		return *this;
	}


	void structModelName::reset()
	{
		model_ID = 0;
		mesh_ID = 0;
		changeStateSelection = false;
	}

	shader_SetType::shader_SetType(){};
	shader_SetType::shader_SetType(std::string name_shader, shader_type type) : name_shader(name_shader), type(type) {};
	shader_SetType::shader_SetType(const shader_SetType&& insertNew) noexcept :  name_shader(insertNew.name_shader), type(insertNew.type){};
	shader_SetType::shader_SetType(const shader_SetType& insertNew) : name_shader(insertNew.name_shader), type(insertNew.type){};
	//shader_SetType::shader_SetType(shader_SetType* insertNew) : name_shader(insertNew->name_shader), type(insertNew->type){};

	bool shader_SetType::operator!=(const shader_SetType&& sShader) noexcept
	{
	  bool toReturn{false};

	  if (name_shader != sShader.name_shader &&
	  	  type != sShader.type)
	  {
	    toReturn = true;
	  }

      return toReturn;
	}
	bool shader_SetType::operator!=(const shader_SetType& sShader)
	{
		bool toReturn{false};

		if (name_shader != sShader.name_shader &&
			  type != sShader.type)
		{
			toReturn = true;
		}

		return toReturn;
	}
	bool shader_SetType::operator==(const shader_SetType&& sShader) noexcept
	{
		bool toReturn{false};

		if (name_shader == sShader.name_shader &&
			  type == sShader.type)
		{
			toReturn = true;
		}

		return toReturn;
	}
	bool shader_SetType::operator==(const shader_SetType& sShader)
	{
		if (name_shader == sShader.name_shader &&
			  type == sShader.type)
		{
			return true;
		}

		return false;
	}
	shader_SetType shader_SetType::operator=(const shader_SetType&& sShader) noexcept
	{
	  name_shader = sShader.name_shader;
	  type = sShader.type;

	  return *this;
	}
	shader_SetType shader_SetType::operator=(const shader_SetType& sShader)
	{
		name_shader = sShader.name_shader;
		type = sShader.type;

		return *this;
	}

	//shader_SetType::shader_SetType() : name_shader(""), type(shader_type::standard_Shader){};
	//shader_SetType::shader_SetType(std::string name_shader, shader_type type) : name_shader(name_shader), type(type){}

	Mesh::Mesh() {};
	Mesh::Mesh(std::vector<vertexD> ver, std::vector<unsigned int> ind, std::vector<textureD> texture)
	{
		vertices = ver;
		indices = ind;

		for (auto& vertex : vertices)
		{
			verticesPos.emplace_back(vertex.posicion);

		}

		MeshCoord.posModel_Base = transformation_basics::calcCenterGeo(verticesPos);
		MeshCoord.posModel = MeshCoord.posModel_Base;

	   //para setear las texturas
		if (static_cast<int>(texture.size()) > 0)
		{
			/*
			bool existDiffuse{ false };
			std::string texturePath{};
			*/

			for (int i = 0; i < static_cast<int>(texture.size()); i++)
			{
			//	textures.textures_LoadCache.emplace_back(texDataManager::TextureData_File(texture[i].id, texture[i].type, texture::textureUnits_Data[i]));
				/*
				if (texture[i].type == "texture_diffuse")
				{
					existDiffuse = true;
					texturePath = texture[i].path.C_Str();
				}
				*/
			}

			/*
			if (existDiffuse == true)
			{
				std::reverse(texturePath.begin(), texturePath.end());

				int posPoint{ static_cast<int>(texturePath.find(".")) };

				std::string reversePath{ texturePath.substr(0, posPoint) };
				std::reverse(reversePath.begin(), reversePath.end());

				if (reversePath == "png")
				{
					renderP = renderSeq::renderFar;
				}

				else if (reversePath == "jpg")
				{
					renderP = renderSeq::renderNear;

				}

			}
			*/
		}

		renderP = renderSeq::renderNear;

		setupMesh();

	}

	Mesh::Mesh(Assimp_D::loadToCPU::MeshData_loadCPU loadData)
	{
		//nameMesh = loadData.nameMesh;
		vertices = loadData.vertices;
		indices = loadData.indices;

		for (auto& vertex : vertices)
		{
			verticesPos.emplace_back(vertex.posicion);
			normalsPos.emplace_back(vertex.posicion + vertex.Normal);

		}

		MeshCoord.posModel_Base = transformation_basics::calcCenterGeo(verticesPos);
		MeshCoord.posModel = MeshCoord.posModel_Base;

		int seqUnit{};


		for (auto textureDataCache : loadData.textures)
		{
			textures.textures_LoadCache.emplace_back(textureDataCache);

			if (textureDataCache.blendTexture == true && textureDataCache.typeTex == texDataManager::typeTexture::diffuse)
			{
				textures.active_BlendMode = true;

			}
		}

		//OLD WAY TO LOAD TEXTURES
		/*
		for (int i = 0; i < static_cast<int>(loadData.textures.size()); i++)
		{
			unsigned int texID{ TextureFromData(loadData.textures[i].dataTexture, loadData.textures[i].width, loadData.textures[i].height, loadData.textures[i].nrChannels) };
			textures.texU_Data.emplace_back(texture::textureData(texID, loadData.textures[i].typeTex, loadData.textures[i].completePath, texture::textureUnits_Data[seqUnit]));
			seqUnit++;

		//	if()

		}
		*/
		/*
		for (auto& texLoad : loadData.textures)
		{
			unsigned int texID{ TextureFromData(texLoad.dataTexture, texLoad.width, texLoad.height, texLoad.nrChannels) };
			textures.texU_Data.emplace_back(texture::textureData(texID, texLoad.typeTexture, texLoad.path, texture::textureUnits_Data[seqUnit]));
			seqUnit++;
		}
		*/

		renderP = renderSeq::renderNear;

		setupMesh();
		//setupMesh_subBuffer();
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
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexD), (void*)offsetof(vertexD, TexCoord));

		//glBindBuffer(GL_ARRAY_BUFFER,S 0);
		glBindVertexArray(0);



	}

	void Mesh::setupMesh_subBuffer()
	{
		std::vector<glm::vec3> position{};
		std::vector<glm::vec3> normals{};
		std::vector<glm::vec2> texCoord{};

		for (auto vertex : vertices)
		{
			position.emplace_back(vertex.posicion);
			normals.emplace_back(vertex.Normal);
			texCoord.emplace_back(vertex.TexCoord);
		}

		size_t sizeOfPosition{ position.size() * sizeof(glm::vec3) };
		size_t sizeOfNormals{ normals.size() * sizeof(glm::vec3) };
		size_t sizeOfTexCoords{ texCoord.size() * sizeof(glm::vec2) };
;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexD), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, position.size() * sizeof(glm::vec3), &position[0]);
		glBufferSubData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), &normals[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (position.size() * sizeof(glm::vec3)) + (normals.size() * sizeof(glm::vec3)), texCoord.size() * sizeof(glm::vec2), &texCoord[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(position.size() * sizeof(glm::vec3)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)((position.size() * sizeof(glm::vec3)) + (normals.size() * sizeof(glm::vec3))));

		glBindVertexArray(0);

	}

	void Mesh::update_editMode() //CHANGE LATER IF I WILL USE THE SAME NAME OF THE MESH BUT IN OTHER MODEL
	{

		auto find_pos = [&]()
		{
			int pos{-1};
			auto find_Mesh = std::ranges::find_if(edit_visualize::nameSelect_Model,
				[&](edit_visualize::edit_MV& nSM) {
				   pos++;
					return nSM.names.mesh_ID == ID;
				});

			if (find_Mesh != std::ranges::end(edit_visualize::nameSelect_Model))
			{
				pos_editMode = pos;
			}

			else if (find_Mesh == std::ranges::end(edit_visualize::nameSelect_Model))
			{
				pos_editMode = -1;
			}

			last_Size_EM = static_cast<int>(edit_visualize::nameSelect_Model.size());
		};

		if (last_Size_EM != static_cast<int>(edit_visualize::nameSelect_Model.size()))
		{
			find_pos();
		}

		else if (pos_editMode != -1)
		{
		    if (edit_visualize::nameSelect_Model[pos_editMode].names.mesh_ID != ID)
		  {
		    	find_pos();
		  }
		}

	}
	void Mesh::Draw_Normals(std::string& shader_ID)  ////this need to set the shader for normals
	{
		if (edit_visualize::show_normals_active == true)
		{
			if (pos_editMode != -1)
			{
                shading::shader& shader_set {RenderData_Set::shader_D[shader_ID]};
				shader_set.use();

				///VERTEX SHADER
				shader_set.transformMat("model", MeshCoord.model);

				///GEOMETRY SHADER
				shader_set.setBool("active_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.active);
				shader_set.setFloat("dist_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.dist_explode);


				glBindVertexArray(VAO);
			    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
				glBindVertexArray(0);
			}
		}
	}
	void Mesh::Draw(camera::camera1 cam1, light::light1 light, shading::shader& shader, std::string& shaderNormals_ID)
	{
		shader.use();

		shader.transformMat("model", MeshCoord.model);
		//shader.transformMat("view", cam1.cam);
		//shader.transformMat("projection", cam1.camProjection);
		shader.setVec3("objectColor", shaderSet.objectColor);

		shader.setVec3("lightColor", light.Color);
		shader.setVec3("lightPos", light.Posicion);

		shader.setVec3("Mat.ambient", shaderSet.ambient);
		shader.setVec3("Mat.difusse", shaderSet.difusse);  ///SEE IF THIS AFFECTS THE SHADER
		shader.setVec3("Mat.specular", shaderSet.specular);
		shader.setFloat("Mat.shiness", shaderSet.shiness);

		if (static_cast<int>(textures.textures_LoadCache.size()) > 0)
		{
			//textures.useTextures_PerMaterial(shader, 1); //OLD WAY
			textures.use_MaterialTextures(shader, 1);
		}

		shader.setVec3("viewPos", cam1.posCam);
		shader.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);

		if (pos_editMode != -1)
		{
			shader.setBool("active_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.active);
			shader.setFloat("dist_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.dist_explode);
		}

		else if (pos_editMode == -1)
		{
			shader.setBool("active_exploded", false);
			shader.setFloat("dist_exploded", 0.0f);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		if (edit_visualize::editMode_active == true)
		{
			Draw_Normals(shaderNormals_ID); //DRAW NORMALS IF I SELECT THE OPTION
		}
	}
	void Mesh::Draw_WithLights(shading::shader& shader, std::string& shaderNormals_ID)
	{
		if (!render::oneTimeSee)
		{
	//		SDL_Log(glm::to_string(MeshCoord.model).c_str());  ///TO PRINT THE MATRIX
		}

		shader.use();

		shader.transformMat("model", MeshCoord.model);
	//	shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
	//	shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
		shader.setVec3("objectColor", shaderSet.objectColor);

		/*
		if (static_cast<int>(RenderData_Set::pointLights_D.size()) > 0)
		{

			for (int i = 0; i < static_cast<int>(RenderData_Set::pointLights_D.size()); i++)
			{
				std::string pL_name{ "pointLights_Array[" + std::to_string(i) + "]" };

				std::string pL_color{ pL_name + ".lightColor" };
				std::string pL_Posicion{ pL_name + ".lightPos" };
				std::string pL_constant{ pL_name + ".constant" };
				std::string pL_linear{ pL_name + ".linear" };
				std::string pL_quadratic{ pL_name + ".quadratic" };
				std::string pL_ambient{ pL_name + ".ambient" };
				std::string pL_diffuse{ pL_name + ".diffuse" };
				std::string pL_specular{ pL_name + ".specular" };

				shader.setVec3(pL_color, RenderData_Set::pointLights_D[i].Color);
				shader.setVec3(pL_Posicion, RenderData_Set::pointLights_D[i].Posicion);
				shader.setFloat(pL_constant, RenderData_Set::pointLights_D[i].constant);
				shader.setFloat(pL_linear, RenderData_Set::pointLights_D[i].linear);
				shader.setFloat(pL_quadratic, RenderData_Set::pointLights_D[i].quadratic);
				shader.setVec3(pL_ambient, RenderData_Set::pointLights_D[i].Mat.ambient);
				shader.setVec3(pL_diffuse, RenderData_Set::pointLights_D[i].Mat.diffuse);
				shader.setVec3(pL_specular, RenderData_Set::pointLights_D[i].Mat.specular);

			}
		}

		////////////Corregir aqui y revisar si funciona nullptr
		if (static_cast<int>(RenderData_Set::directionalLights_D.size()) > 0)
		{
			int dirLight_pos{ 1 };

			for (int i = 0; i < static_cast<int>(RenderData_Set::directionalLights_D.size()); i++)
			{
				//for (auto& dL : *directionalLights)
				//{
				std::string dL_name{ "directionalLight_" + std::to_string(dirLight_pos++) };
			//	std::string dL_color{ dL_name + ".lightColor" };
				std::string dL_direction{ dL_name + ".lightDir" };
				std::string dL_ambient{ dL_name + ".ambient" };
				std::string dL_diffuse{ dL_name + "diffuse" };
				std::string dL_specular{ dL_name + ".specular" };

				//shader.setVec3(dL_color, directionalLights[i].Color);
				shader.setVec3(dL_direction, RenderData_Set::directionalLights_D[i].Direction);
				shader.setVec3(dL_ambient, RenderData_Set::directionalLights_D[i].Mat.ambient);
				shader.setVec3(dL_diffuse, RenderData_Set::directionalLights_D[i].Mat.diffuse);
				shader.setVec3(dL_specular, RenderData_Set::directionalLights_D[i].Mat.specular);


			}

		}

		if (static_cast<int>(RenderData_Set::spotLights_D.size()) > 0)
		{
			int sL_i{};


			for (auto& spotLight : RenderData_Set::spotLights_D)
			{
				std::string sL_name{ "spotLights_Array[" + std::to_string(sL_i) + "]" };

				std::string sL_Posicion{ sL_name + ".lightPos" };
				std::string sL_Direction{ sL_name + ".lightDir" };
				std::string sL_cutOff{ sL_name + ".cutOff" };
				std::string sL_outerCutOff{ sL_name + ".outerCutOff" };
				std::string sL_constant{ sL_name + ".constant" };
				std::string sL_linear{ sL_name + ".linear" };
				std::string sL_quadratic{ sL_name + ".quadratic" };
				std::string sL_ambient{ sL_name + ".ambient" };
				std::string sL_diffuse{ sL_name + ".diffuse" };
				std::string sL_specular{ sL_name + ".specular" };
				std::string sL_lightState{ sL_name + ".lightState" };

				shader.setVec3(sL_Posicion, spotLight.second.Posicion);
				shader.setVec3(sL_Direction, spotLight.second.Direction);
				shader.setFloat(sL_cutOff, glm::cos(glm::radians(spotLight.second.cutOff)));
				shader.setFloat(sL_outerCutOff, glm::cos(glm::radians(spotLight.second.outerCutOff)));
				shader.setFloat(sL_constant, spotLight.second.constant);
				shader.setFloat(sL_linear, spotLight.second.linear);
				shader.setFloat(sL_quadratic, spotLight.second.quadratic);
				shader.setVec3(sL_ambient, spotLight.second.Mat.ambient);
				shader.setVec3(sL_diffuse, spotLight.second.Mat.diffuse);
				shader.setVec3(sL_specular, spotLight.second.Mat.specular);
				shader.setBool(sL_lightState, spotLight.second.stateLight);

				sL_i++;
			}

		}
		*/

		if(!textures.textures_LoadCache.empty())
		//if (!textures.texU_Data.empty()) ///OLD WAY TO USE TEXTURES
		{
			//textures.useTextures_PerMaterial(shader, 1); ///OLD WAY TO USE TEXTURES
			textures.use_MaterialTextures(shader, 1);
			shader.setBool("NotTexture", false);

			if (nameMesh == "CampoVegetacion_1")
			{
			//	SDL_Log(std::to_string(static_cast<int>(textures.texU_Data.size())).c_str());
			}

			if (RenderData_Set::skybox_D::skyBox_Current.active == true && !RenderData_Set::skybox_D::skyBox_Current.nameSkybox.empty())
			{
				texture::textureUnits textureUnit{ static_cast<texture::textureUnits>(static_cast<int>(textures.textures_LoadCache.size()) + 1) };

				RenderData_Set::skybox_D::skyBoxes_D[RenderData_Set::skybox_D::skyBox_Current.nameSkybox].bind_Texture(shader, "skybox", textureUnit);
				shader.transformMat3("transformation_SkyBox", RenderData_Set::skybox_D::skyBoxes_D["skyBox_day"].transform_SkyBox.rotationVec);
				shader.setBool("activeSkybox", true);

			}

		}

		else if(textures.textures_LoadCache.empty())
		{
			shader.setBool("NotTexture", true);
			shader.setBool("Mat_1.use_texture_diffuse", false);
			shader.setBool("Mat_1.use_texture_specular", false);
			//shader.setBool("useSpec", false);

			if (nameMesh == "CampoVegetacion_1")
			{
			//	SDL_Log(std::string("NOT TEXTURE::MESH::" + nameMesh).c_str());
			}

			shader.setBool("activeSkybox", false);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}



		if (data_HitAABB::selectedObj.first.mesh_ID == ID)
		{
			shading::config::change_refractiveIndex(settingsShader.refractiveIndex);
		}

		shader.setFloat("refractiveIndex", settingsShader.refractiveIndex);

		shader.setVec3("Mat.ambient", shaderSet.ambient);
		shader.setVec3("Mat.difusse", shaderSet.difusse);
		shader.setVec3("Mat.specular", shaderSet.specular);
		shader.setFloat("Mat.shiness", shaderSet.shiness);


		shader.setVec3("viewPos", cameras::cameras_D[cameras::name_CurrentCamera].posCam);
		shader.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);

		  if (pos_editMode != -1)
		{
				shader.setBool("active_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.active);
				shader.setFloat("dist_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.dist_explode);
		}

		else if (pos_editMode == -1)
	    {
			shader.setBool("active_exploded", false);
			shader.setFloat("dist_exploded", 0.0f);
		}
			//CHANGE LATER IF I WILL USE THE SAME NAME OF THE MESH BUT IN OTHER MODEL



		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);


		if (edit_visualize::editMode_active == true && shaderNormals_ID != "")
		{
			Draw_Normals(shaderNormals_ID); //DRAW NORMALS IF I SELECT THE OPTION
		}



		///Terminar con la textura
		//shader.setBool("NotTexture", true);

	//	std::cout << glGetError() << '\n';

	}
	void Mesh::Draw_WithLights02(shading::shader& shader)
	{
		shader.use();

		shader.transformMat("model", MeshCoord.model);
		//shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
		//shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
		shader.setVec3("objectColor", shaderSet.objectColor);

		/*
		if (static_cast<int>(RenderData_Set::pointLights_D.size()) > 0)
		{

			for (int i = 0; i < static_cast<int>(RenderData_Set::pointLights_D.size()); i++)
			{
				std::string pL_name{ "pointLights_Array[" + std::to_string(i) + "]" };

				std::string pL_color{ pL_name + ".lightColor" };
				std::string pL_Posicion{ pL_name + ".lightPos" };
				std::string pL_constant{ pL_name + ".constant" };
				std::string pL_linear{ pL_name + ".linear" };
				std::string pL_quadratic{ pL_name + ".quadratic" };
				std::string pL_ambient{ pL_name + ".ambient" };
				std::string pL_diffuse{ pL_name + ".diffuse" };
				std::string pL_specular{ pL_name + ".specular" };

				shader.setVec3(pL_color, RenderData_Set::pointLights_D[i].Color);
				shader.setVec3(pL_Posicion, RenderData_Set::pointLights_D[i].Posicion);
				shader.setFloat(pL_constant, RenderData_Set::pointLights_D[i].constant);
				shader.setFloat(pL_linear, RenderData_Set::pointLights_D[i].linear);
				shader.setFloat(pL_quadratic, RenderData_Set::pointLights_D[i].quadratic);
				shader.setVec3(pL_ambient, RenderData_Set::pointLights_D[i].Mat.ambient);
				shader.setVec3(pL_diffuse, RenderData_Set::pointLights_D[i].Mat.diffuse);
				shader.setVec3(pL_specular, RenderData_Set::pointLights_D[i].Mat.specular);

			}
		}

		////////////Corregir aqui y revisar si funciona nullptr
		if (static_cast<int>(RenderData_Set::directionalLights_D.size()) > 0)
		{
			int dirLight_pos{ 1 };

			for (int i = 0; i < static_cast<int>(RenderData_Set::directionalLights_D.size()); i++)
			{
				//for (auto& dL : *directionalLights)
				//{
				std::string dL_name{ "directionalLight_" + std::to_string(dirLight_pos++) };
			//	std::string dL_color{ dL_name + ".lightColor" };
				std::string dL_direction{ dL_name + ".lightDir" };
				std::string dL_ambient{ dL_name + ".ambient" };
				std::string dL_diffuse{ dL_name + "diffuse" };
				std::string dL_specular{ dL_name + ".specular" };

				//shader.setVec3(dL_color, directionalLights[i].Color);
				shader.setVec3(dL_direction, RenderData_Set::directionalLights_D[i].Direction);
				shader.setVec3(dL_ambient, RenderData_Set::directionalLights_D[i].Mat.ambient);
				shader.setVec3(dL_diffuse, RenderData_Set::directionalLights_D[i].Mat.diffuse);
				shader.setVec3(dL_specular, RenderData_Set::directionalLights_D[i].Mat.specular);


			}

		}

		if (static_cast<int>(RenderData_Set::spotLights_D.size()) > 0)
		{
			int sL_i{};


			for (auto& spotLight : RenderData_Set::spotLights_D)
			{
				std::string sL_name{ "spotLights_Array[" + std::to_string(sL_i) + "]" };

				std::string sL_Posicion{ sL_name + ".lightPos" };
				std::string sL_Direction{ sL_name + ".lightDir" };
				std::string sL_cutOff{ sL_name + ".cutOff" };
				std::string sL_outerCutOff{ sL_name + ".outerCutOff" };
				std::string sL_constant{ sL_name + ".constant" };
				std::string sL_linear{ sL_name + ".linear" };
				std::string sL_quadratic{ sL_name + ".quadratic" };
				std::string sL_ambient{ sL_name + ".ambient" };
				std::string sL_diffuse{ sL_name + ".diffuse" };
				std::string sL_specular{ sL_name + ".specular" };
				std::string sL_lightState{ sL_name + ".lightState" };

				shader.setVec3(sL_Posicion, spotLight.second.Posicion);
				shader.setVec3(sL_Direction, spotLight.second.Direction);
				shader.setFloat(sL_cutOff, glm::cos(glm::radians(spotLight.second.cutOff)));
				shader.setFloat(sL_outerCutOff, glm::cos(glm::radians(spotLight.second.outerCutOff)));
				shader.setFloat(sL_constant, spotLight.second.constant);
				shader.setFloat(sL_linear, spotLight.second.linear);
				shader.setFloat(sL_quadratic, spotLight.second.quadratic);
				shader.setVec3(sL_ambient, spotLight.second.Mat.ambient);
				shader.setVec3(sL_diffuse, spotLight.second.Mat.diffuse);
				shader.setVec3(sL_specular, spotLight.second.Mat.specular);
				shader.setBool(sL_lightState, spotLight.second.stateLight);

				sL_i++;
			}

		}
		*/

		if(!textures.textures_LoadCache.empty())
		//if (!textures.texU_Data.empty()) ///OLD WAY TO USE TEXTURES
		{
			//textures.useTextures_PerMaterial(shader, 1); ///OLD WAY TO USE TEXTURES
			textures.use_MaterialTextures(shader, 1);
			shader.setBool("NotTexture", false);

			if (nameMesh == "CampoVegetacion_1")
			{
			//	SDL_Log(std::to_string(static_cast<int>(textures.texU_Data.size())).c_str());
			}

			if (RenderData_Set::skybox_D::skyBox_Current.active == true && !RenderData_Set::skybox_D::skyBox_Current.nameSkybox.empty())
			{
				texture::textureUnits textureUnit{ static_cast<texture::textureUnits>(static_cast<int>(textures.textures_LoadCache.size()) + 1) };

				RenderData_Set::skybox_D::skyBoxes_D[RenderData_Set::skybox_D::skyBox_Current.nameSkybox].bind_Texture(shader, "skybox", textureUnit);
				shader.transformMat3("transformation_SkyBox", RenderData_Set::skybox_D::skyBoxes_D["skyBox_day"].transform_SkyBox.rotationVec);
				shader.setBool("activeSkybox", true);

			}

		}

		else if(textures.textures_LoadCache.empty())
		{
			shader.setBool("NotTexture", true);
			shader.setBool("Mat_1.use_texture_diffuse", false);
			shader.setBool("Mat_1.use_texture_specular", false);
			//shader.setBool("useSpec", false);

			if (nameMesh == "CampoVegetacion_1")
			{
			//	SDL_Log(std::string("NOT TEXTURE::MESH::" + nameMesh).c_str());
			}

			shader.setBool("activeSkybox", false);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}



		if (data_HitAABB::selectedObj.first.mesh_ID == ID)
		{
			shading::config::change_refractiveIndex(settingsShader.refractiveIndex);
		}

		shader.setFloat("refractiveIndex", settingsShader.refractiveIndex);

		shader.setVec3("Mat.ambient", shaderSet.ambient);
		shader.setVec3("Mat.difusse", shaderSet.difusse);
		shader.setVec3("Mat.specular", shaderSet.specular);
		shader.setFloat("Mat.shiness", shaderSet.shiness);


		shader.setVec3("viewPos", cameras::cameras_D[cameras::name_CurrentCamera].posCam);
		shader.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);

		  if (pos_editMode != -1)
		{
				shader.setBool("active_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.active);
				shader.setFloat("dist_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.dist_explode);
		}

		else if (pos_editMode == -1)
	    {
			shader.setBool("active_exploded", false);
			shader.setFloat("dist_exploded", 0.0f);
		}
			//CHANGE LATER IF I WILL USE THE SAME NAME OF THE MESH BUT IN OTHER MODEL



		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		///Terminar con la textura
		//shader.setBool("NotTexture", true);

	//	std::cout << glGetError() << '\n';
	}
	void Mesh::Draw_Alone()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
	void Mesh::Draw_WithoutModel(shading::shader& shader, std::string shaderNormals_ID)
	{
		//shader.use();
	//	shader.transformMat("model", glm::mat4(1.0f));
		shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
		shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
		shader.setVec3("objectColor", shaderSet.objectColor);

		/*
		if (static_cast<int>(RenderData_Set::pointLights_D.size()) > 0)
		{

			for (int i = 0; i < static_cast<int>(RenderData_Set::pointLights_D.size()); i++)
			{
				std::string pL_name{ "pointLights_Array[" + std::to_string(i) + "]" };

				std::string pL_color{ pL_name + ".lightColor" };
				std::string pL_Posicion{ pL_name + ".lightPos" };
				std::string pL_constant{ pL_name + ".constant" };
				std::string pL_linear{ pL_name + ".linear" };
				std::string pL_quadratic{ pL_name + ".quadratic" };
				std::string pL_ambient{ pL_name + ".ambient" };
				std::string pL_diffuse{ pL_name + ".diffuse" };
				std::string pL_specular{ pL_name + ".specular" };

				shader.setVec3(pL_color, RenderData_Set::pointLights_D[i].Color);
				shader.setVec3(pL_Posicion, RenderData_Set::pointLights_D[i].Posicion);
				shader.setFloat(pL_constant, RenderData_Set::pointLights_D[i].constant);
				shader.setFloat(pL_linear, RenderData_Set::pointLights_D[i].linear);
				shader.setFloat(pL_quadratic, RenderData_Set::pointLights_D[i].quadratic);
				shader.setVec3(pL_ambient, RenderData_Set::pointLights_D[i].Mat.ambient);
				shader.setVec3(pL_diffuse, RenderData_Set::pointLights_D[i].Mat.diffuse);
				shader.setVec3(pL_specular, RenderData_Set::pointLights_D[i].Mat.specular);

			}
		}

		////////////Corregir aqui y revisar si funciona nullptr
		if (static_cast<int>(RenderData_Set::directionalLights_D.size()) > 0)
		{
			int dirLight_pos{ 1 };

			for (int i = 0; i < static_cast<int>(RenderData_Set::directionalLights_D.size()); i++)
			{
				//for (auto& dL : *directionalLights)
				//{
				std::string dL_name{ "directionalLight_" + std::to_string(dirLight_pos++) };
				//	std::string dL_color{ dL_name + ".lightColor" };
				std::string dL_direction{ dL_name + ".lightDir" };
				std::string dL_ambient{ dL_name + ".ambient" };
				std::string dL_diffuse{ dL_name + "diffuse" };
				std::string dL_specular{ dL_name + ".specular" };

				//shader.setVec3(dL_color, directionalLights[i].Color);
				shader.setVec3(dL_direction, RenderData_Set::directionalLights_D[i].Direction);
				shader.setVec3(dL_ambient, RenderData_Set::directionalLights_D[i].Mat.ambient);
				shader.setVec3(dL_diffuse, RenderData_Set::directionalLights_D[i].Mat.diffuse);
				shader.setVec3(dL_specular, RenderData_Set::directionalLights_D[i].Mat.specular);


			}

		}

		if (static_cast<int>(RenderData_Set::spotLights_D.size()) > 0)
		{
			int sL_i{};

			for (auto& spotLight : RenderData_Set::spotLights_D)
			{
				std::string sL_name{ "spotLights_Array[" + std::to_string(sL_i) + "]" };

				std::string sL_Posicion{ sL_name + ".lightPos" };
				std::string sL_Direction{ sL_name + ".lightDir" };
				std::string sL_cutOff{ sL_name + ".cutOff" };
				std::string sL_outerCutOff{ sL_name + ".outerCutOff" };
				std::string sL_constant{ sL_name + ".constant" };
				std::string sL_linear{ sL_name + ".linear" };
				std::string sL_quadratic{ sL_name + ".quadratic" };
				std::string sL_ambient{ sL_name + ".ambient" };
				std::string sL_diffuse{ sL_name + ".diffuse" };
				std::string sL_specular{ sL_name + ".specular" };
				std::string sL_lightState{ sL_name + ".lightState" };

				shader.setVec3(sL_Posicion, spotLight.second.Posicion);
				shader.setVec3(sL_Direction, spotLight.second.Direction);
				shader.setFloat(sL_cutOff, glm::cos(glm::radians(spotLight.second.cutOff)));
				shader.setFloat(sL_outerCutOff, glm::cos(glm::radians(spotLight.second.outerCutOff)));
				shader.setFloat(sL_constant, spotLight.second.constant);
				shader.setFloat(sL_linear, spotLight.second.linear);
				shader.setFloat(sL_quadratic, spotLight.second.quadratic);
				shader.setVec3(sL_ambient, spotLight.second.Mat.ambient);
				shader.setVec3(sL_diffuse, spotLight.second.Mat.diffuse);
				shader.setVec3(sL_specular, spotLight.second.Mat.specular);
				shader.setBool(sL_lightState, spotLight.second.stateLight);

				sL_i++;
			}

		}
		*/

		if (!textures.textures_LoadCache.empty())
		{
			//textures.useTextures_PerMaterial(shader, 1); OLD WAY TO LOAD TEXTURES
			textures.use_MaterialTextures(shader, 1);
			shader.setBool("NotTexture", false);
			//SDL_Log(std::to_string(textureCache::textures.size()).c_str());

 		}

		else if (textures.textures_LoadCache.empty())
		{
			shader.setBool("NotTexture", true);
			shader.setBool("Mat_1.use_texture_diffuse", false);
			shader.setBool("Mat_1.use_texture_specular", false);
			//shader.setBool("useSpec", false);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}


		shader.setVec3("Mat.ambient", shaderSet.ambient);
		shader.setVec3("Mat.difusse", shaderSet.difusse);
		shader.setVec3("Mat.specular", shaderSet.specular);
		shader.setFloat("Mat.shiness", shaderSet.shiness);


		shader.setVec3("viewPos", cameras::cameras_D[cameras::name_CurrentCamera].posCam);
		shader.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);

		if (pos_editMode != -1)
		{
			shader.setBool("active_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.active);
			shader.setFloat("dist_exploded", edit_visualize::nameSelect_Model[pos_editMode].explode.dist_explode);
		}

		else if (pos_editMode == -1)
		{
			shader.setBool("active_exploded", false);
			shader.setFloat("dist_exploded", 0.0f);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (edit_visualize::editMode_active == true)
		{
		Draw_Normals(shaderNormals_ID); //DRAW NORMALS IF I SELECT THE OPTION
		}
	}
	void Mesh::build_PreDraw(shading::shader& shader)
	{

		shader.use();

		shader.transformMat("model", MeshCoord.model);
		//shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
		//shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
		shader.setVec3("objectColor", shaderSet.objectColor);

		/*
		if (static_cast<int>(RenderData_Set::pointLights_D.size()) > 0)
		{

			for (int i = 0; i < static_cast<int>(RenderData_Set::pointLights_D.size()); i++)
			{
				std::string pL_name{ "pointLights_Array[" + std::to_string(i) + "]" };

				std::string pL_color{ pL_name + ".lightColor" };
				std::string pL_Posicion{ pL_name + ".lightPos" };
				std::string pL_constant{ pL_name + ".constant" };
				std::string pL_linear{ pL_name + ".linear" };
				std::string pL_quadratic{ pL_name + ".quadratic" };
				std::string pL_ambient{ pL_name + ".ambient" };
				std::string pL_diffuse{ pL_name + ".diffuse" };
				std::string pL_specular{ pL_name + ".specular" };

				shader.setVec3(pL_color, RenderData_Set::pointLights_D[i].Color);
				shader.setVec3(pL_Posicion, RenderData_Set::pointLights_D[i].Posicion);
				shader.setFloat(pL_constant, RenderData_Set::pointLights_D[i].constant);
				shader.setFloat(pL_linear, RenderData_Set::pointLights_D[i].linear);
				shader.setFloat(pL_quadratic, RenderData_Set::pointLights_D[i].quadratic);
				shader.setVec3(pL_ambient, RenderData_Set::pointLights_D[i].Mat.ambient);
				shader.setVec3(pL_diffuse, RenderData_Set::pointLights_D[i].Mat.diffuse);
				shader.setVec3(pL_specular, RenderData_Set::pointLights_D[i].Mat.specular);

			}
		}

		////////////Corregir aqui y revisar si funciona nullptr
		if (static_cast<int>(RenderData_Set::directionalLights_D.size()) > 0)
		{
			int dirLight_pos{ 1 };

			for (int i = 0; i < static_cast<int>(RenderData_Set::directionalLights_D.size()); i++)
			{
				//for (auto& dL : *directionalLights)
				//{
				std::string dL_name{ "directionalLight_" + std::to_string(dirLight_pos++) };
				//	std::string dL_color{ dL_name + ".lightColor" };
				std::string dL_direction{ dL_name + ".lightDir" };
				std::string dL_ambient{ dL_name + ".ambient" };
				std::string dL_diffuse{ dL_name + "diffuse" };
				std::string dL_specular{ dL_name + ".specular" };

				//shader.setVec3(dL_color, directionalLights[i].Color);
				shader.setVec3(dL_direction, RenderData_Set::directionalLights_D[i].Direction);
				shader.setVec3(dL_ambient, RenderData_Set::directionalLights_D[i].Mat.ambient);
				shader.setVec3(dL_diffuse, RenderData_Set::directionalLights_D[i].Mat.diffuse);
				shader.setVec3(dL_specular, RenderData_Set::directionalLights_D[i].Mat.specular);


			}

		}

		if (static_cast<int>(RenderData_Set::spotLights_D.size()) > 0)
		{
			int sL_i{};

			for (auto& spotLight : RenderData_Set::spotLights_D)
			{
				std::string sL_name{ "spotLights_Array[" + std::to_string(sL_i) + "]" };

				std::string sL_Posicion{ sL_name + ".lightPos" };
				std::string sL_Direction{ sL_name + ".lightDir" };
				std::string sL_cutOff{ sL_name + ".cutOff" };
				std::string sL_outerCutOff{ sL_name + ".outerCutOff" };
				std::string sL_constant{ sL_name + ".constant" };
				std::string sL_linear{ sL_name + ".linear" };
				std::string sL_quadratic{ sL_name + ".quadratic" };
				std::string sL_ambient{ sL_name + ".ambient" };
				std::string sL_diffuse{ sL_name + ".diffuse" };
				std::string sL_specular{ sL_name + ".specular" };
				std::string sL_lightState{ sL_name + ".lightState" };

				shader.setVec3(sL_Posicion, spotLight.second.Posicion);
				shader.setVec3(sL_Direction, spotLight.second.Direction);
				shader.setFloat(sL_cutOff, glm::cos(glm::radians(spotLight.second.cutOff)));
				shader.setFloat(sL_outerCutOff, glm::cos(glm::radians(spotLight.second.outerCutOff)));
				shader.setFloat(sL_constant, spotLight.second.constant);
				shader.setFloat(sL_linear, spotLight.second.linear);
				shader.setFloat(sL_quadratic, spotLight.second.quadratic);
				shader.setVec3(sL_ambient, spotLight.second.Mat.ambient);
				shader.setVec3(sL_diffuse, spotLight.second.Mat.diffuse);
				shader.setVec3(sL_specular, spotLight.second.Mat.specular);
				shader.setBool(sL_lightState, spotLight.second.stateLight);

				sL_i++;
			}

		}
		*/

		if (!textures.textures_LoadCache.empty())
			//if (!textures.texU_Data.empty()) ///OLD WAY TO USE TEXTURES
		{
			//textures.useTextures_PerMaterial(shader, 1); ///OLD WAY TO USE TEXTURES
			textures.use_MaterialTextures(shader, 1);
			shader.setBool("NotTexture", false);

			if (nameMesh == "CampoVegetacion_1")
			{
				//	SDL_Log(std::to_string(static_cast<int>(textures.texU_Data.size())).c_str());
			}

			if (RenderData_Set::skybox_D::skyBox_Current.active == true && !RenderData_Set::skybox_D::skyBox_Current.nameSkybox.empty())
			{
				texture::textureUnits textureUnit{ static_cast<texture::textureUnits>(static_cast<int>(textures.textures_LoadCache.size()) + 1) };

				RenderData_Set::skybox_D::skyBoxes_D[RenderData_Set::skybox_D::skyBox_Current.nameSkybox].bind_Texture(shader, "skybox", textureUnit);
				shader.transformMat3("transformation_SkyBox", RenderData_Set::skybox_D::skyBoxes_D["skyBox_day"].transform_SkyBox.rotationVec);
				shader.setBool("activeSkybox", true);

			}

		}

		else if (textures.textures_LoadCache.empty())
		{
			shader.setBool("NotTexture", true);
			shader.setBool("Mat_1.use_texture_diffuse", false);
			shader.setBool("Mat_1.use_texture_specular", false);
			//shader.setBool("useSpec", false);

			if (nameMesh == "CampoVegetacion_1")
			{
				//	SDL_Log(std::string("NOT TEXTURE::MESH::" + nameMesh).c_str());
			}

			shader.setBool("activeSkybox", false);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (data_HitAABB::selectedObj.first.mesh_ID == ID)
		{
			shading::config::change_refractiveIndex(settingsShader.refractiveIndex);
		}

		shader.setFloat("refractiveIndex", settingsShader.refractiveIndex);

		shader.setVec3("Mat.ambient", shaderSet.ambient);
		shader.setVec3("Mat.difusse", shaderSet.difusse);
		shader.setVec3("Mat.specular", shaderSet.specular);
		shader.setFloat("Mat.shiness", shaderSet.shiness);


		shader.setVec3("viewPos", cameras::cameras_D[cameras::name_CurrentCamera].posCam);
		shader.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);


	//	glBindVertexArray(VAO);
	//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//	glBindVertexArray(0);
	}
	unsigned int& Mesh::outVAO()
	{
		return VAO;
	}


	void Mesh::setMeshCoord(glm::vec3 posicionMesh, glm::vec3 scaleMesh)
	{
		MeshCoord.translateModel(posicionMesh);
		MeshCoord.scaleModel(scaleMesh);
		MeshCoord.setInverseTransformsAll();
	}

	void Mesh::updateVerticesPos()
	{
		int currentVertex{};
		for (auto& vertex : vertices)
		{
		//	glm::mat4 pos{ glm::mat4(1.0f) };
		//	pos = glm::translate(pos, vertex.posicion);

		//	pos = MeshCoord.model * pos;

			//----ACTUALIZACION DE LA POSICION DE LOS EJES----//
			glm::vec4 pos{ MeshCoord.model * glm::vec4(vertex.posicion.x, vertex.posicion.y, vertex.posicion.z, 1.0f) };
			verticesPos[currentVertex] = glm::vec3(pos.x, pos.y, pos.z);

			//----ACTUALIZACION DE LA POSICION DE LAS NORMALES----//
			glm::vec3 posNormal{ glm::mat3(glm::transpose(glm::inverse(MeshCoord.model))) * vertex.Normal };
			posNormal = glm::normalize(posNormal);

			glm::vec3 globalPosNormal{ glm::vec3(pos.x, pos.y, pos.z) + posNormal };

			normalsPos[currentVertex] = globalPosNormal;

			currentVertex += 1;
		}

	}
	void Mesh::destroyMesh()
	{

		ID = 0;

		nameMesh = "";
		vertices.clear();
		indices.clear();

	    verticesPos.clear();
		normalsPos.clear();

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}


	void Model::loadModel_CPU(Assimp_D::loadToCPU::ModelData_loadCPU model)
	{
        ID = FNV::str_to_hash(model.nameModel);  ///TO SAVE THE ID OF THE MODEL

		nameModel = model.nameModel;
		directory = model.directory;

        for (Uint8 i = 0; i < size_shaderType; i++)
        {
        	if (i != size_shaderType) {
        		shader_type find_type {static_cast<shader_type>(i)};

        		Uint8 pos {};
        		auto find_s = std::ranges::find_if(model.shader,
					[&](shader_SetType& find_sT)
					{
					   ++pos;
					  return  find_sT.type == find_type;
					}
					);

        		if (find_s != std::ranges::end(model.shader))
        		{
        			shaders_set.emplace_back(shader_SetType(find_s->name_shader, find_s->type));
        		}

        		else if (find_s == std::ranges::end(model.shader) )
        		{
        			shaders_set.emplace_back(shader_SetType("", find_type));
        		}
        	}
        }

		//nameShader = model.nameShader;


		for (auto& meshData : model.Meshes_LoadCPU)
		{
			meshes.emplace_back(Mesh(meshData)); //// ESCENCIAL PARA CARGA DE TEXTURAS, CAMBIAR LOS DATOS PARA QUE SOLAMENTE SE INGRESE EL NOMBRE DE LA TEXTURA JUNTO CON EL TIPO
		}

		calculate_centerBoundingBox();  ///TO CALCULATE THE CENTER OF THE MODEL

		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			std::string nameMeshNew{ nameModel + "_" + std::to_string(i + 1) };

			meshes[i].ID = FNV::str_to_hash(nameMeshNew);
			meshes[i].nameMesh = nameMeshNew;

			pos_find_secuencial.emplace(meshes[i].ID, static_cast<uint32_t>(i));  ///TO SAVE THE POSICION TO RESEARCH MORE FASTER MY MESHES
			pos_meshes.emplace_back(i);
			++num_meshes_secuencial;

			if (nameMeshNew == "CampoVegetacion_1")
			{
				std::cout << "FINDING \n";

			}

			meshes[i].setMeshCoord(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			meshes[i].MeshCoord.model = ModelCoord.model * meshes[i].MeshCoord.model;
			meshes[i].MeshCoord.setNormalModelMatrix();
			//meshes[i].MeshCoord.nameModel =
			//meshes[i].shaderSet = shaderSettings;

		}



	}
	void Model::loadModel(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings, unsigned int processFlags)
	{
		//IMPORT SHADER
		shaders.shaderCreation(vertexPath, fragmentPath);

		//set transform Model General
		setModelCoord(modelCoords);
		ModelGlobal_Coord = modelCoords;

		//import all Assimp
		Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


		const aiScene* scene = importer.ReadFile(path, processFlags);

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

			if (mesh->HasTextureCoords(0))
			{
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;

			}

			else
			{
			//	texCoords = glm::vec2(0.0f, 0.0f);
				SDL_Log("ERROR::TEXTURE_COORDS::NOT_FIND");
			}

			vertices.emplace_back(vertex, normals, texCoords);
			//SDL_Log(std::to_string(texCoords.x).c_str());
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

//			shaders.use();

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

		//aiGetMaterialTexture()
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

	Model::Model() {};
	Model::Model(std::string path, const char* vertexPath, const char* fragmentPath, coordModel modelCoords, shaderSettings shaderSettings, unsigned int processFlags)
	{
		loadModel(path, vertexPath, fragmentPath, modelCoords, shaderSettings, processFlags);
	}
	Model::Model(loadToCPU::ModelData_loadCPU model)
	{
		shaders_set.reserve(static_cast<int>(model.shader.size()));

		loadModel_CPU(model);
	}
 	Model::~Model()
	{
	 std::string log_destroy {"DELETING MODEL | " + nameModel};

	 SDL_Log(log_destroy.c_str());

	 destroyModel();  ////SEE IF THIS DESTROYS

	}

	void Model::setModelSettings(coordModel modelCoords, shaderSettings shaderSettings)
	{
		//set transform Model General
		setModelCoord(modelCoords);
		ModelGlobal_Coord = modelCoords;
		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			meshes[i].shaderSet = shaderSettings;
		}

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

		//	meshes[i].Draw(cam1, light, RenderData_Set::shader_D[nameShader]);  ///LAST WAY TO LOAD SHADERS
			meshes[i].Draw(cam1, light,
				RenderData_Set::shader_D[shaders_set[static_cast<Uint8>(shader_type::standard_Shader)].name_shader],
				shaders_set[static_cast<Uint8>(shader_type::viewNormals_Shader)].name_shader);
		}

			ModelCoord.lastModel = ModelCoord.model;


	}
	void Model::Draw_WL()
	{
		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			//meshes[i].Draw_WithLights(shaders); //DESACTIVADO TEMPORALMENTERE
		//	meshes[i].Draw_WithLights(RenderData_Set::shader_D[nameShader]); ///LAST WAY TO LOAD SHADERS
			shading::shader& standard_shader {RenderData_Set::shader_D[shaders_set[static_cast<Uint8>(shader_type::standard_Shader)].name_shader]};
			meshes[i].Draw_WithLights(
				standard_shader,
				shaders_set[static_cast<Uint8>(shader_type::viewNormals_Shader)].name_shader
				);
		}

	}

	void Model::Draw_singleMesh_ID(uint32_t mesh_ID, int shaderOp)
	{
		auto find_mesh {pos_find_secuencial.find(mesh_ID)};

		if (find_mesh != pos_find_secuencial.end())
	    {
			int pos = find_mesh->second;

			if (shaderOp == 0)
			{
				meshes[pos].Draw_Alone();
			}

			else if (shaderOp == 1) {
				shading::shader& standard_shader {RenderData_Set::shader_D[shaders_set[static_cast<Uint8>(shader_type::standard_Shader)].name_shader]};
				meshes[pos].Draw_WithLights(
					standard_shader,
					shaders_set[static_cast<Uint8>(shader_type::viewNormals_Shader)].name_shader
					);
			}
		}

		else if (find_mesh == pos_find_secuencial.end())
		{
			register_error_RM::register_inexistence_Mesh(ID, mesh_ID);
		}

	}

	void Model::DrawSingleMesh(std::string nameMesh, int shaderOp)
	{
		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			if (meshes[i].nameMesh == nameMesh)
			{
				if (shaderOp == 0)
				{
					meshes[i].Draw_Alone();
				}

				else if (shaderOp == 1)
				{
					//meshes[i].Draw_WithLights(shaders);//DESACTIVADO TEMPORALMENTE
				//	meshes[i].Draw_WithLights(RenderData_Set::shader_D[nameShader]); ///LAST WAY TO LOAD SHADERS
					meshes[i].Draw_WithLights(
						RenderData_Set::shader_D[shaders_set[static_cast<Uint8>(shader_type::standard_Shader)].name_shader],
				        shaders_set[static_cast<Uint8>(shader_type::viewNormals_Shader)].name_shader);
				}
			}
		}

	}

	void Model::DrawExcludeMesh(std::string nameMesh)
	{

		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			if (meshes[i].nameMesh != nameMesh)
			{
				//meshes[i].Draw_WithLights(shaders)
				meshes[i].Draw_WithLights(RenderData_Set::shader_D[shaders_set[static_cast<Uint8>(shader_type::standard_Shader)].name_shader],
							shaders_set[static_cast<Uint8>(shader_type::viewNormals_Shader)].name_shader);;  ///changes if i wrong
			}
		}

	}

	void Model::destroyModel()
	{
		for(int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			meshes[i].destroyMesh();
		}

		pos_find_secuencial.clear();
		pos_meshes.clear();
		num_meshes_secuencial = 0;

		ID = 0;
		nameModel = "";
		shaders_set.clear();

		meshes.clear();
		directory = "";
		textures_Loaded.clear();
		//glDeleteProgram(shaders.ID);   ////SEE IF I DONT INSERT SHADERS ANY MORE
	}

	void Model::calculate_centerBoundingBox()
	{
		glm::vec3 minPos{};
		glm::vec3 maxPos{};

		bool init_loop{};
      for (auto& mesh : meshes)
      {
      	if (init_loop == false)
      	{
      	 minPos = mesh.MeshCoord.posModel;
      	 maxPos = mesh.MeshCoord.posModel;
      	 init_loop = true;
      	}

      	if (init_loop == true)
      	{
      		if (mesh.MeshCoord.posModel.x < minPos.x)
      		{
              minPos.x = mesh.MeshCoord.posModel.x;
      		}

      		else if (mesh.MeshCoord.posModel.x > maxPos.x)
      		{
      			maxPos.x = mesh.MeshCoord.posModel.x;
      		}

      		if (mesh.MeshCoord.posModel.y < minPos.y)
      		{
      			minPos.y = mesh.MeshCoord.posModel.y;
      		}

      		else if (mesh.MeshCoord.posModel.y > maxPos.y)
      		{
      			maxPos.y = mesh.MeshCoord.posModel.y;
      		}

      		if (mesh.MeshCoord.posModel.z < minPos.z)
      		{
      			minPos.z = mesh.MeshCoord.posModel.z;
      		}

      		else if (mesh.MeshCoord.posModel.z > maxPos.z)
      		{
      			maxPos.z = mesh.MeshCoord.posModel.z;
      		}
      	}

      }
		ModelCoord.posModel_Base =(minPos + maxPos) * 0.5f;
		ModelCoord.posModel = ModelCoord.posModel_Base;

	}

	void Model::updateModel()
	{
		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{

			if (ModelCoord.model != ModelCoord.lastModel)
			{
				meshes[i].MeshCoord.model = ModelCoord.model * meshes[i].MeshCoord.modelCurrent;
				//meshes[i].MeshCoord.model = ModelCoord.modelCurrent;
				meshes[i].MeshCoord.setNormalModelMatrix();
				//meshes[i].updateVerticesPos();

				meshes[i].MeshCoord.lastModel = meshes[i].MeshCoord.model;
				//	meshes[i].MeshCoord.lastModel = meshes[i].MeshCoord.model;   /////CHECAR SI AFECTA A OTRAS FUNCIONES
			}

			meshes[i].updateVerticesPos();
			meshes[i].MeshCoord.refreshCenter_Pos();
		}

		if (ModelCoord.model != ModelCoord.lastModel)
		{
			ModelCoord.refreshCenter_Pos();
		}
	//	ModelCoord.lastModel = ModelCoord.model;
	}
	void Model::setNameModel(const std::string name)
	{
		nameModel = name;
		
		int seqNumMesh{1};
		for (auto& mesh : meshes)
		{
			std::string MeshName{ name + '_' + std::to_string(seqNumMesh) };
			mesh.nameMesh = MeshName;
			seqNumMesh++;
		}
		

	}
	void Model::setModelCoord(coordModel modelCoords)
	{
		ModelCoord.translateModel(modelCoords.posicion);
		ModelCoord.scaleModel(modelCoords.scale);
		ModelCoord.setPivotRotModel(modelCoords.pivotRot);
		ModelCoord.setAngRotModel(modelCoords.angRot);
		ModelCoord.setTransformsAll();
		ModelCoord.setNormalModelMatrix();

		ModelCoord.refreshCenter_Pos();  ///TO UPDATE THE POSICION OF THE MODEL
	}
	void Model::SetShinessTex_Mesh(int numMesh, float valueShiness)
	{
		meshes[numMesh].textures.shiness = valueShiness;

	}
	void Model::SetTexture_Mesh(const char* pathTexture, std::string nameMesh, texDataManager::typeTexture tex)
	{

		for (int i = 0; i < static_cast<int>(meshes.size()); i++)
		{
			if (meshes[i].nameMesh == nameMesh)
			{
				meshes[i].textures.insertNewTexture(pathTexture, tex);

				////OLD WAY TO LOAD TEXTURES
				/*
				textureD texture{};

				texture.id = TextureFromFile(pathTexture);
				texture.path = pathTexture;
				texture.type = texture::typeTexture[tex];

				textures_Loaded.emplace_back(texture);
				meshes[i].textures.insertTexture(texture.id, texture.path.C_Str(), texture.type);
				*/
			}

		}

	}
	void Model::SetOrderRender_Mesh(const std::string nameMesh, renderSeq renderOrder)
	{
		for (auto& mesh : meshes)
		{
			if (nameMesh == mesh.nameMesh)
			{
				mesh.renderP = renderOrder;

			}

		}

	}
	void Model::BlendModeTexture_Mesh(const std::string nameMesh, bool op)
	{
		
		for (auto& mesh : meshes)
		{
			if (nameMesh == mesh.nameMesh)
			{
				mesh.textures.active_BlendMode = op;

			}

		}
	}
	void Model::loadTemporalShaders(const char* vertexPath, const char* fragmentPath)
	{
		shaders.shaderCreation(vertexPath, fragmentPath);
	}

	std::vector<Mesh>& Model::outMeshes()
	{
		return meshes;
	}
	shading::shader& Model::outShader()
	{
		return shaders;
	}
	const int& Model::out_numSec_meshes()
	{
		return num_meshes_secuencial;
	}


	Mesh& Model::outSpecificMesh(uint32_t mesh_ID)
	{
		auto find_mesh {pos_find_secuencial.find(mesh_ID)};

		if (find_mesh != pos_find_secuencial.end())
		{
		   uint32_t& m_pos{find_mesh->second};
           uint32_t& pos_vector{pos_meshes[m_pos]};

		   return meshes[pos_vector];
		}

		else if (find_mesh == pos_find_secuencial.end())
		{
			std::string log_error_mesh {"ERROR::WE CANNOT FIND A MESH IN THE MODEL (" + nameModel + ")"};
			register_error_RM::register_error_withSentence(log_error_mesh.c_str());
		}

       return RenderData_Set::error_MeshF;
	}
	Mesh& Model::out_MeshByPos(uint32_t pos)
	{
	   if (pos <= num_meshes_secuencial - 1)
	   {
	  // 	SDL_Log(meshes[pos].nameMesh.c_str());
         return meshes[pos];
	   }

		return RenderData_Set::error_MeshF;
	}

	void Model::insert_mesh(loadToCPU::MeshData_loadCPU mesh_D)
	{
		meshes.emplace_back(mesh_D);

		pos_find_secuencial.emplace(meshes[num_meshes_secuencial - 1].ID, num_meshes_secuencial);
		pos_meshes.emplace_back(num_meshes_secuencial);

		++num_meshes_secuencial;

	}
	void Model::delete_mesh(uint32_t mesh_ID)
	{
		auto find_mesh {pos_find_secuencial.find(mesh_ID)};

		if (find_mesh != pos_find_secuencial.end())
	    {
           uint32_t current_pos {pos_find_secuencial[mesh_ID]};
           int pos = pos_meshes[current_pos];

		   std::swap(meshes[pos], meshes[num_meshes_secuencial - 1]);
		   meshes[num_meshes_secuencial - 1].destroyMesh();
		   meshes.pop_back();

		   std::swap(pos_meshes[pos], pos_meshes[num_meshes_secuencial - 1]);
		   pos_meshes[pos] = pos;
		   pos_meshes.pop_back();

		   pos_find_secuencial[meshes[pos].ID] = pos_meshes[pos];
		   pos_find_secuencial.erase(mesh_ID);

		   --num_meshes_secuencial;
		}

		else if (find_mesh == pos_find_secuencial.end())
		{
			register_error_RM::register_error_withSentence("ERROR::WE CANNOT DELETE THE MESH::WE CANNOT FIND IT");
		}
	}



	int Model::numMeshes()
	{
		return static_cast<int>(meshes.size());
	}
	void Model::refresh_ModelCoord()
	{
		ModelCoord.translateModel(ModelGlobal_Coord.posicion);
		ModelCoord.scaleModel(ModelGlobal_Coord.scale);
		ModelCoord.setPivotRotModel(ModelGlobal_Coord.pivotRot);
		ModelCoord.setAngRotModel(ModelGlobal_Coord.angRot);
		ModelCoord.setTransformsAll();
		ModelCoord.setNormalModelMatrix();
	}
	

	namespace loadToCPU
	{
		texDataManager::TextureData_File LoadTextureFromFile(const char* path, std::string directory, std::string typeTexture, bool gamma)
		{
			std::string pa = std::string(path);

			if (directory != "")
			{
				pa = directory + "/" + pa;
				SDL_Log(pa.c_str());
			}

			int width{};
			int height{};
			int nrChannels{};
			unsigned char* DataT = stbi_load(pa.c_str(), &width, &height, &nrChannels, 0);

			stbi_set_flip_vertically_on_load(true);

			//return texDataManager::TextureData_File("", texDataManager::typeTexture{}, typeTexture, std::move(DataT), width, height, nrChannels)
			return texDataManager::TextureData_File();  ////INSERVIBLE, NO FUNCIONA PARA CARGAR TEXTURAS
		}

		std::queue<ModelData_loadCPU> modelsData{};
		std::atomic<int> atomic_CounterModel(0);
		std::atomic<int> atomic_sizeModels(0);
		std::atomic<bool> flagsAtomic(false);
		std::atomic<bool> finishLoadModels(false);
		std::mutex mutexModel;

		std::mutex mutexImporter;
		std::condition_variable manageImporter;
		Assimp::Importer importer{};

		///FUNCTION LEGACY::LOAD TEXTURES
		std::vector<texDataManager::TextureData_File> loadMatTextures(aiMaterial* mat, aiTextureType matType, std::string typeName, std::string directory)
		{
			std::vector<texDataManager::TextureData_File> tex{};
			std::vector<texDataManager::TextureData_File> tex_Loaded{};
			//aiGetMaterialTexture()
			for (int i = 0; i < mat->GetTextureCount(matType); i++)
			{
				aiString str{};
				mat->GetTexture(matType, i, &str);
				//textureD texture{};
				bool skip{ false };

			//	SDL_Log(str.C_Str());

				for (int i = 0; i < static_cast<int>(tex_Loaded.size()); i++)
				{
					if (std::strcmp(tex_Loaded[i].completePath.c_str(), str.C_Str()) == 0)
					{
						skip = true;
						break;
					}

				}

				if (!skip)
				{

					//texDataManager::TextureData_File texture{ LoadTextureFromFile(str.C_Str(), directory, typeName) }; ////OLD WAY TO LOAD TEXTURES
					texDataManager::TextureData_File textureLoad{ textureCache::manageLoadTexture(str.C_Str(), directory, typeName) }; ////NEW WAY TO LOAD TEXTURES
					tex.emplace_back(textureLoad);
					tex_Loaded.emplace_back(textureLoad);

					//SDL_Log(texDataManager::typeTex_String[texture.typeTexture].c_str());
					//SDL_Log(texture.typeTexture.c_str());

				}
			}

			return tex;
		} ///FUNCTION LEGACY::LOAD TEXTURES

		std::vector<texDataManager::TextureData_File> loadMatTextures_Cache(aiMaterial* mat, aiTextureType matType, std::string typeName, std::string directory)
		{
			std::vector<texDataManager::TextureData_File> dataTextures{};

			for (int i = 0; i < mat->GetTextureCount(matType); i++)
			{
				aiString pathStr{};
				mat->GetTexture(matType, i, &pathStr);
				
				const char* path = pathStr.C_Str();
				dataTextures.emplace_back(textureCache::manageLoadTexture(std::string(path), directory, typeName));
		
			}

			return dataTextures;
		}

		void loadModelsThread(std::queue<insertProcessModel> models)
		{
			int countUpload{};
			auto processModelsFunc = [&]()
				{
					ModelData_loadCPU modelAssimp{ processModel(models.front()) };
					{
						std::lock_guard<std::mutex> lock(mutexModel);
						modelsData.push(modelAssimp);
					}

					++atomic_CounterModel;
					models.pop();
					countUpload++;
				};

			//for (auto& loadModel : models)
		
			while (!models.empty())
			{
				if (countUpload == 0)
				{
					processModelsFunc();
				}

				std::unique_lock<std::mutex> lockImporter(mutexImporter);

				manageImporter.wait(lockImporter, [&]
					{
						processModelsFunc();
						//lockImporter.unlock();
						//atomic_sizeModels.fetch_add(1);
						return true;
					}
				);
			}

			flagsAtomic = true;
		}

		ModelData_loadCPU processModel(insertProcessModel dataModel)
		{
			ModelData_loadCPU data_Out{};

		//	Assimp::Importer importer;

			std::filesystem::path pathModel{ dataModel.path };
			const aiScene* scene = importer.ReadFile(pathModel.string(), dataModel.flagsProcessModel);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
				return ModelData_loadCPU();
			}

			data_Out.nameModel = dataModel.nameModel;

			//for (auto& shader_p : dataModel.shader)
		//	{
			//	data_Out.shader.emplace_back(shader_p);
			data_Out.shader = dataModel.shader;
			//}
		//	data_Out.nameShader = dataModel.nameShader;
			data_Out.directory = dataModel.path.substr(0, dataModel.path.find_last_of('/'));

			std::vector<MeshData_loadCPU> dataMeshes{};
			processNode(scene->mRootNode, scene, dataMeshes, data_Out.directory);

			data_Out.Meshes_LoadCPU = dataMeshes;

			return data_Out;
		}
		void processNode(aiNode* node, const aiScene* scene, std::vector<MeshData_loadCPU>& meshes, std::string directory)
		{
			int countMeshName{ 1 };
			for (int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.emplace_back(processMesh(mesh, scene, directory));
				countMeshName++;
				//SDL_Log(nameMesh.c_str());
			}

			for (int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene, meshes, directory);

			}

		}
		MeshData_loadCPU processMesh(aiMesh* mesh, const aiScene* scene, std::string directory)
		{
			std::vector<vertexD> vertices{};
			std::vector<unsigned int> indices{};
			std::vector<texDataManager::TextureData_File> textures{};

			for (int i = 0; i < mesh->mNumVertices; i++)
			{
				glm::vec3 vert{};

				vert.x = mesh->mVertices[i].x;
				vert.y = mesh->mVertices[i].y;
				vert.z = mesh->mVertices[i].z;

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
					SDL_Log("ERROR::NOT::TEXCOORDS");
				}

				vertices.emplace_back(vertexD(vert, normals, texCoords));
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

				std::vector<texDataManager::TextureData_File> difusseMaps{ loadMatTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory) };

				textures.insert(textures.end(), difusseMaps.begin(), difusseMaps.end());

				std::vector<texDataManager::TextureData_File> specularMaps{ loadMatTextures(material, aiTextureType_SPECULAR, "texture_specular", directory) };

				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			return MeshData_loadCPU(vertices, indices, textures);

		}


	}

}

namespace individualComp
{
	singleTriangle::singleTriangle() {};
	singleTriangle::singleTriangle(std::vector<Assimp_D::vertexD> vertex)
	{
		setTriangle(vertex);
	}
	void singleTriangle::setTriangle(std::vector<Assimp_D::vertexD> vertex)
	{
	    if (!this->vertex.empty())
		{
			this->vertex.clear();
			this->vertex.shrink_to_fit();
			destroy();
		}

		this->vertex = vertex;

		///CALCULATE CENTROID TRIANGLE

		centroidTriangle = transformation_basics::centroidObj(std::vector<glm::vec3>{vertex[0].posicion, vertex[1].posicion, vertex[2].posicion});

	}
	void singleTriangle::insertTriangle()
	{

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Assimp_D::vertexD), &vertex[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Assimp_D::vertexD), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Assimp_D::vertexD), (void*)offsetof(Assimp_D::vertexD, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Assimp_D::vertexD), (void*)offsetof(Assimp_D::vertexD, TexCoord));

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void singleTriangle::setShaderSettings(Assimp_D::shaderSettings shader)
	{
		shaderSet = shader;
	}
	void singleTriangle::draw()
	{
        shading::shader& shader_set {RenderData_Set::shader_D[shaderName]};

		shader_set.use();

		shader_set.transformMat("model", MeshCoord.model);
	//	shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
	//	shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
		shader_set.setVec3("objectColor", shaderSet.objectColor);


		/*
		if (static_cast<int>(RenderData_Set::pointLights_D.size()) > 0)
		{

			for (int i = 0; i < static_cast<int>(RenderData_Set::pointLights_D.size()); i++)
			{
				std::string pL_name{ "pointLights_Array[" + std::to_string(i) + "]" };

				std::string pL_color{ pL_name + ".lightColor" };
				std::string pL_Posicion{ pL_name + ".lightPos" };
				std::string pL_constant{ pL_name + ".constant" };
				std::string pL_linear{ pL_name + ".linear" };
				std::string pL_quadratic{ pL_name + ".quadratic" };
				std::string pL_ambient{ pL_name + ".ambient" };
				std::string pL_diffuse{ pL_name + ".diffuse" };
				std::string pL_specular{ pL_name + ".specular" };

				shader.setVec3(pL_color, RenderData_Set::pointLights_D[i].Color);
				shader.setVec3(pL_Posicion, RenderData_Set::pointLights_D[i].Posicion);
				shader.setFloat(pL_constant, RenderData_Set::pointLights_D[i].constant);
				shader.setFloat(pL_linear, RenderData_Set::pointLights_D[i].linear);
				shader.setFloat(pL_quadratic, RenderData_Set::pointLights_D[i].quadratic);
				shader.setVec3(pL_ambient, RenderData_Set::pointLights_D[i].Mat.ambient);
				shader.setVec3(pL_diffuse, RenderData_Set::pointLights_D[i].Mat.diffuse);
				shader.setVec3(pL_specular, RenderData_Set::pointLights_D[i].Mat.specular);

			}
		}

		////////////Corregir aqui y revisar si funciona nullptr
		if (static_cast<int>(RenderData_Set::directionalLights_D.size()) > 0)
		{
			int dirLight_pos{ 1 };

			for (int i = 0; i < static_cast<int>(RenderData_Set::directionalLights_D.size()); i++)
			{
				//for (auto& dL : *directionalLights)
				//{
				std::string dL_name{ "directionalLight_" + std::to_string(dirLight_pos++) };
				//	std::string dL_color{ dL_name + ".lightColor" };
				std::string dL_direction{ dL_name + ".lightDir" };
				std::string dL_ambient{ dL_name + ".ambient" };
				std::string dL_diffuse{ dL_name + "diffuse" };
				std::string dL_specular{ dL_name + ".specular" };

				//shader.setVec3(dL_color, directionalLights[i].Color);
				shader.setVec3(dL_direction, RenderData_Set::directionalLights_D[i].Direction);
				shader.setVec3(dL_ambient, RenderData_Set::directionalLights_D[i].Mat.ambient);
				shader.setVec3(dL_diffuse, RenderData_Set::directionalLights_D[i].Mat.diffuse);
				shader.setVec3(dL_specular, RenderData_Set::directionalLights_D[i].Mat.specular);


			}

		}

		if (static_cast<int>(RenderData_Set::spotLights_D.size()) > 0)
		{
			int sL_i{};

			for (auto& spotLight : RenderData_Set::spotLights_D)
			{
				std::string sL_name{ "spotLights_Array[" + std::to_string(sL_i) + "]" };

				std::string sL_Posicion{ sL_name + ".lightPos" };
				std::string sL_Direction{ sL_name + ".lightDir" };
				std::string sL_cutOff{ sL_name + ".cutOff" };
				std::string sL_outerCutOff{ sL_name + ".outerCutOff" };
				std::string sL_constant{ sL_name + ".constant" };
				std::string sL_linear{ sL_name + ".linear" };
				std::string sL_quadratic{ sL_name + ".quadratic" };
				std::string sL_ambient{ sL_name + ".ambient" };
				std::string sL_diffuse{ sL_name + ".diffuse" };
				std::string sL_specular{ sL_name + ".specular" };
				std::string sL_lightState{ sL_name + ".lightState" };

				shader.setVec3(sL_Posicion, spotLight.second.Posicion);
				shader.setVec3(sL_Direction, spotLight.second.Direction);
				shader.setFloat(sL_cutOff, glm::cos(glm::radians(spotLight.second.cutOff)));
				shader.setFloat(sL_outerCutOff, glm::cos(glm::radians(spotLight.second.outerCutOff)));
				shader.setFloat(sL_constant, spotLight.second.constant);
				shader.setFloat(sL_linear, spotLight.second.linear);
				shader.setFloat(sL_quadratic, spotLight.second.quadratic);
				shader.setVec3(sL_ambient, spotLight.second.Mat.ambient);
				shader.setVec3(sL_diffuse, spotLight.second.Mat.diffuse);
				shader.setVec3(sL_specular, spotLight.second.Mat.specular);
				shader.setBool(sL_lightState, spotLight.second.stateLight);

				sL_i++;
			}


		}
*/

		if (!texture.textures_LoadCache.empty())
		{
			texture.use_MaterialTextures(shader_set, 1);

		}

		shader_set.setVec3("Mat.ambient", shaderSet.ambient);
		shader_set.setVec3("Mat.difusse", shaderSet.difusse);
		shader_set.setVec3("Mat.specular", shaderSet.specular);
		shader_set.setFloat("Mat.shiness", shaderSet.shiness);

		shader_set.setVec3("viewPos", cameras::cameras_D[cameras::name_CurrentCamera].posCam);
		shader_set.transformMat3("modelMatrix", MeshCoord.normalModelMatrix);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


	}
	void singleTriangle::drawTest_2()
	{
		utilities::entity* entity_model {RenderData_Set::ModelsScene_D->out_entity_model(name.model_ID)}; ///SEE IF THIS WORKS

       if (entity_model != nullptr)
       	{
	       std::vector<Assimp_D::Mesh>& meshes{ entity_model->model_entity->outMeshes() };

       	for (int i = 0; i < static_cast<int>(meshes.size()); i++)
       	{
       		if (meshes[i].ID == name.mesh_ID)
       		{
       			//shading::shader& shader{ RenderData_Set::AssimpModel_D[name.nameModel].outShader() };///DESACTIVADO TEMPORALMENTE
       			//	shading::shader& shader{ RenderData_Set::shader_D[RenderData_Set::AssimpModel_D[name.nameModel].nameShader]};

       			///		meshes[i].build_PreDraw(RenderData_Set::shader_D[RenderData_Set::AssimpModel_D[name.nameModel].nameShader]);  ///LAST WAY TO LOAD SHADERS
       			Uint8 pos_shader = static_cast<Uint8>(Assimp_D::shader_type::standard_Shader);
       			meshes[i].build_PreDraw(RenderData_Set::shader_D[entity_model->model_entity->shaders_set[pos_shader].name_shader]);
       			//std::cout << RenderData_Set::AssimpModel_D[name.nameModel].nameShader << '\n';


       			//std::cout << "TRIANGLE::FIND\n";
       			break;
       		}
       	}

       //	std::cout << VAO << '\n';

       	glBindVertexArray(VAO);
       	glDrawArrays(GL_TRIANGLES, 0, 3);
       	glBindVertexArray(0);
       }

	   else if (entity_model == nullptr)
	   {
	   	 std::string error_log { RenderData_Set::AssimpModel_D->get_nameModel(name.model_ID)};

	   	error_log = "SINGLE TRIANGLE ERROR::WE CANNOT FIND THE MODEL --" + error_log + " --  | ModelAssimp.cpp ---> singleTriangle::drawTest_2()";

         SDL_Log(error_log.c_str());
	   }
	}

	void singleTriangle::drawSelection()
	{

		//std::vector<Assimp_D::Mesh>& meshes{ RenderData_Set::AssimpModel_D[name.nameModel]->outMeshes() };

			glm::mat4 model{ glm::mat4(1.0f) };

	//	utilities::entity* entity_model {RenderData_Set::ModelsScene_D->out_entity_model(name.model_ID)};  ///TEST IF WORKS
		Assimp_D::Mesh* model_Entity {RenderData_Set::ModelsScene_D->out_mesh_fromModel(name.model_ID, name.mesh_ID)};///TEST IF WORKS


			if (model_Entity != nullptr)
			{
			    model = model_Entity->MeshCoord.model;

				model_Entity = nullptr;
			}

			else  //MAKE A REGISTER ERROR
			{
					register_error_RM::register_inexistence_Mesh(name.model_ID, name.mesh_ID);  ///SEE IF I HAVE AN ERROR AND THIS REGISTER THEM
				//	register_error_RM::register_inexistence_Model(name.model_ID);
			}
			//	scaleModel = scaleModel * MeshCoord.model;

			RenderData_Set::stencilTest::stencilTest_shader.use();

			RenderData_Set::stencilTest::stencilTest_shader.transformMat("model", model);
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
			RenderData_Set::stencilTest::stencilTest_shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

			RenderData_Set::stencilTest::stencilTest_shader.setVec3("color_stencil", glm::vec3(1.0));
			RenderData_Set::stencilTest::stencilTest_shader.setInt("selectionStencil", 1);
			RenderData_Set::stencilTest::stencilTest_shader.setVec3("centroidTriangle", centroidTriangle);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}


	void singleTriangle::updateTexture()
	{
	//	std::unique_ptr<std::vector<Assimp_D::Mesh>> meshes{ std::make_unique<std::vector<Assimp_D::Mesh>>(RenderData_Set::AssimpModel_D[name.nameModel]->outMeshes()) };

	    Assimp_D::Mesh* mesh_Entity {RenderData_Set::ModelsScene_D->out_mesh_fromModel(name.model_ID, name.mesh_ID)};

		if (mesh_Entity != nullptr)
		{
		    texture = mesh_Entity->textures;

		//	delete mesh_Entity;
			mesh_Entity = nullptr;

		}

		else if (mesh_Entity == nullptr)
		{
			register_error_RM::register_inexistence_Mesh(name.model_ID, name.mesh_ID); /// SEE IF WORKS
		}

	}
	void singleTriangle::updateModel()
	{
		Assimp_D::Mesh* mesh_Entity {RenderData_Set::ModelsScene_D->out_mesh_fromModel(name.model_ID, name.mesh_ID)};

		if (mesh_Entity != nullptr)
		{
//		MeshCoord = meshes->at(i).MeshCoord;
		 MeshCoord.model = mesh_Entity->MeshCoord.model;
		 MeshCoord.normalModelMatrix = mesh_Entity->MeshCoord.normalModelMatrix;

		// delete mesh_Entity;
		 mesh_Entity = nullptr;

		}

		else if (mesh_Entity == nullptr)
		{
			register_error_RM::register_inexistence_Mesh(name.model_ID, name.mesh_ID); /// SEE IF WORKS
		}

	}

	void singleTriangle::destroy() /// I ONLY CHANGE THE RETURN TYPE OF THE MODEL AND MESH, FROM string TO uint32_t
	{
		name.model_ID = 0;
		name.mesh_ID = 0;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}


	Multiple_AssimpMesh::Multiple_AssimpMesh() = default;
	Multiple_AssimpMesh::Multiple_AssimpMesh(Assimp_D::structModelName meshToCopy, std::vector<glm::vec3> quantityMesh)
	{
		setMultipleMesh(meshToCopy, quantityMesh);

	};
	Multiple_AssimpMesh::Multiple_AssimpMesh(Multiple_AssimpMesh&& mAssimp_copy) noexcept = default;
	Multiple_AssimpMesh::Multiple_AssimpMesh(Multiple_AssimpMesh& mAssimp_copy) = default;


	Multiple_AssimpMesh Multiple_AssimpMesh::operator=(Multiple_AssimpMesh&& mAssimp_copy) noexcept
	{
			name = mAssimp_copy.name;

			for (auto mAC : mAssimp_copy.setDataMesh_Multi)
			{

				setDataMesh_Multi.emplace_back
				(
				mAC.subNameMesh_ID,
				mAC.posicion,
				mAC.verticesPos,
				mAC.model
				);
			}

			ActiveMesh = mAssimp_copy.ActiveMesh;

	}
	Multiple_AssimpMesh Multiple_AssimpMesh::operator=(Multiple_AssimpMesh& mAssimp_copy)
	{
		name = mAssimp_copy.name;

		for (auto mAC : mAssimp_copy.setDataMesh_Multi)
		{

			setDataMesh_Multi.emplace_back
			(
			mAC.subNameMesh_ID,
			mAC.posicion,
			mAC.verticesPos,
			mAC.model
			);
		}

		ActiveMesh = mAssimp_copy.ActiveMesh;
	}


	Multiple_AssimpMesh::~Multiple_AssimpMesh() = default;

	void Multiple_AssimpMesh::setMultipleMesh(Assimp_D::structModelName meshToCopy, std::vector<glm::vec3> quantityMesh)
	{

		Assimp_D::Mesh* mesh_Entity {RenderData_Set::ModelsScene_D->out_mesh_fromModel(meshToCopy.model_ID, meshToCopy.mesh_ID)};

			if (mesh_Entity != nullptr)///Si lo encuentra hara una compia dependiendo de la cantidad de posiciones de quantityMesh
			{
				int countMeshCopy{1};
				for (auto& meshSet : quantityMesh)
				{
					glm::mat4 posModel{ glm::mat4(1.0f) };
					posModel = glm::translate(posModel, meshSet);

					std::string SubNameMesh{ mesh_Entity->nameMesh + "_" + std::to_string(countMeshCopy) };

					std::vector<glm::vec3> newVerticesPos{};
					for (int m = 0; m < static_cast<int>(mesh_Entity->verticesPos.size()); m++)
					{
						glm::vec4 ver{ posModel * glm::vec4(mesh_Entity->verticesPos[m].x, mesh_Entity->verticesPos[m].y, mesh_Entity->verticesPos[m].z, 1.0f) };

						newVerticesPos.emplace_back(glm::vec3(ver.x, ver.y, ver.z));

					}

					setDataMesh_Multi.emplace_back(FNV::str_to_hash(SubNameMesh), meshSet, newVerticesPos, posModel);
					countMeshCopy++;
				}
				
				name = meshToCopy;
				ActiveMesh = true;

			//	delete mesh_Entity;
				mesh_Entity = nullptr;

			}

		else if (mesh_Entity == nullptr)
		{
			register_error_RM::register_inexistence_Mesh(meshToCopy.model_ID, meshToCopy.mesh_ID); /// SEE IF WORKS
		}
	}

	void Multiple_AssimpMesh::drawMultipleMesh()   /////Renderizar los Objetos
	{
		Assimp_D::Mesh* mesh_Entity {RenderData_Set::ModelsScene_D->out_mesh_fromModel(name.model_ID, name.mesh_ID)};

		if (mesh_Entity != nullptr)
					{

						if (mesh_Entity->textures.active_BlendMode == true)
						{
							glEnable(GL_BLEND);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glDepthMask(GL_FALSE);

						}

							std::vector<uint32_t> meshesTo_Render{};
							meshesTo_Render.resize(static_cast<int>(setDataMesh_Multi.size()));

							for (auto& meshCopy : setDataMesh_Multi)
							{
								int posicionInMesh{};

								if (mesh_Entity->renderP == Assimp_D::renderSeq::renderFar)
								{
									posicionInMesh = static_cast<int>(setDataMesh_Multi.size()) - 1 ;
								}

									float distLenght_Current{ glm::length(cameras::cameras_D[cameras::name_CurrentCamera].posCam - meshCopy.posicion) };

				 				for (int i = 0; i < static_cast<int>(setDataMesh_Multi.size()); i++)
									{
										float distLenght_seq{ glm::length(cameras::cameras_D[cameras::name_CurrentCamera].posCam - setDataMesh_Multi[i].posicion) };

									
										if (distLenght_seq < distLenght_Current)
										{
											if (mesh_Entity->renderP == Assimp_D::renderSeq::renderNear)
											{
												posicionInMesh++;
											}
											else if (mesh_Entity->renderP == Assimp_D::renderSeq::renderFar)
											{
												posicionInMesh--;
											}
										}


									}

									meshesTo_Render[posicionInMesh] = meshCopy.subNameMesh_ID;

								}

								for (auto& nameMesh : meshesTo_Render)
								{
									for (auto& meshCopy : setDataMesh_Multi)
									{
										///////AQUI CONTINUAR
										if (nameMesh == meshCopy.subNameMesh_ID)
										{
											//shading::shader& shader_Set{ model_Assimp.second.outShader() };
											//shading::shader& shader_Set{ RenderData_Set::shader_D[model_Assimp.second.nameShader]}; ///LAST WAY TO LOAD SHADERS
											const Assimp_D::shader_SetType& shader_t {RenderData_Set::ModelsScene_D->out_shaderSet_fromModel(name.model_ID, Assimp_D::shader_type::standard_Shader)};

											if (!shader_t.name_shader.empty()) {
												shading::shader& shader_Set{ RenderData_Set::shader_D[shader_t.name_shader]};
												shader_Set.use();
												shader_Set.transformMat("model", meshCopy.model);

												const Assimp_D::shader_SetType& shader_N{RenderData_Set::ModelsScene_D->out_shaderSet_fromModel(name.model_ID, Assimp_D::shader_type::viewNormals_Shader)};

												mesh_Entity->Draw_WithoutModel(
													shader_Set,
													shader_N.name_shader);  /////CHANGE IF I DON HAVE THE CORRECT NORMALS
												break;
											}
										}
									}

								}

						if (mesh_Entity->textures.active_BlendMode == true)
						{
							glDisable(GL_BLEND);
							glDepthMask(GL_TRUE);
						}

			mesh_Entity = nullptr;
					}

		else if (mesh_Entity == nullptr)
		{
			register_error_RM::register_inexistence_Mesh(name.model_ID, name.mesh_ID); /// SEE IF WORKS

		}

	}


}
