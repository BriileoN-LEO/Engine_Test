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



	 standardTexture::standardTexture() {};
	 standardTexture::standardTexture(unsigned char* dataTexture, int width, int height, int nrChannels) : 
		 width(width), 
		 height(height), 
		 nrChannels(nrChannels) {
	 
		 if (dataTexture)
		 {
			 this->dataTexture = dataTexture;
			// stbi_image_free(dataTexture);
			 dataTexture = nullptr;
		 }

	 };

	 standardTexture::standardTexture(standardTexture&& oT) noexcept :
		 width(oT.width),
		 height(oT.height),
		 nrChannels(oT.nrChannels)
	 {
		// if (dataTexture)
		// {
			 this->dataTexture = oT.dataTexture;
			 //stbi_image_free(oT.dataTexture);
			 oT.dataTexture = nullptr;
		//
	 };

	 standardTexture::standardTexture(standardTexture& oT) :
		 width(oT.width),
		 height(oT.height),
		 nrChannels(oT.nrChannels)
	 {
		 if (dataTexture)
		 {
			 this->dataTexture = oT.dataTexture;
			// stbi_image_free(oT.dataTexture);
			 oT.dataTexture = nullptr;
		 }
	 };

	 standardTexture::~standardTexture()
	 {
		 if (dataTexture != nullptr)
		 {
			// stbi_image_free(dataTexture);
			 dataTexture = nullptr;
		 }
		 this->width = 0;
		 this->height = 0;
		 this->nrChannels = 0;
	 };

	 	 standardTexture& standardTexture::operator=(standardTexture&& oT) noexcept
		{
			 if (this != &oT)
			 {
				 this->dataTexture = oT.dataTexture;
				 // stbi_image_frese(oT.dataTexture);
				 oT.dataTexture = nullptr;

				 this->width = oT.width;
				 this->height = oT.height;
				 this->nrChannels = oT.nrChannels;
			 }
			return *this;
		};

		standardTexture& standardTexture::operator=(standardTexture& oT)
		{
			this->dataTexture = oT.dataTexture;
			//stbi_image_free(oT.dataTexture);
			oT.dataTexture = nullptr;

			this->width = oT.width;
			this->height = oT.height;
			this->nrChannels = oT.nrChannels;

			return *this;
		};

		void standardTexture::data_debug()
		{
			if (dataTexture)
			{
				std::cout << "CORRECT_MOVE\n";
			}
		}

		preloaded_TextureD_info::preloaded_TextureD_info() {};
		preloaded_TextureD_info::preloaded_TextureD_info(std::string nameTexture, typeTexture typeTex, std::string completePath, texTypeFile textureData_F, formatImage format) :
			nameTexture(nameTexture),
			typeTex(typeTex),
			completePath(completePath),
			textureData_F(std::move(textureData_F)),
			format(format),
			tex_coord(tex_coord) {};

		preloaded_TextureD_info::preloaded_TextureD_info (preloaded_TextureD_info&& preTex) noexcept :
			nameTexture(preTex.nameTexture),
			typeTex(preTex.typeTex),
			completePath(preTex.completePath),
			format(preTex.format),
			tex_coord(preTex.tex_coord) {
		
			if (std::holds_alternative<texDataManager::standardTexture>(preTex.textureData_F))
			{
				texDataManager::standardTexture& texGet{ std::get<0>(preTex.textureData_F) };
				this->textureData_F = std::move(texGet);
			}

			else if (std::holds_alternative<ktxTexture*>(preTex.textureData_F))
			{
				this->textureData_F = std::move(std::get<1>(preTex.textureData_F));
			}


		};

		preloaded_TextureD_info::preloaded_TextureD_info(preloaded_TextureD_info& preTex) :
			nameTexture(preTex.nameTexture),
			typeTex(preTex.typeTex),
			completePath(preTex.completePath),
			format(preTex.format),
			tex_coord(preTex.tex_coord){

			if (std::holds_alternative<texDataManager::standardTexture>(preTex.textureData_F))
			{
				this->textureData_F = std::move(std::get<0>(preTex.textureData_F));
			}

			else if (std::holds_alternative<ktxTexture*>(preTex.textureData_F))
			{
				this->textureData_F = std::move(std::get<1>(preTex.textureData_F));
			}

		};

		preloaded_TextureD_info& preloaded_TextureD_info::operator=(preloaded_TextureD_info&& preTex) noexcept
		{
			nameTexture = preTex.nameTexture;
			typeTex = preTex.typeTex;
			completePath = preTex.completePath;
			textureData_F = std::move(preTex.textureData_F);
			format = preTex.format;
			tex_coord = preTex.tex_coord;

			return *this;
		}

		preloaded_TextureD_info& preloaded_TextureD_info::operator=(preloaded_TextureD_info& preTex)
		{
			nameTexture = preTex.nameTexture;
			typeTex = preTex.typeTex;
			completePath = preTex.completePath;
			textureData_F = std::move(preTex.textureData_F);
			format = preTex.format;
			tex_coord = preTex.tex_coord;
			return *this;
		}


		preloaded_TextureD_UI::preloaded_TextureD_UI() {};
		preloaded_TextureD_UI::preloaded_TextureD_UI(std::string nameMenuUI, std::string nameSectionUI, std::vector<preloaded_TextureD_info> texturesID) :
			nameMenuUI(nameMenuUI), nameSectionUI(nameSectionUI), texturesID(std::move(texturesID))
		{};

		preloaded_TextureD_UI::preloaded_TextureD_UI(preloaded_TextureD_UI&& preTex) noexcept :
			nameMenuUI(preTex.nameMenuUI), nameSectionUI(preTex.nameSectionUI), texturesID(std::move(preTex.texturesID))
		{};
		preloaded_TextureD_UI::preloaded_TextureD_UI(preloaded_TextureD_UI& preTex) :
			nameMenuUI(preTex.nameMenuUI), nameSectionUI(preTex.nameSectionUI), texturesID(std::move(preTex.texturesID))
		{
		};
		preloaded_TextureD_UI& preloaded_TextureD_UI::operator=(preloaded_TextureD_UI&& preTex) noexcept
		{
			this->nameMenuUI = preTex.nameMenuUI;
			this->nameSectionUI = preTex.nameSectionUI;
			this->texturesID = std::move(preTex.texturesID);
			return *this;
		};
		preloaded_TextureD_UI& preloaded_TextureD_UI::operator=(preloaded_TextureD_UI& preTex)
		{
			this->nameMenuUI = preTex.nameMenuUI;
			this->nameSectionUI = preTex.nameSectionUI;
			this->texturesID = std::move(preTex.texturesID);
			return *this;
		}
		preloaded_TextureD_UI::~preloaded_TextureD_UI() = default;


}

