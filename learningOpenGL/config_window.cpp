#include "config_window.h"


int screenSettings::screen_w{ 1920 };
int screenSettings::screen_h{ 1080 };
bool screenSettings::detectChangeSizeWindow{ false };
bool screenSettings::outWindow{ false };
int screenSettings::fps{ 60 };

void screenSettings::framebuffer_size_callback(SDL_Window* window)
{
	int w_back = screenSettings::screen_w;
	int h_back = screenSettings::screen_h;
	
	SDL_GetWindowSize(window, &screenSettings::screen_w, &screenSettings::screen_h);

	if (w_back != screenSettings::screen_w || h_back != screenSettings::screen_h)
	{
		screenSettings::detectChangeSizeWindow = true;
	}
	
}

Uint64 screenSettings::vSync::maxNS_PerSecond{ 0 };
Uint64 screenSettings::vSync::count_Time { 0 };
Uint64 screenSettings::vSync::stopTimeNS { 0 };
bool screenSettings::vSync::frameT { false };
void screenSettings::vSync::inFPS(int fps)
{
	 maxNS_PerSecond = 1000000000 / fps;
}
void screenSettings::vSync::countTimeRender()
{
	count_Time = SDL_GetTicksNS() - stopTimeNS;

	if (count_Time > maxNS_PerSecond)
	{
		frameT = true;
	}
}
