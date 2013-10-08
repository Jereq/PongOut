#ifndef __GUIACTIONHANDLER_H
#define __GUIACTIONHANDLER_H

#include <string>

class GUIActionHandler
{
public:
	virtual void buttonPressed(const std::string& _func) =0;
};

#endif