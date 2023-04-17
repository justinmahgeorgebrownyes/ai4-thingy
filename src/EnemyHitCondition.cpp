#include "EnemyHitCondition.h"

EnemyHitCondition::EnemyHitCondition(Agent* agent, const bool hit) : ConditionNode(agent)
{
	m_name = "Enemy Hit Condition";
	SetIsHit(hit);
}

EnemyHitCondition::~EnemyHitCondition()
= default;

void EnemyHitCondition::SetIsHit(const bool state)
{
	if(this != nullptr)
	m_isHit = state;
}

bool EnemyHitCondition::Condition()
{
	return m_isHit;
}
