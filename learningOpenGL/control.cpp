#include "control.h"

namespace controlMove
{
	bool detectMove = false;
	
	std::map<SDL_Keycode, bool> detectSDLK_code::keys
	{
		{SDLK_UP, false},
		{ SDLK_DOWN, false },
		{ SDLK_LEFT, false },
		{ SDLK_RIGHT, false },
	};

	std::map<SDL_Keycode, bool> detectSDLK_code::keys_Color
	{
		{ SDLK_R, false},
		{ SDLK_G, false},
		{ SDLK_B, false}
	};
	
	std::map<SDL_Keycode, bool> detectSDLK_code::keys_Scale
	{
		{SDLK_M, false},
		{SDLK_P, false}

	};

	std::map<SDL_Keycode, bool> detectSDLK_code::keys_Translate
	{
		{SDLK_UP, false},
		{SDLK_DOWN, false},
		{SDLK_LEFT, false},
		{SDLK_RIGHT, false},

	};

	std::map<SDL_Keycode, bool> detectSDLK_code::keys_UI
	{
		{SDLK_TAB, false}
	};

	bool detectSDLK_code::detectKey(SDL_Event event)
	{
		bool detectKey{ true };

		if (keys.find(event.key.key) != keys.end())
		{
			detectKey = keys[event.key.key];
			keys[event.key.key] = true;
		}

		return detectKey;
	}
	bool detectSDLK_code::detectKeyColor(SDL_Event event)
	{
		bool detectKey{ true };

		if (keys_Color.find(event.key.key) != keys_Color.end())
		{
			detectKey = keys_Color[event.key.key];
			keys_Color[event.key.key] = true;
		}

		return detectKey;
	}
	bool detectSDLK_code::detectKeyScale(SDL_Event event)
	{
		bool findKey{ false };

		if (keys_Scale.find(event.key.key) != keys_Scale.end())
		{
			if (!keys_Scale[event.key.key])
			{
				keys_Scale[event.key.key] = true;
				findKey = keys_Scale[event.key.key];
			}
		}
		
		return findKey;
	}
	bool detectSDLK_code::detectKeyTranslate(SDL_Event* event)
	{
		bool detect{ false };
		if (keys_Translate.find(event->key.key) != keys_Translate.end())
		{
			if (keys_Translate[event->key.key] == false)
			{
				keys_Translate[event->key.key] = true;
				detect = keys_Translate[event->key.key];
			}

		}
	
		return detect;
	}

	bool detectSDLK_code::detectKeyUI(SDL_Event& event)
	{
		bool findKey{};

		if (keys_UI.find(event.key.key) != keys_UI.end())
		{
			findKey = true;
			
			keys_UI[event.key.key] = keys_UI[event.key.key] ? false : true;
		
		}

		return findKey;
	}

	void detectSDLK_code::resetKeys()
	{
		for (auto& val : keys)
		{
			keys[val.first] = false;

		}

		for (auto& val : keys_Color)
		{
			keys_Color[val.first] = false;


		}

		for (auto& val : keys_Scale)
		{
			keys_Scale[val.first] = false;

		}

		for (auto& val : keys_Translate)
		{
			keys_Translate[val.first] = false;

		}

	}

	void moveTriangle(float(&vertices)[9])
	{
		const bool* stateKey = SDL_GetKeyboardState(nullptr);

		if (stateKey[SDL_SCANCODE_UP] == true)
		{
			vertices[1] += 0.05f;
			vertices[4] += 0.05f;
			vertices[7] += 0.05f;
			detectMove = true;
		}

		if (stateKey[SDL_SCANCODE_DOWN] == true)
		{
			vertices[1] -= 0.05f;
			vertices[4] -= 0.05f;
			vertices[7] -= 0.05f;
			detectMove = true;
		}

		if (stateKey[SDL_SCANCODE_LEFT] == true)
		{
			vertices[0] -= 0.05f;
			vertices[3] -= 0.05f;
			vertices[6] -= 0.05f;
			detectMove = true;
		}

		if (stateKey[SDL_SCANCODE_RIGHT] == true)
		{
			vertices[0] += 0.05f;
			vertices[3] += 0.05f;
			vertices[6] += 0.05f;
			detectMove = true;
		}
		
	}
	void moveVec3(vec::vec3& vertex)
	{
		const bool* stateKey = SDL_GetKeyboardState(nullptr);

		if (stateKey[SDL_SCANCODE_UP] == true)
		{
			vertex.y += 0.05f;
			detectMove = true;
		}

		if (stateKey[SDL_SCANCODE_DOWN] == true)
		{
			vertex.y -= 0.05f;
			detectMove = true;
		}

		if (stateKey[SDL_SCANCODE_LEFT] == true)
		{
			vertex.x -= 0.05f;
			detectMove = true;
		}

		if (stateKey[SDL_SCANCODE_RIGHT] == true)
		{
			vertex.x += 0.05f;
			detectMove = true;
		}
	}


}

namespace ControlScenarios
{

	stateScenarios scene{ stateScenarios::normalSceneario };
	scenarioAABB sceneAABB{ scenarioAABB::Triangle };

	std::map<SDL_Keycode, stateScenarios> scenarios
	{
		{SDLK_N, stateScenarios::normalSceneario}, //normalScenario ---- N
		{SDLK_P, stateScenarios::stencilTestAll}, //stencilTestAll ---- P
		{SDLK_L, stateScenarios::detectAABB} //detectAABB ---- L

	};

	void detectScenario_Key(SDL_Event* event)
	{
		if (scenarios.find(event->key.key) != scenarios.end())
		{
			if (scene != scenarios[event->key.key])
			{
				scene = scenarios[event->key.key];
		    }
		
			else if (scene == scenarios[event->key.key])
			{
				controlSubScenearios();

			}

		}

	}

	void controlSubScenearios()
	{
		if (scene == stateScenarios::detectAABB)
		{
			if (sceneAABB == scenarioAABB::Triangle)
			{
				sceneAABB = scenarioAABB::Mesh;
			}
			
			else if (sceneAABB == scenarioAABB::Mesh)
			{
				sceneAABB = scenarioAABB::Model;

			}

			else if (sceneAABB == scenarioAABB::Model)
			{
				sceneAABB = scenarioAABB::Triangle;
			}
		}

	}


}

namespace controlMouse
{
	glm::vec2 getDistanceMotionMouse()
	{
		float x{};
		float y{};
		SDL_GetMouseState(&x, &y);

		float posX{ x - (static_cast<float>(screenSettings::screen_w) * 0.5f) };
		float posY{ y - (static_cast<float>(screenSettings::screen_w) * 0.5f) };

		//SDL_Log(std::to_string(posX).c_str());
		return glm::vec2(posX, -posY);
	}
}
namespace timeS
{
	getTimeSec::getTimeSec() {};
	void getTimeSec::setTimeSec(float time)
	{
		sec = time;
		MaxTimeS = static_cast<Uint64>(1000 * sec);
		RestTimeS = SDL_GetTicks();
		timeStop = false;

	}
	bool getTimeSec::stopTime()
	{
		if (timeStop == false)
		{
			TimeCountS = SDL_GetTicks() - RestTimeS;

			if (TimeCountS > MaxTimeS)
			{ 
				timeStop = true;
			}
			std::cout << SDL_GetTicks();
		}	

		else if (timeStop == true)
		{
			RestTimeS = SDL_GetTicks();
			timeStop = false;
		}

		return timeStop;
	}

}