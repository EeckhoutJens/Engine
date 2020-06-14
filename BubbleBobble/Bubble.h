#pragma once
#include <memory>
namespace dae
{
	class GameObject;
}
class Bubble
{
public:
	Bubble(float velocity, float x, float y);
	std::shared_ptr<dae::GameObject> GetBubble() { return m_pBubble; }
	bool IsAddedToScene() { return m_AddedToScene; }
	void SetAddedToScene() { m_AddedToScene = true; }
	void UpdateBubble(float dt);

private:
	std::shared_ptr<dae::GameObject> m_pBubble{nullptr};
	bool m_AddedToScene{false};
	float m_BubbleVelocity{};
};

