#include "config_window.h"
#include "threadSystem/thread_System.h"

glm::vec4 viewportOpenGL{};

namespace screenSettings
{
	int screen_w{ 1920 };
	int screen_h{ 1080 };
	bool detectChangeSizeWindow{ false };
	bool outWindow{ false };
	int fps{ 120 };

	void screenSettings::framebuffer_size_callback(SDL_Window* window)
	{
		int w_back = screen_w;
		int h_back = screen_h;

		SDL_GetWindowSize(window, &screen_w, &screen_h);

		if (w_back != screen_w || h_back != screen_h)
		{
			detectChangeSizeWindow = true;
		}

	}

	//Uint64 screenSettings::vSync::maxNS_PerSecond{ 0 };
	//Uint64 screenSettings::vSync::count_Time { 0 };
	//Uint64 screenSettings::vSync::stopTimeNS { 0 };
	//bool screenSettings::vSync::frameT { false };

	void vSync::inFPS(int fps)
	{
		maxNS_PerSecond = 1000000000 / fps;
	}
	void vSync::countTimeRender()
	{
		count_Time = SDL_GetTicksNS() - stopTimeNS;

		if (count_Time > maxNS_PerSecond)
		{
			frameT = true;
		}
	}

	void vSync::resetTime()
	{

		frameT = false;
		stopTimeNS = SDL_GetTicksNS();

	}

	void vSync::syncThread_Time(std::condition_variable& unlock_Mutex, bool& start)
	{
		start = true;
		unlock_Mutex.notify_one();

	}

}