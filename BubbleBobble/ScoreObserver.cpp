#include "ScoreObserver.h"
#include "GameObject.h"
#include "TypeComponent.h"
ScoreObserver::ScoreObserver()
{
}
void ScoreObserver::onNotify(dae::GameObject* entity , Events event )
{
	if (event == Events::Event_AddScore)
	{
		NFE::TypeComponent* typeComp = entity->GetComponent<NFE::TypeComponent>();
		if (typeComp->GetType() == GameObjectType::Watermelon)
		{
			m_Score += 100;
		}
		if (typeComp->GetType() == GameObjectType::Fries)
		{
			m_Score += 200;
		}
	}
}

