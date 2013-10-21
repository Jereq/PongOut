#ifndef __INPUTCOMPONENT_H
#define __INPUTCOMPONENT_H

#include <boost/filesystem.hpp>
#include "InputState.h"

class GameObject;
class Paddle;

class InputComponent
{
public:
	InputComponent();
	~InputComponent();

	typedef std::shared_ptr<InputComponent> ptr;

	virtual bool initialize(InputState* _inputState);
	virtual void update(GameObject* _gameObject, double _dt);
	virtual void moveToTarget(Paddle& _paddle, double _dt);
private:
	InputState* inputState;

	bool keyLeft;
	bool keyRight;
};

#endif