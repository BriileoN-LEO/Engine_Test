#include "system_UI_B.h"

namespace brSystemUI
{

	Icon_UI::Icon_UI() {};
	Icon_UI::Icon_UI(dataIcons icon, event_Icon eventUI, event_IconProperty icon_prop, glm::vec3 posicion) :
		dataIconUI(icon), eventUI(eventUI), icon_prop(icon_prop), posicion(posicion)
	{};
	Icon_UI::Icon_UI(Icon_UI&& icon) noexcept :
		dataIconUI(icon.dataIconUI), eventUI(icon.eventUI), icon_prop(icon.icon_prop), posicion(icon.posicion)
	{
		sprite_B = std::move(icon.sprite_B);

	}
	Icon_UI::Icon_UI(Icon_UI& icon) :
		dataIconUI(icon.dataIconUI), eventUI(icon.eventUI), icon_prop(icon.icon_prop), posicion(icon.posicion)
	{
		sprite_B = std::move(icon.sprite_B);

	}
	Icon_UI& Icon_UI::operator=(Icon_UI&& oT) noexcept
	{
		dataIconUI = oT.dataIconUI;
		eventUI = oT.eventUI;
		icon_prop = oT.icon_prop;
		posicion = oT.posicion;
		
		sprite_B = std::move(oT.sprite_B);
		
		return *this;
	}
	Icon_UI& Icon_UI::operator=(Icon_UI& oT)
	{
		dataIconUI = oT.dataIconUI;
		eventUI = oT.eventUI;
		icon_prop = oT.icon_prop;
		posicion = oT.posicion;

		sprite_B = std::move(oT.sprite_B);

		return *this;
	}

	section_Icon::section_Icon() {};
	section_Icon::section_Icon(event_SecUI eventSec, std::vector<Icon_UI>iconsUI) :
		eventSec(eventSec),
		iconsUI(iconsUI) {};

	section_Icon::section_Icon(event_SecUI eventSec, Icon_UI iconUI) : eventSec(eventSec)
	{
		//insert_iconUI(iconUI);
	}

	void section_Icon::insert_iconUI(Icon_UI iconUI_)
	{
		iconsUI.emplace_back(iconUI_);
	};


 
}