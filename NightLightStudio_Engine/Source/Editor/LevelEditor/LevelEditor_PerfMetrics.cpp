#include "LevelEditor_PerfMetrics.h"

#include "psapi.h"

PerformanceMetrics::PerformanceMetrics()
	: _totalMem{}, _memInUse{}, _memSize{ BYTES },
	_usage{}
{
	
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	_totalMem = memInfo.ullTotalPhys;
	
}

PerformanceMetrics::~PerformanceMetrics()
{
}

void PerformanceMetrics::Init()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);
}

void PerformanceMetrics::Run()
{
	// Framerate
	_levelEditor->LE_AddChildWindow("FrameRate", ImVec2(0, 30),
		{
			[&]()
			{
				_levelEditor->LE_AddText("FPS: ");
				ImGui::SameLine();
				_levelEditor->LE_AddText(std::to_string(ImGui::GetIO().Framerate));
			}
		}, true);

	// Memory Usage
	_levelEditor->LE_AddChildWindow("Memory Usage", ImVec2(0, 125) ,
		{
			[&]()
			{
				_levelEditor->LE_AddText("Memory");
				PROCESS_MEMORY_COUNTERS_EX pmc;
				GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
				_memInUse = (float)pmc.PrivateUsage;

				_levelEditor->LE_AddRadioButton("Bytes", (_memSize == BYTES), [&]() {_memSize = BYTES; });
				ImGui::SameLine();
				_levelEditor->LE_AddRadioButton("MegaBytes", (_memSize == MEGABYTES), [&]() {_memSize = MEGABYTES; });
				ImGui::SameLine();
				_levelEditor->LE_AddRadioButton("GigaBytes", (_memSize == GIGABYTES), [&]() {_memSize = GIGABYTES; });

				std::string text;
				std::string text2;

				switch (_memSize)
				{
				case BYTES:
				{
					text = std::to_string(_totalMem).append(" B");
					text2 = std::to_string((size_t)_memInUse).append(" B");
					break;
				}
				case MEGABYTES:
				{
					text = std::to_string(_totalMem / 1048576.0f).append(" MB");
					text2 = std::to_string(_memInUse / 1048576.0f).append(" MB");
					break;
				}
				case GIGABYTES:
				{
					text = std::to_string(_totalMem / 1073741824.0f).append(" GB");
					text2 = std::to_string(_memInUse / 1073741824.0f).append(" GB");
					break;
				}
				}

				_levelEditor->LE_AddText("Total Used:   ");
				ImGui::SameLine();
				_levelEditor->LE_AddText(text);

				_levelEditor->LE_AddText("Current Used: ");
				ImGui::SameLine();
				_levelEditor->LE_AddText(text2);

				_levelEditor->LE_AddProgressBar(_memInUse / _totalMem);
			}
		}, true);

	// CPU Usage
	_levelEditor->LE_AddChildWindow("CPU Usage", ImVec2(0, 60),
		{
			[&]() 
			{
				_levelEditor->LE_AddText("CPU Usage: ");

				_levelEditor->LE_AddProgressBar(_usage.GetUsage() / 100.0f, ImVec2(), std::to_string(_usage.GetUsage()).append(" %"));
			}
		}, true);
}

void PerformanceMetrics::Exit()
{
}
