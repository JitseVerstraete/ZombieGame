#include "stdafx.h"
#include "KnownHousesInfo.h"
#include <algorithm>

void KnownHousesInfo::Update(float dt, IExamInterface* pInterface)
{
	AgentInfo aInfo = pInterface->Agent_GetInfo();




	for (HouseRecord& h : m_KnownHouses)
	{
		if (Elite::Distance(aInfo.Position, h.houseInfo.Center) < std::min(h.houseInfo.Size.x, h.houseInfo.Size.y) / 20)
		{
			h.explored = true;
		}
	}
}

void KnownHousesInfo::AddHouse(const HouseInfo& house)
{
	//check that the house does not exist yet
	for (auto& h : m_KnownHouses)
	{
		if (!(h.houseInfo < house) && !(house < h.houseInfo))
			return;
	}

	m_KnownHouses.push_back({ house, false });
}

HouseInfo KnownHousesInfo::GetClosestUnexploredHouse(const Elite::Vector2& agentPos)
{
	auto it = find_if(m_KnownHouses.begin(), m_KnownHouses.end(), [](const HouseRecord& h) {return !h.explored; });

	HouseInfo toReturn{};

	if (it != m_KnownHouses.end())
	{
		toReturn = (*it).houseInfo;

		for (const auto& h : m_KnownHouses)
		{
			if (h.explored)
				continue;

			if (Elite::Distance(agentPos, h.houseInfo.Center) * 0.9f > Elite::Distance(agentPos, toReturn.Center))
				continue;

			toReturn = h.houseInfo;
		}
	}

	return toReturn;
}

const vector<HouseRecord>& KnownHousesInfo::GetKnownHouses() const
{
	return m_KnownHouses;
}

