#include "RangedCombatEnemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"
#include "EventManager.h"
#include "PatrolAction.h"
#include "MoveToLOSAction.h"
#include "AttackAction.h"
#include "MoveToRangeAction.h"
#include "MoveToCoverAction.h"
#include "WaitBehindCoverAction.h"
#include "FleeAction.h"

RangedCombatEnemy::RangedCombatEnemy(Scene* scene) : m_maxSpeed(20.0f),
m_turnRate(5.0f), m_accelerationRate(2.0f), m_startPosition(glm::vec2(300.0f, 500.0f)),
m_fireCounter(0), m_fireCounterMax(60), m_pScene(scene)
{
	TextureManager::Instance().Load("../Assets/textures/leOdoge.png", "ranged_combat_enemy");

	const auto size = TextureManager::Instance().GetTextureSize("ranged_combat_enemy");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->bounds = glm::vec2(GetWidth(), GetHeight());
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	setIsCentered(true);
	SetType(GameObjectType::AGENT);

	
	SetCurrentHeading(0.0f); // Current facing angle
	SetLOSDistance(400.0f);
	SetWhiskerAngle(45.0f);
	SetLOSColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Default LOS Colour = Red

	// New for Lab 7.1
	SetActionState(ActionState::NO_ACTION);
	m_buildPatrolPath();

	//7.2
	m_tree = new DecisionTree(this);
	m_buildTree();
	m_tree->Display();

	SetType(GameObjectType::SPACE_SHIP);

}

RangedCombatEnemy::~RangedCombatEnemy()
= default;

void RangedCombatEnemy::Draw()
{
	// draw the RangedCombatEnemy
	TextureManager::Instance().Draw("ranged_combat_enemy", 
		GetTransform()->position, static_cast<double>(GetCurrentHeading()), 255, true);

	if (EventManager::Instance().IsIMGUIActive()) {
		//draw the los line
		Util::DrawLine(GetTransform()->position + GetCurrentDirection() * 0.5f * static_cast<float>(GetWidth()), GetMiddleLOSEndPoint(), GetLOSColour());
	}
}

void RangedCombatEnemy::Update()
{
	// Determine which action to perform
	m_tree->MakeDecision();
}

void RangedCombatEnemy::Clean()
{
}

float RangedCombatEnemy::GetMaxSpeed() const
{
	return m_maxSpeed;
}

float RangedCombatEnemy::GetTurnRate() const
{
	return m_turnRate;
}

float RangedCombatEnemy::GetAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 RangedCombatEnemy::GetDesiredVelocity() const
{
	return m_desiredVelocity;
}

void RangedCombatEnemy::SetMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void RangedCombatEnemy::SetTurnRate(const float angle)
{
	m_turnRate = angle;
}

void RangedCombatEnemy::SetAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void RangedCombatEnemy::SetDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::Normalize(target_position - GetTransform()->position);
}

void RangedCombatEnemy::Seek()
{
	// New for Lab 7.1
	// Find Next Waypoint if within 10px of the current waypoint

	if(Util::Distance(m_patrolPath[m_wayPoint], GetTransform()->position) < 10)
	{
		// check to see if you are at the last point in the path
		if(++m_wayPoint == m_patrolPath.size())
		{
			// if so..reset
			m_wayPoint = 0;
		}
		SetTargetPosition(m_patrolPath[m_wayPoint]);
	}

	SetDesiredVelocity(GetTargetPosition());

	const glm::vec2 steering_direction = GetDesiredVelocity() - GetCurrentDirection();

	LookWhereYoureGoing(steering_direction);

	GetRigidBody()->acceleration = GetCurrentDirection() * GetAccelerationRate();
}

