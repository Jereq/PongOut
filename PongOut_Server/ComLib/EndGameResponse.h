#pragma once

#include "GameMessage.h"

class EndGameResponse :	public GameMessage
{
public:

	typedef boost::shared_ptr<EndGameResponse> ptr;

	EndGameResponse(void);
	virtual ~EndGameResponse(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
	void setResult(CommonTypes::MatchResult _res);
	CommonTypes::MatchResult getResult();

private:

	CommonTypes::MatchResult res;
};

