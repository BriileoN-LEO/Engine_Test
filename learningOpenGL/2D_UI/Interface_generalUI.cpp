#include "Interface_generalUI.h"
#include "Render/RenderData.h"
#include "data_save.h"

namespace brii_UI
{

	const glm::mat4 projection_UI{ glm::ortho(0.0f, static_cast<float>(screenSettings::screen_w),  static_cast<float>(screenSettings::screen_h), 0.0f,-1.0f, 1.0f) };

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
		
		vert_1 = posicion + vert_1;
		vert_2 = posicion + vert_2;
		vert_3 = posicion + vert_3;
		vert_4 = posicion + vert_4;


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
	spriteUI_Dynamic::spriteUI_Dynamic(sectionsUI type_sectionUI, int size_Sprites, texDataManager::TextureData_File_UI texture, std::string nameShader) :
		type(type_sectionUI), 
		texture(texture),
		nameShader(nameShader)
	{
	  //creation of the vertex
		const size_t maxVertex{ static_cast<size_t>(size_Sprites * 4) };
		const size_t maxIndices{ static_cast<size_t>(size_Sprites * 6) };
		const size_t maxLayerTex{ static_cast<size_t>(size_Sprites) };

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, maxVertex * sizeof(spriteData), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(spriteData), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(spriteData), (void*)offsetof(spriteData, texCoords));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(spriteData), (void*)offsetof(spriteData, layerTexture));
		
		glBindVertexArray(0);

	}
	void spriteUI_Dynamic::renderBatch_UI()
	{
		std::vector<spriteData> vertex_Textures{};
		std::vector<unsigned int> EBO_sprites{};

		for (auto& texSprite : sprites)
		{
			glm::vec2 re_posicion{ static_cast<float>(screenSettings::screen_w * 0.5f), static_cast<float>(screenSettings::screen_h * 0.5f) };
			re_posicion = re_posicion - texSprite.second.posicion;
			re_posicion *= glm::vec2(-1.0f);

			re_posicion = glm::vec2(0.0f);

			vertex_Textures.emplace_back(texSprite.second.vertexData[0].posicion + re_posicion, texSprite.second.vertexData[0].texCoords, texSprite.second.vertexData[0].layerTexture);
			vertex_Textures.emplace_back(texSprite.second.vertexData[1].posicion + re_posicion, texSprite.second.vertexData[1].texCoords, texSprite.second.vertexData[1].layerTexture);
			vertex_Textures.emplace_back(texSprite.second.vertexData[2].posicion + re_posicion, texSprite.second.vertexData[2].texCoords, texSprite.second.vertexData[2].layerTexture);
			vertex_Textures.emplace_back(texSprite.second.vertexData[3].posicion + re_posicion, texSprite.second.vertexData[3].texCoords, texSprite.second.vertexData[3].layerTexture);

			for (auto EBOs : indicesSprite)
			{
				EBO_sprites.emplace_back(EBOs);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		register_Errors::debug_renderError(); 

		glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_Textures.size() * sizeof(spriteData), &vertex_Textures[0]);
		register_Errors::debug_renderError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, EBO_sprites.size() * sizeof(unsigned int), &EBO_sprites[0]);

		shading::shader& shaderUI{ RenderData_Set::shader_D[nameShader] };

		shaderUI.use();
		shaderUI.transformMat("projection_UI", projection_UI);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<int>(vertex_Textures.size() / 4) * 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void spriteUI_Dynamic::destroy()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
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

					if (find_size != findTex->second.textures.end())
					{
						for (int s = 0; s < static_cast<int>(prite_eM_tex.second.vertexData.size()); s++)
						{
							glm::vec2& coord_tex{ prite_eM_tex.second.vertexData[s].texCoords };

							coord_tex.x = find_size->coords_tex.first * coord_tex.x;
							coord_tex.y = find_size->coords_tex.second * coord_tex.y;

						}
					}


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