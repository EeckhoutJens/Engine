#include "MiniginPCH.h"
#include "StateComponent.h"
#include "BaseState.h"


NFE::StateComponent::StateComponent(dae::GameObject* parent, BaseState* pDefaultState) 
:  BaseComponent(parent)
,pState(pDefaultState)
{
}

NFE::StateComponent::~StateComponent()
{
	delete pState;
	pState = nullptr;
}

BaseState* NFE::StateComponent::GetState() const
{
	return pState;
}

void NFE::StateComponent::SetState(BaseState* newState)
{
	delete pState;
	pState = nullptr;
	pState = newState;
}