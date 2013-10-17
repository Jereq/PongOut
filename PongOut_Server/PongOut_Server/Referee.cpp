#include "stdafx.h"
#include "Referee.h"
//#include "TrollML.h"
#include "Map.h"
#include "TMLReader.h"
#include "CreateGameResponse.h"


Referee::Referee(void)
{
}


Referee::~Referee(void)
{
}

void Referee::init( User::ptr _u0, User::ptr _u1, CommonTypes::GameInitInfo _info)
{
	u0 = _u0;
	u1 = _u1;
	info = _info;

	//TrollML t;
	Map m;
	//t.readFile(info.mapID, m);

	TMLReader tml;
	tml.readFile("../test.tml");
	
	//std::string mapblock = tml.getValueFromKey("Maps", "name");

	std::vector<CommonTypes::Block> mapData;

	CreateGameResponse::ptr cgrp = CreateGameResponse::ptr(new CreateGameResponse());

	cgrp->setResponse(mapData, _info);

	u0->addMsgToMsgQueue(cgrp);
	u1->addMsgToMsgQueue(cgrp);

	int i = 0;
}
