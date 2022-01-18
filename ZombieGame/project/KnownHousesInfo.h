#pragma once
#include <Exam_HelperStructs.h>
#include <IExamInterface.h>
#include <vector>

#undef min

inline bool operator<(const HouseInfo& h1, const HouseInfo& h2){ return (h1.Center.x < h2.Center.x || h1.Center.y < h2.Center.y); }

struct HouseRecord
{
	HouseInfo houseInfo;
	bool explored = false;

	bool operator<(const HouseInfo& rhs)
	{
		return houseInfo < rhs;
	}
};



class KnownHousesInfo
{
public:
	KnownHousesInfo() = default;
	~KnownHousesInfo() = default;


	void Update(float dt, IExamInterface* pInterface);

	void AddHouse(const HouseInfo& house);
	HouseInfo GetClosestUnexploredHouse(const Elite::Vector2& agentPos);
	const vector<HouseRecord>& GetKnownHouses()const;

	int GetNrHouses() const { return m_KnownHouses.size(); }
	int GetNrUnexploredHouses() const { return std::count_if(m_KnownHouses.begin(), m_KnownHouses.end(), [](const auto& h) {return !h.explored; }); }


private:
	vector<HouseRecord> m_KnownHouses;
};
