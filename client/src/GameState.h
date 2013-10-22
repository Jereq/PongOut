#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "GUIActionHandler.h"
#include "ScreenState.h"
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
	void addStateAction(GUIActionHandler* _actionHandler);
	void load(CreateGameResponse::ptr _cgrp);

private:
	virtual bool initialize(std::shared_ptr<ICoreSystem> _iCoreSystem, Server::ptr _server);
	virtual void onInput(const std::vector<IInput::Event> _events);
	virtual bool onEntry() override;
	virtual bool onExit() override;
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);

	void handleNetworkMessages();
	
	std::vector<GameObject::ptr> gameObjects;

	IGraphics::ptr graphics;
	InputState	inputState;
	Map* world;
	GUIActionHandler* actionHandler;
	GraphicsComponent::ptr gc;
	InputComponent::ptr ic;
	PhysicsComponent::ptr pc;

	CommonTypes::PlayerMatchInfo myInfo;
	int myMatchScore;
	int opponentMatchScore;
	int time;
	bool inGame;
};

#endif