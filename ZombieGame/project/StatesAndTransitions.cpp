#include "stdafx.h"
#include "StatesAndTransitions.h"


#include "ExplorationGrid.h"
#include "KnownHousesInfo.h"
#include "ItemsManager.h"
#include <IExamInterface.h>
#include "Steering\SteeringBehaviors.h"

#include <math.h>


#pragma region Movement

//================
//STATES
//================

//EXPLORING

void Exploring::OnEnter(Elite::Blackboard* pBlackboard)
{
	std::cout << "EXPLORE STATE\n";
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
	std::cout << "HOUSE SEEK STATE\n";
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
	std::cout << "LOOT SEEK STATE\n";
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
		if (pItemsManager->GetOpenSlotForItemType(eItemType(i)) >= 0 && pItemsManager->GetClosestKnownItemPosOfType(eItemType(i), aInfo.Position, tempPos))
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
	KnownHousesInfo* pKnownHouses;
	ExplorationGrid* pExplorationGrid;

	pBlackboard->GetData("ItemsManager", pItemsManager);
	pBlackboard->GetData("Interface", pInterface);
	pBlackboard->GetData("KnownHouses", pKnownHouses);
	pBlackboard->GetData("ExplorationGrid", pExplorationGrid);

	//dont transition if there is a known house at least half the distance away
	AgentInfo aInfo{ pInterface->Agent_GetInfo() };
	

	bool itemFound{ false };
	float closestItemDistance{FLT_MAX};
	Elite::Vector2 tempItemPos{};

	if (pItemsManager->GetNrPistols() > 0 && pItemsManager->GetOpenSlotForItemType(eItemType::PISTOL) >= 0)
	{
		itemFound = true;
		pItemsManager->GetClosestKnownItemPosOfType(eItemType::PISTOL, aInfo.Position, tempItemPos);

		if (!itemFound || Elite::Distance(tempItemPos, aInfo.Position) < closestItemDistance)
			closestItemDistance = Elite::Distance(tempItemPos, aInfo.Position);
	}

	if (pItemsManager->GetNrMedkits() > 0 && pItemsManager->GetOpenSlotForItemType(eItemType::MEDKIT) >= 0)
	{
		itemFound = true;
		pItemsManager->GetClosestKnownItemPosOfType(eItemType::MEDKIT, aInfo.Position, tempItemPos);

		if (!itemFound || Elite::Distance(tempItemPos, aInfo.Position) < closestItemDistance)
			closestItemDistance = Elite::Distance(tempItemPos, aInfo.Position);
	}

	if (pItemsManager->GetNrFoods() > 0 && pItemsManager->GetOpenSlotForItemType(eItemType::FOOD) >= 0)
	{
		itemFound = true;
		pItemsManager->GetClosestKnownItemPosOfType(eItemType::FOOD, aInfo.Position, tempItemPos);

		if (!itemFound || Elite::Distance(tempItemPos, aInfo.Position) < closestItemDistance)
			closestItemDistance = Elite::Distance(tempItemPos, aInfo.Position);
	}


	std::cout << "distance to item: " << closestItemDistance
		<< "\nhouse distance " << Elite::Distance(aInfo.Position, pKnownHouses->GetClosestUnexploredHouse(aInfo.Position).Center)
		<< "\ncell distance " << Elite::Distance(aInfo.Position, pExplorationGrid->GetClosestHouseCell(aInfo.Position).GetCellCenter()) << endl << endl;

	if (Elite::Distance(aInfo.Position, pKnownHouses->GetClosestUnexploredHouse(aInfo.Position).Center) < closestItemDistance / 2)
		return false;

	if (Elite::Distance(aInfo.Position, pExplorationGrid->GetClosestHouseCell(aInfo.Position).GetCellCenter()) < closestItemDistance / 3)
		return false;
	




	return itemFound;



	return false;
}



#pragma endregion Movement


#pragma region Rotation

//AUTO ORIENT
void FaceTargetState::OnEnter(Elite::Blackboard* pBlackboard)
{
	cout << "AUTO ORIENT STATE\n";
}

void FaceTargetState::OnExit(Elite::Blackboard* pBlackboard)
{
}

void FaceTargetState::Update(Elite::Blackboard* pBlackboard, float dt)
{
	SteeringOutput* pRotation;
	Face* pFace{ new Face()};
	Elite::Vector2* pTarget{};
	IExamInterface* pInterface{};

	pBlackboard->GetData("RotationOutput", pRotation);
	pBlackboard->GetData("TargetPoint", pTarget);
	pBlackboard->GetData("Interface", pInterface);

	pFace->SetTargetInfo(TargetInfo(*pTarget, Elite::Vector2()));

	SteeringOutput steering = pFace->CalculateSteering(dt, pInterface->Agent_GetInfo());

	pRotation->AutoOrient = false;
	pRotation->AngularVelocity = steering.AngularVelocity;

	delete pFace;
}

