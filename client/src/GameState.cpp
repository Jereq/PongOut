#include "GameState.h"
#include "Paddle.h"
#include "Ball.h"

GameState::GameState(const std::string _screenName)
	: ScreenState(_screenName)
{

}

GameState::~GameState()
{

}

bool GameState::initialize(std::shared_ptr<ICoreSystem> _iCoreSystem)
{
	if (!ScreenState::initialize(_iCoreSystem))
	{
		return false;
	}
	bool res;
	world = new Map();

	graphics = iCoreSystem->getGraphics();

	GraphicsComponent* gc = new GraphicsComponent();
	gc->initialize(iCoreSystem->getGraphics());
	InputComponent* ic = new InputComponent(); 
	ic->initialize(InputState::ptr(&inputState));
	PhysicsComponent* pc = new PhysicsComponent();
	pc->initialize(world);

	Ball::ptr b = Ball::ptr(new Ball());
	res = b->initialize("ball1", glm::vec3(600,400,-0.1), glm::vec2(32,32), 0, GraphicsComponent::ptr(gc), PhysicsComponent::ptr(pc));
	world->addObject(b);

	Paddle::ptr p = Paddle::ptr(new Paddle());
	res = p->initialize("paddle1", glm::vec3(400,100,0), glm::vec2(128,32), 0, GraphicsComponent::ptr(gc), InputComponent::ptr(ic), PhysicsComponent::ptr(pc));
	world->addObject(p);

	world->initialize(glm::vec2(800,800),20,"HELLO","HELLO");
	world->initBlockArray(30, GraphicsComponent::ptr(gc));
	//for(Block& b : world->blockList)
	//{
	//	gameObjects.push_back(GameObject::ptr(&b));
	//}


	return true;
}

std::string GameState::getText(const std::string& _elemId) const
{
	return "HELLO WORLD";
}

void GameState::onInput(const std::vector<IInput::Event> _events)
{
	inputState.setFrameEvents(_events);
}

bool GameState::onEntry()
{
	iCoreSystem->getSounds()->changeBackgroundMusic("techno");

	return ScreenState::onEntry();
}

void GameState::update(const float _dt)
{
	//for(GameObject::ptr g : gameObjects)
	//{
	//	g->update(_dt);
	//}
	world->update(_dt);
}

void GameState::draw(std::shared_ptr<IGraphics> _graphics)
{
	glm::vec2 playArea = world->getSize();
	glm::vec2 clipArea = glm::vec2( playArea.x / 1280. * 2.f, playArea.y / 1024. * 2.f);
	_graphics->addRectangle(glm::vec3(0,0,0.01), clipArea, 0, "background/ingame_01");
	_graphics->addRectangle(glm::vec3(0,0,0.1), glm::vec2(2,2), 0, "output/chat_window");
}