#pragma once
#include "Grid.h"
namespace dae
{
	class GameObject;
}
class Level
{
public:
	Level(float Width, float Height, int rows, int cols);
	Level(BinaryReader& reader, const char* filename);
	~Level();
	Level(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level& operator=(Level && other) = delete;
	std::shared_ptr<dae::GameObject> CreateNewEnemy(float posX, float posY, GameObjectType type);
	Grid* GetGrid() { return m_pGrid; }
	std::vector<std::shared_ptr<dae::GameObject>>& GetEnemies() { return m_pEnemies; }
	void WriteToFile(BinaryWriter& writer, const char* filename);

private:
	Grid* m_pGrid;
	std::vector<std::shared_ptr<dae::GameObject>> m_pEnemies{};
};

