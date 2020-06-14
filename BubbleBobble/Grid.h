#pragma once
#include "GameObject.h"
#include "Block.h"
#include "BinaryWriter.h"
#include "BinaryReader.h"
namespace dae
{
	class GameObject;
}
class Block;

#include "vector"
#include "memory"
class Grid
{
public:
	Grid(float Width, float Height, int rows, int cols);
	Grid(BinaryReader& reader, const char* fileName);
	~Grid();
	std::vector<std::shared_ptr<dae::GameObject>> GetGameObjects() const;
	Block* GetBlocks() const;
	SDL_Rect* GetGridRects();
	int PositionToIndexRect(SDL_Point mousePos);
	void WriteToFile(BinaryWriter& writer, const char* filename);

private:
	Block* m_pGrid{};
	float m_TotWidth{};
	float m_TotHeight{};
	float m_BlockHeight{};
	float m_BlockWidth{};
	int m_Rows{};
	int m_Cols{};
	void CreateGrid();
	std::vector<std::shared_ptr<dae::GameObject>> m_pGameObjects{};
	SDL_Rect* m_pRects;
};

