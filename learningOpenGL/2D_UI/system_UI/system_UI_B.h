#ifndef system_UI_B
#define system_UI_B

#include "learningOpenGL.h"
#include "2D_UI/Interface_generalUI.h"

namespace brSystemUI
{
	struct dataIcons
	{
		brii_UI::menuSpriteUI menuS_UI{};
		brii_UI::sectionsUI sectionS_UI{};
		brii_UI::typeSection_UI typeS_UI{};

	};

	enum class event_Icon
	{
		SLICE_UI_VERTICAL,
		SLICE_UI_HORIZONTAL

	};

	enum class event_IconProperty
	{
	    MOVE_LEFT,
		MOVE_RIGHT

	};


	class Icon_UI
	{
	private:

		dataIcons dataIconUI{};
		std::unique_ptr<brii_UI::spriteUI> sprite_B{ nullptr };

	public:

		event_Icon eventUI{};
		event_IconProperty icon_prop{};

		glm::vec3 posicion{};
		
		Icon_UI();
		Icon_UI(dataIcons icon, event_Icon eventUI, event_IconProperty icon_prop, glm::vec3 posicion);
		Icon_UI(Icon_UI&& icon) noexcept;
		Icon_UI(const Icon_UI& icon) = delete; ////////////REVISIT THIS FUNCTIONS AND WHAT I CAN CHANGE
		~Icon_UI();
		Icon_UI& operator=(Icon_UI&& oT) noexcept;
		Icon_UI& operator=(Icon_UI& oT);

	};

	enum class event_SecUI
	{
		CONTROL_SHADERS

	};

	class section_Icon
	{
	public:
		
		event_SecUI eventSec{};
		std::vector<Icon_UI> iconsUI{};
		
		section_Icon();
		section_Icon(event_SecUI eventSec, std::vector<Icon_UI>& iconsUI);
		section_Icon(event_SecUI eventSec, Icon_UI iconUI);

		void insert_iconUI(Icon_UI& iconUI_);

	};

	std::map<event_SecUI, section_Icon> editMode_UI_B{};

}




#endif //system_UI_B