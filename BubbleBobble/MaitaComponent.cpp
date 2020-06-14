#include "pch.h"
#include "MaitaComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "TextureComponent.h"
#include "FireBall.h"

NFE::MaitaComponent::MaitaComponent(dae::GameObject* parent) : NFE::BaseComponent(parent)
{
}

NFE::MaitaComponent::~MaitaComponent()
{
	for (FireBall* pFireBall : m_vFireballs)
	{
		delete pFireBall;
		pFireBall = nullptr;
	}

	m_vFireballs.clear();
}

void NFE::MaitaComponent::Update(const float& deltaTime, float, float, float, float)
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
	if (!m_IsPerformingAction && m_CurrAction != MaitaAction::Bubbled)
	{
		MaitaAction randomAction = MaitaAction(rand() % 4);
		if (m_CurrAction == MaitaAction::Jump)
		{
			if (randomAction == MaitaAction::Jump)
			{
				randomAction = MaitaAction::Left;
			}
		}
		switch (randomAction)
		{
		case MaitaAction::Jump:
			m_Velocity.x = 0.f;
			m_Velocity.y = -150.f;
			Jump(deltaTime);
			m_CurrAction = MaitaAction::Jump;
			m_ActionTimer = 0.75f;
			break;

		case MaitaAction::Fire:
			Fire();
			break;

		case MaitaAction::Right:
			m_Velocity.x = 50.f;
			m_Velocity.y = 0.f;
			Move(deltaTime);
			m_CurrAction = MaitaAction::Right;
			m_PrevDirectionAction = MaitaAction::Right;
			m_ActionTimer = 1.0f;
			break;

		case MaitaAction::Left:
			m_Velocity.x = -50.f;
			m_Velocity.y = 0.f;
			Move(deltaTime);
			m_CurrAction = MaitaAction::Left;
			m_PrevDirectionAction = MaitaAction::Left;
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
		case  MaitaAction::Jump:
			Jump(deltaTime);
			break;


		case MaitaAction::Left:
			Move(deltaTime);
			break;


		case MaitaAction::Right:
			Move(deltaTime);
			break;

		case MaitaAction::Bubbled:
			FloatUp(deltaTime);
			break;
		}
		m_ActionTimer -= deltaTime;
		if (m_ActionTimer <= 0)
		{
			m_IsPerformingAction = false;
		}
	}
		
}

void NFE::MaitaComponent::SetBubbled()
{
	m_CurrAction = MaitaAction::Bubbled;
	NFE::TextureComponent* texComp = m_pParent->GetComponent<NFE::TextureComponent>();
	NFE::CollisionComponent* collComp = m_pParent->GetComponent<NFE::CollisionComponent>();
	collComp->SetCollisionType(NFE::CollisionType::TrappedEnemy);
	texComp->ChangeTexture("MaitaBubble.png");
}

void NFE::MaitaComponent::SetUnBubbled()
{
	m_CurrAction = MaitaAction::Right;
	NFE::TextureComponent* texComp = m_pParent->GetComponent<NFE::TextureComponent>();
	NFE::CollisionComponent* collComp = m_pParent->GetComponent<NFE::CollisionComponent>();
	collComp->SetCollisionType(NFE::CollisionType::Enemy);
	texComp->ChangeTexture("Maita.png");
	m_BubbledTimer = 0.f;
}

void NFE::MaitaComponent::Jump(const float& deltaTime)
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

void NFE::MaitaComponent::Fire()
{
	if (m_PrevDirectionAction == MaitaAction::Left)
	{
		dae::float3 enemyPos = m_pParent->GetPosition();
		FireBall* newFireball = new FireBall{ -250.f,enemyPos.x,enemyPos.y };
		m_vFireballs.push_back(newFireball);
	}
	if (m_PrevDirectionAction == MaitaAction::Right)
	{
		dae::float3 enemyPos = m_pParent->GetPosition();
		FireBall* newFireball = new FireBall{ 250.f,enemyPos.x,enemyPos.y };
		m_vFireballs.push_back(newFireball);
	}
}

void NFE::MaitaComponent::Move(const float& deltaTime)
{
	m_IsPerformingAction = true;
	dae::float3 currPos = m_pParent->GetPosition();
	m_pParent->SetPosition(currPos.x += m_Velocity.x * deltaTime, currPos.y);
	m_pParent->SetPosition(currPos.x, currPos.y += 98.1f * deltaTime);
}

void NFE::MaitaComponent::FloatUp(const float& deltaTime)
{
	m_BubbledTimer += deltaTime;
	dae::float3 currPos = m_pParent->GetPosition();
	m_pParent->SetPosition(currPos.x, currPos.y -= 20 * deltaTime);
}
