#pragma once
#include <memory>
namespace dae
{
	class GameObject;
}
namespace NFE
{
	class Command;
}
class Bubble;
class PlayerCharacter
{
public:
	enum class PlayerState
	{
		IDLE,
		MOVELEFT,
		MOVERIGHT,
		JUMP,
		SHOOT,
		FALLING
	};
	PlayerCharacter(int characterID, std::string textureFile, std::vector<NFE::Command*> commands);
	~PlayerCharacter();
	std::shared_ptr<dae::GameObject> getObject() { return m_pPlayer; };
	void UpdatePlayer(float dt);
	float& GetJumpTimer() { return m_JumpTimer; }
	const float GetMaxJump() { return m_MaxJumpTimer; }
	PlayerState GetState() { return currState; };
	std::vector<Bubble*>* GetBubbles() { return &m_vBubbles; }
	void PlayerHit();
	bool CanBeHit() { return canBeHit; }
	bool GetJumping() { return isJumping; }

private:
	void UpdateState();
	std::shared_ptr<dae::GameObject> m_pPlayer{ nullptr };
	PlayerState currState{ PlayerState::IDLE };
	PlayerState prevDirectionState{PlayerState::MOVERIGHT};
	std::vector<Bubble*> m_vBubbles{};
	float m_JumpTimer{};
	const float m_MaxJumpTimer{0.3f};
	const float m_MaxInvinciblityTimer{ 3.f };
	float m_CurrTimer{};
	bool isJumping{};
	bool canBeHit{ true };
};

