//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();


	if (pAgent->CanRenderBehavior())
	{
		//DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0.f,1.f,0.f,1.f });
	}


	return steering;
}

//FLEE
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = { Seek::CalculateSteering(deltaT, pAgent) };
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
SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering{ Seek::CalculateSteering(deltaT, pAgent) };

	Elite::Vector2 toTarget{ m_Target.Position - pAgent->GetPosition() };
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
SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	pAgent->SetAutoOrient(false);
	SteeringOutput steering{};

	Elite::Vector2 toTarget{ m_Target.Position - pAgent->GetPosition() };


	float currentAngle{ pAgent->GetOrientation() - float(M_PI) / 2 };
	Elite::Vector2 forward{ cos(currentAngle), sin(currentAngle) };

	steering.AngularVelocity = Elite::AngleBetween(forward, toTarget) * 2;


	return steering;
}

SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{

	if ((pAgent->GetDirection().x < 0.01f && pAgent->GetDirection().x > -0.01f) && (pAgent->GetDirection().y < 0.01f && pAgent->GetDirection().y > -0.01f))
	{
		pAgent->SetLinearVelocity({ 0.f,1.f });
	}


	//add random offset to angle
	m_currentAngle += (float(rand() % 200 - 100) / 100.f) * m_MaxAngleChange;


	//calculate target point on circle
	Elite::Vector2 targetPoint{};
	Elite::Vector2 dir{ pAgent->GetDirection() };
	Elite::Vector2 perpDir{ dir.y, -dir.x };


	targetPoint = pAgent->GetPosition() +
		m_WanderDistance * dir +
		sin(m_currentAngle) * dir * m_WanderRadius +
		cos(m_currentAngle) * perpDir * m_WanderRadius;

	
	Elite::Vector2 toTarget{ targetPoint - pAgent->GetPosition() };

	SetTarget(TargetData(targetPoint));



	SteeringOutput steering{ Seek::CalculateSteering(deltaT, pAgent) };

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition() + dir * m_WanderDistance, m_WanderRadius, { 1.f, 0.f, 0.f, 1.f }, 0);
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), toTarget, toTarget.Magnitude(), { 1.f, 0.f, 0.f, 1.f }, 0);
		DEBUGRENDERER2D->DrawPoint(targetPoint, 5, { 1.f, 0.f, 0.f, 1.f }, 0);
	}



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

SteeringOutput Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	Elite::Vector2 interceptionPoint{(m_Target.Position + (m_Target.LinearVelocity * deltaT * 10.f)) };

	steering.LinearVelocity = interceptionPoint - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	return steering;
}

SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{


	float distanceToTarget = Elite::Distance(pAgent->GetPosition(), m_Target.Position);
	if (distanceToTarget > m_FleeRadius)
	{
		return SteeringOutput(Elite::ZeroVector2, 0, false);

	}

	DEBUGRENDERER2D->DrawPoint(m_Target.Position + m_Target.LinearVelocity * deltaT, 5.f, { 1,1,0 }, 0.f);
	 

	SteeringOutput steering{ Pursuit::CalculateSteering(deltaT, pAgent) };
	steering.LinearVelocity = -steering.LinearVelocity;

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), m_FleeRadius, { 0,1,0 }, 0);
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, { 1,1,0 }, 0.f);

	}
	return steering;
}

void Evade::SetFleeRadius(float radius)
{
	m_FleeRadius = radius;
	return;
}
