#include "pch.h"
#include "ZenChanComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "TextureComponent.h"

NFE::ZenChanComponent::ZenChanComponent(dae::GameObject* parent) : NFE::BaseComponent(parent)
{
}

void NFE::ZenChanComponent::Update(const float& deltaTime, float, float, float, float)
{
	dae::float3 currPos = m_pParent->GetPosition();
	if (currPos.y > 550.f)
	{
		m_pParent->SetPosition(currPos.x, 0);
	}
	if (m_BubbledTimer >= m_MaxBubbleTimer)
	{
		SetUnBubbled();
	}
	if (!m_IsPerformingAction && m_CurrAction != Action::Bubbled)
	{
		Action randomAction = Action(rand() % 3);
		if (m_CurrAction == Action::Jump)
		{
			if (randomAction == Action::Jump)
			{
				randomAction = Action::Left;
			}
		}
		switch (randomAction)
		{
		case Action::Jump:
			m_Velocity.x = 0.f;
			m_Velocity.y = -150.f;
			Jump(deltaTime);
			m_CurrAction = Action::Jump;
			m_ActionTimer = 0.75f;
			break;

		case Action::Right:
			m_Velocity.x = 100.f;
			m_Velocity.y = 0.f;
			Move(deltaTime);
			m_CurrAction = Action::Right;
			m_ActionTimer = 1.0f;
			break;

		case Action::Left:
			m_Velocity.x = -100.f;
			m_Velocity.y = 0.f;
			Move(deltaTime);
			m_CurrAction = Action::Left;
			m_ActionTimer = 1.0f;
			break;

		default:
			break;

		}
	}
	else
	{
		switch (m_CurrAction)
		{
		case  Action::Jump:
			Jump(deltaTime);
			break;


		case Action::Left:
			Move(deltaTime);
			break;


		case Action::Right:
			Move(deltaTime);
			break;

		case Action::Bubbled:
			FloatUp(deltaTime);
		}
		m_ActionTimer -= deltaTime;
		if (m_ActionTimer <= 0)
		{
			m_IsPerformingAction = false;
		}
	}
}

void NFE::ZenChanComponent::SetBubbled()
{
	m_CurrAction = Action::Bubbled;
	NFE::TextureComponent* texComp = m_pParent->GetComponent<NFE::TextureComponent>();
	NFE::CollisionComponent* collComp = m_pParent->GetComponent<NFE::CollisionComponent>();
	collComp->SetCollisionType(NFE::CollisionType::TrappedEnemy);
	texComp->ChangeTexture("ZenChanBubble.png");
}

void NFE::ZenChanComponent::SetUnBubbled()
{
	m_CurrAction = Action::Right;
	NFE::TextureComponent* texComp = m_pParent->GetComponent<NFE::TextureComponent>();
	NFE::CollisionComponent* collComp = m_pParent->GetComponent<NFE::CollisionComponent>();
	collComp->SetCollisionType(NFE::CollisionType::Enemy);
	texComp->ChangeTexture("ZenChan.png");
	m_BubbledTimer = 0.f;
}

void NFE::ZenChanComponent::Jump(const float& deltaTime)
{

	m_IsPerformingAction = true;
	dae::float3 currPos = m_pParent->GetPosition();
	NFE::CollisionComponent* collisionComp = getParent()->GetComponent<CollisionComponent>();
	if (collisionComp != nullptr)
	{
		if (collisionComp->GetIsOnGround())
		{
			m_pParent->SetPosition(currPos.x, currPos.y += m_Velocity.y * deltaTime);
		}
	}
	
}

void NFE::ZenChanComponent::Move(const float& deltaTime)
{ 
	m_IsPerformingAction = true;
	dae::float3 currPos = m_pParent->GetPosition();
	m_pParent->SetPosition(currPos.x +=  m_Velocity.x * deltaTime, currPos.y);
	m_pParent->SetPosition(currPos.x, currPos.y += 98.1f * deltaTime);
}

void NFE::ZenChanComponent::FloatUp(const float& deltaTime)
{
	m_BubbledTimer += deltaTime;
	dae::float3 currPos = m_pParent->GetPosition();
	m_pParent->SetPosition(currPos.x, currPos.y -= 20 * deltaTime);
}
