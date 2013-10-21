#include "stdafx.h"
#include "LaunchBallRequest.h"


LaunchBallRequest::LaunchBallRequest(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::LAUNCH_BALL_REQUEST;
	msgHeader.length = sizeof(GameMsgType);
}


LaunchBallRequest::~LaunchBallRequest(void)
{
}

std::vector<char> LaunchBallRequest::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);

	pack(msgHeader, it);
	pack(gType, it);

	return res;
}

msgBase::ptr LaunchBallRequest::interpretPacket( const std::deque<char>& _buffer )
{
	LaunchBallRequest::ptr egr = LaunchBallRequest::ptr(new LaunchBallRequest());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(egr->msgHeader, it);
	it = unpack(egr->gType, it);

	return egr;
}

