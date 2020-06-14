#include "pch.h"
#include "BubbleBobbleScene.h"
#include "Level.h"
#include "CollisionComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "TypeComponent.h"
#include "StateComponent.h"
#include "ControllerButton.h"
#include "KeyboardButton.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Commands.h"
#include "JumpCommand.h"
#include "LeftCommand.h"
#include "RightCommand.h"
#include "FireCommand.h"
#include "ZenChanComponent.h"
#include "MaitaComponent.h"
#include "Bubble.h"
#include "FireBall.h"
#include "Item.h"
#include "BaseSubject.h"
#include "ScoreObserver.h"
#include "Score.h"
#include "TextObject.h"
#include "Font.h"
#include "Audio.h"
#include "Locator.h"

BubbleBobbleScene::BubbleBobbleScene(const std::string& name) : Scene(name)
{
}

BubbleBobbleScene::~BubbleBobbleScene()
{

	//RemoveCurrentLevel();

	delete m_pLevel;
	m_pLevel = nullptr;

	delete m_pPlayer1;
	m_pPlayer1 = nullptr;

	delete m_pScore;
	m_pScore = nullptr;

	delete m_pControllerRestart;
	m_pControllerRestart = nullptr;

	delete m_pKeyboardRestart;
	m_pKeyboardRestart = nullptr;

	delete m_pKeyboardSelect1P;
	m_pKeyboardSelect1P = nullptr;

	delete m_pControllerSelectP1;
	m_pControllerSelectP1 = nullptr;

	delete m_pControllerSelectP2;
	m_pControllerSelectP2 = nullptr;

	delete m_pKeyboardSelect2P;
	m_pKeyboardSelect2P = nullptr;

	for (Item* pItem : m_vItems)
	{
		Remove(pItem->GetItemObject());
		delete pItem;
		pItem = nullptr;
	}

	delete m_pPlayer2;
	m_pPlayer2 = nullptr;
}

void BubbleBobbleScene::Initialize()
{
	m_pKeyboardSelect1P = new KeyboardButton(VK_NUMPAD1);
	m_pKeyboardSelect2P = new KeyboardButton(VK_NUMPAD2);
	m_pControllerSelectP1 = new ControllerButton(XINPUT_GAMEPAD_A);
	m_pControllerSelectP2 = new ControllerButton(XINPUT_GAMEPAD_B);
	m_pTitleScreen = std::make_shared<dae::GameObject>();
	NFE::TextureComponent* pTexture = new NFE::TextureComponent(m_pTitleScreen.get(), "TitleScreen.png");
	pTexture->Update(0, 375, 275, 375 * 2, 275 * 2);
	m_pTitleScreen->AddComponentToVector(pTexture);
	Add(m_pTitleScreen);
}

void BubbleBobbleScene::InitializeGame()
{
	m_pScore = new Score();
	m_pFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	m_pRetryMessage = std::make_shared<dae::TextObject>("PRESS R TO RESTART", m_pFont);
	m_pRetryMessage->SetPosition(125.f, 275.f);
	m_pKeyboardRestart = new KeyboardButton('R');
	m_pControllerRestart = new ControllerButton(XINPUT_GAMEPAD_START);
	CreateLogo();
	CreateP1LiveHUD();
	if (m_CurrMode != SelectedMode::OnePlayer)
	{
		CreateP2LiveHUD();
	}
	Add(m_pScore->GetScore());
	CreateLevel(m_vLevels[m_CurrLevelInd]);
}

