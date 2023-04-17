#pragma once
#ifndef __RANGED_COMBAT_ENEMY__
#define __RANGED_COMBAT_ENEMY__

#include "Agent.h"
#include "DecisionTree.h"
class RangedCombatEnemy : public Agent
{
public:
	// Constructor Function
	RangedCombatEnemy(Scene* scene);

	// Destructor Function
	~RangedCombatEnemy();

	// LifeCycle Functions
	void Draw() override;
	void Update() override;
	void Clean() override;

	// getters and setters (accessors and mutators)
	float GetMaxSpeed() const;
	float GetTurnRate() const;
	float GetAccelerationRate() const;
	glm::vec2 GetDesiredVelocity() const;

	void SetMaxSpeed(float speed);
	void SetTurnRate(float angle);
	void SetAccelerationRate(float rate);
	void SetDesiredVelocity(glm::vec2 target_position);

	// public functions
	void Seek();
	void LookWhereYoureGoing(glm::vec2 target_direction, bool direction_hack = true);
	void Reset();

	//7.2
	void Patrol() override;
	void MoveToRange() override;
	

	//7.3
	void Flee() override;
	void MoveToLOS() override;
	void MoveToCover() override;
	void WaitBehindCover() override;
	void Attack() override;

	DecisionTree* GetTree() const;

private:
	// private movement variables
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	glm::vec2 m_startPosition;

	// where we want to go
	glm::vec2 m_desiredVelocity;

	// private function
	void m_move();

	// New for Lab 7.1
	std::vector<glm::vec2> m_patrolPath;
	int m_wayPoint;
	void m_buildPatrolPath();

	//7.2
	DecisionTree* m_tree;
	void m_buildTree();

	//lab 8
	int m_fireCounter; // num frames epals
	int m_fireCounterMax; //framge delay
	Scene* m_pScene;
};

#endif /* defined (__RANGED_COMBAT_ENEMY__)*/

