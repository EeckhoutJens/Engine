#pragma once
#include "BaseComponent.h"
#include "Structs.h"

enum class Action
{
	Jump,
	Left,
	Right,
	Bubbled
};


namespace NFE
{
	class ZenChanComponent final : public BaseComponent
	{
	public:
		ZenChanComponent(dae::GameObject* parent);
		~ZenChanComponent() = default;
		void Update(const float& deltaTime, float, float, float, float) override;
		void Render() override { ; };
		void WriteToFile(BinaryWriter&, const char*) {};
		void SetBubbled();
		void SetUnBubbled();

	private:
		float m_ActionTimer{};
		float2 m_Velocity{ 100.f,100.f };
		Action m_CurrAction{};
		float m_BubbledTimer{};
		const float m_MaxBubbleTimer{5.f};
		bool m_IsPerformingAction{ false };
		bool m_IsOnGround{ false };
		void Jump(const float& deltaTime);
		void Move(const float& deltaTime);
		void FloatUp(const float& deltaTime);
	};
}