void BubbleBobbleScene::CreateLevel(std::string fileName)
{
	m_pReader = new BinaryReader();
	m_pLevel = new Level(*m_pReader,fileName.c_str());
	delete m_pReader;
	m_pReader = nullptr;
	auto vecBlocks = m_pLevel->GetGrid()->GetGameObjects();
	auto vecEnemies = m_pLevel->GetEnemies();

	for (std::shared_ptr<dae::GameObject> pObject : vecEnemies)
	{
		if (pObject->GetComponent<NFE::MaitaComponent>())
		{
			m_vMaitas.push_back(pObject);
		}
		if (pObject->GetComponent<NFE::ZenChanComponent>())
		{
			m_vZenChans.push_back(pObject);
		}
	}

	m_NrOfRemainingEnemies = int(vecEnemies.size());

	switch (m_CurrMode)
	{
	case SelectedMode::OnePlayer:
		CreateSingle();

		for (size_t currZenChan{}; currZenChan < m_vZenChans.size(); ++currZenChan)
		{

			NFE::CollisionComponent* collComp = m_vZenChans[currZenChan]->GetComponent<NFE::CollisionComponent>();
			if (collComp)
			{
				m_vCollCompsToAdd.push_back(collComp);
				collComp->AddCollComp(m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>());
			}
		}

		for (size_t currMaita{}; currMaita < m_vMaitas.size(); ++currMaita)
		{
			NFE::CollisionComponent* collComp = m_vMaitas[currMaita]->GetComponent<NFE::CollisionComponent>();
			if (collComp)
			{
				m_vCollCompsToAdd.push_back(collComp);
				collComp->AddCollComp(m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>());
			}
		}

		for (size_t currBlock{}; currBlock < vecBlocks.size(); ++currBlock)
		{
			NFE::CollisionComponent* collComp = vecBlocks[currBlock]->GetComponent<NFE::CollisionComponent>();
			if (collComp)
			{
				for (NFE::CollisionComponent* pComp : m_vCollCompsToAdd)
				{
					collComp->AddCollComp(pComp);
				}
				collComp->AddCollComp(m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>());
				m_vBlockCollComps.push_back(collComp);
			}
		}
		break;
	case SelectedMode::TwoPlayers:
		CreateCoop();
		for (size_t currZenChan{}; currZenChan < m_vZenChans.size(); ++currZenChan)
		{

			NFE::CollisionComponent* collComp = m_vZenChans[currZenChan]->GetComponent<NFE::CollisionComponent>();
			if (collComp)
			{
				m_vCollCompsToAdd.push_back(collComp);
				if (m_pPlayer1)
				{
					collComp->AddCollComp(m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>());
				}
				if (m_pPlayer2)
				{
					collComp->AddCollComp(m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>());
				}
			}
		}

		for (size_t currMaita{}; currMaita < m_vMaitas.size(); ++currMaita)
		{
			NFE::CollisionComponent* collComp = m_vMaitas[currMaita]->GetComponent<NFE::CollisionComponent>();
			if (collComp)
			{
				m_vCollCompsToAdd.push_back(collComp);
				if (m_pPlayer1)
				{
					collComp->AddCollComp(m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>());
				}
				if (m_pPlayer2)
				{
					collComp->AddCollComp(m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>());
				}
			}
		}

		for (size_t currBlock{}; currBlock < vecBlocks.size(); ++currBlock)
		{
			NFE::CollisionComponent* collComp = vecBlocks[currBlock]->GetComponent<NFE::CollisionComponent>();
			if (collComp)
			{
				for (NFE::CollisionComponent* pComp : m_vCollCompsToAdd)
				{
					collComp->AddCollComp(pComp);
				}
				if (m_pPlayer1)
				{
					collComp->AddCollComp(m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>());
				}
				if (m_pPlayer2)
				{
					collComp->AddCollComp(m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>());
				}
				m_vBlockCollComps.push_back(collComp);
			}
		}
		break;
	default:
		break;
	}

	

	for (size_t currBlock{}; currBlock < vecBlocks.size(); ++currBlock)
	{
		Add(vecBlocks[currBlock]);
	}

	for (size_t currZenChan{}; currZenChan < m_vZenChans.size(); ++currZenChan)
	{

		Add(m_vZenChans[currZenChan]);
	}

	for (size_t currMaita{}; currMaita < m_vMaitas.size(); ++currMaita)
	{

		Add(m_vMaitas[currMaita]);
	}

	if (m_pPlayer1)
	{
		Add(m_pPlayer1->getObject());
	}
	if (m_CurrMode != SelectedMode::OnePlayer)
	{
		if (m_pPlayer2)
		{
			Add(m_pPlayer2->getObject());
		}
		
	}
}

void BubbleBobbleScene::CreateLogo()
{
	m_pLogo = std::make_shared<dae::GameObject>();
	m_pLogo->SetPosition(550, 10);
	NFE::TextureComponent* pTextureComp = new NFE::TextureComponent(m_pLogo.get(), "Logo.png");
	m_pLogo->AddComponentToVector(pTextureComp);
	Add(m_pLogo);
}

