#pragma once

#include "iCommand.h"

#include <vector>

class cCommandGroup :
	public iCommand
{
public:
	cCommandGroup();
	virtual ~cCommandGroup();

	void AddCommandSerial(iCommand* pCommand);
	void AddCommandParallel(iCommand* pCommand);

	unsigned int getNumberOfParallelCommands(void);
	unsigned int getNumberOfSerialCommands(void);

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);

private:
	// NOTE: Since we are erasing the ones from the start, it 
	//	would likely make more sense to use a list instead of a vector. 
	// (erase with a list is faster than an erase at the START of a vector)
	std::vector< iCommand* > m_vecParallelCommands;
	std::vector< iCommand* > m_vecSerialCommands;

	// (We don't need these, but in case you want to debug what's going on, maybe?)
	std::vector< iCommand* > m_vecSerialCommands_COMPLETED;

	bool m_bSerialCommandsDone;
	bool m_bParallelCommandsDone;

	// Used as a flag to call the AtStart() at the beginning 
	bool m_bAtStartCalled;	// Set to false at start


};
