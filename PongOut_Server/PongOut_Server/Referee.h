#pragma once

#include "User.h"
#include "CreateGame.h"

class Referee
{
public:
	Referee(void);
	~Referee(void);

	void init(User::ptr _u0, User::ptr _u1, CreateGame::GameInitInfo _info);

private:

	User::ptr u0, u1;
	CreateGame::GameInitInfo info;
};

