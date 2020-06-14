#include "pch.h"
#include "FireBall.h"
#include "TextureComponent.h"
#include "CollisionComponent.h"
#include "TypeComponent.h"
#include "GameObject.h"

FireBall::FireBall(float velocity, float x, float y) : m_FireballVelocity(velocity)
{
	m_pFireBall = std::make_shared<dae::GameObject>();
	m_pFireBall->SetPosition(x, y);
	NFE::TextureComponent* texComp = new NFE::TextureComponent(m_pFireBall.get(), "Fireball.png");
	NFE::CollisionComponent* collComp = new NFE::CollisionComponent(m_pFireBall.get());
	NFE::TypeComponent* typeComp = new NFE::TypeComponent(m_pFireBall.get(), GameObjectType::FireBall);
	texComp->Update(0, x, y, 44, 44);
	collComp->SetCollisionType(NFE::CollisionType::FireBall);
	collComp->SetCollisionRect(*texComp->GetTextureData());

	m_pFireBall->AddComponentToVector(texComp);
	m_pFireBall->AddComponentToVector(typeComp);
	m_pFireBall->AddComponentToVector(collComp);
}

void FireBall::UpdateFireball(float dt)
{
	dae::float3 currPosition = m_pFireBall->GetPosition();
	m_pFireBall->SetPosition(currPosition.x += m_FireballVelocity * dt, currPosition.y);
}
