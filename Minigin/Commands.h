#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "BaseButton.h"
#include <iostream>

namespace dae 
{
	class GameObject;
}

namespace NFE
{
	class ReleasedCommand;
	class Command
	{
		public:
		Command() = default;
		virtual ~Command()
		{	
			for (BaseButton* button : m_pButtons)
			{
				delete button;
				button = nullptr;
			}
		}
		virtual void Execute(dae::GameObject*, ObjectType type, const float& deltaTime) = 0;
		void AssignButton(BaseButton* buttonToAssign)
		{
			m_pButtons.push_back(buttonToAssign);
		}
		std::vector<BaseButton*> GetButton() const {return m_pButtons;}

	private:
		std::vector<BaseButton*> m_pButtons{};
		ReleasedCommand* m_pReleasedCommand{};
	};
}

