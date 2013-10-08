#pragma once

#include <string>

class FunctionHandler
{
public:
	virtual void onFunction(const std::string& _func) =0;
};
