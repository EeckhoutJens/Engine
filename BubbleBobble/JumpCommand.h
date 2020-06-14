#pragma once
#include <Commands.h>
class JumpCommand : public NFE::Command
{
public:
	JumpCommand() = default;
	void Execute(dae::GameObject* player, ObjectType, const float& deltaTime) override;

private: 
	float m_JumpTimer{};
};

