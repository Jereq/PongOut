#pragma once

#include "User.h"
#include "CreateGameRequest.h"

class Referee
{
public:
	Referee(void);
	~Referee(void);

	void init(User::ptr _u0, User::ptr _u1, CommonTypes::GameInitInfo _info, int _refID);

private:

	User::ptr u0, u1;
	CommonTypes::GameInitInfo info;
	int id;
};

