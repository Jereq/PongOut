#include "stdafx.h"
#include "msgBase.h"


msgBase::msgBase(MsgType _type)
{
	msgHeader.type = _type;
}


msgBase::~msgBase(void)
{
}

msgBase::header msgBase::getHeader()
{
	return msgHeader;
}
