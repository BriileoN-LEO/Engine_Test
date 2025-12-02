#ifndef thread_System
#define thread_System
#include "Render/render.h"
#include <condition_variable>

namespace controlTime_race
{
	class accumulatorTime
	{
	public:

		const double dt{ 1.0 / 60.0 };
		double accumulator{};
		double currentTime{};
		double calcTime{};
		double lastTime{};

		double alpha{};

		accumulatorTime();
		~accumulatorTime();

		double addTimeAccumulator();
		void loopAcomulator();

	};



}


namespace threadSystem
{
	class ControlPhysics_System
	{
	private:
		std::thread worker_Thread;
		std::mutex mutex_Control;
		//std::condition_variable cv_start;
		std::condition_variable cv_done;

		//bool startSystem{ false };
		bool finishSystem{ false };
		std::atomic<bool> running;

		screenSettings::vSync fps_Thread{};
		//controlTime_race::accumulatorTime timeInterpolation{};

		void control_Loop();
		void control_LoopFast();

	public:

		controlTime_race::accumulatorTime timeInterpolation{};
		std::condition_variable cv_start;
		bool startSystem{ false };

		ControlPhysics_System();
		ControlPhysics_System(int fps);
		~ControlPhysics_System();

		void initializeThread(int fps);
		void update_ControlSystem();
		void controlExternal_System();
		void stopThread();
		void restartThread();
		void destroy();

	};

	extern ControlPhysics_System ControlPhysics_Events;


}




#endif thread_System