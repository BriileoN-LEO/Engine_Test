#include "textureManager.h"
#include "stb_image.h"

namespace texDataManager
{
	 std::map<std::string, typeTexture> typeTex_String
	{
		{"texture_diffuse", typeTexture::diffuse},
		{"texture_specular", typeTexture::specular}
	};
	 std::map<typeTexture, texToShader> typeTex_T
	{
		{typeTexture::diffuse, texToShader(
			"texture_diffuse", 
		    "use_texture_diffuse"
		)},


		{typeTexture::specular, texToShader(
			"texture_specular",
			"use_texture_specular"
		)},
	};

}

namespace textureCache
{
	std::vector<texDataManager::preloaded_TextureD_info> preLoadedTextures{};
	std::map<std::string, texDataManager::textureD_info> textures{};

	///PRELOADED
	ktxTexture* loadTexture_PreCompress_KTX(const char* path, bool& exist)
	{
		ktxTexture* texture;
		KTX_error_code result;

		result = ktxTexture_CreateFromNamedFile(path, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture);

		if (result == KTX_SUCCESS)
		{
			exist = true;
			std::cout << "SUCCESS::LOAD::TEXTURES::KTX\n";
		}

		else
		{
			exist = false;
		}

		return texture;
	}
	texDataManager::standardTexture loadTexture_CompressInCompile(const char* path, bool& exist)
	{
		int width{};
		int height{};
		int nrChannels{};
		unsigned char* DataT = stbi_load(path, &width, &height, &nrChannels, 0);

	//	stbi_set_flip_vertically_on_load(true);

		if (DataT)
		{
			exist = true;
		//	std::cout << "SUCCESS::LOAD::TEXTURES::STANDARD\n";
		}

		else
		{
			exist = false;
		}

		return texDataManager::standardTexture(std::move(DataT), width, height, nrChannels);
	}
	texDataManager::TextureData_File manageLoadTexture(std::string path, std::string directory, std::string typeTextures) ///Insertar texturas en el cache de textura, pasar la informa
	{
		bool existenceTex{};

		std::string path_to_KTX2{ path };
		//std::reverse(path_to_DDS.begin(), path_to_DDS.end());
		path_to_KTX2 = path_to_KTX2.substr(0, path_to_KTX2.find_last_of('.'));
		path_to_KTX2 += ".ktx2";
		path_to_KTX2 = directory + path_to_KTX2;
	//	path_to_KTX2 = directory + '/' + path_to_KTX2;

		std::string path_Normal{ directory + '/' + path };
		
		//std::cout << path_to_KTX2 << "\n";

		texDataManager::TextureData_File texturesData{};



		for (auto tex : textureCache::preLoadedTextures)
		{
			if (tex.completePath == path_to_KTX2 || tex.completePath == path_Normal)
			{
				texturesData.nameTexture = tex.nameTexture;
				texturesData.typeTex = tex.typeTex;
				texturesData.completePath = tex.completePath;
				existenceTex = true;
			}

		}

		if (existenceTex == false)
		{
			std::string nameTexture{ path.substr(0, path.find_last_of('.')) }; ////el nombre de la textura se obtiene apartir del nombre.

			auto loadTexture_standard = [&](bool& correctLoading) -> texDataManager::preloaded_TextureD_info
				{
					texDataManager::standardTexture textureLoadStandard{ loadTexture_CompressInCompile(path_Normal.c_str(), correctLoading) };
					texDataManager::preloaded_TextureD_info dataFile{};

					if (correctLoading == true)
					{
						dataFile.nameTexture = nameTexture;
						dataFile.typeTex = texDataManager::typeTex_String[typeTextures];
						dataFile.completePath = path_Normal;

						dataFile.textureData_F = textureLoadStandard; ///REVISAR SI ESTA LINEA NO DA ERROR
						dataFile.format = texDataManager::formatImage::standard;
					}

					return dataFile;
				};

			auto loadTexture_KTX = [&](bool& correctLoading)->texDataManager::preloaded_TextureD_info
				{
					ktxTexture* textureLoadKTX{ loadTexture_PreCompress_KTX(path_to_KTX2.c_str(), correctLoading) };
					texDataManager::preloaded_TextureD_info dataFile{};

					if (correctLoading == true)
					{
						dataFile.nameTexture = nameTexture;
						dataFile.typeTex = texDataManager::typeTex_String[typeTextures];
						dataFile.completePath = path_Normal;
						dataFile.textureData_F = std::move(textureLoadKTX);
						dataFile.format = texDataManager::formatImage::KTX;
					}
					return dataFile;
				};


			bool existTex_inFile{};
			texDataManager::preloaded_TextureD_info preLoadTex{};

			std::vector<std::function<texDataManager::preloaded_TextureD_info(bool&)>> functions_Load; ///FUNCIONES PARA CARGAR LAS TEXTURAS
			functions_Load.push_back(loadTexture_KTX);
			functions_Load.push_back(loadTexture_standard);

			///CANTIDAD DE TIPOS DE DATOS DE TEXTURA
			for (auto FunctionTex : functions_Load)
			{
				preLoadTex = FunctionTex(existTex_inFile);
				if (existTex_inFile == true)
				{
					textureCache::preLoadedTextures.emplace_back(preLoadTex);
					break;
				}
			}

			if (existTex_inFile)
			{
				texturesData.nameTexture = preLoadTex.nameTexture;
				texturesData.typeTex = preLoadTex.typeTex;
				texturesData.completePath = preLoadTex.completePath;
				texturesData.format = preLoadTex.format;
			}
			////COMPLETAR AQUI

			/// VER SI SE PUEDE GUARDAR EL FORMATO DDS EN UN UNSIGNED CHAR O CONTINUAR

		}

		return texturesData;
	}

