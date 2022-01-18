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

class ToHouseSeek : public Elite::FSMTransition
{
public:
	ToHouseSeek() = default;
	virtual ~ToHouseSeek() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};

class ToExploring : public Elite::FSMTransition
{
public:
	ToExploring() = default;
	virtual ~ToExploring() = default;
	virtual bool ToTransition(Elite::Blackboard * pBlackboard) const;
};

class ToLootSeek : public Elite::FSMTransition
{
public:
	ToLootSeek() = default;
	virtual ~ToLootSeek() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};