void BubbleBobbleScene::CreateP1LiveHUD()
{
	m_pP1LivesSprite = std::make_shared<dae::GameObject>();
	m_pP1LivesSprite->SetPosition(580, 200);
	NFE::TextureComponent* pTextureComp = new NFE::TextureComponent(m_pP1LivesSprite.get(), "Player1Sprite.png");
	pTextureComp->Update(0, 0, 0, 55, 55);
	m_pP1LivesSprite->AddComponentToVector(pTextureComp);
	Add(m_pP1LivesSprite);

	m_pP1LiveText = std::make_shared<dae::TextObject>(std::to_string(m_P1Lives), m_pFont);
	m_pP1LiveText->SetPosition(650, 225);
	Add(m_pP1LiveText);
}

void BubbleBobbleScene::CreateP2LiveHUD()
{
	m_pP2LivesSprite = std::make_shared<dae::GameObject>();
	m_pP2LivesSprite->SetPosition(580, 400);
	NFE::TextureComponent* pTextureComp;
	if (m_CurrMode == SelectedMode::TwoPlayers)
	{
		 pTextureComp = new NFE::TextureComponent(m_pP2LivesSprite.get(), "Player2Sprite.png");
	}
	else
	{
		pTextureComp = new NFE::TextureComponent(m_pP2LivesSprite.get(), "Maita.png");
	}

	pTextureComp->Update(0, 0, 0, 55, 55);
	m_pP2LivesSprite->AddComponentToVector(pTextureComp);
	Add(m_pP2LivesSprite);

	m_pP2LivesText = std::make_shared<dae::TextObject>(std::to_string(m_P2Lives), m_pFont);
	m_pP2LivesText->SetPosition(650, 422);
	Add(m_pP2LivesText);
}

void BubbleBobbleScene::RemoveCurrentLevel()
{

	auto vecBlocks = m_pLevel->GetGrid()->GetGameObjects();

	for (std::shared_ptr<dae::GameObject> pObject : vecBlocks)
	{
		Remove(pObject);
		pObject = nullptr;
	
	}
	
	for (Item* pItem : m_vItems)
	{
		delete pItem;
		pItem = nullptr;
	}

	for (std::shared_ptr<dae::GameObject> pMaita : m_vMaitas)
	{
		if (pMaita)
		{
			Remove(pMaita);
			pMaita = nullptr;
		}
	}

	for (std::shared_ptr<dae::GameObject> pZenchan : m_vZenChans)
	{
		if (pZenchan)
		{
			Remove(pZenchan);
			pZenchan = nullptr;
		}
	}

	if (m_pPlayer1 != nullptr)
	{
		Remove(m_pPlayer1->getObject());
		delete m_pPlayer1;
		m_pPlayer1 = nullptr;
	}


	if (m_CurrMode != SelectedMode::OnePlayer)
	{
		if (m_pPlayer2 != nullptr)
		{
			Remove(m_pPlayer2->getObject());
			delete m_pPlayer2;
			m_pPlayer2 = nullptr;
		}
	}

	delete m_pLevel;
	m_pLevel = nullptr;
}

void BubbleBobbleScene::Update(float deltaTime)
{
	if (!m_HasChosenMode)
	{
		m_pTitleScreen->Update(deltaTime);
		if(dae::InputManager::GetInstance().IsPressed(m_pKeyboardSelect1P)
			|| dae::InputManager::GetInstance().IsPressed(m_pControllerSelectP1))
		{
			m_HasChosenMode = true;
			m_CurrMode = SelectedMode::OnePlayer;
			Remove(m_pTitleScreen);
			InitializeGame();
		}
		else if (dae::InputManager::GetInstance().IsPressed(m_pKeyboardSelect2P)
				|| dae::InputManager::GetInstance().IsPressed(m_pControllerSelectP2))
		{
			m_HasChosenMode = true;
			m_CurrMode = SelectedMode::TwoPlayers;
			InitializeGame();
			Remove(m_pTitleScreen);
		}
	}
	if (!m_IsSwitchingLevels && !m_GameEnd && m_HasChosenMode)
	{
		for (auto gameObject : mObjects)
		{
			gameObject->Update(deltaTime);
		}
		
		UpdatePlayers(deltaTime);
		UpdateBubbles(deltaTime);
		UpdateFireBalls(deltaTime);
		UpdateItems(deltaTime);
		m_pScore->Update();
		CheckEnemyCollision();
		CheckPlayerCollision();
		if (m_LevelEnd)
		{
			m_currTimer += deltaTime;
			if (m_currTimer >= m_maxTimer)
			{
				m_currTimer = 0;
				m_LevelEnd = false;
				LevelSwitch();
			}
		}
	}
	if (m_GameEnd)
	{
		if (!UpdateRetryMessage)
		{
			m_pRetryMessage->Update(deltaTime);
			UpdateRetryMessage = true;
		}

		if (dae::InputManager::GetInstance().IsPressed(m_pKeyboardRestart)
			|| dae::InputManager::GetInstance().IsPressed(m_pControllerRestart))
		{
			Reset();
		}
	}
}

