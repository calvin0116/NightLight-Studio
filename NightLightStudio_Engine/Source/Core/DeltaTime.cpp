#include "DeltaTime.h"
#include  <iostream>

//DeltaTime delta_t;

void DeltaTime::load()
{
	t_start = clock();
}

void DeltaTime::start()
{
	currentNumberOfSteps = 0;

	dt = (float)clock() - t_start;
	t_start = clock();

	//while (dt < fixed_dt)
	//{
	//	dt = (float)clock() - t_start;
	//}
	// fps is limited in system gfx

	accumulatedTime += dt;

	while (accumulatedTime >= fixed_dt)
	{
		accumulatedTime -= fixed_dt;
		++currentNumberOfSteps;
	}
}

void DeltaTime::end()
{
	fps = 1000.0f / ((dt + _prevdt) * 0.5f);

	_prevdt = dt;
}
