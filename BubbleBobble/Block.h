#pragma once
#include "memory"
#include "SDL_rect.h"
#include "TypeComponent.h"
#include <string>
namespace dae
{
	class GameObject;
}
namespace NFE
{
	class TextureComponent;
}

class Block
{
public:
	Block() = default;
	~Block();
	std::shared_ptr<dae::GameObject> GetGameObject() const;
	void CreateObject();
	void CreateObject(SDL_Rect textRect, GameObjectType type); 
	void SetWidthAndHeight(float width, float height);
	void ChangeTextureAndType(std::string fileName,GameObjectType type);
	void WriteToFile(BinaryWriter& writer, const char* fileName);
	SDL_Rect GetRect();

private:
	std::shared_ptr<dae::GameObject> m_GameObj{};
	NFE::TextureComponent* m_TextComp{};
	NFE::TypeComponent* m_TypeComp{};
	float m_Width{};
	float m_Height{};
	SDL_Rect m_Rect{};
};

