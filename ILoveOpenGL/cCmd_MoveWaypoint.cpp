#include "cCmd_MoveWaypoint.h"

cCmd_MoveWaypoint::cCmd_MoveWaypoint()
{
}

void cCmd_MoveWaypoint::Initialize(sMessage startStateData)
{
	for (int i = 0; i < startStateData.vecData.size(); i++)
	{
		sMessage move;
		move.voidPointers.push_back(startStateData.voidPointers[0]);
		move.vecData.push_back( startStateData.vecData[i]);
		cCmd_MoveRelative* moveCmd = new cCmd_MoveRelative();
		moveCmd->Initialize(move);
		this->serialMoves.push_back(moveCmd);
	}
}

void cCmd_MoveWaypoint::AtStart(sMessage startStateData)
{
	// Call AtStart() on the 1st serial command
	if (!this->serialMoves.empty())
	{
		sMessage emptyMessage;
		this->serialMoves[0]->AtStart(emptyMessage);
	}

	return;
}

void cCmd_MoveWaypoint::AtEnd(sMessage endStateData)
{
	if (!this->serialMoves.empty())
	{
		for (int i = 0; i < serialMoves.size(); i++)
		{
			delete serialMoves[i];
		}
		this->serialMoves.clear();
	}
}

void cCmd_MoveWaypoint::Update(double deltaTime)
{
	if (!this->serialMoves.empty())
	{
		// Still some commands
		iCommand* pCurrentCommand = this->serialMoves[0];

		pCurrentCommand->Update(deltaTime);

		// Is it done? 
		if (pCurrentCommand->bIsDone())
		{

			// Call AtEnd() on the current command
			sMessage atEndMessage;
			pCurrentCommand->AtEnd(atEndMessage);

			// Erase this from the vector
			this->serialMoves.erase(this->serialMoves.begin());

			// If there are still serial commands, call AtStart() on new command 
			if (!this->serialMoves.empty())
			{
				iCommand* pNewCurrentCommand = this->serialMoves[0];

				sMessage atStartMessage;
				pNewCurrentCommand->AtStart(atStartMessage);
			}
			else
			{
				// Serial commands are all done
				this->isDone = true;
			}

		}
	}//if ( ! this->m_vecSerialCommands.empty() )
}

bool cCmd_MoveWaypoint::bIsDone(void)
{
	return this->isDone;
}
