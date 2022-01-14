#pragma once
#include <Exam_HelperStructs.h>

struct Cell
{

	Cell(float left, float bottom, float width, float height);

	std::vector<Elite::Vector2> GetRectPoints() const;
	Elite::Vector2 GetCellCenter() const { return Elite::Vector2(boundingBox.bottomLeft.x + boundingBox.width / 2, boundingBox.bottomLeft.y + boundingBox.height / 2); }


	// all the agents currently in this cell
	bool isExplored = false;
	Elite::Rect boundingBox;
};


class ExplorationGrid
{
public:
	ExplorationGrid() {};
	ExplorationGrid(const Elite::Vector2& center, float width, float height, int rows, int cols);

	void Update(float dt, const AgentInfo& aInfo);
	const Cell& GetRandomUnexploredCell() const;
	

	const std::vector<Cell>& GetCells() const { return m_Cells; }

private:
	// Cells and properties
	std::vector<Cell> m_Cells;

	float m_SpaceWidth = 0.f;
	float m_SpaceHeight = 0.f;

	int m_NrOfRows = 0;
	int m_NrOfCols = 0;

	float m_CellWidth = 0.f;
	float m_CellHeight = 0.f;

	// Helper functions
	int PositionToIndex(const Elite::Vector2 pos) const;
};

