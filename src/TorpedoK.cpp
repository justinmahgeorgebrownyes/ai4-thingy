#include "TorpedoK.h"
#include "TextureManager.h"

TorpedoK::TorpedoK(const float speed, glm::vec2 direction): m_currentAnimationState(TorpedoAnimationState::FIRED), m_speed(speed)
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torpedo_k.png", 
		"torpedoKsheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("torpedoKsheet"));
	
	// set frame width
	SetWidth(64);

	// set frame height
	SetHeight(64);

	GetTransform()->position = glm::vec2(400.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	SetType(GameObjectType::PROJECTILE);

	m_direction = { direction.x * speed, direction.y * speed };

	BuildAnimations();
}

TorpedoK::~TorpedoK()
= default;

void TorpedoK::Draw()
{
	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case TorpedoAnimationState::FIRED:
		TextureManager::Instance().PlayAnimation("torpedoKsheet", GetAnimation("fired"),
			GetTransform()->position, 5, 0, 255, true);
		break;
	default:
		break;
	}
}

void TorpedoK::Update()
{
	GetTransform()->position += m_direction;
}

void TorpedoK::Clean()
{
}

void TorpedoK::SetAnimationState(const TorpedoAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void TorpedoK::BuildAnimations()
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
