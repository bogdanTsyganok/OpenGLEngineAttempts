#include "cCmd_Rotation.h"


cCmd_Rotate::cCmd_Rotate()
{
}

void cCmd_Rotate::Initialize(sMessage startStateData)
{
	objectToMove = (cMesh*)startStateData.voidPointers[0];
	direction = startStateData.vecData[0];
	time = startStateData.vecData[0].a;
}

void cCmd_Rotate::AtStart(sMessage startStateData)
{
	float length = glm::sqrt(glm::pow(direction.x, 2) + glm::pow(direction.y, 2) + glm::pow(direction.z, 2));
	speed = length / time;
}

void cCmd_Rotate::AtEnd(sMessage endStateData)
{
}

void cCmd_Rotate::Update(double deltaTime)
{
	objectToMove->orientationXYZ += ((direction * speed) * (float)deltaTime);
	time -= deltaTime;
}

bool cCmd_Rotate::bIsDone(void)
{
	return (time < 0);
}
