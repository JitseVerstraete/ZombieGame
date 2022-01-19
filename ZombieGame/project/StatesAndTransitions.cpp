#include "stdafx.h"
#include "StatesAndTransitions.h"


#include "ExplorationGrid.h"
#include "KnownHousesInfo.h"
#include "ItemsManager.h"
#include <IExamInterface.h>


//================
//STATES
//================

//EXPLORING

void Exploring::OnEnter(Elite::Blackboard* pBlackboard)
{
	std::cout << "Enter Exploring State\n";
}

void Exploring::OnExit(Elite::Blackboard* pBlackboard)
{

}

void Exploring::Update(Elite::Blackboard* pBlackboard, float dt)
{
	//data needed
	ExplorationGrid* pExplorationGrid{};
	IExamInterface* pInterface{};
	Elite::Vector2* pTargetPoint{};

	//get data from blackboard
	pBlackboard->GetData("ExplorationGrid", pExplorationGrid);
	pBlackboard->GetData("Interface", pInterface);
	pBlackboard->GetData("TargetPoint", pTargetPoint);
	
	
	Elite::Vector2 agentPos{ pInterface->Agent_GetInfo().Position };
	Elite::Vector2 newTargetPoint{ pExplorationGrid->GetClosestHouseCell(agentPos).GetCellCenter() };

	*pTargetPoint = newTargetPoint;
}


//GO TO HOUSE

void HouseSeek::OnEnter(Elite::Blackboard* pBlackboard)
{
	std::cout << "Enter HouseSeek State\n";
}


void HouseSeek::OnExit(Elite::Blackboard* pBlackboard)
{

}

void HouseSeek::Update(Elite::Blackboard* pBlackboard, float dt)
{
	//data needed
	KnownHousesInfo* pKnownHouses{};
	IExamInterface* pInterface{};
	Elite::Vector2* pTargetPoint{};

	//get data from blackboard
	pBlackboard->GetData("KnownHouses", pKnownHouses);
	pBlackboard->GetData("Interface", pInterface);
	pBlackboard->GetData("TargetPoint", pTargetPoint);

	Elite::Vector2 agentPos{ pInterface->Agent_GetInfo().Position };
	Elite::Vector2 newTargetPoint{ pKnownHouses->GetClosestUnexploredHouse(agentPos).Center };

	*pTargetPoint = newTargetPoint;
}

//GO TO LOOT
void LootSeek::OnEnter(Elite::Blackboard* pBlackboard)
{
	std::cout << "Enter LootSeek State\n";
}

void LootSeek::OnExit(Elite::Blackboard* pBlackboard)
{
}

void LootSeek::Update(Elite::Blackboard* pBlackboard, float dt)
{
	ItemsManager* pItemsManager{};
	IExamInterface* pInterface{};
	Elite::Vector2* pTargetPoint{};

	pBlackboard->GetData("ItemsManager", pItemsManager);
	pBlackboard->GetData("Interface", pInterface);
	pBlackboard->GetData("TargetPoint", pTargetPoint);

	AgentInfo aInfo{ pInterface->Agent_GetInfo() };

	Elite::Vector2 newTarget{ FLT_MAX, FLT_MAX };
	Elite::Vector2 tempPos{};
	bool itemFound{ false };


	for (int i{}; i < 3; ++i)
	{
		if (pItemsManager->GetClosestKnownItemPosOfType(eItemType(i), aInfo.Position, tempPos))
		{
			if (Elite::DistanceSquared(aInfo.Position, tempPos) < Elite::DistanceSquared(aInfo.Position, newTarget) || !itemFound)
			{
				newTarget = tempPos;
				itemFound = true;
			}
		}
	}

	if (itemFound)
	{
		*pTargetPoint = newTarget;
	}
	else
	{
		*pTargetPoint = aInfo.Position;
	}
}

//================
//TRANSITIONS
//================

bool ExploreToHouseSeek::ToTransition(Elite::Blackboard* pBlackboard) const
{
	//using the blackboard, check if there is an unexplored house known
	KnownHousesInfo* pHousesInfo;
	pBlackboard->GetData("KnownHouses", pHousesInfo);
	return (pHousesInfo->GetNrUnexploredHouses() > 0);
}

bool LootSeekToHouseSeek::ToTransition(Elite::Blackboard* pBlackboard) const
{
	ToLootSeek tempTransition{};
	return !tempTransition.ToTransition(pBlackboard);
}

bool HouseSeekToExploring::ToTransition(Elite::Blackboard* pBlackboard) const
{
	KnownHousesInfo* pHousesInfo;
	pBlackboard->GetData("KnownHouses", pHousesInfo);
	return (pHousesInfo->GetNrUnexploredHouses() <= 0);
}

bool ToLootSeek::ToTransition(Elite::Blackboard* pBlackboard) const
{
	ItemsManager* pItemsManager{};
	IExamInterface* pInterface{};
	pBlackboard->GetData("ItemsManager", pItemsManager);
	pBlackboard->GetData("Interface", pInterface);

	//return true if you know about a pistol and there is room for it 
	if (pItemsManager->GetNrPistols() > 0 && pItemsManager->GetSlotForItemType(eItemType::PISTOL, pInterface) >= 0)
		return true;

	//return true if you know about a medkit and there is room for it
	if (pItemsManager->GetNrMedkits() > 0 && pItemsManager->GetSlotForItemType(eItemType::MEDKIT, pInterface) >= 0)
		return true;

	//return true if you know about a food item and there is room for it
	if (pItemsManager->GetNrFoods() > 0 && pItemsManager->GetSlotForItemType(eItemType::FOOD, pInterface) >= 0)
		return true;

	return false;

}
