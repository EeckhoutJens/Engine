#include "pch.h"
#include "PlayerCharacter.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "TypeComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "JumpCommand.h"
#include "LeftCommand.h"
#include "RightCommand.h"
#include "FireCommand.h"
#include "Bubble.h"


PlayerCharacter::PlayerCharacter(int characterID, std::string textureFile, std::vector<NFE::Command*> commands)
{
	m_pPlayer = std::make_shared<dae::GameObject>();
	NFE::TextureComponent* text = new NFE::TextureComponent{ m_pPlayer.get(),textureFile };
	NFE::InputComponent* input = new NFE::InputComponent(m_pPlayer.get(), characterID, ObjectType::Player);
	NFE::TypeComponent* type = new NFE::TypeComponent(m_pPlayer.get(), GameObjectType::Player);
	NFE::CollisionComponent* coll = new NFE::CollisionComponent(m_pPlayer.get());
	text->Update(0, 0, 0, 44, 44);
	coll->SetCollisionType(NFE::CollisionType::Player);
	coll->SetCollisionRect(*text->GetTextureData());
	for (NFE::Command* pCommand : commands)
	{
		input->AssignCommand(pCommand);
	}

	m_pPlayer->AddComponentToVector(text);
	m_pPlayer->AddComponentToVector(type);
	m_pPlayer->AddComponentToVector(input);
	m_pPlayer->AddComponentToVector(coll);
}

PlayerCharacter::~PlayerCharacter()
{
	for (Bubble* pBubble : m_vBubbles)
	{
		delete pBubble;
		pBubble = nullptr;
	}

	m_vBubbles.clear();
}

void PlayerCharacter::PlayerHit()
{
	if (canBeHit)
	{
		canBeHit = false;
	}
}

void PlayerCharacter::UpdateState()
{
	NFE::InputComponent* inputComp = m_pPlayer->GetComponent<NFE::InputComponent>();
	NFE::CollisionComponent* collComp = m_pPlayer->GetComponent<NFE::CollisionComponent>();
	RightCommand* testRC = dynamic_cast<RightCommand*>(inputComp->GetLastTriggeredCommand());
	LeftCommand* testLC = dynamic_cast<LeftCommand*>(inputComp->GetLastTriggeredCommand());
	JumpCommand* testJC = dynamic_cast<JumpCommand*>(inputComp->GetLastTriggeredCommand());
	FireCommand* testFC = dynamic_cast<FireCommand*>(inputComp->GetLastTriggeredCommand());

	if (testRC)
	{
		prevDirectionState = PlayerState::MOVERIGHT;
	}

	if (testLC)
	{
		prevDirectionState = PlayerState::MOVELEFT;
	}

	if (testJC)
	{
			currState = PlayerState::JUMP;
			isJumping = true;
	}

	else if (testFC)
	{
		if (currState != PlayerState::SHOOT)
		{
			currState = PlayerState::SHOOT;
			if (prevDirectionState == PlayerState::MOVELEFT)
			{
				dae::float3 playerPos = m_pPlayer->GetPosition();
				Bubble* newBubble = new Bubble{ -250.f,playerPos.x,playerPos.y };
				m_vBubbles.push_back(newBubble);
			}
			if (prevDirectionState == PlayerState::MOVERIGHT)
			{
				dae::float3 playerPos = m_pPlayer->GetPosition();
				Bubble* newBubble = new Bubble{250.f,playerPos.x,playerPos.y};
				m_vBubbles.push_back(newBubble);
			}
		}
		
	}

	else if (collComp->GetIsOnGround())
	{
		if (testRC)
		{
			currState = PlayerState::MOVERIGHT;
		}
		else if (testLC)
		{
			currState = PlayerState::MOVELEFT;
		}

		else
			currState = PlayerState::IDLE;

		m_JumpTimer = 0;
		isJumping = false;
	}

	else if (m_JumpTimer >= m_MaxJumpTimer)
	{
		currState = PlayerState::FALLING;
		isJumping = false;
	}
		
}

void PlayerCharacter::UpdatePlayer(float dt)
{
	dae::float3 pos = m_pPlayer->GetPosition();
	m_pPlayer->SetPosition(pos.x, pos.y += 98.1f * dt);
	if (pos.y > 550.f)
	{
		m_pPlayer->SetPosition(pos.x, 0);
	}
	if (!canBeHit)
	{
		m_CurrTimer += dt;
		if (m_CurrTimer >= m_MaxInvinciblityTimer)
		{
			canBeHit = true;
			m_CurrTimer = 0;
		}
	}
	UpdateState();
}
