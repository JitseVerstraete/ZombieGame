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


class GoToHouse : public Elite::FSMState
{
public:
	GoToHouse() = default;
	virtual ~GoToHouse() = default;

	virtual void OnEnter(Elite::Blackboard* pBlackboard) override;
	virtual void OnExit(Elite::Blackboard* pBlackboard) override;
	virtual void Update(Elite::Blackboard* pBlackboard, float dt) override;
};

//-----------
//TRANSITIONS
//-----------

class UnexploredHouseFound : public Elite::FSMTransition
{
public:
	UnexploredHouseFound() = default;
	virtual ~UnexploredHouseFound() = default;
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const;
};