void RangedCombatEnemy::LookWhereYoureGoing(const glm::vec2 target_direction, const bool direction_hack)
{




	float target_rotation = Util::SignedAngle(GetCurrentDirection(), target_direction) - ((direction_hack) ? 90.0f : 0.0f); //-90.0f

	float last_rotation = target_rotation;


	const float turn_sensitivity = 3.0f;

	if (target_rotation < 0) {
		target_rotation += 180.0f;
	}

	if (target_rotation < 176.0f && target_rotation < 184.0f) {
		target_rotation = last_rotation;
	}


	if(GetCollisionWhiskers()[0] || GetCollisionWhiskers()[1] || GetCollisionWhiskers()[2])
	{
		target_rotation += GetTurnRate() * turn_sensitivity;
	}
	else if(GetCollisionWhiskers()[3] || GetCollisionWhiskers()[4])
	{
		target_rotation -= GetTurnRate() * turn_sensitivity;
	}

	SetCurrentHeading(Util::LerpUnclamped(GetCurrentHeading(), 
		GetCurrentHeading() + target_rotation, GetTurnRate() * Game::Instance().GetDeltaTime()));

	UpdateWhiskers(GetWhiskerAngle());
}

void RangedCombatEnemy::Reset()
{
	GetTransform()->position = m_startPosition;
}

void RangedCombatEnemy::Patrol()
{
	if (GetActionState() != ActionState::PATROL) {
		SetActionState(ActionState::PATROL);
	}
	m_move();
}

void RangedCombatEnemy::MoveToRange()
{
	if (GetActionState() != ActionState::MOVE_TO_RANGE)
	{

		// Initialize the Action
		SetActionState(ActionState::MOVE_TO_RANGE);
		//TODO: setup another action to take when moving to the player
	}
}

void RangedCombatEnemy::Flee()
{
	if (GetActionState() != ActionState::FLEE)
	{

		// Initialize the Action
		SetActionState(ActionState::FLEE);
		//TODO: setup another action to take when moving to the player
	}
}

void RangedCombatEnemy::MoveToLOS()
{
	if (GetActionState() != ActionState::MOVE_TO_LOS)
	{

		// Initialize the Action
		SetActionState(ActionState::MOVE_TO_LOS);
		//TODO: setup another action to take when moving to the player
	}
}

void RangedCombatEnemy::MoveToCover()
{
	if (GetActionState() != ActionState::MOVE_TO_COVER)
	{

		// Initialize the Action
		SetActionState(ActionState::MOVE_TO_COVER);
		//TODO: setup another action to take when moving to the player
	}
}

void RangedCombatEnemy::WaitBehindCover()
{
	if (GetActionState() != ActionState::WAIT_BEHIND_COVER)
	{

		// Initialize the Action
		SetActionState(ActionState::WAIT_BEHIND_COVER);
		//TODO: setup another action to take when moving to the player
	}
}

void RangedCombatEnemy::Attack()
{

	auto scene = dynamic_cast<PlayScene*>(m_pScene);

	if (GetActionState() != ActionState::ATTACK)
	{

		// Initialize the Action
		SetActionState(ActionState::ATTACK);
		//TODO: setup another action to take when moving to the player
	}

	//lab 8
	glm::vec2 target_direction = Util::Normalize(scene->GetTarget()->GetTransform()->position - GetTransform()->position);
	LookWhereYoureGoing(target_direction, false);

	//wait for number of frames firintg = frame dealay
	if (m_fireCounter++ % m_fireCounterMax == 0) {
		scene->SpawnEnemyTorpedo();
	}
}

DecisionTree* RangedCombatEnemy::GetTree() const
{
	if (this != nullptr) {
		return m_tree;
	}
}

void RangedCombatEnemy::m_move()
{
	Seek();

	//                      final Position  Position Term   Velocity      Acceleration Term
	// Kinematic Equation-> Pf            = Pi +            Vi * (time) + (0.5) * Ai * (time * time)

	const float dt = Game::Instance().GetDeltaTime();

	// accessing the position Term
	const glm::vec2 initial_position = GetTransform()->position;

	// compute the velocity Term
	const glm::vec2 velocity_term = GetRigidBody()->velocity * dt;

	// compute the acceleration Term
	const glm::vec2 acceleration_term = GetRigidBody()->acceleration * 0.5f; // * dt * dt


	// compute the new position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	GetTransform()->position = final_position;

	// add our acceleration to velocity
	GetRigidBody()->velocity += GetRigidBody()->acceleration;

	// clamp our velocity at max speed
	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());
}

