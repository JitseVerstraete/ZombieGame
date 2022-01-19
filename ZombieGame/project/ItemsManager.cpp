#include "stdafx.h"
#include "ItemsManager.h"

void ItemsManager::Update(float dt, IExamInterface* pInterface)
{
	ItemInfo tempItem{};
	int slotIndex{ -1 };


	//grab pistol
	slotIndex = GetSlotForItemType(eItemType::PISTOL, pInterface);
	if (slotIndex >= 0)
	{

		for (auto it = m_Pistols.begin(); it != m_Pistols.end(); )
		{

			if (pInterface->Item_Grab(*it, tempItem))
			{
				pInterface->Inventory_AddItem(slotIndex, tempItem);

				it = m_Pistols.erase(it);
				std::cout << "PISTOL PICKED UP\n";
			}
			else
			{
				++it;
			}
		}

	}

	//grab medkit
	slotIndex = GetSlotForItemType(eItemType::MEDKIT, pInterface);
	if (slotIndex >= 0)
	{

		for (auto it = m_Medkits.begin(); it != m_Medkits.end(); )
		{

			if (pInterface->Item_Grab(*it, tempItem))
			{
				pInterface->Inventory_AddItem(slotIndex, tempItem);

				it = m_Medkits.erase(it);
				std::cout << "MEDKIT PICKED UP\n";
			}
			else
			{
				++it;
			}
		}
	}

	//grab food
	slotIndex = GetSlotForItemType(eItemType::FOOD, pInterface);
	if (slotIndex >= 0)
	{

		for (auto it = m_Foods.begin(); it != m_Foods.end(); )
		{

			if (pInterface->Item_Grab(*it, tempItem))
			{
				pInterface->Inventory_AddItem(slotIndex, tempItem);


				it = m_Foods.erase(it);
				std::cout << "FOOD PICKED UP\n";
			}
			else
			{
				++it;
			}
		}

	}


	ItemInfo temp{};
	for (const auto& i : m_Pistols)
	{
		pInterface->Draw_Point(i.Location, 5, { 1.f, 0.f, 0.f }, 0.f);
		pInterface->Item_GetInfo(i, temp);
		std::cout << (int)temp.Type;
	}

	for (const auto& i : m_Medkits)
	{
		pInterface->Draw_Point(i.Location, 5, { 0.f, 1.f, 0.f }, 0.f);

	}

	for (const auto& i : m_Foods)
	{
		pInterface->Draw_Point(i.Location, 5, { 0.f, 0.f, 1.f }, 0.f);
	}

}

void ItemsManager::AddItem(EntityInfo ent, eItemType type)
{
	pair<set<EntityInfo>::iterator, bool> it{};
	switch (type)
	{
	case eItemType::PISTOL:
		it = m_Pistols.insert(ent);
		if (it.second)
			std::cout << "PISTOL FOUND!\n";
		

		break;

	case eItemType::MEDKIT:
		it = m_Medkits.insert(ent);
		if (it.second)
			std::cout << "MEDKIT FOUND!\n";

		break;

	case eItemType::FOOD:
		it = m_Foods.insert(ent);
		if (it.second)
			std::cout << "FOOD FOUND!\n";

		break;

	case eItemType::GARBAGE:
		break;


	default:
		break;
	}




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
	switch (type)
	{
	case eItemType::PISTOL:
	{

		if (m_Pistols.size() <= 0)
			return false;

		auto it = min_element(m_Pistols.begin(), m_Pistols.end(), [agentPos](const EntityInfo& i1, const EntityInfo& i2) {return Elite::Distance(agentPos, i1.Location) < Elite::Distance(agentPos, i2.Location); });
		itemPos = (*it).Location;

		return true;
		break;
	}
	case eItemType::MEDKIT:
	{

		if (m_Medkits.size() <= 0)
			return false;

		auto it = min_element(m_Medkits.begin(), m_Medkits.end(), [agentPos](const EntityInfo& i1, const EntityInfo& i2) {return Elite::Distance(agentPos, i1.Location) < Elite::Distance(agentPos, i2.Location); });
		itemPos = (*it).Location;

		return true;
		break;
	}
	case eItemType::FOOD:
	{

		if (m_Foods.size() <= 0)
			return false;

		auto it = min_element(m_Foods.begin(), m_Foods.end(), [agentPos](const EntityInfo& i1, const EntityInfo& i2) {return Elite::Distance(agentPos, i1.Location) < Elite::Distance(agentPos, i2.Location); });
		itemPos = (*it).Location;

		return true;
		break;
	}
	default:

		return false;
		break;
	}

}
