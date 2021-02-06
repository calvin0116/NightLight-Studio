#pragma once

#include <time.h>
#include "Singleton.h"
#include <chrono>

//constexpr auto MAX_DT = 15; // in ms

constexpr auto DT_SCALE = 0.001f; // 1 sec / 1000 to scale dt to ms

//constexpr auto FPS_60 = 1000.0f / 60.0f;
constexpr auto FPS_60 = 16.0f; // in ms

class DeltaTime : public Singleton<DeltaTime>
{
	friend Singleton<DeltaTime>;
public:
	/*
	float fixed_dt = FPS_60;
	float _prevdt = 0.0f;
	float real_dt = 0.0f;

	float accumulatedTime = 0.0f;
	clock_t t_start = 0;

	//float dt = 0.0f;
	float dt = fixed_dt;
	float fps = 0.0f;
	int currentNumberOfSteps = 0;
	*/
	std::chrono::system_clock::time_point timeThisRound;
	std::chrono::system_clock::time_point timeLastRound;
	std::chrono::system_clock::time_point stepTime;
	float real_dt;
	float accumulatedTime;
	int currentNumberOfSteps;
	float oneSecond;
	float fpsCounter = 0.0f;
	float fps;
	float fps_counter;
	float fpsLimit;
	float frameDelay;
	float speed = 1.0f;
	const float fixed_dt = 1.0f / 60.0f;//user defined


	void load();

	void start();

	void end();

	void Exit();
	void SetFpsLimit(float f);
	int GetCurrNumberOfSteps();
};

// global variable for dt
//extern DeltaTime delta_t;
static DeltaTime* DELTA_T = DeltaTime::GetInstance();
