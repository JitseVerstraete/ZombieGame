#pragma once
#include "EliteDecisionMaking\EliteFiniteStateMachine\EFiniteStateMachine.h"


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