void RangedCombatEnemy::m_buildPatrolPath()
{
	m_patrolPath.emplace_back(760, 40); // Top-Right Corner of the Screen
	m_patrolPath.emplace_back(760, 560); // Bottom-Right Corner of the Screen
	m_patrolPath.emplace_back(40, 560); // Bottom-Leeft Corner of the Screen
	m_patrolPath.emplace_back(40, 40); // Top-Left Corner of the Screen
	SetTargetPosition(m_patrolPath[m_wayPoint]);
}

void RangedCombatEnemy::m_buildTree()
{

	if (this != nullptr) {
		//root level 0
		m_tree->SetEnemyHealthNode(new EnemyHealthCondition(this));
		m_tree->GetTree().push_back(m_tree->GetEnemyHealthNode());


		//left level 1
		TreeNode* fleeNode = m_tree->AddNode(m_tree->GetEnemyHealthNode(), new FleeAction(this), TreeNodeType::LEFT_TREE_NODE);
		m_tree->GetTree().push_back(fleeNode);

		//right level 1
		m_tree->SetEnemyHitNode(new EnemyHitCondition(this));
		m_tree->AddNode(m_tree->GetEnemyHealthNode(), m_tree->GetEnemyHitNode(), TreeNodeType::RIGHT_TREE_NODE);
		m_tree->GetTree().push_back(m_tree->GetEnemyHitNode());

		//left level 2
		m_tree->SetPlayerDetectedNode(new PlayerDetectedCondition(this));
		m_tree->AddNode(m_tree->GetEnemyHitNode(), m_tree->GetPlayerDetectedNode(), TreeNodeType::LEFT_TREE_NODE);
		m_tree->GetTree().push_back(m_tree->GetEnemyHitNode());


		//right level 2
		LOSCondition* LOSNodeRight = new LOSCondition(this);
		m_tree->AddNode(m_tree->GetEnemyHitNode(), LOSNodeRight, TreeNodeType::RIGHT_TREE_NODE);
		m_tree->GetTree().push_back(LOSNodeRight);


		//patrol action level 3 left left
		TreeNode* patrolNode = m_tree->AddNode(m_tree->GetPlayerDetectedNode(), new PatrolAction(this), TreeNodeType::LEFT_TREE_NODE);
		//dynamic_cast<ActionNode*>(patrolNode)->SetAgent(this);
		m_tree->GetTree().push_back(patrolNode);


		//left right subtree level 3
		LOSCondition* LOSNodeLeft = new LOSCondition(this);
		m_tree->AddNode(m_tree->GetPlayerDetectedNode(), LOSNodeLeft, TreeNodeType::RIGHT_TREE_NODE);
		m_tree->GetTree().push_back(LOSNodeLeft);

		//right left subtree of level 3
		TreeNode* waitBehindCoverNode = m_tree->AddNode(LOSNodeRight, new WaitBehindCoverAction(this), TreeNodeType::LEFT_TREE_NODE);
		//dynamic_cast<ActionNode*>(patrolNode)->SetAgent(this);
		m_tree->GetTree().push_back(waitBehindCoverNode);


		//right right level 3
		TreeNode* moveToCoverNode = m_tree->AddNode(LOSNodeRight, new MoveToCoverAction(this), TreeNodeType::RIGHT_TREE_NODE);
		//dynamic_cast<ActionNode*>(patrolNode)->SetAgent(this);
		m_tree->GetTree().push_back(moveToCoverNode);

		//left level 4
		TreeNode* moveToLOSNode = m_tree->AddNode(LOSNodeLeft, new MoveToLOSAction(this), TreeNodeType::LEFT_TREE_NODE);
		//dynamic_cast<ActionNode*>(moveToLOSNode)->SetAgent(this);
		m_tree->GetTree().push_back(moveToLOSNode);

		//right level 4
		m_tree->SetRangedCombatNode(new RangedCombatCondition(this));
		m_tree->AddNode(LOSNodeLeft, m_tree->GetRangedCombatNode(), TreeNodeType::RIGHT_TREE_NODE);
		m_tree->GetTree().push_back(m_tree->GetRangedCombatNode());


		//left level 5
		TreeNode* moveToRangeNode = m_tree->AddNode(m_tree->GetRangedCombatNode(), new MoveToRangeAction(this), TreeNodeType::LEFT_TREE_NODE);
		//dynamic_cast<ActionNode*>(moveToRangeNode)->SetAgent(this);
		m_tree->GetTree().push_back(moveToRangeNode);


		//right level 5
		TreeNode* attackNode = m_tree->AddNode(m_tree->GetRangedCombatNode(), new AttackAction(this), TreeNodeType::RIGHT_TREE_NODE);
		//dynamic_cast<ActionNode*>(attackNode)->SetAgent(this);
		m_tree->GetTree().push_back(attackNode);
	}


	


	// Conditions

	// Create and Add the Root Node
	//m_tree->SetLOSNode(new LOSCondition());
	//
	//m_tree->GetTree().push_back(m_tree->GetLOSNode());

	//
	//m_tree->SetRadiusNode(new RadiusCondition());
	//m_tree->AddNode(m_tree->GetLOSNode(), m_tree->GetRadiusNode(), TreeNodeType::RIGHT_TREE_NODE);
	//m_tree->GetTree().push_back(m_tree->GetRadiusNode());

	//m_tree->SetCloseCombatNode(new CloseCombatCondition());
	//m_tree->AddNode(m_tree->GetLOSNode(), m_tree->GetCloseCombatNode(), TreeNodeType::RIGHT_TREE_NODE);
	//m_tree->GetTree().push_back(m_tree->GetCloseCombatNode());

	//m_tree->SetRangedCombatNode(new RangedCombatCondition());
	//m_tree->AddNode(m_tree->GetLOSNode(), m_tree->GetRangedCombatNode(), TreeNodeType::RIGHT_TREE_NODE);
	//m_tree->GetTree().push_back(m_tree->GetRangedCombatNode());

	//// Actions

	//// Left Sub-Tree
	//TreeNode* patrolNode = m_tree->AddNode(m_tree->GetRadiusNode(), new PatrolAction(), TreeNodeType::LEFT_TREE_NODE);
	//dynamic_cast<ActionNode*>(patrolNode)->SetAgent(this);
	//m_tree->GetTree().push_back(patrolNode);

	//TreeNode* moveToLOSNode = m_tree->AddNode(m_tree->GetRadiusNode(), new MoveToLOSAction(), TreeNodeType::RIGHT_TREE_NODE);
	//dynamic_cast<ActionNode*>(moveToLOSNode)->SetAgent(this);
	//m_tree->GetTree().push_back(moveToLOSNode);

	//// Right Sub-Tree

	//TreeNode* moveToRangeNode = m_tree->AddNode(m_tree->GetRangedCombatNode(), new MoveToRangeAction(), TreeNodeType::LEFT_TREE_NODE);
	//dynamic_cast<ActionNode*>(moveToRangeNode)->SetAgent(this);
	//m_tree->GetTree().push_back(moveToRangeNode);


	//TreeNode* attackNode = m_tree->AddNode(m_tree->GetRangedCombatNode(), new AttackAction(), TreeNodeType::RIGHT_TREE_NODE);
	//dynamic_cast<ActionNode*>(attackNode)->SetAgent(this);
	//m_tree->GetTree().push_back(attackNode);



}
