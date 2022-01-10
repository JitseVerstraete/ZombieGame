/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../../inc/Exam_HelperStructs.h"



#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) = 0;

	//Seek Functions
	void SetTargetInfo(const EnemyInfo& target) { m_TargetInfo = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{
		return static_cast<T*>(this);
	}

protected:
	EnemyInfo m_TargetInfo;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
};

/////////////////////////
//FLEE
//****
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Flee Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;

};

//Arrive
class Arrive :public Seek
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Setters
	void SetSlowRadius(float slowRadius);
	void SetTargetRadius(float targetRadius);
	

	//Arrive Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
private:
	float m_SlowdownRadius = 15.f;
	float m_TargetRadius = 1.f;
};

class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;

};


//WAnder
class Wander : public Seek
{
public:
	Wander()
	{
		m_currentAngle = Elite::randomFloat(100.f);
	}
	virtual ~Wander() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;

	void SetWanderDistance(float distance);
	void SetWanderRadius(float radius);
	void SetMaxAngleChange(float delta);

	//setters

protected:
	//extra data members
	float m_WanderDistance = 5.f;
	float m_WanderRadius = 4.f;
	float m_MaxAngleChange = Elite::ToRadians(30);
	float m_currentAngle = Elite::ToRadians(90);
};


//Pursuit
class Pursuit : public ISteeringBehavior
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
};

class Evade : public Pursuit
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
	void SetFleeRadius(float radius);

private:
	float m_FleeRadius = 10.f;
};




#endif


