#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "Commands.h"
#include "BaseButton.h"
#include "KeyboardButton.h"
#include "ControllerButton.h"


dae::InputManager::~InputManager()
{

}

bool dae::InputManager::ProcessInput()
{
	if (m_ActiveControllers >= 1)
	{
		for (int i = 0; i < m_ActiveControllers; ++i)
		{
			ZeroMemory(&m_State, sizeof(XINPUT_STATE));
			XInputGetState(i, &m_State);

			SDL_Event e;
			while (SDL_PollEvent(&e)) 
			{
				if (e.type == SDL_QUIT) 
				{
					return false;
				}
				if (e.type == SDL_KEYDOWN) 
				{
				
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) 
				{
				
				}
			}
		}
	return  true;
	}

		SDL_Event e;
		while (SDL_PollEvent(&e)) 
		{
			if (e.type == SDL_QUIT) 
			{
				return false;
			}
			if (e.type == SDL_KEYDOWN) 
			{
				
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) 
			{
				
			}
		}
	return true;
}
	
std::vector<NFE::Command*> dae::InputManager::HandleInput(int controllerID, std::vector<NFE::Command*>& vecCommands)
{
	std::vector<NFE::Command*> triggeredCommands;
		RtlZeroMemory(&m_State, sizeof(XINPUT_STATE));
		if (XInputGetState(controllerID, &m_State) == ERROR_SUCCESS)
		{
		}
		else
		{
			--m_ActiveControllers;
		}


		for(NFE::Command* pCommand : vecCommands)
		{
			auto commandButtons = pCommand->GetButton();
			for (BaseButton* pButton : commandButtons)
			{

				if (IsPressed(pButton))
				{
					triggeredCommands.push_back(pCommand);
				}

			}
			
		}
	
	return triggeredCommands;
}

bool dae::InputManager::IsPressed(BaseButton* button) const
{
	KeyboardButton* pKeyboardButton = dynamic_cast<KeyboardButton*>(button);
	if (pKeyboardButton)
	{
		
		return GetKeyState(pKeyboardButton->GetValue()) & 0x8000;
	}

	ControllerButton* pControllerButton = dynamic_cast<ControllerButton*>(button);
	if (pControllerButton)
	{
		return m_State.Gamepad.wButtons & pControllerButton->GetValue();
	}
	return false;
}


void dae::InputManager::CheckControllers()
{
	m_ActiveControllers = 0;
	for(int i = 0; i < 4; ++i)
	{
		RtlZeroMemory(&m_State, sizeof(XINPUT_STATE));
		if (XInputGetState(i, &m_State) == ERROR_SUCCESS)
		{
			++m_ActiveControllers;
		}
	}
}


