#pragma once

#include <Exam_HelperStructs.h>
#include <IExamInterface.h>

#include <set>


class ItemsManager
{
public:
	ItemsManager(IExamInterface* pInterface);
	~ItemsManager() = default;

	void Update(float dt);
	void RecordItem(EntityInfo ent);

	int GetNrItems() const { return ( GetNrPistols() + GetNrMedkits() + GetNrFoods() ); }

	int GetNrPistols() const { return	m_PistolPositions.size(); }
	int GetNrMedkits() const { return	m_MedkitPositions.size(); }
	int GetNrFoods() const { return	m_FoodPositions.size(); }


	//return the index of the item when found, returns -1 if nothing was found
	int HasItemOfType(eItemType type);

	//get index of items to use
	int GetFirstPistolIndex();
	int GetLowestMedkitIndex();
	int GetLowestFoodIndex();

	
	int GetItemValue(int index);
	int GetTotalAmmo();
	
	//use item by index
	void UseItem(int index);

	//returns -1 if there is no slot available for that item type
	int GetOpenSlotForItemType(eItemType type) const; 
	
	//returns false if there are no items of that type found
	bool GetClosestKnownItemPosOfType(eItemType type, const Elite::Vector2& agentPos, Elite::Vector2& itemPos) const;


private:
	std::set<Elite::Vector2> m_PistolPositions;
	std::set<Elite::Vector2> m_MedkitPositions;
	std::set<Elite::Vector2> m_FoodPositions;

	std::vector<EntityInfo> m_EntitiesInFov;

	IExamInterface* m_pInterface;

	const std::vector<eItemType> m_InventoryLayout = { eItemType::PISTOL,eItemType::PISTOL, eItemType::MEDKIT, eItemType::MEDKIT, eItemType::FOOD };

};

