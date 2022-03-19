#pragma once

#include "iCommand.h"
#include "cMesh.h"

// This is given a relative location and time,
//	then calculates the velocity to get there.
// Then it sets the velocity, and a timer, and waits.

class cCmd_MoveRelative :
	public iCommand
{
	cMesh* objectToMove;
	glm::vec3 destination;
	glm::vec3 direction;
	float time;
	float totalTime;
	float speed;
public:
	cCmd_MoveRelative();
	// Because polymorphic (virtual) inheritance, need virtual destructor
	virtual ~cCmd_MoveRelative() {};

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
};