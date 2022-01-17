#pragma once
#include <Exam_HelperStructs.h>
#include <map>

inline bool operator<(const Elite::Vector2& v1, const Elite::Vector2& v2) { return (v1.x < v2.x&& v1.y < v2.y); }

struct HouseRecord
{
	HouseInfo houseInfo;
	bool explored;
};

class KnownHousesInfo
{
public:
	KnownHousesInfo() = default;
	~KnownHousesInfo() = default;

	void AddHouse(const HouseInfo& house);
	HouseInfo GetClosestUnexploredHouse(const Elite::Vector2& agentPos)const;
	int GetNrHouses() const { return m_KnownHouses.size(); }
	int GetNrUnexploredHouses() const { std::count_if(m_KnownHouses.begin(), m_KnownHouses.end(), [](const auto& h) {return h.second.explored; }); }


private:
	map <Elite::Vector2, HouseRecord> m_KnownHouses;
};

