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
	void RecordItem(EntityInfo ent, IExamInterface* pInterface);

	int GetNrItems() const { return ( GetNrPistols() + GetNrMedkits() + GetNrFoods() ); }
	int GetNrPistols() const { return	m_PistolPositions.size(); }
	int GetNrMedkits() const { return	m_MedkitPositions.size(); }
	int GetNrFoods() const { return	m_FoodPositions.size(); }

	//returns -1 if there is no slot available for that item type
	int GetSlotForItemType(eItemType type, IExamInterface* pInterface) const; 
	
	//returns false if there are no items of that type found
	bool GetClosestKnownItemPosOfType(eItemType type, const Elite::Vector2& agentPos, Elite::Vector2& itemPos) const;


private:
	std::set<Elite::Vector2> m_PistolPositions;
	std::set<Elite::Vector2> m_MedkitPositions;
	std::set<Elite::Vector2> m_FoodPositions;

	std::vector<EntityInfo> m_EntitiesInFov;

	const std::vector<eItemType> m_InventoryLayout = { eItemType::PISTOL,eItemType::PISTOL, eItemType::MEDKIT, eItemType::MEDKIT, eItemType::FOOD };

};