void BubbleBobbleScene::UpdateItems(float dt)
{
	CheckItemCollision();
	for (Item* pItem : m_vItems)
	{
		pItem->ApplyGravity(dt);
	}
}

void BubbleBobbleScene::CheckItemCollision()
{
	for (size_t currItem{}; currItem < m_vItems.size(); ++currItem)
	{
		if (m_vItems[currItem]->GetItemObject()->GetComponent<NFE::CollisionComponent>()->GetIsColliding())
		{
			m_vItems[currItem]->GetSubject()->Notify(m_vItems[currItem]->GetItemObject().get(), Events::Event_AddScore);
			std::swap(m_vItems[currItem], m_vItems[m_vItems.size() - 1]);
			Remove(m_vItems[m_vItems.size() - 1]->GetItemObject());
			delete m_vItems[m_vItems.size() - 1];
			m_vItems[m_vItems.size() - 1] = nullptr;
			m_vItems.pop_back();
		}
	}
}

void BubbleBobbleScene::CheckPlayerCollision()
{


	if (m_pPlayer1)
	{
		if (m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>()->GetIsColliding())
		{
			if (m_pPlayer1->CanBeHit())
			{
				m_pPlayer1->PlayerHit();
				--m_P1Lives;
				m_pP1LiveText->SetText(std::to_string(m_P1Lives));
				m_pPlayer1->getObject()->SetPosition(0, 400);
			}
			m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>()->ResetIsColliding();

		}
	}

	if (m_CurrMode == SelectedMode::OnePlayer)
	{
		if (m_P1Lives < 0)
		{
			m_GameEnd = true;
			Add(m_pRetryMessage);
		}
	}
	else
	{
		if (m_P1Lives < 0 && m_pPlayer1)
		{
			std::vector<Bubble*>* bubbles = m_pPlayer1->GetBubbles();
			for (Bubble* pBubble : *bubbles)
			{
				Remove(pBubble->GetBubble());
				delete pBubble;
				pBubble = nullptr;
			}
			Remove(m_pPlayer1->getObject());
			if (m_pPlayer1)
			{
				delete m_pPlayer1;
				m_pPlayer1 = nullptr;
			}
		}
	}


	if (m_CurrMode == SelectedMode::TwoPlayers)
	{
		if (m_pPlayer2)
		{
			if (m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>()->GetIsColliding())
			{
				if (m_pPlayer2->CanBeHit())
				{
					m_pPlayer2->PlayerHit();
					--m_P2Lives;
					m_pP2LivesText->SetText(std::to_string(m_P2Lives));
					m_pPlayer2->getObject()->SetPosition(400, 400);
					if (m_P2Lives < 0)
					{
						std::vector<Bubble*>* bubbles = m_pPlayer2->GetBubbles();
						for (Bubble* pBubble : *bubbles)
						{
							Remove(pBubble->GetBubble());
							delete pBubble;
							pBubble = nullptr;
						}
						Remove(m_pPlayer2->getObject());
						delete m_pPlayer2;
						m_pPlayer2 = nullptr;
					}
				}

			}
			if (m_pPlayer2)
			{
				m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>()->ResetIsColliding();
			}
				
		}
		if (m_P2Lives < 0 && m_P1Lives < 0)
		{
			m_GameEnd = true;
			Add(m_pRetryMessage);
		}
	}


}

