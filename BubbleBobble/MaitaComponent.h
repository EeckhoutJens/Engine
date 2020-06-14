#pragma once
#include <BaseComponent.h>
#include "Structs.h"
enum class MaitaAction
{
	Jump,
	Left,
	Right,
	Fire,
	Bubbled
};

class FireBall;
namespace NFE
{
	class MaitaComponent final : public BaseComponent
	{
	public:
		MaitaComponent(dae::GameObject* parent);
		~MaitaComponent();
		void Update(const float& deltaTime, float, float, float, float) override;
		void Render() override { ; };
		void WriteToFile(BinaryWriter&, const char*) {};
		std::vector<FireBall*>* GetFireballs() { return &m_vFireballs; }
		void SetBubbled();
		void SetUnBubbled();

	private:
		float m_ActionTimer{};
		std::vector<FireBall*> m_vFireballs{};
		float2 m_Velocity{ 100.f,100.f };
		MaitaAction m_CurrAction{};
		MaitaAction m_PrevDirectionAction{ MaitaAction::Left };
		bool m_IsPerformingAction{ false };
		float m_BubbledTimer{};
		const float m_MaxBubbleTimer{ 5.f };
		void Jump(const float& deltaTime);
		void Fire();
		void Move(const float& deltaTime);
		void FloatUp(const float& deltaTime);
	};
}


