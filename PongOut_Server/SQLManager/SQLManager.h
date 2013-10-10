#pragma once

#include <string>
#include <vector>

class SQLManager
{
public:
	SQLManager(void);
	~SQLManager(void);

	long createUser(std::string _userName, std::string _userPassword);
	long verifyUser(std::string _userName, std::string _userPassword);
	long addFriend(long _myID, long _friendID);
	long deleteUser(long _id);
	long removeFriend(long _myID, long _friendID);
	std::vector<int> getFriends(long _myID);
};

