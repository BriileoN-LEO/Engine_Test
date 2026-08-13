#ifndef textureManager_H
#define textureManager_H

#include "learningOpenGL.h"
#include "SHADER_H.h"
#include "Model_Assimp/Assimp_lib.h"
//#include "2D_UI/Interface_generalUI.h"

namespace KTX_lib
{
  void viewInternalFormat_(ktxTexture* texKTX, std::string& nameTexture, const char* file, int line);
#define viewInternalFormat(texKTX, nameTexture) viewInternalFormat_(texKTX, nameTexture, __FILE__, __LINE__)
}

namespace texDataManager
{
	enum class typeTexture : uint8_t
	{
		diffuse = 0,
		specular = 1,
        ALBEDO = 2,
		ROUGHNESS = 3,
		METALNESS = 4,
        NORMAL = 5,
		HEIGHT = 6,
		AMBIENT_OCCLUSION = 7,
		CAVITY = 8,
		OPACITY = 9,
		EMISSIVE = 10,
		back_Texture = 11
	};

	struct texToShader
	{
		std::string textureInsert{};
		std::string textureBool{};

	};

	extern  std::map<std::string, typeTexture> typeTex_String;
	extern  std::map<typeTexture, texToShader> typeTex_T;
	extern  std::map<typeTexture, texToShader> typeTex_ClusteredShading_prePass;

	enum class formatImage
	{
		standard = 0,
		KTX = 1
	};

	enum class color_space  //TO ASSIGN THE TYPE OF COLOR SPACE
	{
	  RGB = 0,
	  sRGB = 1
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

		void data_debug();

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

	using texTypeFile = std::variant<standardTexture, ktxTexture*, unsigned char*>;

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

	texDataManager::TextureData_File* loadEmbeddedTextures_PreCompress_(const aiTexture* textureAssimp, std::string& directory, std::string& typeTextures, uint8_t& num_tex, const char* file, int line);  ////TO LOAD THE NEW BINARY TEXTURES FROM glFT
#define loadEmbeddedTextures_PreCompress(textureAssimp, directory, typeTextures, num_tex) loadEmbeddedTextures_PreCompress_(textureAssimp, directory, typeTextures, num_tex, __FILE__, __LINE__)

	texDataManager::TextureData_File manageLoadTexture(std::string path, std::string directory, std::string typeTextures); //WORK IN SEPARATE THREAD, THREAD LOADING

	texDataManager::TextureData_File managePreLoadedTexturesUI(std::string directory, std::string nameUI, std::string nameSection);

	GLuint loadTextureKTX_(ktxTexture* texKTX, std::string& nameTexture, const char* file, int line);  ////HERE ANALIZE WHAT TYPE COLOR SPACE I HAVE FOR EACH TEXTURE
#define loadTextureKTX(texKTX, nameTexture) loadTextureKTX_(texKTX, nameTexture, __FILE__, __LINE__)

	GLuint loadTextureStandard_(std::string& nameTexture, texDataManager::standardTexture& texStandard, texDataManager::color_space colorSpace, const char* file, int line);
#define loadTextureStandard(nameTexture, texStandard, colorSpace) loadTextureStandard_(nameTexture, texStandard, colorSpace, __FILE__, __LINE__)

	GLuint loadTexturesUI(std::vector<texDataManager::standardTexture> allTexturesToLoad, int max_w, int max_h);

	void loadAll_PreLoadedTexturesToCache();
	
	void uploadEngineTextures_Preloaded(const char* path);

	class texture_Data
	{
	public:
		std::vector<texDataManager::TextureData_File> textures_LoadCache{};
		uint32_t size_diffuse_texture{};
		uint32_t size_specular_texture{};

		float shiness{};
		bool active_BlendMode{};

		texture_Data();

		void insert_TexturesData(std::vector<texDataManager::TextureData_File>& tex_data);  ////THIS INSERT THE TEXTURE OF ALL THE LOADED ASSIMP
		void use_MaterialTextures(shading::shader& shader, int textureMax);
		void use_Textures_PrePassCS(shading::shader& shader);
		void insertNewTexture(const char* pathTexture, texDataManager::typeTexture tex);

	};


}

namespace textureManager
{
	struct entity_tex
	{
		GLuint textureID{};
		std::string nameTexture{};
		texDataManager::typeTexture type{};
		std::string completePath{};
		texDataManager::formatImage format{};
	};

	class manager_Textures
	{
	private:

		std::vector<uint32_t> tex_find_pos{};
		std::unordered_map<std::string, uint32_t> tex_find_str{};
		std::unordered_map<uint32_t, entity_tex> textures_D{};

	public:

		manager_Textures();

	};
}

namespace texLoad_Data
{
	extern std::unordered_map<GLenum, int> dataImage_map_pos;
	extern std::array<GLenum, 2> dataImage_BitsSize;

	void set_dataImage_info();
	void set_all();

	unsigned int load_Texture_zBuffer(unsigned int& res_width, unsigned int& res_height);
	unsigned int* load_Texture_nBuffer(unsigned int& res_width, unsigned int& res_height, GLenum typeData_Tex, GLenum color_Attachment);
}



#endif // textureManager
