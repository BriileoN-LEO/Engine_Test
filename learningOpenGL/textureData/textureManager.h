#ifndef textureManager
#define textureManager

#include "learningOpenGL.h"
#include "SHADER_H.h"

namespace texDataManager
{
	enum class typeTexture
	{
		diffuse = 0,
		specular = 1
	};
	
	struct texToShader
	{
		std::string textureInsert{};
		std::string textureBool{};

	};

	extern  std::map<std::string, typeTexture> typeTex_String;
	extern  std::map<typeTexture, texToShader> typeTex_T;

	enum class formatImage
	{
		standard = 0,
	    KTX = 1
	};

	struct standardTexture
	{
		unsigned char* dataTexture{ nullptr };
		int width{};
		int height{};
		int nrChannels{};

	};

	struct textureD_info
	{
		GLuint textureID{}; ///Cargar cuando todos los procesos de CARGA del hilo de carga se hayan completado
		std::string nameTexture{};
		typeTexture type{};
		std::string completePath{};
		formatImage format{};
		[[maybe_unused]] int width{};
		[[maybe_unused]] int height{};
		[[maybe_unused]] int nrChannels{};

	};


	using texTypeFile = std::variant<standardTexture, ktxTexture*>;

	struct preloaded_TextureD_info
	{
		std::string nameTexture{};
		typeTexture typeTex{};
		std::string completePath{};
		texTypeFile textureData_F{};
		formatImage format{};

	};

	struct TextureData_File
	{
		std::string nameTexture{};
		typeTexture typeTex{};
		std::string completePath{};
		formatImage format{};

	[[maybe_unused]] unsigned char* dataTexture{ nullptr };///OLD LOADED TEXTURE
	[[maybe_unused]] int width{};///OLD LOADED TEXTURE
	[[maybe_unused]] int height{};///OLD LOADED TEXTURE
	[[maybe_unused]] int nrChannels{};///OLD LOADED TEXTURE
	};


}

namespace textureCache
{
	extern std::vector<texDataManager::preloaded_TextureD_info> preLoadedTextures; ///CARGA DE TEXTURAS ANTES DE PASAR A CARGARLAS A OPENGL EN EL MAIN THREAD
	extern std::map<std::string, texDataManager::textureD_info> textures;  ///AQUI SE GUARDARAN TODAS LAS TEXTURAS QUE SE HAYAN ALMACENADO, NO SE REPETIRAN TEXTURAS
    

	ktxTexture* loadTexture_PreCompress_KTX(const char* path, bool& exist);//WORK IN SEPARATE THREAD, THREAD LOADING
	texDataManager::standardTexture loadTexture_CompressInCompile(const char* path, bool& exist); //WORK IN SEPARATE THREAD, THREAD LOADING
	texDataManager::TextureData_File manageLoadTexture(std::string path, std::string directory, std::string typeTextures); //WORK IN SEPARATE THREAD, THREAD LOADING


	GLuint loadTextureKTX(ktxTexture* texKTX);
	GLuint loadTextureStandard(texDataManager::standardTexture& texStandard);
	void loadAll_PreLoadedTexturesToCache();

	class texture_Data
	{
	public:
		std::vector<texDataManager::TextureData_File> textures_LoadCache{}; 
		float shiness{};
		bool active_BlendMode{};

		void use_MaterialTextures(shading::shader& shader, int textureMax);
		void insertNewTexture(const char* pathTexture, texDataManager::typeTexture tex);

	};


}




#endif textureManager