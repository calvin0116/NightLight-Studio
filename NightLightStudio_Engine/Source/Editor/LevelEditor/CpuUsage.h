#ifndef CPU_USAGE_EXT
#define CPU_USAGE_EXT
#include <windows.h>

class CpuUsage
{
	//system total times
	FILETIME m_ftPrevSysKernel;
	FILETIME m_ftPrevSysUser;

	//process times
	FILETIME m_ftPrevProcKernel;
	FILETIME m_ftPrevProcUser;
	float m_nCpuUsage;
	ULONGLONG m_dwLastRun;

	volatile LONG m_lRunCount;

	ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB);
	bool EnoughTimePassed();
	inline bool IsFirstRun() const { return (m_dwLastRun == 0); }
public:
	CpuUsage(void);

	float GetUsage();
};

#endif