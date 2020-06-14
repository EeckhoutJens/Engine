#pragma once
#include "BinaryWriter.h"
namespace dae 
{
	class GameObject;
}

namespace NFE
{
	class BaseComponent
{
public:
	BaseComponent(dae::GameObject* parent);
	virtual ~BaseComponent() = default;
	virtual void Update(const float& deltaTime, float x, float y, float width = 0, float height = 0) = 0;
	virtual void Render() = 0;
	virtual void WriteToFile(BinaryWriter& writer, const char* filename) = 0;
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(const BaseComponent&& other) = delete;
	dae::GameObject* getParent() const;

protected:
	dae::GameObject* m_pParent;
};
}


