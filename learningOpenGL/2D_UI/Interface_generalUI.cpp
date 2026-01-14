#include "Interface_generalUI.h"

namespace brii_UI
{


	spriteUI::spriteUI() {};
	spriteUI::spriteUI(typeSection_UI name, std::string nameTexture, glm::vec2 posicion, std::pair<coord_x, coord_y> size, int layerTexture) :
		posicion(posicion),
		name(name),
		nameTexture(nameTexture),
		layerTexture(layerTexture)
	{
		glm::vec2 vert_1{ -size.first, -size.second };  /// left down
		glm::vec2 vert_2{ size.first, -size.second };  /// right down
		glm::vec2 vert_3{ size.first, size.second };  ///top left
		glm::vec2 vert_4{ -size.first, size.second };  /// left top
		
		float layer{ static_cast<float>(layerTexture) };

		vertexData.emplace_back(spriteData(vert_1, glm::vec2(0.0, 1.0), layer));
		vertexData.emplace_back(spriteData(vert_2, glm::vec2(1.0, 1.0), layer));
		vertexData.emplace_back(spriteData(vert_3, glm::vec2(1.0, 0.0), layer));
		vertexData.emplace_back(spriteData(vert_4, glm::vec2(0.0, 0.0), layer));

	};


	const std::vector<unsigned int> indicesSprite
	{
		0, 1, 2,
		2, 3, 0
	};

	spriteUI_Dynamic::spriteUI_Dynamic() {};
	spriteUI_Dynamic::spriteUI_Dynamic(sectionsUI type_sectionUI, int size_Sprites, texDataManager::TextureData_File_UI texture) :
		type(type_sectionUI), 
		texture(texture)
	{
	  //creation of the vertex
		const size_t maxVertex{ static_cast<size_t>(size_Sprites * 4) };
		const size_t maxIndices{ static_cast<size_t>(size_Sprites * 6) };
		const size_t maxLayerTex{ static_cast<size_t>(size_Sprites) };

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, maxVertex * sizeof(spriteData), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(brii_UI::spriteData), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(brii_UI::spriteData), (void*)offsetof(brii_UI::spriteData, texCoords));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(brii_UI::spriteData), (void*)offsetof(brii_UI::spriteData, layerTexture));
		
		glBindVertexArray(0);

	}
	void spriteUI_Dynamic::renderBatch_UI()
	{
		std::vector<spriteData> vertex_Textures{};

		for (auto& texSprite : sprites)
		{
			glm::vec2 re_posicion{ static_cast<float>(screenSettings::screen_w * 0.5f), static_cast<float>(screenSettings::screen_h * 0.5f) };
			re_posicion = re_posicion - texSprite.second.posicion;
			re_posicion *= glm::vec2(-1.0f);

			vertex_Textures.emplace_back(texSprite.second.vertexData[0].posicion + re_posicion, texSprite.second.vertexData[0].texCoords, texSprite.second.vertexData[0].layerTexture);
			vertex_Textures.emplace_back(texSprite.second.vertexData[1].posicion + re_posicion, texSprite.second.vertexData[1].texCoords, texSprite.second.vertexData[1].layerTexture);
			vertex_Textures.emplace_back(texSprite.second.vertexData[2].posicion + re_posicion, texSprite.second.vertexData[2].texCoords, texSprite.second.vertexData[2].layerTexture);
			vertex_Textures.emplace_back(texSprite.second.vertexData[3].posicion + re_posicion, texSprite.second.vertexData[3].texCoords, texSprite.second.vertexData[3].layerTexture);

		}


	}


	std::map<sectionsUI, spriteUI_Dynamic> editMode_UI_D;

	void resizeUI_textures()
	{
		for (auto& eM_tex : editMode_UI_D)
		{
			auto findTex = std::ranges::find_if(textureCache::texturesUI, [&](const auto& T) {return T.second.nameSectionUI == eM_tex.second.texture.nameTexture_UI; });
		
			if (findTex != textureCache::texturesUI.end())
			{
				for (auto& prite_eM_tex : eM_tex.second.sprites)
				{
					auto find_size = std::ranges::find_if(findTex->second.textures, [&](const auto& fT) {return fT.nameTexture == prite_eM_tex.second.nameTexture; });

				}
			}
		}

	}

	namespace loadToCPU
	{
		std::queue<maxLevel_SpriteUI> UI_data{};
		std::atomic<int> atomic_CounterUI(0);
		std::atomic<int> atomic_sizeCounterUI(0);
		std::atomic<bool> flagsReady(false);
		std::atomic<bool> finishLoadUI(false);
		std::mutex mutex_UI;

		void resetLoadToCPU()
		{
			std::queue<maxLevel_SpriteUI> clearData{};
			std::swap(UI_data, clearData);

		}

		void loadUIbrii_thread(std::vector<maxLevel_SpriteUI> UI_dataInsert)
		{

			for (auto& maxSpriteUI : UI_dataInsert)
			{
				{
					std::lock_guard<std::mutex> lock(mutex_UI);

					for (auto& midSpriteUI : maxSpriteUI.midLevels_UI)
					{

						if (maxSpriteUI.menuType == brii_UI::menuSpriteUI::editMenu)
						{

							for (auto& basicSpriteUI : midSpriteUI.basicLevels_UI)
							{
								basicSpriteUI.texture = textureCache::managePreLoadedTexturesUI(basicSpriteUI.texture.completePath, maxSpriteUI.nameMaxTexture, midSpriteUI.nameMidTexture);

							}
						}
					}

					UI_data.emplace(maxSpriteUI);

				}
				atomic_CounterUI++;

				flagsReady.store(true);
			}
		}

	}

}