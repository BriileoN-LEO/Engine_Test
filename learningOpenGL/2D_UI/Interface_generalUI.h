#ifndef Interface_generalUI
#define Interface_generalUI

#include "learningOpenGL.h"
#include "textureData/textureManager.h"

namespace brii_UI
{
	extern const glm::mat4 projection_UI;

	using coord_x = float;
	using coord_y = float;


	enum class menuSpriteUI
	{
		editMenu = 0
	};

	enum class sectionsUI
	{
		//EDIT MODE
		controlShaders = 0

	};

	enum class typeSection_UI
	{
		arrow_compareShader = 0

	};

	struct insertSpriteUI
	{
		typeSection_UI name{};
		glm::vec2 posicion{};
		std::pair<coord_x, coord_y> posSC{};
		std::string textureName;
		int layerTexture{};

		texDataManager::TextureData_File texture{};
	};
	
	struct midLevel_SpriteUI
	{
		std::vector<insertSpriteUI> basicLevels_UI {};
		sectionsUI seccion{};	
		std::string nameMidTexture{};
	
	};

	struct maxLevel_SpriteUI
	{
		std::vector<midLevel_SpriteUI> midLevels_UI{};
		menuSpriteUI menuType{};
		std::string nameMaxTexture{};
	};

	struct spriteData
	{
		glm::vec2 posicion{};
		glm::vec2 texCoords{};
		float layerTexture{};
	};

	class spriteUI
	{
	public:

		typeSection_UI name{};
		glm::vec2 posicion{};
		std::vector<spriteData> vertexData{};		
		std::string nameTexture{};
		int layerTexture{};


		spriteUI();
		spriteUI(typeSection_UI name, std::string nameTexture,  glm::vec2 posicion, std::pair<coord_x, coord_y> size, int layerTexture);

	};

	class spriteUI_Dynamic
	{
	private:

		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};

	public:

		sectionsUI type{};
		
		texDataManager::TextureData_File_UI texture{};
		std::map<typeSection_UI, spriteUI> sprites{};
		std::string nameShader{};

		spriteUI_Dynamic();
		spriteUI_Dynamic(sectionsUI type_sectionUI, int size_Sprites, texDataManager::TextureData_File_UI texture, std::string nameShader);

		void renderBatch_UI();

		void destroy();

	};

	extern std::map<sectionsUI, spriteUI_Dynamic> editMode_UI_D;
	
	void resizeUI_textures();

	namespace loadToCPU
	{
		extern std::queue<maxLevel_SpriteUI> UI_data;
		extern std::atomic<int> atomic_CounterUI;
		extern std::atomic<int> atomic_sizeCounterUI;
		extern std::atomic<bool> flagsReady;
		extern std::atomic<bool> finishLoadUI;
		extern std::mutex mutex_UI;

		void resetLoadToCPU();
		void loadUIbrii_thread(std::vector<maxLevel_SpriteUI> UI_dataInsert);

	}

}


#endif Interface_generalUI