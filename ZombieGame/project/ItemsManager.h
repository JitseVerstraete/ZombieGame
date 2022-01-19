#pragma once

#include <Exam_HelperStructs.h>
#include <IExamInterface.h>

#include <set>
#include <map>

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

	//returns -1 if there is no slot available for that item type
	int GetSlotForItemType(eItemType type, IExamInterface* pInterfac) const; 
	
	//returns false if there are no items of that type found
	bool GetClosestKnownItemPosOfType(eItemType type, const Elite::Vector2& agentPos, Elite::Vector2& itemPos) const;


private:
	std::set<EntityInfo> m_Pistols;
	std::set<EntityInfo> m_Medkits;
	std::set<EntityInfo> m_Foods;

	const std::vector<eItemType> m_InventoryLayout = { eItemType::PISTOL,eItemType::PISTOL, eItemType::MEDKIT, eItemType::MEDKIT, eItemType::FOOD };

};

