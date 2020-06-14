#include "JumpCommand.h"
#include "CollisionComponent.h"

void JumpCommand::Execute(dae::GameObject* player, ObjectType, const float&)
{
	auto collComp = player->GetComponent<NFE::CollisionComponent>();
	if (collComp)
	{
		collComp->SetNotOnGround();
	}

}
