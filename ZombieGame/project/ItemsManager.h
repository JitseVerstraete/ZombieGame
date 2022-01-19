#pragma once

#include <Exam_HelperStructs.h>
#include <IExamInterface.h>

#include <set>

class ItemsManager
{
public:
	ItemsManager() = default;
	~ItemsManager() = default;

	void Update(float dt, IExamInterface* pInterface);
	void AddItem(EntityInfo ent, eItemType type);

	int GetNrItems() const { return ( GetNrPistols() + GetNrMedkits() + GetNrFoods() ); }
	int GetNrPistols() const { return	m_Pistols.size(); }
	int GetNrMedkits() const { return	m_Medkits.size(); }
	int GetNrFoods() const { return	m_Foods.size(); }

private:
	std::set<EntityInfo> m_Pistols;
	std::set<EntityInfo> m_Medkits;
	std::set<EntityInfo> m_Foods;

};

