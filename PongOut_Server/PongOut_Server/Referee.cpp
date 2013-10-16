#include "stdafx.h"
#include "Referee.h"


Referee::Referee(void)
{
}


Referee::~Referee(void)
{
}

void Referee::init( User::ptr _u0, User::ptr _u1, CreateGame::GameInitInfo _info)
{
	u0 = _u0;
	u1 = _u1;
	info = _info;
}