namespace textureCache
{
	std::vector<texDataManager::preloaded_TextureD_info> preLoadedTextures{};
	std::map<std::string, texDataManager::textureD_info> textures{};
	std::string backTextureName{};

	std::vector<texDataManager::preloaded_TextureD_UI> preLoadedTexturesUI{};
	std::map<std::string, texDataManager::textureD_info_UI> texturesUI{};

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
	texDataManager::standardTexture loadTexture_CompressInCompile(const char* path, bool& exist, int numChannels)
	{
		int width{};
		int height{};
		int nrChannels{};
		unsigned char* DataT = stbi_load(path, &width, &height, &nrChannels, numChannels);

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

		return texDataManager::standardTexture(DataT, width, height, nrChannels);
	}
	texDataManager::TextureData_File manageLoadTexture(std::string path, std::string directory, std::string typeTextures) ///Insertar texturas en el cache de textura, pasar la informa
	{
		texDataManager::TextureData_File texturesData{};

		bool existenceTex{};

		std::string path_to_KTX2{ path };
		//std::reverse(path_to_DDS.begin(), path_to_DDS.end());
		path_to_KTX2 = path_to_KTX2.substr(0, path_to_KTX2.find_last_of('.'));
		path_to_KTX2 += ".ktx2";
		path_to_KTX2 = directory + path_to_KTX2;
	//	path_to_KTX2 = directory + '/' + path_to_KTX2;


		///SI LA TEXTURA ES PNG, TIENE BLENDMODE 
		std::string formatImage{ path };
		std::reverse(formatImage.begin(), formatImage.end());
		formatImage = formatImage.substr(0, formatImage.find_last_of('.'));
		std::reverse(formatImage.begin(), formatImage.end());

		if (formatImage == "png" && texDataManager::typeTex_String[typeTextures] == texDataManager::typeTexture::diffuse)
		{
			texturesData.blendTexture = true;
		}

		else
		{
			texturesData.blendTexture = false;
		}

		std::string path_Normal{ directory + '/' + path };
		
		//std::cout << path_to_KTX2 << "\n";

		for (auto& tex : textureCache::preLoadedTextures)
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
					texDataManager::standardTexture textureLoadStandard{ loadTexture_CompressInCompile(path_Normal.c_str(), correctLoading, 0) };
					texDataManager::preloaded_TextureD_info dataFile{};

					if (correctLoading == true)
					{
						dataFile.nameTexture = nameTexture;
						dataFile.typeTex = texDataManager::typeTex_String[typeTextures];
						dataFile.completePath = path_Normal;

						dataFile.textureData_F = std::move(textureLoadStandard); ///REVISAR SI ESTA LINEA NO DA ERROR
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
						dataFile.textureData_F = textureLoadKTX;
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
					break;
				}
			}

