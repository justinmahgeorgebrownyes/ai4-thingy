#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png",
		"spritesheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("spritesheet"));

	// set frame width
	SetWidth(53);

	// set frame height
	SetHeight(58);

//	TextureManager::Instance().Load("../Assets/textures/enterprise_small.png","target");

	const auto size = TextureManager::Instance().GetTextureSize("target");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(100.0f, 100.0f);
	GetRigidBody()->velocity = glm::vec2(0, 0);
	GetRigidBody()->isColliding = false;

	SetType(GameObjectType::TARGET);

	BuildAnimations();

}

Target::~Target()
= default;

void Target::Draw()
{
	// draw the target
	//TextureManager::Instance().Draw("target", GetTransform()->position, 0, 255, true);

	switch (m_currentAnimationState)
	{
	case PlayerAnimationState::PLAYER_IDLE_RIGHT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idle"),
			GetTransform()->position, 0.12f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_LEFT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idle"),
			GetTransform()->position, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PlayerAnimationState::PLAYER_RUN_RIGHT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("run"),
			GetTransform()->position, 0.25f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_RUN_LEFT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("run"),
			GetTransform()->position, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}
}

void Target::Update()
{
	Move();
	CheckBounds();
}

void Target::Clean()
{
}

void Target::Move()
{
	GetTransform()->position = GetTransform()->position + GetRigidBody()->velocity * 5.0f;
}

void Target::CheckBounds()
{
}

void Target::Reset()
{
}

void Target::SetCurrentDirection(const glm::vec2 new_direction)
{
	m_currentDirection = new_direction;
}

glm::vec2 Target::GetCurrentDirection() const
{
	return m_currentDirection;
}

void Target::SetAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Target::BuildAnimations()
{
	auto idle_animation = Animation();

	idle_animation.name = "idle";
	idle_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-idle-0"));
	idle_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-idle-1"));
	idle_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-idle-2"));
	idle_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-idle-3"));

	SetAnimation(idle_animation);

	auto run_animation = Animation();

	run_animation.name = "run";
	run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-0"));
	run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-1"));
	run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-2"));
	run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-3"));

	SetAnimation(run_animation);
}