//RADAR
void RadarState::OnEnter(Elite::Blackboard* pBlackboard)
{
	cout << "RADAR STATE\n";
}

void RadarState::OnExit(Elite::Blackboard* pBlackboard)
{
}

void RadarState::Update(Elite::Blackboard* pBlackboard, float dt)
{
	SteeringOutput* pRotation;
	Radar* pRadar;
	IExamInterface* pInterface;

	pBlackboard->GetData("RotationOutput", pRotation);
	pBlackboard->GetData("RadarBehavior", pRadar);
	pBlackboard->GetData("Interface", pInterface);


	*pRotation = pRadar->CalculateSteering(dt, pInterface->Agent_GetInfo());
	pRotation->AutoOrient = false;
}

//AIM
void AimState::OnEnter(Elite::Blackboard* pBlackboard)
{
	cout << "AIM STATE\n";
}

void AimState::OnExit(Elite::Blackboard* pBlackboard)
{
}

void AimState::Update(Elite::Blackboard* pBlackboard, float dt)
{
	SteeringOutput* pRotation{};
	AimZombie* pAim{};
	IExamInterface* pInterface{};
	ItemsManager* pItemsManager{};
	ZombieHordeInfo* pZombieHorde{};

	pBlackboard->GetData("RotationOutput", pRotation);
	pBlackboard->GetData("AimBehavior", pAim);
	pBlackboard->GetData("Interface", pInterface);
	pBlackboard->GetData("ItemsManager", pItemsManager);
	pBlackboard->GetData("ZombieHorde", pZombieHorde);



	*pRotation = pAim->CalculateSteering(dt, pInterface->Agent_GetInfo());
	pRotation->AutoOrient = false;


	// PULL THE TRIGGER IF YOU ARE AIMING AT THE CURRENT TARGET
	Elite::Vector2 v1 = Elite::OrientationToVector(pInterface->Agent_GetInfo().Orientation);
	Elite::Vector2 v2 = pAim->GetCurrentTarget() - pInterface->Agent_GetInfo().Position;


	Elite::Vector3 color{ };
	//current angle
	float angleToTarget{ abs( Elite::AngleBetween(v1, v2)) };
	float allowedAngle{ atan(0.9f / v2.Magnitude()) };

	if (angleToTarget < (allowedAngle ))
	{
		color = { 0.f, 1.f, 0.f };
		pItemsManager->UseItem(pItemsManager->GetFirstPistolIndex());
		cout << "DISTANCE TO TARGET" << v2.Magnitude() << endl;

	}
	else
	{
		color = { 1.f, 0.f, 0.f };
	}

	
	//debug aim lines
	//pInterface->Draw_Direction(pInterface->Agent_GetInfo().Position, v1, v2.Magnitude(), color, 0.f);
	//pInterface->Draw_Direction(pInterface->Agent_GetInfo().Position, v2, v2.Magnitude(), {0.f, 0.f, 1.f}, 0.f);


}

//TRANSITIONS
bool FaceTargetToRadar::ToTransition(Elite::Blackboard* pBlackboard) const
{
	RadarToFaceTarget tempTrans{};

	return !tempTrans.ToTransition(pBlackboard);
}

bool RadarToFaceTarget::ToTransition(Elite::Blackboard* pBlackboard) const
{
	const float threshold{ 16.f };
	ToLootSeek toLootSeek{};
	Elite::Vector2* pTarget;
	IExamInterface* pInterface;


	pBlackboard->GetData("TargetPoint", pTarget);
	pBlackboard->GetData("Interface", pInterface);	


	return Elite::Distance(*pTarget, pInterface->Agent_GetInfo().Position) < threshold && toLootSeek.ToTransition(pBlackboard);
}

bool AimToRadar::ToTransition(Elite::Blackboard* pBlackboard) const
{
	ToAim toAimTransition{};

	return !toAimTransition.ToTransition(pBlackboard);
}

bool ToAim::ToTransition(Elite::Blackboard* pBlackboard) const
{
	IExamInterface* pInterface{};
	ZombieHordeInfo* pZombieHorde{};
	ItemsManager* pItemsManager{};

	pBlackboard->GetData("Interface", pInterface);
	pBlackboard->GetData("ZombieHorde", pZombieHorde);
	pBlackboard->GetData("ItemsManager", pItemsManager);

	return pZombieHorde->GetEnemiesInFov().size() > 0 && pItemsManager->GetTotalAmmo();

	return false;
}

#pragma endregion Rotation