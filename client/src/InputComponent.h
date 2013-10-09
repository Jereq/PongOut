#ifndef __INPUTCOMPONENT_H
#define __INPUTCOMPONENT_H

#include <boost/filesystem.hpp>
#include "InputState.h"

class GameObject;

class InputComponent
{
public:
	InputComponent();
	~InputComponent();

	typedef std::shared_ptr<InputComponent> ptr;

	virtual bool initialize(InputState::ptr _inputState);
	virtual void update(GameObject* _gameObject, double _dt);

private:
	InputState::ptr inputState;
};

#endif