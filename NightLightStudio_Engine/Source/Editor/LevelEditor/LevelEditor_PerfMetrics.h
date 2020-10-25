#ifndef LEVELEDITOR_PERFORMANCE_METRICS
#define LEVELEDITOR_PERFORMANCE_METRICS

#include "LevelEditor.h"
#include "CpuUsage.h"

class PerformanceMetrics : public LE_WinBase_Derived<PerformanceMetrics>
{
	CpuUsage _usage;

	size_t _totalMem;
	float _memInUse;

	std::vector<float> _systemsUsage;

	enum MEM_SIZE
	{
		BYTES,
		MEGABYTES,
		GIGABYTES
	};

	MEM_SIZE _memSize;
public:
	PerformanceMetrics();
	~PerformanceMetrics();

	void Init() override;

	void Run() override;

	void Exit() override;

	std::vector<float>* GetSystemsUsage();
};

#endif