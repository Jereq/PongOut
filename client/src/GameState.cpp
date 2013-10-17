#include "GameState.h"
#include "Paddle.h"
#include "Ball.h"
#include "TMLReader.h"

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

bool GameState::initialize(std::shared_ptr<ICoreSystem> _iCoreSystem)
{
	if (!ScreenState::initialize(_iCoreSystem))
	{
		return false;
	}
	
	gc = GraphicsComponent::ptr(new GraphicsComponent);
	gc->initialize(iCoreSystem->getGraphics());

	ic = InputComponent::ptr(new InputComponent()); 
	ic->initialize(InputState::ptr(&inputState));

	pc = PhysicsComponent::ptr(new PhysicsComponent());
	pc->initialize(world);

	graphics = iCoreSystem->getGraphics();

	world->loadMap("hello", gc, ic, pc);
	return true;
}

void GameState::load()
{
	bool res;
	

	world->initialize(glm::vec2(800,800),20,"HELLO","HELLO");

	res = world->loadMap("Map1", gc, ic, pc);
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
					actionHandler->buttonPressed("back");
					
					break;
				}
			}
			break;
		}
	}
}

bool GameState::onEntry()
{
	iCoreSystem->getSounds()->changeBackgroundMusic("techno");

	load();
	return ScreenState::onEntry();
}

bool GameState::onExit()
{
	//delete world;
	return ScreenState::onExit();
}

void GameState::update(const float _dt)
{
	//for(GameObject::ptr g : gameObjects)
	//{
	//	g->update(_dt);
	//}
	world->update(_dt, graphics);
}

void GameState::draw(std::shared_ptr<IGraphics> _graphics)
{
	_graphics->addRectangle(glm::vec3(0,0,0.1), glm::vec2(2,2), 0, "output/chat_window");
}