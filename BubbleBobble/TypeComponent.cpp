#include "MiniginPCH.h"
#include "TypeComponent.h"

NFE::TypeComponent::TypeComponent(dae::GameObject* parent, GameObjectType type)
: BaseComponent(parent)
,m_Type(type)
{
}

GameObjectType NFE::TypeComponent::GetType()
{
	return m_Type;
}

void NFE::TypeComponent::SetType(GameObjectType newType)
{
	m_Type = newType;
}

void NFE::TypeComponent::WriteToFile(BinaryWriter& writer, const char* filename)
{
	writer.Write(m_Type, filename);
}
