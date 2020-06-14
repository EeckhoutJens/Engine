#pragma once
#include "Commands.h"
class LeftCommand : public NFE::Command
{
public:
	LeftCommand() = default;
	void Execute(dae::GameObject* player, ObjectType, const float& deltaTime) override;
};

