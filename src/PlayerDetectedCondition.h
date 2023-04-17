#pragma once
#ifndef __PLAYER_DETECTED_CONDITION__
#define __PLAYER_DETECTED_CONDITION__

#include "ConditionNode.h"

class PlayerDetectedCondition : public ConditionNode
{
public:
	PlayerDetectedCondition(Agent* agent, bool detected = false);
	virtual ~PlayerDetectedCondition();

	// Setter
	bool GetDetected() const;
	void SetDetected(bool state);

	virtual bool Condition() override;
private:
	bool m_isDetected{};
};
#endif /* defined (__PLAYER_DETECTED_CONDITION__) */