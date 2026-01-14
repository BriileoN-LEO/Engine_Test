#ifndef textureManager
#define textureManager

#include "learningOpenGL.h"
#include "SHADER_H.h"
//#include "2D_UI/Interface_generalUI.h"

namespace texDataManager
{
	enum class typeTexture
	{
		diffuse = 0,
		specular = 1,
		back_Texture = 2
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

		standardTexture();
		standardTexture(unsigned char* dataTexture, int width, int height, int nrChannels);

		
		standardTexture(standardTexture&& oT) noexcept;

		standardTexture(standardTexture& oT);

		~standardTexture();

		
		standardTexture& operator=(standardTexture&& oT) noexcept;

		standardTexture& operator=(standardTexture& oT);
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

	struct textureD_info_contentUI
	{
		std::string nameTexture{};
		typeTexture type{};
		std::string completePath{};
		formatImage format{};

		std::pair<float, float> coords_tex{};  ///texture coord max of the padd of all textures
		int layer_pos{};
	};

	struct textureD_info_UI
	{
		std::string nameMenuUI{};  
		std::string nameSectionUI{};
		GLuint textureID{};
		std::vector<textureD_info_contentUI> textures{};
	};

	using texTypeFile = std::variant<standardTexture, ktxTexture*>;

	struct preloaded_TextureD_info
	{
		std::string nameTexture{};
		typeTexture typeTex{};
		std::string completePath{};
		texTypeFile textureData_F{};
		formatImage format{};

		std::pair<float, float> tex_coord{};

		preloaded_TextureD_info();
		preloaded_TextureD_info(std::string nameTexture, typeTexture typeTex, std::string completePath, texTypeFile textureData_F, formatImage format);
		preloaded_TextureD_info(preloaded_TextureD_info&& preTex) noexcept;
		preloaded_TextureD_info(preloaded_TextureD_info& preTex);
		
		preloaded_TextureD_info& operator=(preloaded_TextureD_info&& preTex) noexcept;
		preloaded_TextureD_info& operator=(preloaded_TextureD_info& preTex);
	};

	struct preloaded_TextureD_UI
	{
		std::string nameMenuUI{};  ///IT WILL BE REPEAT A LOT OF TIMES IF IT THE LOOP CANNOT FIND IT 
		std::string nameSectionUI{}; ///IT WILL BE REPEAT ONLY ONCE FOR ONLY SAVE THE TESTURES ID
		std::vector<preloaded_TextureD_info> texturesID{};

		preloaded_TextureD_UI();

		preloaded_TextureD_UI(std::string nameMenuUI, std::string nameSectionUI, std::vector<preloaded_TextureD_info> texturesID);

		preloaded_TextureD_UI(preloaded_TextureD_UI&& preTex) noexcept;
		preloaded_TextureD_UI(preloaded_TextureD_UI& preTex);

		~preloaded_TextureD_UI();

		preloaded_TextureD_UI& operator=(preloaded_TextureD_UI&& preTex) noexcept;
		preloaded_TextureD_UI& operator=(preloaded_TextureD_UI& preTex);

	};
		//std::vector<
		//	brii_UI::menuSpriteUI name_menuSprite{};
		//	brii_UI::sectionsUI name_sectionUI{}

	using preLoadedVar = std::variant<std::vector<preloaded_TextureD_info>, std::vector<preloaded_TextureD_UI>>;
	using loadedVar = std::variant<std::map<std::string, texDataManager::textureD_info>, std::map<std::string, texDataManager::textureD_info_UI>>&;

	struct TextureData_File
	{
		std::string nameTexture{};
		typeTexture typeTex{};
		std::string completePath{};
		formatImage format{};

	[[maybe_unused]] bool blendTexture{};
	[[maybe_unused]] int index_layer_tex{};
//	[[maybe_unused]] unsigned char* dataTexture{ nullptr };///OLD LOADED TEXTURE
//	[[maybe_unused]] int width{};///OLD LOADED TEXTURE
//	[[maybe_unused]] int height{};///OLD LOADED TEXTURE
//	[[maybe_unused]] int nrChannels{};///OLD LOADED TEXTURE
	};
	
	struct TextureData_File_UI
	{
		std::string nameTexture_UI{};
		std::map<std::string, TextureData_File> texture_saved{};

	};


}

namespace textureCache
{
	extern std::vector<texDataManager::preloaded_TextureD_info> preLoadedTextures; ///CARGA DE TEXTURAS ANTES DE PASAR A CARGARLAS A OPENGL EN EL MAIN THREAD
	extern std::map<std::string, texDataManager::textureD_info> textures;  ///AQUI SE GUARDARAN TODAS LAS TEXTURAS QUE SE HAYAN ALMACENADO, NO SE REPETIRAN TEXTURAS
	extern std::string backTextureName;

	extern std::vector<texDataManager::preloaded_TextureD_UI> preLoadedTexturesUI; ///PRELOADED TEXTURES PARA EL UI
	extern std::map<std::string, texDataManager::textureD_info_UI> texturesUI; ///TEXTURES PARA EL UI ---- Se solicita el acceso con el nameSection UI 

	ktxTexture* loadTexture_PreCompress_KTX(const char* path, bool& exist);//WORK IN SEPARATE THREAD, THREAD LOADING
	texDataManager::standardTexture loadTexture_CompressInCompile(const char* path, bool& exist, int numChannels); //WORK IN SEPARATE THREAD, THREAD LOADING
	texDataManager::TextureData_File manageLoadTexture(std::string path, std::string directory, std::string typeTextures); //WORK IN SEPARATE THREAD, THREAD LOADING

	texDataManager::TextureData_File managePreLoadedTexturesUI(std::string directory, std::string nameUI, std::string nameSection);

	GLuint loadTextureKTX(ktxTexture* texKTX);
	GLuint loadTextureStandard(texDataManager::standardTexture& texStandard);

	GLuint loadTexturesUI(std::vector<std::unique_ptr<texDataManager::standardTexture>> allTexturesToLoad, int max_w, int max_h);

	void loadAll_PreLoadedTexturesToCache();
	
	void uploadEngineTextures_Preloaded(const char* path);

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