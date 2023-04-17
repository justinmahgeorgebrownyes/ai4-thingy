#include "PlayerDetectedCondition.h"

PlayerDetectedCondition::PlayerDetectedCondition(Agent* agent, const bool detected) : ConditionNode(agent)
{
	m_name = "Player Detected Condition";
	SetDetected(detected);
}

PlayerDetectedCondition::~PlayerDetectedCondition()
= default;

void PlayerDetectedCondition::SetDetected(const bool state)
{
	if(this != nullptr)
	m_isDetected = state;
}

bool PlayerDetectedCondition::Condition()
{
	return m_isDetected;
}
