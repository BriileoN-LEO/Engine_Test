#ifndef config_window
#define config_window

#include "learningOpenGL.h"

namespace screenSettings
{
	extern int screen_w;
	extern int screen_h;

	extern bool detectChangeSizeWindow;
	extern bool outWindow;

	extern int fps;


	void  framebuffer_size_callback(SDL_Window* window);

	struct vSync
	{
	public:
		
		static Uint64 maxNS_PerSecond;
		static Uint64 count_Time;
		static Uint64 stopTimeNS; 
		static bool frameT;

		static void inFPS(int fps);
		static void countTimeRender();
	
	};

}

namespace vec
{
	class vec3
	{
	public:

		float x{};
		float y{};
		float z{};

		vec3() {};
		vec3(float x, float y, float z) : x(x), y(y), z(z) {};
		
		auto operator<=>(const vec3& other) const = default;
	};

	/*
	bool operator<(const vec3& v1, const vec3& v2)
	{
		return std::tie(v1.x, v1.y, v1.z) < std::tie(v2.x, v2.y, v2.z);
	}
    */

	class vec2
	{
	public:

		float s{};
		float t{};

		vec2() {};
		vec2(float s, float t) : s(s), t(t) {};
		auto operator+=(const float plus)
		{
			s += plus;
			t += plus;
		}

		auto operator-=(const float plus)
		{
			s -= plus;
			t -= plus;
		}
	};

	class rgb3
	{
	public:

		float r{};
		float g{};
		float b{};

		rgb3() {};
		rgb3(float r, float g, float b) : r(r), g(g), b(b) {};


	};


}


#endif