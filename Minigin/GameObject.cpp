#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureComponent.h"
#include "Font.h"

dae::GameObject::~GameObject()
{
	for (NFE::BaseComponent* pComp : m_pComponents)
	{
		delete pComp;
		pComp = nullptr;
	}
	m_pComponents.clear();
	for(GameObject* pChild : m_pChildren)
	{
		delete pChild;
		pChild = nullptr;
	}
	m_pChildren.clear();
};

void dae::GameObject::Update(float deltaTime)
{
	const auto pos = mTransform.GetPosition();
	for (NFE::BaseComponent* pComp : m_pComponents)
	{
		if (pComp)
		{
			pComp->Update(deltaTime,pos.x,pos.y);
		}
	}
	for( GameObject* pChild : m_pChildren)
	{
		if(pChild)
		{
			pChild->Update(deltaTime);
		}
	}
}

void dae::GameObject::Render() const
{
	for (NFE::BaseComponent* pComp : m_pComponents)
	{
		if(pComp)
		pComp->Render();
	}
	for( GameObject* pChild : m_pChildren)
	{
		if(pChild)
		pChild->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	mTransform.SetPosition(x, y, 0.0f);
}

const dae::float3& dae::GameObject::GetPosition()
{
	return mTransform.GetPosition();
}

void dae::GameObject::AddComponentToVector(NFE::BaseComponent* componentToAdd)
{
	m_pComponents.push_back(componentToAdd);
}


void dae::GameObject::AddChild(GameObject* child)
{
	m_pChildren.push_back(child);
}

void dae::GameObject::DeleteChildren()
{
	for( GameObject* pChild : m_pChildren)
	{
		delete pChild;
		pChild = nullptr;
	}
	m_pChildren.clear();
}


void dae::GameObject::WriteToFile(BinaryWriter& writer, const char* filename)
{
	writer.Write(mTransform, filename);
	for (NFE::BaseComponent* pComp : m_pComponents)
	{
		if (pComp)
			pComp->WriteToFile(writer, filename);
	}
}


std::vector<NFE::BaseComponent*>& dae::GameObject::GetComponents()
{
	// TODO: insert return statement here
	return m_pComponents;
}
