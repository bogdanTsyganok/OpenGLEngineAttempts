#pragma once

#include "cCommandGroup.h"

class cAnimationControl
{
public:
	cAnimationControl();
	~cAnimationControl();

	// Called every frame:
	void Update(double deltaTime);

	void AddCommandGroup(cCommandGroup* pCommandGroup);
	cCommandGroup* pCreateCommandGroup(void);

	void AddPermanentCommandGroup(cCommandGroup* pCommandGroup);

	unsigned int getNumberOfCommandsLeft(void);
	bool areAllCommandsDone(void);

private:
	// We are always looking at the 1st command group (if it's there)
	std::vector< cCommandGroup* > vec_pCommandGroup;

	// If there's no commands in the vector, run this one...
	cCommandGroup* pDefaultCommand;

	// These are run all the time
	std::vector< cCommandGroup* > vec_pPermanentCommands;

	// As commands are completed, they are placed here.
	// (We don't need these, but in case you want to debug what's going on, maybe?)
	std::vector< cCommandGroup* > vec_pCommandGroups_COMPLETED;

	// Used as a flag to call the AtStart() at the beginning or when a command group is added
	bool m_bAtStartCalled;	// Set to false at start
};
#pragma once
