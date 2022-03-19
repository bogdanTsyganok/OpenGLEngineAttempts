#pragma once

#include "iCommand.h"
#include "cMesh.h"

// This is given a relative location and time,
//	then calculates the velocity to get there.
// Then it sets the velocity, and a timer, and waits.

class cCmd_Follow :
	public iCommand
{
	cMesh* follower;
	cMesh* target;
	glm::vec3 offset;
	float speedLimit;
public:
	cCmd_Follow();
	// Because polymorphic (virtual) inheritance, need virtual destructor
	virtual ~cCmd_Follow() {};

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
};
