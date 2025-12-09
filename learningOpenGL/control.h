#ifndef control
#define control

#include "config_window.h"
#include "data_save.h"

namespace controlMove
{
	extern bool detectMove;

	 struct detectSDLK_code
	{
		static std::map<SDL_Keycode, bool> keys;
		static std::map<SDL_Keycode, bool> keys_Color;
		static std::map<SDL_Keycode, bool> keys_Scale;
		static std::map<SDL_Keycode, bool> keys_Translate;
		static std::map<SDL_Keycode, bool> keys_fov;
		static std::map<SDL_Keycode, bool> keys_UI;

		static bool detectKey(SDL_Event event);
		static bool detectKeyColor(SDL_Event event);
		static bool detectKeyScale(SDL_Event event);
		static bool detectKeyTranslate(SDL_Event* event);
		static bool detectKeyUI(SDL_Event& event);
		static void resetKeys();
	};
	 

	void moveTriangle(float(&vertices)[9]);
	void moveVec3(vec::vec3& vertex);

}

namespace ControlScenarios
{
	enum class stateScenarios
	{
		normalSceneario = 0,
		stencilTestAll = 1,
		detectAABB = 2

	};
	
	enum class scenarioAABB
	{
		Model = 0,
		Mesh = 1,
		Triangle = 2

	};

	extern stateScenarios scene;
	extern scenarioAABB sceneAABB;

	extern std::map<SDL_Keycode, stateScenarios> stateScene;

	void detectScenario_Key(SDL_Event* event);
	void controlSubScenearios();


}
namespace controlMouse
{
		glm::vec2 getDistanceMotionMouse();


}
namespace timeS
{
	class getTimeSec
	{
	public:

		Uint64 TimeCountS{};
		Uint64 RestTimeS{};
		Uint64 MaxTimeS{};
	
		float sec{};
		bool timeStop{ false };
		
		getTimeSec();
		void setTimeSec(float time);
		bool stopTime();
		

	};

}




#endif