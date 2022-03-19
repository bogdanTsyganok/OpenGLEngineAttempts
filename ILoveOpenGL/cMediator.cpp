#include "cMediator.h"
#include <algorithm>
#include <iterator>
#include "iShip.h"

cMediator::cMediator()
{

}

cMediator::~cMediator()
{
}

// *******************************************************************
// From the iMessage interface
bool cMediator::SetReciever(iMediator* pTheReciever)
{
	// Doesn't do anything
	return true;
}

// Asyncronous (don't return)
bool cMediator::RecieveMessage(sMessage theMessage)
{
	if (theMessage.command == "Expode something here")
	{
		// #0 is location, #1 is radius of explosion

		// Go through seeing which ships are damaged

		sMessage takeDamage;
		takeDamage.command = "Take Damage";
		takeDamage.vec_fData.push_back(glm::vec4(10.0f, 0.0f, 0.0f, 0.0f));
		this->m_vec_pShips[17]->RecieveMessage(takeDamage);
	}

	return true;
}

// Syncronous (do return with something)
bool cMediator::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{

	if (theMessage.command == "Find Target")
	{
		// TODO: Do a better "finding of targets"
		// For now, just pick some random targets
		for (unsigned int count = 0; count != 10; count++)
		{
			unsigned int randIndex = rand() % this->m_vec_pShips.size();
			messageReply.vec_fData.push_back(glm::vec4(this->m_vec_pShips[randIndex]->getMeshPointer()->positionXYZ, 1.0f));
		}
	}
	return true;
}
// *******************************************************************


void cMediator::SetAllTheShips(std::vector< iShip* > vec_pShips)
{
	this->m_vec_pShips.clear();
	// This is really a loop inside, but is using the STL library 
	//	to "back insert" copy all the data. 
	// It's:
	// * NOT faster
	// * NOT better
	// It's just one line. That's it.
	std::copy(vec_pShips.begin(), vec_pShips.end(), std::back_inserter(this->m_vec_pShips));


	return;
}
