#pragma once
#include <string>
#include <memory>
#include "TypeComponent.h"
namespace dae
{
	class GameObject;
}
class BaseSubject;
class Item
{
public:
	Item(float posX, float posY, std::string fileName,GameObjectType type);
	~Item();
	std::shared_ptr<dae::GameObject> GetItemObject() { return m_pItemObject; }
	void ApplyGravity(float dt);
	BaseSubject* GetSubject() { return m_pSubject; }

private:
	std::shared_ptr<dae::GameObject> m_pItemObject{ nullptr };
	BaseSubject* m_pSubject{};
};

