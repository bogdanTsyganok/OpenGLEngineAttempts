#include "cCmd_MoveRelative.h"

cCmd_MoveRelative::cCmd_MoveRelative()
{
}

void cCmd_MoveRelative::Initialize(sMessage startStateData)
{
	objectToMove = (cMesh*)startStateData.voidPointers[0];
	direction = startStateData.vecData[0];
	time = startStateData.vecData[0].a;
	totalTime = time;
}

void cCmd_MoveRelative::AtStart(sMessage startStateData)
{
	float length = glm::sqrt(glm::pow(direction.x, 2) + glm::pow(direction.y, 2) + glm::pow(direction.z, 2));
	speed = length / time;
}

void cCmd_MoveRelative::AtEnd(sMessage endStateData)
{
}

void cCmd_MoveRelative::Update(double deltaTime)
{
	float elapsedPercent = (time / totalTime) * 100.f;
	float speedMod = 1.f;
	if (elapsedPercent < 10.f)
	{
		speedMod = elapsedPercent * 0.1;
	}else
	if (elapsedPercent > 90.f)
	{
		speedMod = (100.f - elapsedPercent) * 0.1;
	}
	objectToMove->positionXYZ += ((glm::normalize(direction) * speed) * (float)deltaTime * speedMod);
	time -= deltaTime;
}

bool cCmd_MoveRelative::bIsDone(void)
{
	return (time < 0);
}
