#include "cCmd_Follow.h"


cCmd_Follow::cCmd_Follow()
{
}

void cCmd_Follow::Initialize(sMessage startStateData)
{
	follower = (cMesh*)startStateData.voidPointers[0];
	target = (cMesh*)startStateData.voidPointers[1];
	offset = startStateData.vecData[0];
	speedLimit = startStateData.vecData[0].a;
}

void cCmd_Follow::AtStart(sMessage startStateData)
{
}

void cCmd_Follow::AtEnd(sMessage endStateData)
{
}

void cCmd_Follow::Update(double deltaTime)
{
	glm::vec3 direction = target->positionXYZ + offset - follower->positionXYZ;
	if (direction.length() > 1.f)
	{
		float speed = direction.length() / deltaTime;
		if (speed > speedLimit)
		{
			speed = speedLimit;
		}
		follower->positionXYZ += ((glm::normalize(direction) * speed) * (float)deltaTime);
	}
}

bool cCmd_Follow::bIsDone(void)
{
	return false;
}