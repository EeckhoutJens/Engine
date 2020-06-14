#include "MiniginPCH.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Commands.h"

NFE::InputComponent::InputComponent(dae::GameObject *parent, int ID, ObjectType type) 
: BaseComponent(parent),
m_ControllerID(ID),
m_Type{type},
triggeredCommands{},
prevTriggeredCommand{}
{
}

NFE::InputComponent::~InputComponent()
{
	for (NFE::Command* comm : m_VecCommands)
	{
		delete comm;
		comm = nullptr;
	}
}


void NFE::InputComponent::Update(const float& deltaTime, float, float, float,float)
{
	triggeredCommands = dae::InputManager::GetInstance().HandleInput(m_ControllerID,m_VecCommands);
	prevTriggeredCommand = nullptr;
	for (NFE::Command* pCommand : triggeredCommands)
	{
		pCommand->Execute(m_pParent, m_Type, deltaTime);
		prevTriggeredCommand = pCommand;
	}
}

void NFE::InputComponent::AssignCommand(Command* command)
{
	m_VecCommands.push_back(command);
}
