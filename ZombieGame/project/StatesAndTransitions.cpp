#include "stdafx.h"
#include "StatesAndTransitions.h"
#include "KnownHousesInfo.h"


//EXPLORING

void Exploring::OnEnter(Elite::Blackboard* pBlackboard)
{
	std::cout << "Enter Exploring State\n";
}

void Exploring::OnExit(Elite::Blackboard* pBlackboard)
{
	std::cout << "Exit Exploring State\n";
}

void Exploring::Update(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << "Update Exploring State\n";
}


//GO TO HOUSE

void GoToHouse::OnEnter(Elite::Blackboard* pBlackboard)
{
	std::cout << "Enter GoToHouse State\n";
}


void GoToHouse::OnExit(Elite::Blackboard* pBlackboard)
{
	std::cout << "Exit GoToHouse State\n";
}

void GoToHouse::Update(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << "Update GoToHouse State\n";
}

bool UnexploredHouseFound::ToTransition(Elite::Blackboard* pBlackboard) const
{
	//using the blackboard, check if there is an unexplored house known
	KnownHousesInfo* pHousesInfo;
	pBlackboard->GetData("KnownHouses", pHousesInfo);
	return (pHousesInfo->GetNrHouses() > 0);
}
