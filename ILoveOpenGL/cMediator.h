#pragma once

#include "iMediator.h"

class cMediator : public iMediator
{
public:
	cMediator();
	virtual ~cMediator();

	// From the iMessage interface
	virtual bool SetReciever(iMediator* pTheReciever);
	// Asyncronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage);
	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);

	// These methods are being used by the main, or whatever is "higher level"
	// thing that isn't just using the message passing commands

	void SetAllTheShips(std::vector< iShip* > vec_pShips);

private:
	std::vector< iShip* > m_vec_pShips;
};
