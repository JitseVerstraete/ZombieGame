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
#include "../ZombieInfo.h"

struct TargetInfo
{
	TargetInfo() {}
	TargetInfo(const Elite::Vector2& pos, const Elite::Vector2& vel) :Position{ pos }, LinearVelocity{ vel } {}
	Elite::Vector2 Position;
	Elite::Vector2 LinearVelocity;
};

struct SteeringOutput
{
	Elite::Vector2 LinearVelocity;
	float AngularVelocity;
	bool IsValid;
	bool AutoOrient;
};

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) = 0;

	//Seek Functions
	void SetTargetInfo(const TargetInfo& target) { m_TargetInfo = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{
		return static_cast<T*>(this);
	}

protected:
	TargetInfo m_TargetInfo;
};
#pragma endregion

#pragma region SimpleBehaviors
//SEEK
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
};

//FLEE
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Flee Behavior
	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;

};

//ARRIVE
class Arrive :public Seek
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Setters
	void SetSlowRadius(float slowRadius);
	void SetTargetRadius(float targetRadius);
	

	//Arrive Behavior
	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
private:
	float m_SlowdownRadius = 15.f;
	float m_TargetRadius = 1.f;
};

//FACE
class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;

	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;

};

//WANDER
class Wander : public Seek
{
public:
	Wander()
	{
		m_currentAngle = Elite::randomFloat(100.f);
	}
	virtual ~Wander() = default;

	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;

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

//PURSUIT
class Pursuit : public ISteeringBehavior
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
};

//EVADE
class Evade : public Pursuit
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
	void SetFleeRadius(float radius);

private:
	float m_FleeRadius = 10.f;
};

#pragma endregion SimpleBehaviors




#pragma region ZombieAIBehaviors

//EVADE ZOMBIES
class EvadeZombies :public ISteeringBehavior
{
public:
	EvadeZombies() = default;
	virtual ~EvadeZombies() = default;

	void SetZombieInfo(ZombieHordeInfo* info) { m_pZombieInfo = info; }
	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;

private:
	ZombieHordeInfo* m_pZombieInfo;

};

//RADAR
class Radar : public ISteeringBehavior
{
public:
	Radar() = default;
	virtual ~Radar() = default;

	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
};

//AIM ZOMBIE
class AimZombie : public Face
{
public:
	AimZombie() = default;
	virtual ~AimZombie() = default;

	void SetZombieInfo(ZombieHordeInfo* info) { m_pZombieInfo = info; }
	SteeringOutput CalculateSteering(float deltaT, const AgentInfo& agentInfo) override;
	const Elite::Vector2& GetCurrentTarget() const { return m_CurrentTarget; }

private:
	ZombieHordeInfo* m_pZombieInfo;
	Elite::Vector2 m_CurrentTarget;

};

#pragma endregion ZombieAIBehaviors





#endif


