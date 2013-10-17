#include "stdafx.h"
#include "Referee.h"
#include "TrollML.h"
#include "Map.h"

Referee::Referee(void)
{
}


Referee::~Referee(void)
{
}

void Referee::init( User::ptr _u0, User::ptr _u1, CreateGameRequest::GameInitInfo _info)
{
	u0 = _u0;
	u1 = _u1;
	info = _info;

	TrollML t;
	Map m;
	t.readFile(info.mapID, m);

	int i = 0;
}