			if (existTex_inFile)
			{
				texturesData.nameTexture = preLoadTex.nameTexture;
				texturesData.typeTex = preLoadTex.typeTex;
				texturesData.completePath = preLoadTex.completePath;
				texturesData.format = preLoadTex.format;
				textureCache::preLoadedTextures.emplace_back(std::move(preLoadTex));
			}

			////COMPLETAR AQUI

			/// VER SI SE PUEDE GUARDAR EL FORMATO DDS EN UN UNSIGNED CHAR O CONTINUAR

		}

		return texturesData;
 	}

	//PRELOAD TEXTURES FOR UI
	texDataManager::TextureData_File managePreLoadedTexturesUI(std::string directory, std::string nameUI, std::string nameSection) ///put more info 
	{
		texDataManager::TextureData_File textureData{};
	    
		bool existenceTexture{};


		auto find_existenceTex{ std::ranges::find_if(preLoadedTexturesUI,
			[&](const auto& v) { 
				return v.nameMenuUI == nameUI && v.nameSectionUI == nameSection; } //CHANGE IF THE POINTER FAILS
			) };

		if(find_existenceTex != preLoadedTexturesUI.end())
		{
			int find_layer{};
			auto find_insideD{ std::ranges::find_if(find_existenceTex->texturesID,
				[&](const auto& eT) {return directory == eT.completePath; }
			) };
	//	for (auto preLoadedTex : preLoadedTexturesUI)
	//		if (preLoadedTex.nameMenuUI == nameUI && preLoadedTex.nameSectionUI == nameSection)
		//	{
			//	for (auto inside_D : preLoadedTex.texturesID)
			//	{
			//		if (directory == inside_D.completePath)
			//		{
			if (find_insideD != find_existenceTex->texturesID.end())
			{
				textureData.nameTexture = find_insideD->nameTexture;
				textureData.typeTex = find_insideD->typeTex;
				textureData.completePath = directory;
				textureData.format = find_insideD->format;
				textureData.index_layer_tex = find_layer; //find the index layer of the tex if its the same texture but in other vertex data 

				existenceTexture = true;
			}
	
			find_layer++;
				//	}
			//	}
	//		}

		}


		if (existenceTexture == false)
		{
			std::string pathTUI{ directory };
			std::reverse(pathTUI.begin(), pathTUI.end());
			//	std::string pathTUI{ path };
			//	std::reverse(pathTUI.begin(), pathTUI.end());

			pathTUI = pathTUI.substr(0, pathTUI.find_last_of('.'));
			std::reverse(pathTUI.begin(), pathTUI.end());

			std::string pathName{ directory };
			std::reverse(pathName.begin(), pathName.end());
			pathName = pathName.substr(0, pathTUI.find_last_of('/'));
			std::reverse(pathName.begin(), pathName.end());
			pathName = pathName.substr(0, pathTUI.find_last_of("."));
		
			texDataManager::texTypeFile texLoadVariant{};

			bool existTexture{};

			if (pathTUI == "ktx2")
			{
				textureData.format = texDataManager::formatImage::KTX;
				texLoadVariant = std::move(loadTexture_PreCompress_KTX(directory.c_str(), existTexture));
		
			}
			
			else if (pathTUI == "png" || pathTUI == "jpg")
			{
				textureData.format = texDataManager::formatImage::standard;
				texLoadVariant = std::move(loadTexture_CompressInCompile(directory.c_str(), existTexture, 4));
		
			}

			if (existTexture == true)
			{

				texDataManager::preloaded_TextureD_UI preLoadTex{};

				int nextLayer{};

				auto find_sec{ std::ranges::find_if(preLoadedTexturesUI,
					[&](const auto& f) { return f.nameMenuUI == nameUI && f.nameSectionUI == nameSection; }
					) };

					if (find_sec != preLoadedTexturesUI.end())
					{
						nextLayer = static_cast<int>(find_sec->texturesID.size());  ///ADD THE SIZE OF THE CURRENT TEXTURES ID OF preLoadedTexturesID
						find_sec->texturesID.emplace_back(
						     texDataManager::preloaded_TextureD_info(
						     pathName,
						     texDataManager::typeTexture::diffuse,
						     directory,
						     std::move(texLoadVariant),
						     textureData.format
							)
						);
					}

					else
					{
						std::vector<texDataManager::preloaded_TextureD_info> iniVec;
						iniVec.emplace_back(texDataManager::preloaded_TextureD_info(
							pathName,
							texDataManager::typeTexture::diffuse,
							directory,
							std::move(texLoadVariant),
							textureData.format
						));

						preLoadedTexturesUI.emplace_back(
							texDataManager::preloaded_TextureD_UI(
								nameUI,
								nameSection,
								std::move(iniVec)

					));


					}
				//INSERT THE DATA INSIDE for USE
				textureData.nameTexture = pathName;
				textureData.typeTex = texDataManager::typeTexture::diffuse;
				textureData.completePath = directory;
				textureData.index_layer_tex = nextLayer;
				//INFO OF TEXTURE CREATION
			}
			
			else if (existTexture == false)
			{
				SDL_Log(std::string("ERROR::PATH--->" + directory + "DOESN´T FIND A TEXTURE UI").c_str());

			}

		}
		//else if

		return textureData;
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

	GLuint loadTexturesUI(std::vector<texDataManager::standardTexture> allTexturesToLoad, int max_w, int max_h)
	{
		GLuint texID_array{};
		int layers{ static_cast<int>(allTexturesToLoad.size()) };
		
		glGenTextures(1, &texID_array);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texID_array);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 5, GL_RGBA8, max_w, max_h, layers);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		for (int sT = 0; sT < layers; sT++)
		{
	
			//std::unique_ptr<texDataManager::standardTexture> ptr_tex{ std::move(allTexturesToLoad[sT]) };
			
			unsigned char* dataTex{ std::move(allTexturesToLoad[sT].dataTexture) };
			allTexturesToLoad[sT].dataTexture = nullptr;

			if (dataTex)
			{
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, sT, max_w, max_h, 1, GL_RGBA, GL_UNSIGNED_BYTE, dataTex);

				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

				//std::cout << "CORRECT::LOAD::TEXTURE_UI" << dataTex << '\n';

				stbi_image_free(dataTex);
				//allTexturesToLoad[sT].reset();
			}

			else
			{
				std::cout << "ERROR::TEXTURE_UI::ARRAY_2D::LAYER" << dataTex << '\n';
			}

		
		}


		return texID_array;
	}
	
	void loadAll_PreLoadedTexturesToCache()
	{
		auto saveTexuresPreloaded = [&](
			texDataManager::preLoadedVar texToPreloaded)
			{
				if (std::holds_alternative<std::vector<texDataManager::preloaded_TextureD_info>>(texToPreloaded))
				{
					std::vector<texDataManager::preloaded_TextureD_info>& texInfo{ std::get<0>(texToPreloaded) };

					for (auto& preLoaded : texInfo)
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

					//	std::map<std::string, texDataManager::textureD_info>& texToSave{std::get<0>(dirTextureSave)};
						textureCache::textures.emplace(
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

				if (std::holds_alternative<std::vector<texDataManager::preloaded_TextureD_UI>>(texToPreloaded))
				{
					std::vector<texDataManager::preloaded_TextureD_UI> texUI{ std::move(std::get<1>(texToPreloaded)) };
				//	std::map<std::string, texDataManager::textureD_info_UI>& texToSave{ std::get<1>(dirTextureSave) };

					for (auto& preLoaded : texUI)
					{
					//std::vector<texDataManager::standardTexture> allTex{};   ///ANTERIOR METODO
						std::vector<texDataManager::standardTexture> allTex;
						std::vector<texDataManager::textureD_info_contentUI> textures_data_UI{};

						std::pair<float, float> maxCoords_padd{};   

						int batch_layer{};
						for (auto& textures_data : preLoaded.texturesID)
						{
							//std::unique_ptr<texDataManager::standardTexture> ptr_tex_DataF{ std::make_unique<texDataManager::standardTexture>(std::get<texDataManager::standardTexture>(textures_data.textureData_F)) };
						
							//allTex.emplace_back(std::get<texDataManager::standardTexture>(textures_data.textureData_F));;   ///ANTERIOR METODO
							//allTex.emplace_back(std::make_unique<texDataManager::standardTexture>(std::get<texDataManager::standardTexture>(textures_data.textureData_F)));

							int& size_x_width{ std::get<texDataManager::standardTexture>(textures_data.textureData_F).width };
							int& size_y_height{ std::get<texDataManager::standardTexture>(textures_data.textureData_F).height };

							textures_data_UI.emplace_back(
								texDataManager::textureD_info_contentUI(
									textures_data.nameTexture,
									textures_data.typeTex,
									textures_data.completePath,
									textures_data.format,
									std::pair<float, float>(static_cast<float>(size_x_width), static_cast<float>(size_y_height)),
									batch_layer
								)
							);

							if (size_x_width > maxCoords_padd.first)
							{
								maxCoords_padd.first = size_x_width;
							}

							if (size_y_height > maxCoords_padd.second)
							{
								maxCoords_padd.second = size_y_height;
							}

							texDataManager::standardTexture texture_Data{ std::move(std::get<texDataManager::standardTexture>(textures_data.textureData_F)) };

							//texture_Data.data_debug();

							allTex.emplace_back(std::move(texture_Data));
						
							batch_layer++;
						}

						for (auto& vTD_UI : textures_data_UI)
						{
							vTD_UI.coords_tex.first = static_cast<float>(vTD_UI.coords_tex.first / maxCoords_padd.first); /////
							vTD_UI.coords_tex.second = static_cast<float>(vTD_UI.coords_tex.second / maxCoords_padd.second); ////
						}

						GLuint textureID{ loadTexturesUI(std::move(allTex), maxCoords_padd.first, maxCoords_padd.second)};
				//		GLuint textureID{};

						textureCache::texturesUI.emplace(
							preLoaded.nameSectionUI,
							texDataManager::textureD_info_UI(
								preLoaded.nameMenuUI,
								preLoaded.nameSectionUI,
								textureID,
								textures_data_UI
							)
						);

					}

				
				}

					/*
				for (auto preLoaded : texToPreloaded)
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

					dirTextureSave.emplace(
						preLoaded.nameTexture,
					texDataManager::textureD_info(
							textureID,   //textureID
							preLoaded.nameTexture, //nameTexture
							preLoaded.typeTex, //type
							preLoaded.completePath, // completePath
							preLoaded.format //format

						)
					);

					*/
			

				
			};


		saveTexuresPreloaded(texDataManager::preLoadedVar(std::move(preLoadedTextures)));
		preLoadedTextures.erase(preLoadedTextures.begin(), preLoadedTextures.end());

		saveTexuresPreloaded(texDataManager::preLoadedVar(std::move(preLoadedTexturesUI)));
		preLoadedTexturesUI.erase(preLoadedTexturesUI.begin(), preLoadedTexturesUI.end());

	}

	void uploadEngineTextures_Preloaded(const char* path) 
	{
		

	}

	void texture_Data::use_MaterialTextures(shading::shader& shader, int textureMax)
	{
		
		shader.setBool("blendTexture", active_BlendMode);

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
			texDataManager::standardTexture texStandard{ loadTexture_CompressInCompile(pathTexture, existTexture, 0) };

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


