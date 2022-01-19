#include "stdafx.h"
#include "ItemsManager.h"

void ItemsManager::Update(float dt, IExamInterface* pInterface)
{
	ItemInfo tempItem{};

	for (auto it = m_Pistols.begin(); it != m_Pistols.end(); )
	{

		if (pInterface->Item_Grab(*it, tempItem))
		{
			pInterface->Inventory_AddItem(0, tempItem);

			std::cout << m_Pistols.size() << endl;
			it = m_Pistols.erase(it);
			std::cout << m_Pistols.size() << endl;
			std::cout << "FOOD PICKED UP\n";
		}
		else
		{
			++it;
		}
	}

	for (auto it = m_Medkits.begin(); it != m_Medkits.end(); )
	{

		if (pInterface->Item_Grab(*it, tempItem))
		{
			pInterface->Inventory_AddItem(0, tempItem);

			std::cout << m_Medkits.size() << endl;
			it = m_Medkits.erase(it);
			std::cout << m_Medkits.size() << endl;
			std::cout << "FOOD PICKED UP\n";
		}
		else
		{
			++it;
		}
	}


	for (auto it = m_Foods.begin(); it != m_Foods.end(); ) 
	{
		
		if (pInterface->Item_Grab(*it, tempItem)) 
		{
			pInterface->Inventory_AddItem(0, tempItem);

			std::cout << m_Foods.size() << endl;
			it = m_Foods.erase(it);
			std::cout << m_Foods.size() << endl;
			std::cout << "FOOD PICKED UP\n";
		}
		else 
		{
			++it;
		}
	}
	/*
	for (auto& food : m_Foods)
	{
		if (pInterface->Item_Grab(food, tempItem))
		{
			pInterface->Inventory_AddItem(0, tempItem);
			m_Foods.erase(food); 
			std::cout << "FOOD PICKED UP\n";
		}
	}
	*/

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
