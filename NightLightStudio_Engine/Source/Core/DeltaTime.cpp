
#include "DeltaTime.h"
#include  <iostream>
#include <thread>
//DeltaTime delta_t;

void DeltaTime::load()
{
	//t_start = clock();
	fps = 0;
	fpsCounter = 0;
	oneSecond = 0;
	fpsLimit = -1;
	frameDelay = -1;
	SetFpsLimit(30);
	timeLastRound = timeThisRound = std::chrono::system_clock::now();
	real_dt = (float)(timeThisRound - timeLastRound).count() / 10000000;
	accumulatedTime = 0;
}

void DeltaTime::start()
{

	/*
	currentNumberOfSteps = 0;

	dt = (float)clock() - t_start; // dt = fixed_dt
	//t_start = clock();

	while (dt < fixed_dt)
	{
		dt = (float)clock() - t_start;
	}
	// fps is limited in system gfx

	//dt = (float)clock() - t_start; // dt = fixed_dt
	t_start = clock();


	//accumulatedTime += dt;

	//
	int steps = (int)(dt / fixed_dt);
	float timeSteps = steps * fixed_dt;
	//accumulatedTime += timeSteps;
	accumulatedTime = timeSteps;

	const int maxAccFrames = 1;
	if (accumulatedTime > (fixed_dt * maxAccFrames)) // this limits accumulated frames to x frames
		accumulatedTime = (fixed_dt * maxAccFrames);

	// fml
	real_dt = dt;
	dt = fixed_dt;
	//accumulatedTime = fixed_dt;
	*/
	cur_step = 0;

	std::swap(timeLastRound, timeThisRound);
	stepTime = timeThisRound = std::chrono::system_clock::now();
	real_dt = (float)(timeThisRound - timeLastRound).count() / 10000000.0f;
	//DelayFrame();
	//std::cout << deltaTime.count() << std::endl;

	accumulatedTime += real_dt;

	if (accumulatedTime >= fixed_dt)
	{
		cur_step += static_cast<int>(accumulatedTime / fixed_dt);
		accumulatedTime = fmod(accumulatedTime, fixed_dt);//this will save the exact
	}


	if (oneSecond < 1)
	{
		oneSecond += real_dt;
		fpsCounter++;
	}
	else {
		fps = fpsCounter;
		//std::cout << fps << std::endl;
		oneSecond = 0;
		fpsCounter = 0;
	}

	DelayFrame();
}

void DeltaTime::end()
{
	//////
	// fps end
	//fps = 1000.0f / ((dt + _prevdt) * 0.5f);
	//fps = 1000.0f / dt;
	//fps = 1000.0f / real_dt;

	//_prevdt = dt;
	//std::cout << fps << std::endl;
	//std::cout << "60.00" << std::endl; // best solution for 60fps

	//_sysMgr._currentFPS = fps;
	// fps
	//////
}

void DeltaTime::Exit()
{
	DestroyInstance();
}


void DeltaTime::SetFpsLimit(float f)
{
	fpsLimit = f;
	frameDelay = 2000.0f / (fpsLimit)+(0.25f);
	//frameDelay = 1000.0 / (fpsLimit);
}

void DeltaTime::DelayFrame()
{
	auto now = std::chrono::system_clock::now();
	float currentDt = (float)(now - timeLastRound).count();
	std::chrono::duration<float, std::milli> delta_ms(frameDelay - currentDt);
	std::chrono::time_point end = now + std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
	std::this_thread::sleep_until(end);
}

int DeltaTime::GetCurrNumberOfSteps()
{
	std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
	accumulatedTime += (float)(currTime - stepTime).count() / 10000000.0f;

	if (accumulatedTime >= fixed_dt)
	{
		cur_step += static_cast<int>(accumulatedTime / fixed_dt);
		accumulatedTime = fmod(accumulatedTime, fixed_dt);//this will save the exact
		stepTime = currTime;
	}

	if (cur_step > 5)
		cur_step = 5;

	if (prev_step > 4)
		cur_step = 1;

	prev_step = cur_step;

	return cur_step;
}

