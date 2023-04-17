#pragma once
#ifndef __FLEE_ACTION__
#define __FLEE_ACTION__

#include "ActionNode.h"

class FleeAction : public ActionNode
{
public:
	FleeAction(Agent* agent);
	virtual ~FleeAction();

	virtual void Action() override; // Define our action here
};

#endif /* defined (__FLEE_ACTION__) */