#include "GameState.h"
#include "Paddle.h"
#include "Ball.h"
#include "TMLReader.h"
#include "CreateGameResponse.h"
#include "GameTickUpdate.h"
#include <boost/smart_ptr.hpp>

GameState::GameState(const std::string _screenName)
	: ScreenState(_screenName), gc(0), ic(0), pc(0)
{
	world = new Map();
}

GameState::~GameState()
{

}

void GameState::addStateAction(GUIActionHandler* _actionHandler)
{
	actionHandler = _actionHandler;
}

bool GameState::initialize(std::shared_ptr<ICoreSystem> _iCoreSystem, Server::ptr _server)
{
	if (!ScreenState::initialize(_iCoreSystem, _server))
	{
		return false;
	}
	
	gc = GraphicsComponent::ptr(new GraphicsComponent);
	gc->initialize(iCoreSystem->getGraphics());

	ic = InputComponent::ptr(new InputComponent()); 
	ic->initialize(&inputState);

	pc = PhysicsComponent::ptr(new PhysicsComponent());
	pc->initialize(world);

	graphics = iCoreSystem->getGraphics();

	world->initialize(glm::vec2(screenWidth - 80,screenHeight - 120),0,"HELLO","HELLO");

	return true;
}

void GameState::load(CreateGameResponse::ptr _cgrp)
{
	std::vector<CommonTypes::Block> b;
	std::string mapTexture = "background/ingame_01";

	b = _cgrp->getMap();
	world->setUserInfo(_cgrp->getPMIme(), _cgrp->getPMIop());
	world->loadMap(mapTexture, b, gc, ic, pc);

	myInfo = _cgrp->getPMIme();

	myMatchScore		= 0;
	opponentMatchScore	= 0;
	time				= -1;
}

std::string GameState::getText(const std::string& _elemId) const
{
	return "HELLO WORLD";
}

void GameState::onInput(const std::vector<IInput::Event> _events)
{
	inputState.setFrameEvents(_events);

	/* check if any incomming message is a state event */
	std::vector<IInput::Event> stateEvents =  inputState.getEvents();

	for(IInput::Event e : stateEvents)
	{
		switch(e.type)
		{
		case IInput::Event::Type::KEY:
			{
				bool pressed = e.keyEvent.pressed;

				switch (e.keyEvent.key)
				{
				case IInput::KeyCode::ESCAPE:
					actionHandler->buttonPressed("giveUp");
					break;
				case IInput::KeyCode::SPACE:
					{
						server->sendLaunchBall();
						break;
					}
				}
			}
			break;
		}
	}
}

bool GameState::onEntry()
{
	iCoreSystem->getSounds()->changeBackgroundMusic("techno");

	return ScreenState::onEntry();
}

bool GameState::onExit()
{
	//delete world;
	return ScreenState::onExit();
}

void GameState::update(const float _dt)
{
	handleNetworkMessages();

	world->update(_dt, graphics);

	CommonTypes::Paddle paddle;
	Paddle::ptr p	= world->getPaddle(myInfo.paddle.id);
	paddle.id		= p->getId();
	paddle.pos		= p->getCenter().swizzle(glm::X,glm::Y);
	paddle.vel		= p->getVelocity();

	server->sendPaddlePos(paddle);
}

void GameState::draw(std::shared_ptr<IGraphics> _graphics)
{
	_graphics->addText("menu_text_field", glm::vec3(-0.95,0.9,0), glm::vec2(0.07,0.07), "My Score: " + std::to_string(myMatchScore));
	_graphics->addText("menu_text_field", glm::vec3(-0.95,-0.9,0), glm::vec2(0.07,0.07), "Opponent Score: " + std::to_string(opponentMatchScore));
	_graphics->addText("menu_text_field", glm::vec3(-0.1,0.9,0), glm::vec2(0.07,0.07), "Time: " + std::to_string(time));

	_graphics->addRectangle(glm::vec3(0,0,0.1), glm::vec2(2,2), 0, "output/chat_window");
}

void GameState::handleNetworkMessages()
{
	while (server->getMsgQueueSize() > 0)
	{
		message tmp = server->getNextMessage();

		switch (tmp.type)
		{	
		case msgBase::MsgType::GAMEMESSAGE:
			{
				switch (tmp.gType)
				{
				case GameMessage::GameMsgType::GAME_TICK_UPDATE:
					{
						CommonTypes::PlayerMatchInfo pmiMe, pmiOp;

						GameTickUpdate::ptr cgrp = boost::static_pointer_cast<GameTickUpdate>(tmp.gMsg);

						pmiMe = cgrp->getMyInfo();
						pmiOp = cgrp->getOpInfo();

						myMatchScore		= pmiMe.score;
						opponentMatchScore	= pmiOp.score;

						std::vector<CommonTypes::Block> blocks = cgrp->getBlockList();

						/* Shared data */
						world->setBlockData(blocks);

						/* My data */
						world->setBallData(pmiMe.ball);
						
						/* Opponent data */
						world->setBallData(pmiOp.ball);
						world->setPaddleData(pmiOp.paddle);

						//if(sfd.time > -1)
						//{
						//	time = sfd.time;
						//}
					}
					break;

				case GameMessage::GameMsgType::END_GAME_RESPONSE:
					actionHandler->buttonPressed("back");
					break;

				default:
					break;
				}
			}
			break;

		default:
			break;
		}
	}
}
