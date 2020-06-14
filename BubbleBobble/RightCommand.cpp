#include "RightCommand.h"

void RightCommand::Execute(dae::GameObject* player, ObjectType, const float& deltaTime)
{
	auto currPos = player->GetPosition();
	player->SetPosition(currPos.x += 100.f * deltaTime, currPos.y);
}
