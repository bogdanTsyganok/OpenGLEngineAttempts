#include "cCmd_LocationTrigger.h"


cCmd_LocationTrigger::cCmd_LocationTrigger()
{
}

void cCmd_LocationTrigger::Initialize(sMessage startStateData)
{
	triggeredCommand = (iCommand*)startStateData.voidPointers[0];
	monitoredMesh = (cMesh*)startStateData.voidPointers[1];
	center = startStateData.vecData[0];
	radius = startStateData.vecData[0].a;
	isTriggered = false;
}

void cCmd_LocationTrigger::AtStart(sMessage startStateData)
{
}

void cCmd_LocationTrigger::AtEnd(sMessage endStateData)
{
}

void cCmd_LocationTrigger::Update(double deltaTime)
{
	
	if ((glm::distance(center, monitoredMesh->positionXYZ) <= radius) && !isTriggered)
	{
		isTriggered = true;
		triggeredCommand->AtStart(sMessage());
	}
	else if (isTriggered)
	{
		triggeredCommand->Update(deltaTime);
	}
}

bool cCmd_LocationTrigger::bIsDone(void)
{
	return isTriggered && triggeredCommand->bIsDone();
}