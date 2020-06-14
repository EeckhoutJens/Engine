#pragma once
namespace dae
{
	class GameObject;
}


class BaseState
{
public:

	BaseState() = default;
	virtual ~BaseState() = default;

	BaseState(const BaseState& other) = delete;
	BaseState(BaseState&& other) = delete;
	BaseState& operator=(const BaseState& other) = delete;
	BaseState& operator=(const BaseState&& other) = delete;
};

