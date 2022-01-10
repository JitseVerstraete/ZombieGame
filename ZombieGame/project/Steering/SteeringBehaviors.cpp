#include "stdafx.h"
//Precompiled Header [ALWAYS ON TOP IN CPP]

//Includes
#include "SteeringBehaviors.h"




//SEEK
//****
SteeringPlugin_Output Seek::CalculateSteering(float deltaT, const AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering = {};

	steering.LinearVelocity = m_TargetInfo.Location - agentInfo.Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agentInfo.MaxLinearSpeed;


	return steering;
}

//FLEE
//****
SteeringPlugin_Output Flee::CalculateSteering(float deltaT, const AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering = { Seek::CalculateSteering(deltaT, agentInfo) };
	steering.LinearVelocity = -steering.LinearVelocity;
	return steering;
}


void Arrive::SetSlowRadius(float slowRadius)
{
	m_SlowdownRadius = slowRadius;
}

void Arrive::SetTargetRadius(float targetRadius)
{
	m_TargetRadius = targetRadius;
}

//ARRIVE
//******
SteeringPlugin_Output Arrive::CalculateSteering(float deltaT, const AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering{ Seek::CalculateSteering(deltaT, agentInfo) };

	Elite::Vector2 toTarget{ m_TargetInfo.Location - agentInfo.Position };
	float distance{ toTarget.Magnitude() };

	if (distance < m_TargetRadius)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
		return steering;
	}

	if (distance > m_SlowdownRadius)
	{
		return steering;
	}
	else
	{
		steering.LinearVelocity *= distance / m_SlowdownRadius;
		return steering;
	}
}


//FACE
//****
SteeringPlugin_Output Face::CalculateSteering(float deltaT, const AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering{};

	Elite::Vector2 toTarget{ m_TargetInfo.Location - agentInfo.Position };


	float currentAngle{ agentInfo.Orientation - float(M_PI) / 2 };
	Elite::Vector2 forward{ cos(currentAngle), sin(currentAngle) };

	steering.AngularVelocity = Elite::AngleBetween(forward, toTarget) * 2;
	
	return steering;
}

SteeringPlugin_Output Wander::CalculateSteering(float deltaT, const AgentInfo& agentInfo)
{
	Elite::Vector2 agentDirection = agentInfo.LinearVelocity;

	
	if ((agentDirection.x < 0.01f && agentDirection.x > -0.01f) && (agentDirection.y < 0.01f && agentDirection.y > -0.01f))
	{
		agentDirection  = { 0.f,1.f };
	}
	


	//add random offset to angle
	m_currentAngle += (float(rand() % 200 - 100) / 100.f) * m_MaxAngleChange;


	//calculate target point on circle
	Elite::Vector2 targetPoint{};
	Elite::Vector2 dir{ agentDirection };
	Elite::Vector2 perpDir{ dir.y, -dir.x };


	targetPoint = agentInfo.Position +
		m_WanderDistance * dir +
		sin(m_currentAngle) * dir * m_WanderRadius +
		cos(m_currentAngle) * perpDir * m_WanderRadius;


	Elite::Vector2 toTarget{ targetPoint - agentInfo.Position };

	EnemyInfo toSet{};
	toSet.Location = targetPoint;
	m_TargetInfo.Location = targetPoint;



	SteeringPlugin_Output steering{ Seek::CalculateSteering(deltaT, agentInfo) };


	return steering;
}

void Wander::SetWanderDistance(float distance)
{
	m_WanderDistance = distance;
}

void Wander::SetWanderRadius(float radius)
{
	m_WanderRadius = radius;
}

void Wander::SetMaxAngleChange(float delta)
{
	m_MaxAngleChange = delta;
}

SteeringPlugin_Output Pursuit::CalculateSteering(float deltaT, const AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering = {};

	Elite::Vector2 interceptionPoint{(m_TargetInfo.Location + (m_TargetInfo.LinearVelocity * deltaT * 10.f)) };

	steering.LinearVelocity = interceptionPoint - agentInfo.Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agentInfo.MaxLinearSpeed;

	return steering;
}

SteeringPlugin_Output Evade::CalculateSteering(float deltaT, const AgentInfo& agentInfo)
{


	float distanceToTarget = Elite::Distance(agentInfo.Position , m_TargetInfo.Location);
	if (distanceToTarget > m_FleeRadius)
	{
		return SteeringPlugin_Output();

	}

	//DEBUGRENDERER2D->DrawPoint(m_Target.Position + m_Target.LinearVelocity * deltaT, 5.f, { 1,1,0 }, 0.f);
	 

	SteeringPlugin_Output steering{ Pursuit::CalculateSteering(deltaT, agentInfo) };
	steering.LinearVelocity = -steering.LinearVelocity;


	return steering;
}

void Evade::SetFleeRadius(float radius)
{
	m_FleeRadius = radius;
	return;
}
