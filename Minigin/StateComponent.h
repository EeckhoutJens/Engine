#pragma once
#include "BaseComponent.h"
class BaseState;
namespace NFE
{
	class StateComponent final : public BaseComponent
{
public:
	StateComponent(dae::GameObject *parent, BaseState* pDefaultState);
	~StateComponent();
	BaseState* GetState() const;
	void SetState(BaseState* newState);
	void Update(const float&, float, float, float, float) override{;};
	void Render() override {;};
	StateComponent(const StateComponent& other) = delete;
	StateComponent(StateComponent&& other) = delete;
	StateComponent& operator=(const StateComponent& other) = delete;
	StateComponent& operator=(const StateComponent&& other) = delete;

private:
	BaseState* pState{};
};
}


