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
	map = new Map();

	GraphicsComponent* gc = new GraphicsComponent();
	gc->initialize(iCoreSystem->getGraphics());
	InputComponent* ic = new InputComponent();
	ic->initialize(InputState::ptr(&inputState));
	PhysicsComponent* pc = new PhysicsComponent();
	pc->initialize(map);

	Ball* b = new Ball();
	res = b->initialize("ball1", glm::vec3(200,200,0), glm::vec2(64,64), 0, GraphicsComponent::ptr(gc), PhysicsComponent::ptr(pc));
	gameObjects.push_back(GameObject::ptr(b));
	Paddle* p = new Paddle();
	res = p->initialize("paddle1", glm::vec3(400,400,0), glm::vec2(200,200), 0, GraphicsComponent::ptr(gc), InputComponent::ptr(ic));
	gameObjects.push_back(GameObject::ptr(p));
	
	
	map->initialize(glm::vec2(800,800),20,"HELLO","HELLO");
	map->initBlockArray(30, GraphicsComponent::ptr(gc));
	for(Block& b : map->blockList)
	{
		gameObjects.push_back(GameObject::ptr(&b));
	}
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

void GameState::update(const float _dt)
{
	for(GameObject::ptr g : gameObjects)
	{
		g->update(_dt);
	}
}

void GameState::draw(std::shared_ptr<IGraphics> _graphics)
{
	glm::vec2 playArea = map->getSize();
	glm::vec2 clipArea = glm::vec2( playArea.x / 1280. * 2.f, playArea.y / 1024. * 2.f);
	_graphics->addRectangle(glm::vec3(0,0,0.01), clipArea, 0, map->getTextureName());
	//mebbe gui elements?
}