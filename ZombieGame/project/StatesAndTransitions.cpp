#include "stdafx.h"
#include "StatesAndTransitions.h"


#include "ExplorationGrid.h"
#include "KnownHousesInfo.h"
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

	//*pTargetPoint = newTargetPoint;
}


//GO TO HOUSE

void HouseSeek::OnEnter(Elite::Blackboard* pBlackboard)
{
	std::cout << "Enter GoToHouse State\n";
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

	//*pTargetPoint = newTargetPoint;
}

//GO TO LOOT
void LootSeek::OnEnter(Elite::Blackboard* pBlackboard)
{
}

void LootSeek::OnExit(Elite::Blackboard* pBlackboard)
{
}

void LootSeek::Update(Elite::Blackboard* pBlackboard, float dt)
{
}

//================
//TRANSITIONS
//================

bool ToHouseSeek::ToTransition(Elite::Blackboard* pBlackboard) const
{
	//using the blackboard, check if there is an unexplored house known
	KnownHousesInfo* pHousesInfo;
	pBlackboard->GetData("KnownHouses", pHousesInfo);
	return (pHousesInfo->GetNrUnexploredHouses() > 0);
}

bool ToExploring::ToTransition(Elite::Blackboard* pBlackboard) const
{
	KnownHousesInfo* pHousesInfo;
	pBlackboard->GetData("KnownHouses", pHousesInfo);
	return (pHousesInfo->GetNrUnexploredHouses() <= 0);
}

bool ToLootSeek::ToTransition(Elite::Blackboard* pBlackboard) const
{
	return false;
}
