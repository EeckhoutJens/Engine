#pragma once
#include  "Scene.h"
#include "memory"
#include <string>
#include "BinaryReader.h"
#include "PlayerCharacter.h"
#include <vector>
class Level;
class Item;
class Score;
class KeyboardButton;
class ControllerButton;
namespace dae
{
	class GameObject;
	class TextObject;
	class Font;
}
namespace NFE
{
	class CollisionComponent;
	class Command;
}
enum class SelectedMode
{
	OnePlayer,
	TwoPlayers
};
class BubbleBobbleScene final : public dae::Scene
{
public:

	BubbleBobbleScene(const std::string& name);
	~BubbleBobbleScene();

private:
	Level* m_pLevel{};
	Score* m_pScore{};
	std::vector<NFE::CollisionComponent*> m_vCollCompsToAdd{};
	std::vector<NFE::CollisionComponent*> m_vBlockCollComps{};
	std::vector<std::shared_ptr<dae::GameObject>> m_vMaitas{};
	std::vector<std::shared_ptr<dae::GameObject>> m_vZenChans{};
	std::vector<std::string> m_vLevels
	{
		"Levels/Level1.bin",
		"Levels/Level2.bin",
		"Levels/Level3.bin",
	};
	int m_CurrLevelInd{};
	const int m_MaxNrOfLevels{ 3 };
	std::shared_ptr<dae::GameObject> m_pLogo{};
	std::shared_ptr<dae::GameObject> m_pP1LivesSprite{};
	std::shared_ptr<dae::GameObject> m_pTitleScreen{};
	std::shared_ptr<dae::Font> m_pFont{};
	int m_P1Lives{ 3 };
	int m_P2Lives{ 3 };
	std::shared_ptr<dae::TextObject> m_pP1LiveText{};
	std::shared_ptr<dae::GameObject> m_pP2LivesSprite{};
	std::shared_ptr<dae::TextObject> m_pP2LivesText{};
	std::shared_ptr<dae::TextObject> m_pRetryMessage{};
	std::vector<Item*> m_vItems{};
	PlayerCharacter* m_pPlayer1{};
	PlayerCharacter* m_pPlayer2{};
	bool m_HasChosenMode{ false };
	BinaryReader*  m_pReader{};
	SelectedMode m_CurrMode{};
	float m_currTimer{};
	float m_maxTimer{5.f};
	bool m_LevelEnd{};
	void Initialize() override;
	void InitializeGame();
	void CreateLevel(std::string fileName);
	void CreateLogo();
	void CreateP1LiveHUD();
	void CreateP2LiveHUD();
	void RemoveCurrentLevel();
	void Update(float deltaTime) override;
	void UpdateItems(float dt);
	void CheckItemCollision();
	void CheckPlayerCollision();
	void UpdateBubbles(float dt);
	void UpdatePlayers(float dt);
	void UpdateFireBalls(float dt);
	void CheckEnemyCollision();
	void Reset();
	void SetItemCollision(Item* pItem);
	bool m_GameEnd{false};
	bool UpdateRetryMessage{ false };
	void CreateSingle();
	void CreateCoop();
	void LevelSwitch();
	std::vector<NFE::Command*> SetupP1Input();
	std::vector<NFE::Command*> SetupP2Input();
	bool m_HasCreatedMode{false};
	bool m_IsSwitchingLevels{ false };
	int m_NrOfRemainingEnemies{};
	ControllerButton* m_pControllerRestart{};
	ControllerButton* m_pControllerSelectP1{};
	ControllerButton* m_pControllerSelectP2{};
	KeyboardButton* m_pKeyboardRestart{};
	KeyboardButton* m_pKeyboardSelect1P{};
	KeyboardButton* m_pKeyboardSelect2P{};
};

