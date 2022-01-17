#include "stdafx.h"
#include "ExplorationGrid.h"

Cell::Cell(float left, float bottom, float width, float height)
	:boundingBox{ {left, bottom}, width, height }
{

}



std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	return { Elite::Vector2(boundingBox.bottomLeft),
			Elite::Vector2(boundingBox.bottomLeft.x, boundingBox.bottomLeft.y + boundingBox.height),
			Elite::Vector2(boundingBox.bottomLeft.x + boundingBox.width, boundingBox.bottomLeft.y + boundingBox.height),
			Elite::Vector2(boundingBox.bottomLeft.x + boundingBox.width, boundingBox.bottomLeft.y) };
}




ExplorationGrid::ExplorationGrid(const Elite::Vector2& center, float width, float height, int rows, int cols)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_CellWidth{}
	, m_CellHeight{}
{


	//create a certain amount of cells
	m_CellWidth = (width * 2) / cols;
	m_CellHeight = (height * 2) / rows;
	float startX = center.x - width;
	float startY = center.y - height;

	for (int r{}; r < rows; r++)
	{
		for (int c{}; c < cols; c++)
		{
			m_Cells.push_back(Cell(startX + c * m_CellWidth, startY + r * m_CellHeight, m_CellWidth, m_CellHeight));
		}
	}

}

void ExplorationGrid::Update(float dt, const AgentInfo& aInfo, IExamInterface* pInterface)
{

	const float detectionRange{ 8.f };
	Elite::Vector2 detectionCenter = aInfo.Position + Elite::OrientationToVector(aInfo.Orientation).GetNormalized() * (aInfo.FOV_Range - detectionRange);

	for (auto& c : m_Cells)
	{
		Elite::Vector2 cellCenter = { c.boundingBox.bottomLeft.x + c.boundingBox.width / 2, c.boundingBox.bottomLeft.y + c.boundingBox.height / 2 };


		//set the cell as explored if conditions are met
		if (Elite::Distance(cellCenter, detectionCenter) < detectionRange || Elite::Distance(cellCenter, aInfo.Position) < aInfo.GrabRange)
		{
			c.isExplored = true;
		}

	}

	ScanSurroundings(pInterface);
}

const Cell& ExplorationGrid::GetRandomUnexploredCell() const
{
	size_t cellVectorSize{ m_Cells.size() };

	int randomCellIndex{};
	do
	{
		randomCellIndex = rand() % cellVectorSize;


	} while (m_Cells[randomCellIndex].isExplored);

	return m_Cells[randomCellIndex];
}

void ExplorationGrid::ScanSurroundings(IExamInterface* pInterface)
{
	int tries{};
	int nrScans{};
	Elite::Vector2 pathPoint{};

	//loop over all cells
	for ( m_CurrentScanIndex; (nrScans < m_CellsScanPerFrame) && (tries < m_MaxCellScanAttempts); ++m_CurrentScanIndex)
	{
		m_CurrentScanIndex %= m_Cells.size();
		switch (m_Cells[m_CurrentScanIndex].state)
		{
		case CellState::UNKNOWN:

			nrScans++;
			pathPoint = pInterface->NavMesh_GetClosestPathPoint(m_Cells[m_CurrentScanIndex].GetCellCenter()) ;
			if (m_Cells[m_CurrentScanIndex].GetCellCenter() == pathPoint)
			{
				m_Cells[m_CurrentScanIndex].state = CellState::NOHOUSE;
			}
			else
			{
				m_Cells[PositionToIndex(pathPoint)].state = CellState::HOUSE;
			}
			break;
		case CellState::HOUSE:
			//do nothing
			break;
		case CellState::NOHOUSE:
			//do nothing
			break;
		default:
			break;
		}


		tries++;
	}

	std::cout << tries << std::endl;

}

int ExplorationGrid::PositionToIndex(const Elite::Vector2 pos) const
{

	//TODO:: debug this function
	int index{ 0 };
	float cellWidth = (m_SpaceWidth * 2) / m_NrOfCols;
	float cellHeight = (m_SpaceHeight * 2) / m_NrOfRows;

	float offsetXPos{ pos.x + m_SpaceWidth };
	float offsetYPos{ pos.y + m_SpaceHeight };

	//clamp the value
	if (offsetXPos >= m_SpaceWidth * 2)
	{
		offsetXPos = m_SpaceWidth * 2 - 0.001f;
	}
	else if (offsetXPos <= 0)
	{
		offsetXPos = 0.001f;
	}

	if (offsetYPos >= m_SpaceHeight * 2)
	{
		offsetYPos = m_SpaceHeight * 2 - 0.001f;
	}
	else if (offsetYPos <= 0)
	{
		offsetYPos = 0.001f;
	}

	//calulate the index
	index += int(offsetXPos / cellWidth);
	index += int(offsetYPos / cellHeight) * m_NrOfCols;

	return index;
}
