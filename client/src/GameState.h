#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "ScreenState.h"
#include "Block.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"

#include "InputState.h"

#include "Map.h"

class GameState : public ScreenState
{
public:
	GameState(const std::string _screenName = "UNDEFINED");
	~GameState();

	typedef std::shared_ptr<GameState> ptr;

	virtual std::string getText(const std::string& _elemId) const;

private:
	virtual bool initialize(std::shared_ptr<ICoreSystem> _iCoreSystem);
	virtual void onInput(const std::vector<IInput::Event> _events);
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);

	std::vector<GameObject::ptr> gameObjects;

	IGraphics::ptr _graphics;
	InputState	inputState;
	Map* map;
};

#endif