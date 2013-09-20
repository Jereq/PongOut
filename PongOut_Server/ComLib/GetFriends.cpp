#include "stdafx.h"
#include "GetFriends.h"


GetFriends::GetFriends(void) : msgBase(msgBase::MsgType::GETFRIENDS)
{
}


GetFriends::~GetFriends(void)
{
}

void GetFriends::addToFriendList( std::string _userName, boost::uuids::uuid _uuid )
{
	friends.push_back(msgBase::userData(_userName, _uuid));
}

std::vector<char> GetFriends::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(friends.size(), iter);
	
	for (unsigned int i = 0; i < friends.size(); i++)
	{
		pack(friends[i].userName, iter);
		pack(friends[i].uuid, iter);
	}

	msgHeader.length = res.size() - sizeof(msgBase::header);
	pack(msgHeader, res.begin());
	return res;
}

msgBase::ptr GetFriends::interpretPacket( const std::deque<char>& _buffer )
{
	GetFriends::ptr gfp = GetFriends::ptr(new GetFriends());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(gfp->msgHeader, it);

	unsigned int tmpSize;
	it = unpack(tmpSize, it);
	gfp->friends.resize(tmpSize);

	for (unsigned int i = 0; i < friends.size(); i++)
	{
		it = unpack(gfp->friends[i].userName, it);
		it = unpack(gfp->friends[i].uuid, it);
	}

	return gfp;
}

std::vector<msgBase::userData> GetFriends::getAllFriends()
{
	return friends;
}
