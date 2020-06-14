#include "pch.h"
#include "Grid.h"
#include "GameObject.h"
#include "TypeComponent.h"
#include "Transform.h"


Grid::Grid(float Width, float Height, int rows, int cols) : m_TotWidth(Width)
,m_TotHeight(Height)
,m_Rows(rows)
,m_Cols(cols)
{
	CreateGrid();
}

Grid::Grid(BinaryReader& reader, const char* fileName)
{
	int nrOfBlocks{};
	reader.Read(nrOfBlocks,fileName);
	m_pGrid = new Block[nrOfBlocks]{};
	m_pRects = new SDL_Rect[nrOfBlocks]{};
	for (int i{}; i < nrOfBlocks; ++i)
	{
		dae::Transform readTransform{};
		reader.Read(readTransform, fileName);
		SDL_Rect readRect{};
		reader.Read(readRect, fileName);
		GameObjectType readType{};
		reader.Read(readType, fileName);

		m_pGrid[i].CreateObject(readRect,readType);
		m_pGrid[i].GetGameObject()->SetPosition(readTransform.GetPosition().x, readTransform.GetPosition().y);
		m_pRects[i] = m_pGrid[i].GetRect();
		m_pGameObjects.push_back(m_pGrid[i].GetGameObject());
	}

}

Grid::~Grid()
{
	delete[] m_pGrid;
	delete[] m_pRects;
	m_pGrid = nullptr;
	m_pRects = nullptr;
	m_pGameObjects.clear();
}

std::vector<std::shared_ptr<dae::GameObject>> Grid::GetGameObjects() const
{
	return m_pGameObjects;
}

Block* Grid::GetBlocks() const
{
	return m_pGrid;
}

int Grid::PositionToIndexRect(SDL_Point mousePos)
{
	for (size_t i{}; i < float(m_Rows * m_Cols); ++i)
	{
		if (mousePos.x >= m_pRects[i].x && mousePos.x <= m_pRects[i].x + m_pRects[i].w &&
			mousePos.y >= m_pRects[i].y && mousePos.y <= m_pRects[i].y + m_pRects[i].h)
		{
			return int(i);
		}
	}
	return 0;
}

void Grid::WriteToFile(BinaryWriter& writer, const char* filename)
{
	int gridSize = m_Cols * m_Rows;
	writer.Write(gridSize, filename); 
	for (int i{}; i < m_Cols * m_Rows; ++i)
	{
		m_pGrid[i].WriteToFile(writer, filename);
	}
}

void Grid::CreateGrid()
{
	m_BlockHeight = m_TotHeight / m_Rows;
	m_BlockWidth = m_TotWidth / m_Cols;
	m_pGrid = new Block[m_Cols * m_Rows]{};
	m_pRects = new SDL_Rect[m_Cols * m_Rows]{};
	float currHeight{};
	for(int currRow = 0; currRow < m_Rows; ++currRow)
	{
		float currWidth{};
		for (int currCol = 0; currCol < m_Cols; ++currCol)
		{
			int currPos = (currRow * m_Cols) + currCol;
			m_pGrid[currPos].CreateObject(); 
			m_pGrid[currPos].GetGameObject()->SetPosition(currWidth, (550 - m_BlockHeight) - currHeight);
			m_pGrid[currPos].SetWidthAndHeight(m_BlockWidth,m_BlockHeight);
			m_pRects[currPos] = m_pGrid[currPos].GetRect();
			m_pGameObjects.push_back(m_pGrid[currPos].GetGameObject());
			currWidth += m_BlockWidth;
		}
		currHeight += m_BlockHeight;
	}
}

SDL_Rect* Grid::GetGridRects()
{
	return m_pRects;
}
