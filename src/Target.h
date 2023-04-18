#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "Agent.h"
#include "PlayerAnimationState.h"
#include "Sprite.h"

class Target final : public Sprite {
public:
	Target();
	~Target() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	void SetCurrentDirection(const glm::vec2 new_direction);
	glm::vec2 GetCurrentDirection() const;

	void SetAnimationState(PlayerAnimationState new_state);

private:
	void BuildAnimations();
	PlayerAnimationState m_currentAnimationState;


	glm::vec2 m_currentDirection;
	void Move();
	void CheckBounds();
	void Reset();
	
	
};


#endif /* defined (__TARGET__) */