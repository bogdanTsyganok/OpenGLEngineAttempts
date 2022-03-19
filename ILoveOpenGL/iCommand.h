#pragma once
#include <glm/vec4.hpp>
#include <string>
#include <vector>

struct sMessage
{
public:
	std::string command;
	std::vector<std::string> stringData;
	std::vector<glm::vec4> vecData;
	std::vector<void*> voidPointers;
};

class iCommand
{
public:
	virtual ~iCommand() {};

	// Called by whoever instantiated this command.
	// It's +NOT+ called by the groups or animation control.
	// (It's basically stuff you might send via a constructor)
	virtual void Initialize(sMessage startStateData) = 0;

	// Called once when created
	virtual void AtStart(sMessage startStateData) = 0;
	// Called once when done
	virtual void AtEnd(sMessage endStateData) = 0;

	// Called every frame
	virtual void Update(double deltaTime) = 0;

	// When command is finished, this returns true;
	virtual bool bIsDone(void) = 0;

	std::string friendlyName;
};