void BubbleBobbleScene::UpdateBubbles(float dt)
{
	if (m_pPlayer1)
	{
		std::vector<Bubble*>* bubbles = m_pPlayer1->GetBubbles();
		for (int currBubble{}; currBubble < int(bubbles->size()); ++currBubble)
		{
			if (!(*bubbles)[currBubble]->IsAddedToScene())
			{
				Add((*bubbles)[currBubble]->GetBubble());
				(*bubbles)[currBubble]->SetAddedToScene();
				for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
				{
					pColl->AddCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
				}
				for (NFE::CollisionComponent* pColl : m_vCollCompsToAdd)
				{
					pColl->AddCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
				}
			}
			(*bubbles)[currBubble]->UpdateBubble(dt);

			if ((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>()->GetIsColliding())
			{
				for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
				{
					pColl->RemoveCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
				}
				for (NFE::CollisionComponent* pColl : m_vCollCompsToAdd)
				{
					pColl->RemoveCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
				}
				std::swap((*bubbles)[currBubble], (*bubbles)[bubbles->size() - 1]);
				Remove((*bubbles)[bubbles->size() - 1]->GetBubble());
				delete (*bubbles)[bubbles->size() - 1];
				(*bubbles)[bubbles->size() - 1] = nullptr;
				bubbles->pop_back();
			}
		}
	}
	

	if (m_CurrMode == SelectedMode::TwoPlayers)
	{
		if (m_pPlayer2)
		{
			std::vector<Bubble*>* bubbles = m_pPlayer2->GetBubbles();
			for (int currBubble{}; currBubble < int(bubbles->size()); ++currBubble)
			{
				if (!(*bubbles)[currBubble]->IsAddedToScene())
				{
					Add((*bubbles)[currBubble]->GetBubble());
					(*bubbles)[currBubble]->SetAddedToScene();
					for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
					{
						pColl->AddCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
					}
					for (NFE::CollisionComponent* pColl : m_vCollCompsToAdd)
					{
						pColl->AddCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
					}
				}
				(*bubbles)[currBubble]->UpdateBubble(dt);

				if ((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>()->GetIsColliding())
				{
					for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
					{
						pColl->RemoveCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
					}
					for (NFE::CollisionComponent* pColl : m_vCollCompsToAdd)
					{
						pColl->RemoveCollComp((*bubbles)[currBubble]->GetBubble()->GetComponent<NFE::CollisionComponent>());
					}
					std::swap((*bubbles)[currBubble], (*bubbles)[bubbles->size() - 1]);
					Remove((*bubbles)[bubbles->size() - 1]->GetBubble());
					delete (*bubbles)[bubbles->size() - 1];
					(*bubbles)[bubbles->size() - 1] = nullptr;
					bubbles->pop_back();
				}
			}
		}
		
	}
	
}

void BubbleBobbleScene::UpdatePlayers(float dt)
{
	if (m_pPlayer1)
	{
		m_pPlayer1->UpdatePlayer(dt);
		if (m_pPlayer1->GetJumping())
		{
			dae::float3 currPos = m_pPlayer1->getObject()->GetPosition();
			m_pPlayer1->GetJumpTimer() += dt;
			if (m_pPlayer1->GetJumpTimer() <= m_pPlayer1->GetMaxJump())
			{
				m_pPlayer1->getObject()->SetPosition(currPos.x, currPos.y -= 500.f * dt);
			}
		}
	}


	if (m_CurrMode != SelectedMode::OnePlayer)
	{
		if (m_pPlayer2)
		{
			m_pPlayer2->UpdatePlayer(dt);
			if (m_pPlayer2->GetJumping())
			{
				dae::float3 currPos = m_pPlayer2->getObject()->GetPosition();
				m_pPlayer2->GetJumpTimer() += dt;
				if (m_pPlayer2->GetJumpTimer() <= m_pPlayer2->GetMaxJump())
				{
					m_pPlayer2->getObject()->SetPosition(currPos.x, currPos.y -= 500.f * dt);
				}
			}
		}

	}
}

void BubbleBobbleScene::UpdateFireBalls(float dt)
{
	for (std::shared_ptr<dae::GameObject> pMaita : m_vMaitas)
	{
		if (pMaita)
		{
			std::vector<FireBall*>* fireBalls = pMaita->GetComponent<NFE::MaitaComponent>()->GetFireballs();
			for (int currFireBall{}; currFireBall < int(fireBalls->size()); ++currFireBall)
			{
				if (!(*fireBalls)[currFireBall]->IsAddedToScene())
				{
					Add((*fireBalls)[currFireBall]->GetFireBall());
					(*fireBalls)[currFireBall]->SetAddedToScene();
					for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
					{
						pColl->AddCollComp((*fireBalls)[currFireBall]->GetFireBall()->GetComponent<NFE::CollisionComponent>());
					}

					if (m_pPlayer1)
					{
						m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>()->AddCollComp((*fireBalls)[currFireBall]->GetFireBall()->GetComponent<NFE::CollisionComponent>());
					}
					if (m_CurrMode == SelectedMode::TwoPlayers)
					{
						if (m_pPlayer2)
						{
							m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>()->AddCollComp((*fireBalls)[currFireBall]->GetFireBall()->GetComponent<NFE::CollisionComponent>());
						}
					}
				}

				(*fireBalls)[currFireBall]->UpdateFireball(dt);

				if ((*fireBalls)[currFireBall]->GetFireBall()->GetComponent<NFE::CollisionComponent>()->GetIsColliding())
				{
					for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
					{
						pColl->RemoveCollComp((*fireBalls)[currFireBall]->GetFireBall()->GetComponent<NFE::CollisionComponent>());
					}
					if (m_pPlayer1)
					{
						m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>()->RemoveCollComp((*fireBalls)[currFireBall]->GetFireBall()->GetComponent<NFE::CollisionComponent>());
					}
					if (m_CurrMode == SelectedMode::TwoPlayers)
					{
						if (m_pPlayer2)
						{
							m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>()->RemoveCollComp((*fireBalls)[currFireBall]->GetFireBall()->GetComponent<NFE::CollisionComponent>());
						}
					}
					std::swap((*fireBalls)[currFireBall], (*fireBalls)[fireBalls->size() - 1]);
					Remove((*fireBalls)[fireBalls->size() - 1]->GetFireBall());
					delete (*fireBalls)[fireBalls->size() - 1];
					(*fireBalls)[fireBalls->size() - 1] = nullptr;
					fireBalls->pop_back();
				}
			}
		}
		

	}
}

void BubbleBobbleScene::CheckEnemyCollision()
{
	for (size_t currColl{}; currColl < m_vCollCompsToAdd.size(); ++currColl)
	{
		if (m_vCollCompsToAdd[currColl]->GetIsColliding())
		{
			NFE::ZenChanComponent* ZCComp = m_vCollCompsToAdd[currColl]->getParent()->GetComponent<NFE::ZenChanComponent>();
			NFE::MaitaComponent* MComp = m_vCollCompsToAdd[currColl]->getParent()->GetComponent<NFE::MaitaComponent>();
			if (ZCComp)
			{
				ZCComp->SetBubbled();
				m_vCollCompsToAdd[currColl]->ResetIsColliding();
			}
			if (MComp)
			{
				MComp->SetBubbled();
				m_vCollCompsToAdd[currColl]->ResetIsColliding();
			}
		}

		if (m_vCollCompsToAdd[currColl]->GetIsHitByPlayer())
		{
			std::swap(m_vCollCompsToAdd[currColl], m_vCollCompsToAdd[m_vCollCompsToAdd.size() - 1]);

			for (std::shared_ptr<dae::GameObject> pZenChan : m_vZenChans)
			{
				if (pZenChan)
				{
					if (pZenChan->GetComponent<NFE::CollisionComponent>() == m_vCollCompsToAdd[m_vCollCompsToAdd.size() - 1])
					{
						dae::float3 enemyPos = pZenChan->GetPosition();
						Item* itemToSpawn{};
						if (pZenChan->GetComponent<NFE::ZenChanComponent>())
						{
							itemToSpawn = new Item{ enemyPos.x,enemyPos.y,"WaterMelon.png",GameObjectType::Watermelon };
							for (size_t currZenChan{}; currZenChan < m_vZenChans.size(); ++currZenChan)
							{
								if (m_vZenChans[currZenChan] == pZenChan)
								{
									itemToSpawn->GetSubject()->AddObserver(m_pScore->GetObserver());
									SetItemCollision(itemToSpawn);
									Add(itemToSpawn->GetItemObject());
									m_vItems.push_back(itemToSpawn);
									for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
									{
										pColl->RemoveCollComp(pZenChan->GetComponent<NFE::CollisionComponent>());
									}
									std::swap(m_vZenChans[currZenChan], m_vZenChans[m_vZenChans.size() - 1]);
									Remove(m_vZenChans[m_vZenChans.size() - 1]);
									m_vZenChans[m_vZenChans.size() - 1] = nullptr;
									m_vZenChans.pop_back();
									--m_NrOfRemainingEnemies;
									break;
								}
							}
						}
					}
				}
				
			}

			for (std::shared_ptr<dae::GameObject> pMaita : m_vMaitas)
			{
				if (pMaita)
				{
					if (pMaita->GetComponent<NFE::CollisionComponent>() == m_vCollCompsToAdd[m_vCollCompsToAdd.size() - 1])
					{
						dae::float3 enemyPos = pMaita->GetPosition();
						Item* itemToSpawn{};
						if (pMaita->GetComponent<NFE::MaitaComponent>())
						{
							itemToSpawn = new Item{ enemyPos.x,enemyPos.y,"Fries.png",GameObjectType::Fries };
							for (size_t currMaita{}; currMaita < m_vMaitas.size(); ++currMaita)
							{
								if (m_vMaitas[currMaita] == pMaita)
								{
									auto fireballs = pMaita->GetComponent<NFE::MaitaComponent>()->GetFireballs();
									for (FireBall* pFireball : *fireballs)
									{
										for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
										{
											pColl->RemoveCollComp(pFireball->GetFireBall()->GetComponent<NFE::CollisionComponent>());
										}
										Remove(pFireball->GetFireBall());
									}
									itemToSpawn->GetSubject()->AddObserver(m_pScore->GetObserver());
									SetItemCollision(itemToSpawn);
									Add(itemToSpawn->GetItemObject());
									m_vItems.push_back(itemToSpawn);
									for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
									{
										pColl->RemoveCollComp(pMaita->GetComponent<NFE::CollisionComponent>());
									}
									std::swap(m_vMaitas[currMaita], m_vMaitas[m_vMaitas.size() - 1]);
									Remove(m_vMaitas[m_vMaitas.size() - 1]);
									m_vMaitas[m_vMaitas.size() - 1] = nullptr;
									m_vMaitas.pop_back();
									--m_NrOfRemainingEnemies;
									break;
								}
							}
						}
					}
				}
				
			}
			m_vCollCompsToAdd.pop_back();
		}

		if (m_NrOfRemainingEnemies == 0)
		{
			m_LevelEnd = true;
		}
	}

}

void BubbleBobbleScene::Reset()
{
	m_GameEnd = false;
	m_pScore->Reset();
	m_P1Lives = 3;
	m_pP1LiveText->SetText(std::to_string(m_P1Lives));
	if (m_CurrMode != SelectedMode::OnePlayer)
	{
		m_P2Lives = 3;
		m_pP2LivesText->SetText(std::to_string(m_P2Lives));
	}
	UpdateRetryMessage = false;
	Remove(m_pRetryMessage);
	m_CurrLevelInd = m_MaxNrOfLevels;
	LevelSwitch();
}

void BubbleBobbleScene::SetItemCollision(Item* pItem)
{
	for (NFE::CollisionComponent* pColl : m_vBlockCollComps)
	{
		pColl->AddCollComp(pItem->GetItemObject()->GetComponent<NFE::CollisionComponent>());
	}
	if (m_pPlayer1)
	{
		pItem->GetItemObject()->GetComponent<NFE::CollisionComponent>()->AddCollComp(m_pPlayer1->getObject()->GetComponent<NFE::CollisionComponent>());
	}
	if (m_CurrMode == SelectedMode::TwoPlayers)
	{
		if (m_pPlayer2)
		{
			pItem->GetItemObject()->GetComponent<NFE::CollisionComponent>()->AddCollComp(m_pPlayer2->getObject()->GetComponent<NFE::CollisionComponent>());
		}
	}
}

void BubbleBobbleScene::CreateSingle()
{
	std::vector<NFE::Command*> commands = SetupP1Input();

	m_pPlayer1 = new PlayerCharacter(0,"Player1Sprite.png",commands);
	m_pPlayer1->getObject()->SetPosition(0, 400);
}

void BubbleBobbleScene::CreateCoop()
{
	if (m_P1Lives >= 0)
	{
		std::vector<NFE::Command*> commands = SetupP1Input();
		m_pPlayer1 = new PlayerCharacter(0, "Player1Sprite.png", commands);
		m_pPlayer1->getObject()->SetPosition(0, 400);
	}

	if (m_P2Lives >= 0)
	{
		std::vector<NFE::Command*> commands2 = SetupP2Input();
		m_pPlayer2 = new PlayerCharacter(1, "Player2Sprite.png", commands2);
		m_pPlayer2->getObject()->SetPosition(400, 400);
	}
}

void BubbleBobbleScene::LevelSwitch()
{
	m_IsSwitchingLevels = true;
	RemoveCurrentLevel();
	m_vBlockCollComps.clear();
	m_vCollCompsToAdd.clear();
	m_vItems.clear();
	m_vMaitas.clear();
	m_vZenChans.clear();
	++m_CurrLevelInd;
	if (m_CurrLevelInd >= m_MaxNrOfLevels)
	{
		m_CurrLevelInd = 0;
	}
	CreateLevel(m_vLevels[m_CurrLevelInd]);
	m_IsSwitchingLevels = false;
}

std::vector<NFE::Command*> BubbleBobbleScene::SetupP1Input()
{
	ControllerButton* DpadLeft{ new ControllerButton{XINPUT_GAMEPAD_DPAD_LEFT } };
	ControllerButton* DpadRight{ new ControllerButton{XINPUT_GAMEPAD_DPAD_RIGHT} };
	ControllerButton* ButtonA{ new ControllerButton{XINPUT_GAMEPAD_A } };
	ControllerButton* ButtonB{ new ControllerButton{XINPUT_GAMEPAD_B} };

	KeyboardButton* LeftA{ new KeyboardButton{'A'} };
	KeyboardButton* RightD{ new KeyboardButton{'D'} };
	KeyboardButton* JumpSpacce{ new KeyboardButton{VK_SPACE} };
	KeyboardButton* FireQ{ new KeyboardButton{'Q'} };

	NFE::Command* cmdLeft = new LeftCommand();
	NFE::Command* cmdRight = new RightCommand();
	NFE::Command* cmdJump = new JumpCommand();
	NFE::Command* cmdFire = new FireCommand();

	cmdLeft->AssignButton(DpadLeft);
	cmdLeft->AssignButton(LeftA);

	cmdRight->AssignButton(DpadRight);
	cmdRight->AssignButton(RightD);

	cmdJump->AssignButton(ButtonA);
	cmdJump->AssignButton(JumpSpacce);

	cmdFire->AssignButton(ButtonB);
	cmdFire->AssignButton(FireQ);

	std::vector<NFE::Command*> P1Commands{};
	P1Commands.push_back(cmdLeft);
	P1Commands.push_back(cmdRight);
	P1Commands.push_back(cmdJump);
	P1Commands.push_back(cmdFire);

	return P1Commands;
}

std::vector<NFE::Command*> BubbleBobbleScene::SetupP2Input()
{
	ControllerButton* DpadLeft{ new ControllerButton{XINPUT_GAMEPAD_DPAD_LEFT } };
	ControllerButton* DpadRight{ new ControllerButton{XINPUT_GAMEPAD_DPAD_RIGHT} };
	ControllerButton* ButtonA{ new ControllerButton{XINPUT_GAMEPAD_A } };
	ControllerButton* ButtonB{ new ControllerButton{XINPUT_GAMEPAD_B} };

	KeyboardButton* LeftArrow{ new KeyboardButton{VK_LEFT} };
	KeyboardButton* RightArrow{ new KeyboardButton{VK_RIGHT} };
	KeyboardButton* UpArrow{ new KeyboardButton{VK_UP} };
	KeyboardButton* RightCtrl{ new KeyboardButton{VK_CONTROL} };

	NFE::Command* cmdLeft = new LeftCommand();
	NFE::Command* cmdRight = new RightCommand();
	NFE::Command* cmdJump = new JumpCommand();
	NFE::Command* cmdFire = new FireCommand();

	cmdLeft->AssignButton(DpadLeft);
	cmdLeft->AssignButton(LeftArrow);

	cmdRight->AssignButton(DpadRight);
	cmdRight->AssignButton(RightArrow);

	cmdJump->AssignButton(ButtonA);
	cmdJump->AssignButton(UpArrow);

	cmdFire->AssignButton(RightCtrl);
	cmdFire->AssignButton(ButtonB);

	std::vector<NFE::Command*> P2Commands{};
	P2Commands.push_back(cmdLeft);
	P2Commands.push_back(cmdRight);
	P2Commands.push_back(cmdJump);
	P2Commands.push_back(cmdFire);

	return P2Commands;
}
