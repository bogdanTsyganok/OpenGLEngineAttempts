#include "cCommandGroup.h"

cCommandGroup::cCommandGroup()
{
	this->m_bSerialCommandsDone = false;
	this->m_bParallelCommandsDone = false;

	this->m_bAtStartCalled = false;
}

cCommandGroup::~cCommandGroup()
{
}


void cCommandGroup::AddCommandSerial(iCommand* pCommand)
{
	this->m_vecSerialCommands.push_back(pCommand);
	return;
}

void cCommandGroup::AddCommandParallel(iCommand* pCommand)
{
	this->m_vecParallelCommands.push_back(pCommand);
	return;
}

unsigned int cCommandGroup::getNumberOfParallelCommands(void)
{
	return (unsigned int)this->m_vecParallelCommands.size();
}

unsigned int cCommandGroup::getNumberOfSerialCommands(void)
{
	return (unsigned int)this->m_vecSerialCommands.size();
}

// From the iCommand interface:

// Treat like a constructor type thing. It's never called by the Animation control
void cCommandGroup::Initialize(sMessage startStateData)
{

	return;
}



void cCommandGroup::AtStart(sMessage startStateData)
{
	// Called already (should be only called once)
	if (!this->m_bAtStartCalled)
	{
		// Call AtStart() on the 1st serial command
		if (!this->m_vecSerialCommands.empty())
		{
			sMessage emptyMessage;
			this->m_vecSerialCommands[0]->AtStart(emptyMessage);
		}

		// Call AtStart() an all the parallel commands
		for (std::vector< iCommand* >::iterator itPC = this->m_vecParallelCommands.begin();
			itPC != this->m_vecParallelCommands.end(); itPC++)
		{
			iCommand* pParallelCommmand = *itPC;
			sMessage emptyMessage;
			pParallelCommmand->AtStart(startStateData);
		}
	}//if ( ! this->m_bAtStartCalled )

	this->m_bAtStartCalled = true;

	return;
}

void cCommandGroup::AtEnd(sMessage endStateData)
{
	// TODO: Maybe you are passing some information here.
	return;
}

void cCommandGroup::Update(double deltaTime)
{

	//    ____            _       _    ____                                          _     
	//   / ___|  ___ _ __(_) __ _| |  / ___|___  _ __ ___  _ __ ___   __ _ _ __   __| |___ 
	//   \___ \ / _ \ '__| |/ _` | | | |   / _ \| '_ ` _ \| '_ ` _ \ / _` | '_ \ / _` / __|
	//    ___) |  __/ |  | | (_| | | | |__| (_) | | | | | | | | | | | (_| | | | | (_| \__ \
	//   |____/ \___|_|  |_|\__,_|_|  \____\___/|_| |_| |_|_| |_| |_|\__,_|_| |_|\__,_|___/
	//                                                                                     

	// Call the Update() on JUST the ONE "current" serial command
	if (!this->m_vecSerialCommands.empty())
	{
		// Still some commands
		iCommand* pCurrentCommand = this->m_vecSerialCommands[0];

		pCurrentCommand->Update(deltaTime);

		// Is it done? 
		if (pCurrentCommand->bIsDone())
		{

			// Call AtEnd() on the current command
			sMessage atEndMessage;
			pCurrentCommand->AtEnd(atEndMessage);

			// Move this completed command onto the finished commands
			this->m_vecSerialCommands_COMPLETED.push_back(pCurrentCommand);

			// Erase this from the vector
			this->m_vecSerialCommands.erase(this->m_vecSerialCommands.begin());

			// If there are still serial commands, call AtStart() on new command 
			if (!this->m_vecSerialCommands.empty())
			{
				iCommand* pNewCurrentCommand = this->m_vecSerialCommands[0];

				sMessage atStartMessage;
				pNewCurrentCommand->AtStart(atStartMessage);
			}
			else
			{
				// Serial commands are all done
				this->m_bSerialCommandsDone = true;
			}

		}
	}//if ( ! this->m_vecSerialCommands.empty() )
	else
	{
		this->m_bSerialCommandsDone = true;
	}



	//    ____                 _ _      _    ____                                          _     
	//   |  _ \ __ _ _ __ __ _| | | ___| |  / ___|___  _ __ ___  _ __ ___   __ _ _ __   __| |___ 
	//   | |_) / _` | '__/ _` | | |/ _ \ | | |   / _ \| '_ ` _ \| '_ ` _ \ / _` | '_ \ / _` / __|
	//   |  __/ (_| | | | (_| | | |  __/ | | |__| (_) | | | | | | | | | | | (_| | | | | (_| \__ \
	//   |_|   \__,_|_|  \__,_|_|_|\___|_|  \____\___/|_| |_| |_|_| |_| |_|\__,_|_| |_|\__,_|___/
	//                                                                                           
		// Are there any parallel commands? 

	bool bAnyCommandsStillRunning = false;	// Assume they are all done

	for (std::vector< iCommand* >::iterator itPC = this->m_vecParallelCommands.begin();
		itPC != this->m_vecParallelCommands.end(); itPC++)
	{
		iCommand* pCommmand = *itPC;

		// Is it done?
		if (!pCommmand->bIsDone())
		{
			// No, it's still running
			bAnyCommandsStillRunning = true;

			pCommmand->Update(deltaTime);
		}

	}//for (std::vector< iCommand* >::iterator itPC

	// Are all the commands done?
	if (!bAnyCommandsStillRunning)
	{
		// Yes, they are all done.
		this->m_bParallelCommandsDone = true;
		// TODO: Call AtEnd() on all the commands
		// TODO: Delete them all 
		// Yes, they are all done
	}


	return;
}

bool cCommandGroup::bIsDone(void)
{
	if (!this->m_bParallelCommandsDone)
	{
		return false;
	}
	if (!this->m_bSerialCommandsDone)
	{
		return false;
	}
	// BOTH serial AND parallel are done
	return true;
}