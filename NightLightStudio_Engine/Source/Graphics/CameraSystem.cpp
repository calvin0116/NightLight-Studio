#include "CameraSystem.h"
#include "../Input/SystemInput.h"

namespace NS_GRAPHICS
{
	CameraSystem::CameraSystem()
	{
	}

	CameraSystem::~CameraSystem()
	{

	}
	void CameraSystem::Init()
	{
		/*SYS_INPUT->GetSystemKeyPress().CreateNewEvent("SOME", SystemInput_ns::IKEY_A, "A" ,SystemInput_ns::OnPress, nullptr);

		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_A))
			std::cout << "A is pressed" << std::endl;

		SYS_INPUT->GetSystemKeyPress().ALL_THE_KEYS();*/
	}
}