#pragma once
#include <Commands.h>
class FireCommand : public NFE::Command
{
public:
	FireCommand() = default;
	void Execute(dae::GameObject * player, ObjectType, const float& deltaTime) override;
};

