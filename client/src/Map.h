/*
 * Map.h
 *
 *
 *
 */

#ifndef MAP_H_
#define MAP_H_

#include "BlockC.h"
#include "MapResources.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"

#include "Paddle.h"
#include "Ball.h"
#include "CommonTypes.h"
#include "Server.h"
/* Ball/Paddle Data (BPD) */
struct BPD
{
	glm::vec3 p;
	glm::vec2 v;
	int id;
};

/* Block Data (BD)*/
struct BLOCKD
{
	int health;
	int id;
};

struct SERVERFRAMEDATA
{
	BPD paddleData;
	std::vector<BPD>	ballData;
	std::vector<BLOCKD>	blockData;
	int time;
};

class Map 
{
public:
	Map();
	virtual ~Map();
	void initialize(glm::vec2 _playAreaSize, float _frameThickness,
					std::string _bgTextureName, std::string _frameTextureName);

	void update(double _dt, IGraphics::ptr _graphics);
	std::string getTextureName();
	std::string getFrameTextureName();
	glm::vec2 getSize();

	bool loadMap(std::string mapName, std::vector<CommonTypes::Block> _blocks, 
					GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc);

	bool addObject(Paddle::ptr _paddle);
	bool addObject(Ball::ptr _ball);

public:
	void resetBall(const int _id);
	void setBallPosition(const int, glm::vec3 _position);
	glm::vec3 getPaddlePosition(const int _paddleId);
	void setPlayAreaBounds(glm::vec2 _size);

	void setBallData(const std::vector<BPD>& _bd);
	void setBlockData(const std::vector<BLOCKD>& _bd);
	void setPaddleData(const BPD& _pd);

private:
	std::vector<BlockC::ptr>	blocks;
	std::vector<Paddle::ptr>	paddles;
	std::vector<Ball::ptr>		balls;

	std::string	bgTextureName;
	std::string	frameTextureName;
	glm::vec2	playAreaSize;
	float 		frameThickness;

	friend class PhysicsComponent;
};

#endif /* MAP_H_ */
