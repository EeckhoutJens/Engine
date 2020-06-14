#pragma once
#include <XInput.h>
#include "Singleton.h"
namespace NFE
{
	class Command;
}
class BaseButton;
namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		~InputManager();
		bool ProcessInput();
		std::vector<NFE::Command*> HandleInput(int controllerID, std::vector<NFE::Command*>& vecCommands);
		bool IsPressed(BaseButton* button) const;
		void CheckControllers();

	private:
		XINPUT_STATE m_State{};
		int m_ActiveControllers{};
		bool m_IsAPressed{false};
	};

}
