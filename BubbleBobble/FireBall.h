#pragma once
#include <memory>
namespace dae
{
	class GameObject;
}
class FireBall
{
public:
	FireBall(float velocity, float x, float y);
	std::shared_ptr<dae::GameObject> GetFireBall() { return m_pFireBall; }
	bool IsAddedToScene() { return m_AddedToScene; }
	void SetAddedToScene() { m_AddedToScene = true; }
	void UpdateFireball(float dt);

private:
	std::shared_ptr<dae::GameObject> m_pFireBall{ nullptr };
	bool m_AddedToScene{ false };
	float m_FireballVelocity{};
};

