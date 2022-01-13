#include "stdafx.h"
#include "CombinedSteeringBehaviors.h"
#include <algorithm>


BlendedSteering::BlendedSteering(vector<WeightedBehavior> weightedBehaviors)
	:m_WeightedBehaviors(weightedBehaviors)
{
};

//****************
//BLENDED STEERING
SteeringOutput BlendedSteering::CalculateSteering(float deltaT, const AgentInfo& aInfo)
{
	SteeringOutput blendedSteering = {};
	auto totalWeight = 0.f;

	for (auto weightedBehavior : m_WeightedBehaviors)
	{
		auto steering = weightedBehavior.pBehavior->CalculateSteering(deltaT, aInfo);
		blendedSteering.LinearVelocity += weightedBehavior.weight * steering.LinearVelocity;
		blendedSteering.AngularVelocity += weightedBehavior.weight * steering.AngularVelocity;

		totalWeight += weightedBehavior.weight;
	}

	if (totalWeight > 0.f)
	{
		auto scale = 1.f / totalWeight;
		blendedSteering.LinearVelocity *= scale;

	}


	return blendedSteering;
}

//*****************
//PRIORITY STEERING
SteeringOutput PrioritySteering::CalculateSteering(float deltaT, const AgentInfo& aInfo)
{
	SteeringOutput steering = {};

	for (auto pBehavior : m_PriorityBehaviors)
	{
		steering = pBehavior->CalculateSteering(deltaT, aInfo);

		if (steering.IsValid)
			break;
	}

	//If non of the behavior return a valid output, last behavior is returned
	return steering;
}