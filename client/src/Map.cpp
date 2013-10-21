/*
 * Map.cpp
 *
 *
 *
 */

#include "Map.h"

#define BALL_Z		0.0f
#define PADDLE_Z	0.0f

Map::Map()
{
	frameThickness 	= 0.f;
	bgTextureName 	= "";
}

Map::~Map()
{

}

void Map::initialize( glm::vec2 _playAreaSize, float _frameThickness,
						std::string _bgTextureName, std::string _frameTextureName )
{
	frameThickness 			= _frameThickness;
	bgTextureName 			= _bgTextureName;
	frameTextureName		= _frameTextureName;

	setPlayAreaBounds(_playAreaSize);
}

bool Map::addObject(Paddle::ptr _paddle)
{
	for(unsigned int i = 0; i < paddles.size(); i++)
	{
		//we found a duplicate
		if(paddles[i]->getId() == _paddle->getId())
			return false;
	}

	//new paddle, add accordingly
	paddles.push_back(_paddle);

	return true;
}

bool Map::addObject(Ball::ptr _ball)
{
	for(unsigned int i = 0; i < balls.size(); i++)
	{
		if(balls[i]->getId() == _ball->getId())
			return false;
	}

	balls.push_back(_ball);

	return true;
}

void Map::setPlayAreaBounds(glm::vec2 _size)
{
	playAreaSize.x 		= _size.x;// + frameThickness;
	playAreaSize.y		= _size.y;// + frameThickness;
}

inline glm::vec2 screenSizeToClip( const glm::vec2 _screenDimension, const glm::vec2 _screenSize )
{
	return glm::vec2( _screenSize.x / _screenDimension.x * 2.f, _screenSize.y / _screenDimension.y * 2.f);
}
 
inline glm::vec3 screenPositionToClip( const glm::vec2 _screenDimension, const glm::vec3 _screenPosition )
{
	return glm::vec3(_screenPosition.x / _screenDimension.x * 2.f - 1.f, _screenPosition.y / _screenDimension.y * 2.f - 1.f, _screenPosition.z);
}

bool Map::loadMap(std::string _mapName, std::vector<CommonTypes::Block> _blockData, GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc )
{
	bgTextureName = _mapName;

	if(blocks.size() != 0)
		blocks.clear();
	if(balls.size() != 0)
		balls.clear();
	if(paddles.size() != 0)
		paddles.clear();

	Paddle::ptr p = Paddle::ptr(new Paddle);
	p->initialize(userInfos[0].paddle.id, glm::vec3(userInfos[0].paddle.pos, PADDLE_Z), glm::vec2(128, 32), 0, gc, ic, pc);
	paddles.push_back(p);

	Ball::ptr b = Ball::ptr(new Ball);
	b->initialize(userInfos[0].ball.id, glm::vec3(userInfos[0].ball.pos, BALL_Z), glm::vec2(userInfos[0].ball.radius), glm::vec2(0,0), 0, gc, pc);
	balls.push_back(b);

	p = Paddle::ptr(new Paddle);
	p->initialize(userInfos[1].paddle.id, glm::vec3(userInfos[1].paddle.pos,PADDLE_Z), glm::vec2(128, 32), 0, gc, nullptr, pc);
	paddles.push_back(p);

	b = Ball::ptr(new Ball);
	b->initialize(userInfos[1].ball.id, glm::vec3(userInfos[1].ball.pos, BALL_Z), glm::vec2(userInfos[0].ball.radius), glm::vec2(0,0), 0, gc, pc);
	balls.push_back(b);
	
	for(CommonTypes::Block bd : _blockData)
	{
		BlockC::ptr block = BlockC::ptr(new BlockC);
		block->initialize(bd, 0, gc);
		blocks.push_back(block);
	}

	return true;
}

void Map::setUserInfo(const CommonTypes::PlayerMatchInfo& _me, const CommonTypes::PlayerMatchInfo& _op)
{
	userInfos[0] = _me;
	userInfos[1] = _op;
}

void Map::resetBall(const int _id)
{
	for(GameObject::ptr b : balls)
	{
		if(b->getId() == _id)
		{
			b->setInPlay(false);
		}
	}
}

std::string Map::getTextureName()
{
	return bgTextureName;
}
std::string Map::getFrameTextureName()
{
	return frameTextureName;
}
glm::vec2 Map::getSize()
{
	return playAreaSize;
}


Paddle::ptr Map::getPaddle(const int _paddleId)
{
	Paddle::ptr p;

	for(Paddle::ptr paddle : paddles)
	{
		if(paddle->getId() == _paddleId)
		{
			p = paddle;
			break;
		}
	}
	return p;
}

void Map::setBallPosition(const int _ballId, glm::vec3 _position)
{
	for(Ball::ptr ball : balls)
	{
		if(ball->getId() == _ballId)
		{
			ball->setCenter(_position);
			break;
		}
	}
}

void Map::update(double _dt, IGraphics::ptr _graphics)
{
	glm::vec2 clipArea = glm::vec2( playAreaSize.x / 1280. * 2.f, playAreaSize.y / 720 * 2.f);
	_graphics->addRectangle(glm::vec3(0,0,0.01), clipArea, 0, bgTextureName);
	
	/* Update Paddles */
	for(GameObject::ptr paddle : paddles)
	{	
		paddle->update(_dt);
	}

	/* Update balls */
	for(GameObject::ptr ball : balls)
	{
		ball->update(_dt);
	}

	/* Update Blocks */
	for(GameObject::ptr block : blocks)
	{
		block->update(_dt);
	}

}

void Map::setBallData(const CommonTypes::Ball& _bd)
{
	/* set data for any ball that serverFrameData provides */
	for(GameObject::ptr ball : balls)
	{
		if(_bd.id == ball->getId())
		{
			ball->setPosition( glm::vec3(_bd.pos, BALL_Z), _bd.vel);
			ball->setInPlay(_bd.inPlay == 0);
			break;
		}
	}
}

void Map::setBlockData(const std::vector<CommonTypes::Block>& _bd)
{
	for(CommonTypes::Block bd : _bd)
	{
		for(BlockC::ptr block : blocks)
		{
			if(bd.id == block->getId())
			{
				block->setCollided(bd.health);
				block->setInPlay(bd.inPlay == 0);
			}
		}
	}
}

/* set data from serverFrameData before update if paddle is opponent */
void Map::setPaddleData(const CommonTypes::Paddle& _pd)
{
	for(GameObject::ptr paddle : paddles)
	{
		/* set data from serverFrameData before update if paddle is opponent */
		if(paddle->getId() == _pd.id)
		{
			paddle->setPosition( glm::vec3(_pd.pos, PADDLE_Z), _pd.vel);
			paddle->setInPlay( _pd.inPlay != 0);
		}
	}
}