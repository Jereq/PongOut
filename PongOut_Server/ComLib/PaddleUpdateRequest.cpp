#include "stdafx.h"
#include "PaddleUpdateRequest.h"


PaddleUpdateRequest::PaddleUpdateRequest(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::PADDLEUPDATEREQUEST;
}


PaddleUpdateRequest::~PaddleUpdateRequest(void)
{
}

void PaddleUpdateRequest::setPaddle( const CommonTypes::Paddle& _p )
{
	p = _p;

	msgHeader.length = sizeof(CommonTypes::Paddle);
}

CommonTypes::Paddle PaddleUpdateRequest::getPaddle()
{
	return p;
}

std::vector<char> PaddleUpdateRequest::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);

	pack(msgHeader, it);
	pack(gType, it);
	pack(p, it);

	return res;
}

msgBase::ptr PaddleUpdateRequest::interpretPacket( const std::deque<char>& _buffer )
{
	PaddleUpdateRequest::ptr pur = PaddleUpdateRequest::ptr(new PaddleUpdateRequest());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(pur->msgHeader, it);
	it = unpack(pur->gType, it);
	it = unpack(pur->p, it);

	return pur;
}