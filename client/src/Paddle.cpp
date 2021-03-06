#include "Paddle.h"

Paddle::Paddle()
	: GameObject(),
	keyDir(0.f),
	currentInput(InputType::KEYBOARD)
{
}

Paddle::~Paddle()
{

}

bool Paddle::initialize(const int _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
						GraphicsComponent::ptr _graphicsComponent, InputComponent::ptr _inputComponent, PhysicsComponent::ptr _physicsComponent)
{
	if(!GameObject::initialize(_id, _center, _size, _rotation, _graphicsComponent))
		return false;

	inputComponent		= _inputComponent;
	physicsComponent	= _physicsComponent;
	inPlay				= false;
	return true;
}

void Paddle::setInPlay(bool _inPlayState)
{
	GameObject::setInPlay(_inPlayState);
}

void Paddle::update(double _dt)
{
	//inputComponent->update(this, _dt);
	

	float speedMult = 2.f;
	float maxSpeed = 600.f;
	float accelaration = 2.f;

	if(inputComponent != nullptr)
	{
		inputComponent->moveToTarget(*this, _dt);

		float KEY_ACCELERATION = 3000.f;
		float KEY_DEACCELERATION = 0.001f;
	
		if (currentInput == InputType::KEYBOARD)
		{
			if (keyDir != 0.f)
			{
				velocity.x += (float)(keyDir * KEY_ACCELERATION * _dt);
			}
			else
			{
				velocity.x *= glm::pow(KEY_DEACCELERATION, (float)_dt);
			}
		}
		else
		{
			float diffX = targetPos.x - center.x;
			velocity.x = diffX * accelaration;
		}
	}

	velocity.x = glm::max(velocity.x, -maxSpeed);
	velocity.x = glm::min(velocity.x, maxSpeed);

	center.x = (float)(center.x + velocity.x * speedMult * _dt);

	physicsComponent->restrictToPlayArea(this, _dt);
	
	graphicsComponent->addSpriteToFrame("player/paddle", center, size, rotation);
}