	///LOADED TO GPU
	GLuint loadTextureKTX(ktxTexture* texKTX)
	{
		KTX_error_code resultLoad;
		GLuint idTexture{};
		GLenum target{}, glerror{};

		resultLoad = ktxTexture_GLUpload(texKTX, &idTexture, &target, &glerror);

		glBindTexture(target, idTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		ktxTexture_Destroy(texKTX);
		glBindTexture(target, 0);

		std::cout << "LOADING::KTX\n";

		return idTexture;
	}
	GLuint loadTextureStandard(texDataManager::standardTexture& texStandard)
	{
		GLuint idTexture{};
		GLenum format{};
		GLenum sec_Format{};
	//	std::cout << "INSERTING::TEXTURES\n";
		if(texStandard.dataTexture)
		{ 
			//std::cout << "INSERTING::TEXTURES\n";
		if (texStandard.nrChannels == 1)
		{
			format = GL_RED;

		}

		if (texStandard.nrChannels == 3)
		{
			//format = GL_RGB;
			format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			sec_Format = GL_RGB;
		}

		if (texStandard.nrChannels == 4)
		{
			//format = GL_RGBA;
			//format = GL_RGBA8;
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			sec_Format = GL_RGBA;
		}

		glGenTextures(1, &idTexture);

		//	glBindTexture(GL_TEXTURE_2D, id);
		//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dataTexture);
		//	glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, idTexture);
		//size_t sizeImage{ sizeof(dataTexture) };
		//glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, sizeImage, dataTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, texStandard.width, texStandard.height, 0, sec_Format, GL_UNSIGNED_BYTE, texStandard.dataTexture);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		std::cout << "LOADING::STANDARD\n";

		stbi_image_free(texStandard.dataTexture);
	}

		else
		{
			SDL_Log("ERROR::NOT LOAD::TEXTURE");
			stbi_image_free(texStandard.dataTexture);
		}

		return idTexture;
	}

	void loadAll_PreLoadedTexturesToCache()
	{
		for (auto preLoaded : preLoadedTextures)
		{
			GLuint textureID{};

			if (std::holds_alternative<ktxTexture*>(preLoaded.textureData_F))
			{
				textureID = loadTextureKTX(std::get<ktxTexture*>(preLoaded.textureData_F));
			}

			else if (std::holds_alternative<texDataManager::standardTexture>(preLoaded.textureData_F))
			{
				textureID = loadTextureStandard(std::get<texDataManager::standardTexture>(preLoaded.textureData_F));
			}

			textures.emplace(
				preLoaded.nameTexture,
				texDataManager::textureD_info(
					textureID,   //textureID
					preLoaded.nameTexture, //nameTexture
					preLoaded.typeTex, //type
					preLoaded.completePath, // completePath
					preLoaded.format //format

				)
			);


		}
	}

	void texture_Data::use_MaterialTextures(shading::shader& shader, int textureMax)
	{
		std::map<texDataManager::typeTexture, int> texturesNr
		{
			{texDataManager::typeTexture::diffuse, 1},
			{texDataManager::typeTexture::specular, 1 }
		};

		std::string numberTex{};
		std::string prefixTexture{};
		int totalSizeTex{};

	
		auto indices{ std::views::iota(0) };
		for (auto [idx, texMat] : std::views::zip(indices, textures_LoadCache))
		{
			if (texturesNr[texMat.typeTex] <= textureMax)
			{
				numberTex = std::to_string(texturesNr[texMat.typeTex]);
				prefixTexture = "Mat_" + numberTex + ".";
				std::string nameTexBool{ prefixTexture + texDataManager::typeTex_T[texMat.typeTex].textureBool };
				shader.setBool(nameTexBool, true);

				////ACABAR DE INSERTAR LAS TEXTURAS 

				std::string nameTexT{ prefixTexture + texDataManager::typeTex_T[texMat.typeTex].textureInsert };
				shader.setInt(nameTexT, idx);
				glActiveTexture(GL_TEXTURE0 + idx);
				glBindTexture(GL_TEXTURE_2D, textures[texMat.nameTexture].textureID);
	
				texturesNr[texMat.typeTex]++; //SUMA DE PATRON;
				totalSizeTex = idx;
			}
		}
		
		for (auto texNr : texturesNr)
		{
			int texPlus{};
			bool texTestPlus{};
			for (auto texPostNr : texturesNr)
			{
				if (texNr.second < texPostNr.second && texNr.second < textureMax)
				{
					texPlus = texPostNr.second - texNr.second;
					texTestPlus = true;
				}
			}

			if (texTestPlus == true)
			{
				for (int t = 0; t < texPlus; t++)
				{
					texNr.second++;
					numberTex = std::to_string(texNr.second);
					prefixTexture = "Mat_" + numberTex + "."; //prefix del numero del material que se le va asignar esa textura

					std::string nameTexBool{ prefixTexture + texDataManager::typeTex_T[texNr.first].textureBool };
					shader.setBool(nameTexBool, false);

					std::string nameTexT{ prefixTexture + texDataManager::typeTex_T[texNr.first].textureInsert };
					shader.setInt(nameTexT, 0);
				    glActiveTexture(GL_TEXTURE0 + (++totalSizeTex));
					glBindTexture(GL_TEXTURE_2D, 0);

					//texNr.second++;
				}

			}

		}

	}
	void texture_Data::insertNewTexture(const char* pathTexture, texDataManager::typeTexture tex)
	{
		std::string nameTex{ pathTexture };
		std::reverse(nameTex.begin(), nameTex.end());
		
		std::string pathFormat = nameTex.substr(0, nameTex.find_last_of("."));
		nameTex = nameTex.substr(0, nameTex.find_last_of("/"));

		std::reverse(pathFormat.begin(), pathFormat.end());
		std::reverse(nameTex.begin(), nameTex.end());
		
		nameTex = nameTex.substr(0, nameTex.find_last_of("."));

		bool existTexture{};
		texDataManager::textureD_info textureToLoad{};

		if (pathFormat == "ktx2")
		{
			ktxTexture* texKTX{ loadTexture_PreCompress_KTX(pathTexture, existTexture) };

			if (existTexture == true)
			{
				GLuint texID{ loadTextureKTX(texKTX) };

				textureToLoad.textureID = texID;
				textureToLoad.nameTexture = nameTex;
				textureToLoad.type = tex;
				textureToLoad.completePath = static_cast<std::string>(pathTexture);
				textureToLoad.format = texDataManager::formatImage::KTX;
			}
		}

		else if (pathFormat == "png" || pathFormat == "jpg")
		{
			texDataManager::standardTexture texStandard{ loadTexture_CompressInCompile(pathTexture, existTexture) };

			if (existTexture == true)
			{
		
				GLuint texID{ loadTextureStandard(texStandard) };

				textureToLoad.textureID = texID;
				textureToLoad.nameTexture = nameTex;
				textureToLoad.type = tex;
				textureToLoad.completePath = static_cast<std::string>(pathTexture);
				textureToLoad.format = texDataManager::formatImage::standard;

				textureToLoad.width = texStandard.width;
				textureToLoad.height = texStandard.height;
				textureToLoad.nrChannels = texStandard.nrChannels;
			}
		}
		
		if (existTexture == true)
		{
			textures.emplace(
				nameTex,
				textureToLoad
			);

			textures_LoadCache.emplace_back(
				nameTex,
				textureToLoad.type,
				textureToLoad.completePath,
				textureToLoad.format
			);
		}

		else if (existTexture == false)
		{
			SDL_Log("ERROR::INSERT TEXTURE::IN::MESH");
		}

	}

}


