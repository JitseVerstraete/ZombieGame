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

void ExplorationGrid::Update(float dt, IExamInterface* pInterface)
{
	AgentInfo aInfo{ pInterface->Agent_GetInfo() };
	const float detectionRange{ 8.f };
	Elite::Vector2 detectionCenter = aInfo.Position + Elite::OrientationToVector(aInfo.Orientation).GetNormalized() * (aInfo.FOV_Range - detectionRange);

	for (auto& c : m_Cells)
	{
		Elite::Vector2 cellCenter = { c.GetCellCenter() };


		//set the cell as explored if conditions are met
		if (Elite::Distance(cellCenter, aInfo.Position) < c.boundingBox.width / 2)
		{
			c.state = CellState::VISITED;
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

const Cell& ExplorationGrid::GetClosestHouseCell(const Elite::Vector2& agentPos) const
{

	auto it = std::find_if(m_Cells.begin(), m_Cells.end(), [](const Cell& c) {return c.state == CellState::HOUSE; });


	if (it != m_Cells.end())
	{
		Cell toReturn = *it;

		for (const auto& c : m_Cells)
		{
			if (c.state != CellState::HOUSE)
				continue;

			if ((Elite::Distance(agentPos, c.GetCellCenter()) * 0.9f) > Elite::Distance(agentPos, toReturn.GetCellCenter()))
				continue;

			toReturn = c;

		}

		return toReturn;
	}

	it = std::find_if(m_Cells.begin(), m_Cells.end(), [](const Cell& c) {return c.state == CellState::UNKNOWN; });
	
	if (it != m_Cells.end())
	{
		Cell toReturn = *it;

		for (const auto& c : m_Cells)
		{
			if (c.state != CellState::UNKNOWN)
				continue;

			if ((Elite::Distance(agentPos, c.GetCellCenter()) * 0.9f) > Elite::Distance(agentPos, toReturn.GetCellCenter()))
				continue;

			toReturn = c;

		}

		return toReturn;
	}

}

void ExplorationGrid::FullSurroundingsScan(IExamInterface* pInterface)
{
	std::cout << "Starting Surroundings Scan\n";
	Elite::Vector2 pathPoint{};
	for (auto& cell : m_Cells)
	{
		switch (cell.state)
		{
		case CellState::UNKNOWN:

			pathPoint = pInterface->NavMesh_GetClosestPathPoint(cell.GetCellCenter());
			if (cell.GetCellCenter() == pathPoint)
			{
				cell.state = CellState::NOHOUSE;
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
	}

}

void ExplorationGrid::ScanSurroundings(IExamInterface* pInterface)
{
	int tries{};
	int nrScans{};
	Elite::Vector2 pathPoint{};

	//loop over all cells
	for (m_CurrentScanIndex; (nrScans < m_CellsScanPerFrame) && (tries < m_MaxCellScanAttempts); ++m_CurrentScanIndex)
	{
		m_CurrentScanIndex %= m_Cells.size();
		switch (m_Cells[m_CurrentScanIndex].state)
		{
		case CellState::UNKNOWN:

			nrScans++;
			pathPoint = pInterface->NavMesh_GetClosestPathPoint(m_Cells[m_CurrentScanIndex].GetCellCenter());
			if (m_Cells[m_CurrentScanIndex].GetCellCenter() == pathPoint)
			{
				m_Cells[m_CurrentScanIndex].state = CellState::NOHOUSE;
			}
			else
			{
				if (m_Cells[PositionToIndex(pathPoint)].state != CellState::VISITED)
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
}

void ExplorationGrid::CheckCellsForHouseOverlap(const HouseInfo& pHouseinfo)
{
	float cenX{ pHouseinfo.Center.x };
	float cenY{ pHouseinfo.Center.y };
	float w{ pHouseinfo.Size.x / 2 };
	float h{ pHouseinfo.Size.y / 2 };


	//check 9 points on the edge of the house and update their states

	//corners
	m_Cells[PositionToIndex(Elite::Vector2(cenX + w, cenY + h))].state = CellState::VISITED;
	m_Cells[PositionToIndex(Elite::Vector2(cenX - w, cenY + h))].state = CellState::VISITED;
	m_Cells[PositionToIndex(Elite::Vector2(cenX + w, cenY - h))].state = CellState::VISITED;
	m_Cells[PositionToIndex(Elite::Vector2(cenX - w, cenY - h))].state = CellState::VISITED;

	//edges
	m_Cells[PositionToIndex(Elite::Vector2(cenX, cenY + h))].state = CellState::VISITED;
	m_Cells[PositionToIndex(Elite::Vector2(cenX, cenY - h))].state = CellState::VISITED;
	m_Cells[PositionToIndex(Elite::Vector2(cenX + w, cenY))].state = CellState::VISITED;
	m_Cells[PositionToIndex(Elite::Vector2(cenX - w, cenY))].state = CellState::VISITED;

	//center
	m_Cells[PositionToIndex(Elite::Vector2(cenX, cenY))].state = CellState::VISITED;
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
