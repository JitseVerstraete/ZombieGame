#include "stdafx.h"
#include "KnownHousesInfo.h"

void KnownHousesInfo::AddHouse(const HouseInfo& house)
{
	std::pair<Elite::Vector2, HouseRecord> pair{house.Center, { house, false }};
	m_KnownHouses.insert(pair);
}

HouseInfo KnownHousesInfo::GetClosestUnexploredHouse(const Elite::Vector2& agentPos) const
{
	return HouseInfo();
}

