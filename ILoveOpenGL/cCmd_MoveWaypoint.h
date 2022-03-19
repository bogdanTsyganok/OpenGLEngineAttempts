#pragma once

#include "cCmd_MoveRelative.h"
#include "iCommand.h"
#include "cMesh.h"

// This is given a relative location and time,
//	then calculates the velocity to get there.
// Then it sets the velocity, and a timer, and waits.

class cCmd_MoveWaypoint :
	public iCommand
{
	std::vector<cCmd_MoveRelative*> serialMoves;
	bool isDone = false;

public:
	cCmd_MoveWaypoint();
	// Because polymorphic (virtual) inheritance, need virtual destructor
	virtual ~cCmd_MoveWaypoint() {};

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
};