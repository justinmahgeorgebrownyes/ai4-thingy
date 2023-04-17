#include "Torpedo.h"
#include "TextureManager.h"

Torpedo::Torpedo(const float speed): m_currentAnimationState(TorpedoAnimationState::FIRED), m_speed(speed)
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torpedo.png", 
		"torpedosheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("torpedosheet"));
	
	// set frame width
	SetWidth(64);

	// set frame height
	SetHeight(64);

	GetTransform()->position = glm::vec2(400.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	SetType(GameObjectType::PROJECTILE);

	BuildAnimations();
}

Torpedo::~Torpedo()
= default;

void Torpedo::Draw()
{
	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case TorpedoAnimationState::FIRED:
		TextureManager::Instance().PlayAnimation("torpedosheet", GetAnimation("fired"),
			GetTransform()->position, 5, 0, 255, true);
		break;
	default:
		break;
	}
}

void Torpedo::Update()
{
	GetTransform()->position.x += m_speed;
}

void Torpedo::Clean()
{
}

void Torpedo::SetAnimationState(const TorpedoAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Torpedo::BuildAnimations()
{
	auto fired_animation = Animation();

	fired_animation.name = "fired";
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired1"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired2"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired3"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired4"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired5"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired6"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired7"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired8"));

	SetAnimation(fired_animation);
 
}
