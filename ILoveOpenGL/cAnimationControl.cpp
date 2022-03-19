#include "cAnimationControl.h"

cAnimationControl::cAnimationControl()
{
	this->m_bAtStartCalled = false;

	this->pDefaultCommand = NULL;
}

cAnimationControl::~cAnimationControl()
{
}

void cAnimationControl::Update(double deltaTime)
{
	// This calls update on the current command group
	// Are we at the end of the vector already (any more commands?)
	if (!this->vec_pCommandGroup.empty())
	{
		// There's a current command
		cCommandGroup* pCurGroup = this->vec_pCommandGroup[0];

		// Has AtStart() been called?
		// NOTE: This is only used for the very 1st Command Group
		//	or if a Command Group was added after all of them were run.
		if (!this->m_bAtStartCalled)
		{
			sMessage atStartMessage;
			pCurGroup->AtStart(atStartMessage);
			this->m_bAtStartCalled = true;
		}

		// Calls update on all the commands contained inside
		pCurGroup->Update(deltaTime);

		// Are we done with this command group?
		if (pCurGroup->bIsDone())
		{
			// Yes, we are done
			// 1. Delete this command from the command group
			// 2. Move to the next command group

			// Calling erase() on a vector is slow, but that's not an issue here
			// REMOVES this FROM THE VECTOR, but DOESN'T delete the command
			// Again, we might want to use a list instead of a vector if this becomes an issue			

			sMessage atEndMessage;
			// For now, there's nothing being send
			pCurGroup->AtEnd(atEndMessage);

			// Move this completed command 
			this->vec_pCommandGroups_COMPLETED.push_back(pCurGroup);
			// Remove the current one
			this->vec_pCommandGroup.erase(this->vec_pCommandGroup.begin());

			// If there is another command group, then call AtStart() here
			//	as this is the 1st time we "see" it

			if (!this->vec_pCommandGroup.empty())
			{
				// For now, there's nothing being send
				sMessage atStartMessage;
				// For now, there's nothing being send

				this->vec_pCommandGroup[0]->AtStart(atStartMessage);
			}

		}

	}//if (this->itCurrenCommandGroup 
	// You could do the default command if there aren't any command
//	else
//	{
//		this->pDefaultCommand->Update(deltaTime);
//	}


	// Run all the "permanent" commands (that run all the time)
	for (cCommandGroup* pCurCmd : this->vec_pPermanentCommands)
	{
		pCurCmd->Update(deltaTime);
	}


	return;
}

cCommandGroup* cAnimationControl::pCreateCommandGroup(void)
{
	cCommandGroup* pNC = new cCommandGroup();

	return pNC;
}

void cAnimationControl::AddCommandGroup(cCommandGroup* pCommandGroup)
{
	// Are there any commands yet?
	if (this->vec_pCommandGroup.empty())
	{
		// No, either:
		// 1. This is the 1st command group called (ever), or
		// 2. A new Command Group was added to an empty queue.
		this->m_bAtStartCalled = false;
	}

	this->vec_pCommandGroup.push_back(pCommandGroup);

	return;
}

void cAnimationControl::AddPermanentCommandGroup(cCommandGroup* pCommandGroup)
{
	this->vec_pPermanentCommands.push_back(pCommandGroup);
	return;
}


unsigned int cAnimationControl::getNumberOfCommandsLeft(void)
{
	// Since I'm deleting the commands from the vector,
	// I just need to see if anything is still there.
	return (unsigned int)this->vec_pCommandGroup.size();

	// I could also just check to see if the iterator is at the end of the vector
	// (like if I was just incrementing not deleting)
}

bool cAnimationControl::areAllCommandsDone(void)
{
	// See if the vector has anything in it
	if (this->getNumberOfCommandsLeft() == 0)
	{
		return true;
	}
	// There are still commands to do
	return false;
}
