#include "thread_System.h"
#include "playTest.h"
#include "collision/ScreenHit.h"
//#include "Render/RenderData.h"

namespace controlTime_race
{
	accumulatorTime::accumulatorTime()
	{
		calcTime = static_cast<double>(static_cast<Uint32>(SDL_GetTicks()) / 1000.0);
		currentTime = calcTime;

	}
	accumulatorTime::~accumulatorTime()
	{

	}

	double accumulatorTime::addTimeAccumulator()
	{
		double calcNewTime = static_cast<double>(static_cast<Uint32>(SDL_GetTicks()) / 1000.0);

		calcTime = calcNewTime - lastTime;

		lastTime = calcNewTime;

		return calcNewTime;
	}
	void accumulatorTime::loopAcomulator()
	{
		double newTime = addTimeAccumulator();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		if (frameTime > 0.25)
		{
			frameTime = 0.25;

		}

		accumulator += newTime;

		while (accumulator >= dt)
		{
			////AQUI PUEDEN IR LAS FISICAS
			accumulator -= dt;
		}


		alpha = accumulator / dt;
	}


}


namespace threadSystem
{

	void ControlPhysics_System::control_Loop()
	{
		while (running.load())
		{
			std::unique_lock<std::mutex> lock(mutex_Control);

			cv_start.wait(lock, [this]
				{
					return startSystem || !running;
				});


			if (!running.load())
			{
				break;
			}

			else
			{


				//testPlay::renderTranformations_Objects();
				cameras::updateStateCurrentCamera();
				refresh_Models::refreshAll_Models();
				AABB::updateCoordAABB_All();
				ScreenCalc_Hit::calc_IntersectALL();

				///SDL_Log("UPDATE_THREAD");

				startSystem = false;
				finishSystem = true;

				lock.unlock();
				cv_done.notify_one();
			}

			//testPlay::renderTranformations_Objects();
			//cameras::updateStateCurrentCamera();
			//refresh_Models::refreshAll_Models();
			//AABB::updateCoordAABB_All();
			//ScreenCalc_Hit::calc_IntersectALL();

			///SDL_Log("UPDATE_THREAD");
		}

	}
	void ControlPhysics_System::control_LoopFast()
	{

		while (running.load())
		{
			std::unique_lock<std::mutex> lock(mutex_Control);
			cv_start.wait(lock, [this]
				{
					return startSystem;
				});

		//	if (fps_Thread.frameT == true)
		//	{
			    
			//	testPlay::renderTranformations_Objects();
				//cameras::updateStateCurrentCamera();
				//refresh_Models::refreshAll_LastModels();

				//timeInterpolation.loopAcomulator();

				testPlay::renderTranformations_Objects();
			
				//refresh_Models::refreshAll_Models();
				
				AABB::updateCoordAABB_All();
				ScreenCalc_Hit::calc_IntersectALL();
			
			//	fps_Thread.resetTime();
		//	}


			lock.unlock();
			startSystem = false;
			finishSystem = true;
			cv_done.notify_one();
		//	fps_Thread.countTimeRender();

		}
	}

	ControlPhysics_System::ControlPhysics_System() : running(true) {};

	ControlPhysics_System::ControlPhysics_System(int fps) : running(true)
	{
		
		fps_Thread.inFPS(fps);

		worker_Thread = std::thread(&ControlPhysics_System::control_LoopFast, this);
		if (worker_Thread.joinable())
		{
			worker_Thread.detach();
		}
	}

	ControlPhysics_System::~ControlPhysics_System()
	{
		{
			std::lock_guard<std::mutex> lock(mutex_Control);
			running = false;
			startSystem = true;
		}

		cv_start.notify_one();
		//worker_Thread.join();

	}


	void ControlPhysics_System::initializeThread(int fps)
	{
		fps_Thread.inFPS(fps);

		worker_Thread = std::thread(&ControlPhysics_System::control_LoopFast, this);
		if (worker_Thread.joinable())
		{
			worker_Thread.detach();
		}
	}
	void ControlPhysics_System::update_ControlSystem()
	{
		{
			std::lock_guard<std::mutex> lock(mutex_Control);

			startSystem = true;
			finishSystem = false;

		}
		cv_start.notify_one();

		//cameras::updateStateCurrentCamera();
		//refresh_Models::refreshAll_Models();
		//AABB::updateCoordAABB_All();
		//ScreenCalc_Hit::calc_IntersectALL();
		
		{
			std::unique_lock<std::mutex> lock(mutex_Control);

			cv_done.wait(lock, [this]
				{
					return finishSystem;
				});

		}



	}
	void ControlPhysics_System::controlExternal_System()
	{
		//timeInterpolation.addTimeAccumulator();
		//refresh_Models::refreshUI_point();

		if (fps_Thread.frameT == true)
		{
			startSystem = true;
			finishSystem = false;
			cv_start.notify_one();

			{
				std::unique_lock<std::mutex> lock(mutex_Control);
				
				cv_done.wait(lock, [this]
					{
						return finishSystem;

					});
			}

			fps_Thread.resetTime();
		}

		fps_Thread.countTimeRender();
	}
	void ControlPhysics_System::stopThread()
	{
		{ 
			std::lock_guard<std::mutex> lock(mutex_Control);
			startSystem = false;
			finishSystem = false;
		}
	}
	void ControlPhysics_System::restartThread()
	{
		{
			std::lock_guard<std::mutex> lock(mutex_Control);
			startSystem = true;
		}
		cv_start.notify_one();

	}
	void ControlPhysics_System::destroy()
	{
		this->~ControlPhysics_System();

	}


	ControlPhysics_System ControlPhysics_Events{};


}
