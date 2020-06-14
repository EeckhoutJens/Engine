#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SDL.h"


NFE::TextureComponent::TextureComponent(dae::GameObject *parent, std::string fileName) : BaseComponent(parent)
{
	mTexture = dae::ResourceManager::GetInstance().LoadTexture(fileName);
}

NFE::TextureComponent::~TextureComponent()
{
}

void NFE::TextureComponent::Render()
{
	if (m_ShouldRender)
	{
		dae::Renderer::GetInstance().RenderTexture(*mTexture, textureData);
	}
}

void NFE::TextureComponent::Update(const float&, float x, float y, float width, float height)
{
	textureData.x = int(x);
	textureData.y = int(y);
	if (width != 0 && height != 0)
	{
		textureData.w = int(width);
		textureData.h = int(height);
	}
}

void NFE::TextureComponent::WriteToFile(BinaryWriter& writer, const char* filename)
{
	writer.Write(textureData, filename);
}

void NFE::TextureComponent::ChangeTexture(std::string fileName)
{
	mTexture = dae::ResourceManager::GetInstance().LoadTexture(fileName);
}

void NFE::TextureComponent::DeactivateRendering()
{
	m_ShouldRender = false;
}

SDL_Rect* NFE::TextureComponent::GetTextureData()
{
	return &textureData;
}

void NFE::TextureComponent::SetTextureData(SDL_Rect rect)
{
	textureData = rect;
}
