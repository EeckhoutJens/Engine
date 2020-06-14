#include "pch.h"
#include "Bubble.h"
#include "TextureComponent.h"
#include "CollisionComponent.h"
#include "TypeComponent.h"
#include "GameObject.h"


Bubble::Bubble(float velocity, float x, float y) : m_BubbleVelocity(velocity)
{
	m_pBubble = std::make_shared<dae::GameObject>();
	m_pBubble->SetPosition(x, y);
	NFE::TextureComponent* texComp = new NFE::TextureComponent(m_pBubble.get(), "Bubble.png");
	NFE::CollisionComponent* collComp = new NFE::CollisionComponent(m_pBubble.get());
	NFE::TypeComponent* typeComp = new NFE::TypeComponent(m_pBubble.get(), GameObjectType::Bubble);
	texComp->Update(0, x, y, 44, 44);
	collComp->SetCollisionType(NFE::CollisionType::Bubble);
	collComp->SetCollisionRect(*texComp->GetTextureData());

	m_pBubble->AddComponentToVector(texComp);
	m_pBubble->AddComponentToVector(typeComp);
	m_pBubble->AddComponentToVector(collComp);
}

void Bubble::UpdateBubble(float dt)
{
	dae::float3 currPosition = m_pBubble->GetPosition();
	m_pBubble->SetPosition(currPosition.x += m_BubbleVelocity * dt, currPosition.y);
}
