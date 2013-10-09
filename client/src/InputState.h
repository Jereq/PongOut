#ifndef __INPUTSTATE_H
#define __INPUTSTATE_H

#include "Input/IInput.h"

class InputState
{
private:
	std::vector<IInput::Event> events;

public:
	typedef std::shared_ptr<InputState> ptr;

	void setFrameEvents(std::vector<IInput::Event> _events);
	std::vector<IInput::Event>& getEvents();
};

#endif