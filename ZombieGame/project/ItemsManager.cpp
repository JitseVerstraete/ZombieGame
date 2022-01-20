#include "stdafx.h"
#include "ItemsManager.h"

void ItemsManager::Update(float dt, IExamInterface* pInterface)
{
	//try to grab all items currently in fov


	for (const auto& i : m_EntitiesInFov)
	{
		/*
		ItemInfo grabbedItem{};
		ItemInfo temp{};

		if (pInterface->Item_Grab(i, grabbedItem))
		{
			if (pInterface->Inventory_GetItem(0, temp))
			{
				pInterface->Inventory_RemoveItem(0);
			}
			pInterface->Inventory_AddItem(0, grabbedItem);


			switch (grabbedItem.Type) 
			{
			case eItemType::PISTOL:
				m_PistolPositions.erase(grabbedItem.Location);
				break;
			case eItemType::MEDKIT:
				m_MedkitPositions.erase(grabbedItem.Location);
				break;
			case eItemType::FOOD:
				m_FoodPositions.erase(grabbedItem.Location);
				break;
			default:
				break;
			}
		}
		*/
		

		int slot{ -1 };
		ItemInfo tempItem{};


		
		pInterface->Item_GetInfo(i, tempItem);
		switch (tempItem.Type)
		{

		case eItemType::PISTOL:
			slot = GetSlotForItemType(eItemType::PISTOL, pInterface);
			if (slot >= 0)
			{
				if (pInterface->Item_Grab(i, tempItem))
				{
					pInterface->Inventory_AddItem(slot, tempItem);
					m_PistolPositions.erase(tempItem.Location);
				}
			}
			break;


		case eItemType::MEDKIT:
			slot = GetSlotForItemType(eItemType::MEDKIT, pInterface);
			if (slot >= 0)
			{
				if (pInterface->Item_Grab(i, tempItem))
				{
					pInterface->Inventory_AddItem(slot, tempItem);
					m_MedkitPositions.erase(tempItem.Location);
				}
			}
			break;


		case eItemType::FOOD:
			slot = GetSlotForItemType(eItemType::FOOD, pInterface);
			if (slot >= 0)
			{
				if (pInterface->Item_Grab(i, tempItem))
				{
					pInterface->Inventory_AddItem(slot, tempItem);
					m_FoodPositions.erase(tempItem.Location);
				}
			}
			break;


		default:
			break;
		}
		
	}



	//clear entities in fov
	m_EntitiesInFov.resize(0);


	for (const auto& p : m_PistolPositions)
	{
		pInterface->Draw_Point(p, 5, { 1.f, 0.f, 0.f }, 0.f);
	}

	for (const auto& m : m_MedkitPositions)
	{
		pInterface->Draw_Point(m, 5, { 0.f, 1.f, 0.f }, 0.f);
	}

	for (const auto& p : m_FoodPositions)
	{
		pInterface->Draw_Point(p, 5, { 0.f, 0.f, 1.f }, 0.f);
	}
}

void ItemsManager::RecordItem(EntityInfo ent, IExamInterface* pInterface)
{
	ItemInfo tempItem{};
	pInterface->Item_GetInfo(ent, tempItem);


	if (ent.Location != tempItem.Location)
	{
		return;
	}

	pair<set<Elite::Vector2>::iterator, bool> it{};
	switch (tempItem.Type)
	{
	case eItemType::PISTOL:
		it = m_PistolPositions.insert(ent.Location);
		if (it.second)
			std::cout << "PISTOL FOUND!\n";


		break;

	case eItemType::MEDKIT:
		it = m_MedkitPositions.insert(ent.Location);
		if (it.second)
			std::cout << "MEDKIT FOUND!\n";

		break;

	case eItemType::FOOD:
		it = m_FoodPositions.insert(ent.Location);
		if (it.second)
			std::cout << "FOOD FOUND!\n";

		break;

	case eItemType::GARBAGE:
		break;


	default:
		break;
	}


	m_EntitiesInFov.push_back(ent);



}

int ItemsManager::GetSlotForItemType(eItemType type, IExamInterface* pInterface) const
{
	ItemInfo tempItem{};
	for (int i{}; i < m_InventoryLayout.size(); ++i)
	{
		//check if current slot is reserved for the asked type
		if (type != m_InventoryLayout[i])
			continue;

		//check if there is no item allready in this slot
		if (pInterface->Inventory_GetItem(i, tempItem))
			continue;

		return i;
	}

	return -1;
}

bool ItemsManager::GetClosestKnownItemPosOfType(eItemType type, const Elite::Vector2& agentPos, Elite::Vector2& itemPos) const
{
	float currentDistance{ FLT_MAX };
	Elite::Vector2 currentPos{};

	switch (type)
	{
	case eItemType::PISTOL:
	{

		if (m_PistolPositions.size() <= 0)
			return false;

		for (const Elite::Vector2& p : m_PistolPositions)
		{
			if (Elite::Distance(p, agentPos) < currentDistance)
			{
				currentDistance = Elite::Distance(p, agentPos);
				currentPos = p;
			}
		}

		/*
		auto it = min_element(m_PistolPositions.begin(), m_PistolPositions.end(), [agentPos](const Elite::Vector2& i1, const Elite::Vector2& i2) {return Elite::Distance(agentPos, i1) < Elite::Distance(agentPos, i2); });
		itemPos = (*it);
		*/

		itemPos = currentPos;

		return true;
		break;
	}
	case eItemType::MEDKIT:
	{

		if (m_MedkitPositions.size() <= 0)
			return false;



		for (const Elite::Vector2& m : m_MedkitPositions)
		{
			if (Elite::Distance(m, agentPos) < currentDistance)
			{
				currentDistance = Elite::Distance(m, agentPos);
				currentPos = m;
			}
		}

		/*
		auto it = min_element(m_MedkitPositions.begin(), m_MedkitPositions.end(), [agentPos](const Elite::Vector2& i1, const Elite::Vector2& i2) {return Elite::Distance(agentPos, i1) < Elite::Distance(agentPos, i2); });
		itemPos = (*it);
		*/

		itemPos = currentPos;

		return true;
		break;
	}
	case eItemType::FOOD:
	{

		if (m_FoodPositions.size() <= 0)
			return false;



		for (const Elite::Vector2& f : m_FoodPositions)
		{
			if (Elite::Distance(f, agentPos) < currentDistance)
			{
				currentDistance = Elite::Distance(f, agentPos);
				currentPos = f;
			}
		}

		/*
		auto it = min_element(m_FoodPositions.begin(), m_FoodPositions.end(), [agentPos](const Elite::Vector2& i1, const Elite::Vector2& i2) {return Elite::Distance(agentPos, i1) < Elite::Distance(agentPos, i2); });
		itemPos = (*it);
		*/

		itemPos = currentPos;

		return true;
		break;
	}
	default:

		return false;
		break;
	}

	

}
