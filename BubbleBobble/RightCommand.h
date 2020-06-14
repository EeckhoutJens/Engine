#pragma once
#include <Commands.h>
class RightCommand : public NFE::Command
{
public:
	RightCommand() = default;
	void Execute(dae::GameObject* player, ObjectType, const float& deltaTime) override;
};

