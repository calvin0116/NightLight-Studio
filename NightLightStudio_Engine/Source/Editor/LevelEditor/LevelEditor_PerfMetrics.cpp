#include "LevelEditor_PerfMetrics.h"
#include "../../Component/Components.h"
#include "psapi.h"

PerformanceMetrics::PerformanceMetrics()
	: _totalMem{}, _memInUse{}, _memSize{ MEGABYTES },
	_usage{}, _timePassed{ 0 }, _startTime{ std::chrono::steady_clock::now() }, _endTime{ std::chrono::steady_clock::now() },
	_elapsedTime{0}
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

	_endTime = std::chrono::steady_clock::now();
	_elapsedTime += std::chrono::duration_cast<std::chrono::microseconds>(_endTime - _startTime).count() / 1000000.0f;
	if (_elapsedTime > 0.5f)
	{
		_elapsedTime = 0.0f;
		_systemsUsageShow = _systemsUsage;
	}
}

void PerformanceMetrics::Run()
{
	/*
	auto itr = G_ECMANAGER->begin<ComponentTransform>();
	auto itrend = G_ECMANAGER->end<ComponentTransform>();
	for (; itr != itrend; ++itr)
	{
		ComponentTransform* comp = G_ECMANAGER->getComponent<ComponentTransform>(itr);
		if (comp->name)
		{
			_levelEditor->LE_AddText(comp->name);
		}
		else
		{
			_levelEditor->LE_AddText("Object");
			std::string pos = std::string("X: ").append(std::to_string(comp->_position.x));
			pos.append(" Y: ").append(std::to_string(comp->_position.y));
			pos.append(" Z: ").append(std::to_string(comp->_position.z));
			_levelEditor->LE_AddText(pos);
		}
	}
	*/
	// Framerate
	_levelEditor->LE_AddChildWindow("FrameRate", ImVec2(0, 30),
		[this]()
		{
			_levelEditor->LE_AddText("FPS: ");
			ImGui::SameLine();
			_levelEditor->LE_AddText(std::to_string(ImGui::GetIO().Framerate));
		}, true);

	// Memory Usage
	_levelEditor->LE_AddCollapsingHeader("Memory Usage",
		[this]()
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
		);

	// CPU Usage
	_levelEditor->LE_AddCollapsingHeader("CPU Usage",
		[this]()
		{
			_levelEditor->LE_AddText("CPU Usage: ");

			_levelEditor->LE_AddProgressBar(_usage.GetUsage() / 100.0f, ImVec2(), std::to_string(_usage.GetUsage()).append(" %"));
		}
		);

	// System Usage
	_levelEditor->LE_AddCollapsingHeader("Systems Usage",
		[this]()
		{
			float total = 0;

			std::vector<float> orig = _systemsUsageShow;

			for (int i = 0; i < orig.size(); ++i)
				total += orig[i];

			for (int i = 0; i < orig.size(); ++i)
			{
				orig[i] /= total;
			}

			_levelEditor->LE_AddText("Systems Usage: ");
			ImGui::SameLine();
			_levelEditor->LE_AddText(std::to_string(total / 1000.0f).append(" ms Per Update"));

			/*
				SP_TOOLS = 0,
				SP_WINDOW,
				SP_IO,
				SP_INPUT,
				SP_GRAPHICS,
				SP_COLLISION,
				SP_PHYSICS,
				SP_AUDIO,
				SP_SCENEMANAGER,
				SP_COMPONENT,
				SP_EDITOR
				*/

			std::vector<std::string> sysNamesManual =
			{ "Window", "Graphics", "Input", "IO", "Audio", "SceneManager", "Physics", "Component", "Collision", "Logic", "Editor"};

			//_levelEditor->LE_AddHistogram("Systems Use", _systemsUsage, false, 0, 0.0f, 100.0f, ImVec2(0, 50));
			for (int i = 0; i < orig.size(); ++i)
			{
				int sysNum = i - 1;
				if (sysNum == -1)
					sysNum = (int)orig.size() - 1;

				_levelEditor->LE_AddProgressBar(orig[i], ImVec2(100, 0));
				if (i < sysNamesManual.size())
				{
					ImGui::SameLine();
					_levelEditor->LE_AddText(sysNamesManual[i]);
					ImGui::SameLine();
					_levelEditor->LE_AddText(std::to_string(orig[i] * 100.0f).append("%% :: ").append(std::to_string(_systemsUsageShow[i] / 1000.0f)).append(" ms"));
				}
			}
		});

	// Resets usage data
	for (float& i : _systemsUsage)
		i = 0;
}

void PerformanceMetrics::Exit()
{
	_startTime = std::chrono::steady_clock::now();
}

std::vector<float>* PerformanceMetrics::GetSystemsUsage()
{
	return &_systemsUsage;
}


