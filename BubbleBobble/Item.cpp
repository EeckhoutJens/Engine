#include "pch.h"
#include "Item.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "CollisionComponent.h"
#include "BaseSubject.h"
Item::Item(float posX, float posY, std::string fileName, GameObjectType type)
{
	m_pSubject = new BaseSubject();
	m_pItemObject = std::make_shared<dae::GameObject>();
	m_pItemObject->SetPosition(posX, posY);
	NFE::TextureComponent* text = new NFE::TextureComponent{ m_pItemObject.get(),fileName };
	NFE::TypeComponent* typeComp = new NFE::TypeComponent(m_pItemObject.get(), type);
	NFE::CollisionComponent* coll = new NFE::CollisionComponent(m_pItemObject.get());

	text->Update(0, posX, posY, 44, 44);
	coll->SetCollisionType(NFE::CollisionType::Pickup);
	coll->SetCollisionRect(*text->GetTextureData());

	m_pItemObject->AddComponentToVector(text);
	m_pItemObject->AddComponentToVector(typeComp);
	m_pItemObject->AddComponentToVector(coll);
}

Item::~Item()
{
	delete m_pSubject;
	m_pSubject = nullptr;
}

void Item::ApplyGravity(float dt)
{
	dae::float3 currPos = m_pItemObject->GetPosition();
	m_pItemObject->SetPosition(currPos.x, currPos.y += 98.1f * dt);
}
