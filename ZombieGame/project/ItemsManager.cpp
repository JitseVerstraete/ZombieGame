#include "stdafx.h"
#include "ItemsManager.h"

ItemsManager::ItemsManager(IExamInterface* pInterface)
	:m_pInterface{ pInterface }
{
}

void ItemsManager::Update(float dt)
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



		m_pInterface->Item_GetInfo(i, tempItem);
		switch (tempItem.Type)
		{

		case eItemType::PISTOL:
			slot = GetOpenSlotForItemType(eItemType::PISTOL);
			if (slot >= 0)
			{
				if (m_pInterface->Item_Grab(i, tempItem))
				{
					m_pInterface->Inventory_AddItem(slot, tempItem);
					m_PistolPositions.erase(tempItem.Location);
				}
			}
			break;


		case eItemType::MEDKIT:
			slot = GetOpenSlotForItemType(eItemType::MEDKIT);
			if (slot >= 0)
			{
				if (m_pInterface->Item_Grab(i, tempItem))
				{
					m_pInterface->Inventory_AddItem(slot, tempItem);
					m_MedkitPositions.erase(tempItem.Location);
				}
			}
			break;


		case eItemType::FOOD:
			slot = GetOpenSlotForItemType(eItemType::FOOD);
			if (slot >= 0)
			{
				if (m_pInterface->Item_Grab(i, tempItem))
				{
					m_pInterface->Inventory_AddItem(slot, tempItem);
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


	//remove empty items
	ItemInfo tempItem{};
	for (int i{}; i < 5; ++i)
	{
		if (m_pInterface->Inventory_GetItem(i, tempItem))
		{
			switch (tempItem.Type)
			{

			case eItemType::PISTOL:
				if (m_pInterface->Weapon_GetAmmo(tempItem) <= 0)
				{
					m_pInterface->Inventory_RemoveItem(i);
					//cout << "TOSSED EMPTY WEAPON\n";
				}
				break;


			case eItemType::MEDKIT:
				if (m_pInterface->Medkit_GetHealth(tempItem) <= 0)
				{
					m_pInterface->Inventory_RemoveItem(i);
					//cout << "TOSSED EMPTY MEDKIT\n";
				}
				break;


			case eItemType::FOOD:
				if (m_pInterface->Food_GetEnergy(tempItem) <= 0)
				{
					m_pInterface->Inventory_RemoveItem(i);
					//cout << "TOSSED EMPTY FOOD\n";
				}
				break;

			default:
				break;
			}
		}

	}



	//draw the items on screen
	for (const auto& p : m_PistolPositions)
	{
		m_pInterface->Draw_Point(p, 5, { 1.f, 0.f, 0.f }, 0.f);
	}

	for (const auto& m : m_MedkitPositions)
	{
		m_pInterface->Draw_Point(m, 5, { 0.f, 1.f, 0.f }, 0.f);
	}

	for (const auto& p : m_FoodPositions)
	{
		m_pInterface->Draw_Point(p, 5, { 0.f, 0.f, 1.f }, 0.f);
	}
}

void ItemsManager::RecordItem(EntityInfo ent)
{
	ItemInfo tempItem{};
	m_pInterface->Item_GetInfo(ent, tempItem);


	if (ent.Location != tempItem.Location)
	{
		return;
	}

	pair<set<Elite::Vector2>::iterator, bool> it{};
	switch (tempItem.Type)
	{
	case eItemType::PISTOL:
		it = m_PistolPositions.insert(ent.Location);
		//if (it.second)
		//	std::cout << "PISTOL FOUND!\n";


		break;

	case eItemType::MEDKIT:
		it = m_MedkitPositions.insert(ent.Location);
		//if (it.second)
		//	std::cout << "MEDKIT FOUND!\n";

		break;

	case eItemType::FOOD:
		it = m_FoodPositions.insert(ent.Location);
		//if (it.second)
		//	std::cout << "FOOD FOUND!\n";

		break;

	case eItemType::GARBAGE:
		break;


	default:
		break;
	}


	m_EntitiesInFov.push_back(ent);



}



int ItemsManager::HasItemOfType(eItemType type)
{
	ItemInfo tempItem{};
	for (int i{}; i < 5; i++)
	{
		if (m_pInterface->Inventory_GetItem(i, tempItem))
		{
			if (type == tempItem.Type)
			{
				return i;
			}
		}
	}


	return false;
}

int ItemsManager::GetFirstPistolIndex()
{
	ItemInfo tempItem{};
	for (int i{}; i < 5; i++)
	{
		if (m_pInterface->Inventory_GetItem(i, tempItem))
		{
			if (tempItem.Type == eItemType::PISTOL)
			{
				return i;
			}
		}
	}

	return -1;
}

int ItemsManager::GetLowestMedkitIndex()
{
	int toReturn{ -1 };
	int healAmount{ INT_MAX };
	ItemInfo tempItem{};

	for (int i{}; i < 5; i++)
	{
		if (m_pInterface->Inventory_GetItem(i, tempItem))
		{
			if (tempItem.Type == eItemType::MEDKIT && m_pInterface->Medkit_GetHealth(tempItem) < healAmount)
			{
				toReturn = i;
				healAmount = m_pInterface->Medkit_GetHealth(tempItem);
			}
		}
	}
	return toReturn;
}

int ItemsManager::GetLowestFoodIndex()
{
	int toReturn{ -1 };
	int energyAmount{ INT_MAX };
	ItemInfo tempItem{};

	for (int i{}; i < 5; i++)
	{
		if (m_pInterface->Inventory_GetItem(i, tempItem))
		{
			if (tempItem.Type == eItemType::FOOD && m_pInterface->Food_GetEnergy(tempItem) < energyAmount)
			{
				toReturn = i;
				energyAmount = m_pInterface->Food_GetEnergy(tempItem);
			}
		}
	}
	return toReturn;
}

int ItemsManager::GetItemValue(int index)
{
	ItemInfo temp{};
	if (m_pInterface->Inventory_GetItem(index, temp))
	{
		switch (temp.Type)
		{
		case eItemType::PISTOL:
			return m_pInterface->Weapon_GetAmmo(temp);
			break;
		case eItemType::MEDKIT:
			return m_pInterface->Medkit_GetHealth(temp);
			break;
		case eItemType::FOOD:
			return m_pInterface->Food_GetEnergy(temp);
			break;

		default:
			break;
		}
	}

	return -1;
}

int ItemsManager::GetTotalAmmo()
{
	int totalAmmo{};
	ItemInfo tempItem{};
	for (int i{}; i < 5; ++i)
	{
		//check if current slot is reserved for the asked type
		if (m_pInterface->Inventory_GetItem(i, tempItem))
		{
			if (tempItem.Type == eItemType::PISTOL)
			{
				totalAmmo += m_pInterface->Weapon_GetAmmo(tempItem);
			}

		}
	}

	return totalAmmo;
}

void ItemsManager::UseItem(int index)
{
	m_pInterface->Inventory_UseItem(index);
}

int ItemsManager::GetOpenSlotForItemType(eItemType type) const
{
	ItemInfo tempItem{};
	for (int i{}; i < m_InventoryLayout.size(); ++i)
	{
		//check if current slot is reserved for the asked type
		if (type != m_InventoryLayout[i])
			continue;

		//check if there is no item allready in this slot
		if (m_pInterface->Inventory_GetItem(i, tempItem))
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
