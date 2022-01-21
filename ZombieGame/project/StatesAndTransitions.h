#pragma once
#include "EliteDecisionMaking\EliteFiniteStateMachine\EFiniteStateMachine.h"


#pragma region Movement

//------
//STATES
//------

class Exploring : public Elite::FSMState
{
public:

	Exploring() = default;
	virtual ~Exploring() = default;

	virtual void OnEnter(Elite::Blackboard* pBlackboard) override;
	virtual void OnExit(Elite::Blackboard* pBlackboard) override;
	virtual void Update(Elite::Blackboard* pBlackboard, float dt) override;
};


class HouseSeek : public Elite::FSMState
{
public:
	HouseSeek() = default;
	virtual ~HouseSeek() = default;

	virtual void OnEnter(Elite::Blackboard* pBlackboard) override;
	virtual void OnExit(Elite::Blackboard* pBlackboard) override;
	virtual void Update(Elite::Blackboard* pBlackboard, float dt) override;
};



class LootSeek : public Elite::FSMState
{
public:
	LootSeek() = default;
	virtual ~LootSeek() = default;

	virtual void OnEnter(Elite::Blackboard* pBlackboard) override;
	virtual void OnExit(Elite::Blackboard* pBlackboard) override;
	virtual void Update(Elite::Blackboard* pBlackboard, float dt) override;
};


//-----------
//TRANSITIONS
//-----------

class ExploreToHouseSeek : public Elite::FSMTransition
{
public:
	ExploreToHouseSeek() = default;
	virtual ~ExploreToHouseSeek() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};

class LootSeekToHouseSeek : public Elite::FSMTransition
{
public:
	LootSeekToHouseSeek() = default;
	virtual ~LootSeekToHouseSeek() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};


class HouseSeekToExploring : public Elite::FSMTransition
{
public:
	HouseSeekToExploring() = default;
	virtual ~HouseSeekToExploring() = default;
	virtual bool ToTransition(Elite::Blackboard * pBlackboard) const;
};

class ToLootSeek : public Elite::FSMTransition
{
public:
	ToLootSeek() = default;
	virtual ~ToLootSeek() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};

#pragma endregion Movement


#pragma region Rotation

//------
//STATES
//------

class FaceTargetState : public Elite::FSMState
{
public:

	FaceTargetState() = default;
	virtual ~FaceTargetState() = default;

	virtual void OnEnter(Elite::Blackboard* pBlackboard) override;
	virtual void OnExit(Elite::Blackboard* pBlackboard) override;
	virtual void Update(Elite::Blackboard* pBlackboard, float dt) override;
};


class RadarState : public Elite::FSMState
{
public:

	RadarState() = default;
	virtual ~RadarState() = default;

	virtual void OnEnter(Elite::Blackboard* pBlackboard) override;
	virtual void OnExit(Elite::Blackboard* pBlackboard) override;
	virtual void Update(Elite::Blackboard* pBlackboard, float dt) override;
};


class AimState : public Elite::FSMState
{
public:

	AimState() = default;
	virtual ~AimState() = default;

	virtual void OnEnter(Elite::Blackboard* pBlackboard) override;
	virtual void OnExit(Elite::Blackboard* pBlackboard) override;
	virtual void Update(Elite::Blackboard* pBlackboard, float dt) override;
};




//-----------
//TRANSITIONS
//-----------

class FaceTargetToRadar : public Elite::FSMTransition
{
public:
	FaceTargetToRadar() = default;
	virtual ~FaceTargetToRadar() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};

class RadarToFaceTarget : public Elite::FSMTransition
{
public:
	RadarToFaceTarget() = default;
	virtual ~RadarToFaceTarget() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};

class AimToRadar : public Elite::FSMTransition
{
public:
	AimToRadar() = default;
	virtual ~AimToRadar() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};

class ToAim : public Elite::FSMTransition
{
public:
	ToAim() = default;
	virtual ~ToAim() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};

#pragma endregion Rotation