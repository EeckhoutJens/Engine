#pragma once
#include "BaseComponent.h"
#include "SDL.h"
namespace NFE
{
	struct float2
	{
		float x{};
		float y{};
	};
	struct RectPoints
	{
		float2 BL{};
		float2 TL{};
		float2 TR{};
		float2 BR{};
	};
	enum class CollisionType
	{
		Enemy,
		Player,
		Pickup,
		Wall,
		Platform,
		Bubble,
		FireBall,
		TrappedEnemy
	};
	class CollisionComponent final : public BaseComponent
	{
	public:
		CollisionComponent(dae::GameObject* parent);
		~CollisionComponent() = default;
		void Update(const float& deltaTime, float, float, float, float) override;
		void Render() override { ; };
		RectPoints GetCollisionrect() { return m_CollisionRectPoints; }
		void WriteToFile(BinaryWriter&, const char*) {};
		void SetCollisionType(CollisionType type) { m_Type = type; }
		void AddCollComp(CollisionComponent* collisionComp) { m_VecCollComps.push_back(collisionComp); }
		void RemoveCollComp(CollisionComponent* collisionComp)
		{
			for (size_t currColl{}; currColl < m_VecCollComps.size(); ++currColl)
			{
				if (m_VecCollComps[currColl] == collisionComp)
				{
					std::swap(m_VecCollComps[currColl], m_VecCollComps[m_VecCollComps.size() - 1]);
					m_VecCollComps.pop_back();
				}
			}
		}
		bool GetIsOnGround() { return m_OnGround; }
		void SetNotOnGround() { m_OnGround = false; }
		void SetCollisionRect(SDL_Rect colRect);
		bool GetIsColliding() { return m_IsColliding; }
		bool GetIsHitByPlayer() { return m_HitByPlayer; }
		void ResetIsColliding() { m_IsColliding = false; }

	private:
		void CheckCollision();
		RectPoints m_CollisionRectPoints{};
		std::vector<CollisionComponent*> m_VecCollComps{};
		CollisionType m_Type{};
		bool m_OnGround{false};
		bool m_IsColliding{ false };
		bool m_HitByPlayer{ false };
	};
}


