#pragma once
#ifndef __TORPEDO_K__
#define __TORPEDO_K__

#include "TorpedoAnimationState.h"
#include "Sprite.h"

class TorpedoK final : public Sprite
{
public:
	TorpedoK(float speed = 0.0f, glm::vec2 direction = {0,0});
	~TorpedoK() override;

	// Life Cycle Methods
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	// setters
	void SetAnimationState(TorpedoAnimationState new_state);

private:
	void BuildAnimations();

	TorpedoAnimationState m_currentAnimationState;
	float m_speed;

	glm::vec2 m_direction;
};

#endif /* defined (__TORPEDO_K__) */