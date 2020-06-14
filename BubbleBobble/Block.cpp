#include "pch.h"
#include "Block.h"
#include "TextureComponent.h"
#include "TypeComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "Renderer.h"

Block::~Block()
{

}

std::shared_ptr<dae::GameObject> Block::GetGameObject() const
{
	return m_GameObj;
}

void Block::CreateObject()
{
	m_GameObj = std::make_shared<dae::GameObject>();
	m_TextComp = new NFE::TextureComponent{m_GameObj.get(),"AirBlock.png"};
	m_TextComp->Update(0,0,0,22,22);
	m_TypeComp = new NFE::TypeComponent{ m_GameObj.get(),GameObjectType::Airblock };
	m_GameObj->AddComponentToVector(m_TextComp);
	m_GameObj->AddComponentToVector(m_TypeComp);
}

void Block::CreateObject(SDL_Rect textRect, GameObjectType type)
{
	m_GameObj = std::make_shared<dae::GameObject>();
	NFE::CollisionComponent* pCollComp{};
	if (type == GameObjectType::Airblock)
	{
		m_TextComp = new NFE::TextureComponent{ m_GameObj.get(),"AirBlock.png" };
	}
	else if (type == GameObjectType::Platformblock)
	{
		m_TextComp = new NFE::TextureComponent{ m_GameObj.get(),"PlatformBlock.png" };
		pCollComp = new NFE::CollisionComponent(m_GameObj.get());
		pCollComp->SetCollisionType(NFE::CollisionType::Platform);
	}
	else if (type == GameObjectType::Wallblock)
	{
		m_TextComp = new NFE::TextureComponent{ m_GameObj.get(),"WallBlock.png" };
		pCollComp = new NFE::CollisionComponent(m_GameObj.get());
		pCollComp->SetCollisionType(NFE::CollisionType::Wall);
	}
	m_TextComp->SetTextureData(textRect);
	m_TypeComp = new NFE::TypeComponent{ m_GameObj.get(),type};
	m_Rect = textRect;
	m_GameObj->AddComponentToVector(m_TextComp);
	m_GameObj->AddComponentToVector(m_TypeComp);
	if (pCollComp)
	{
		pCollComp->SetCollisionRect(m_Rect);
		m_GameObj->AddComponentToVector(pCollComp);
	}
}

void Block::SetWidthAndHeight(float width, float height)
{
	m_Width = width;
	m_Height = height;
	m_TextComp->Update(0,0,0,m_Width,m_Height);
	m_Rect = SDL_Rect{ int(m_GameObj->GetPosition().x), int(m_GameObj->GetPosition().y), int(m_Width),int(m_Height)};
}

void Block::ChangeTextureAndType(std::string fileName, GameObjectType type)
{
	m_TextComp->ChangeTexture(fileName);
	m_TypeComp->SetType(type);
}

void Block::WriteToFile(BinaryWriter& writer, const char* fileName)
{
	m_GameObj->WriteToFile(writer, fileName);
}

SDL_Rect Block::GetRect()
{
	return m_Rect;
}
