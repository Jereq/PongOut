#include "Game.h"

#include "PongOutConfig.h"
//#include <chrono>
#include <iostream>
#include <stdio.h>
//#include "Map.h"

//#include <thread>
#include <vector>

Game::Game(ICoreSystem::ptr _system)
	: system(_system)
{
	map = new Map();
	map->initialize(glm::vec2(800.0f, 600.0f), 8, "background/mainmenu_01", "blocks/orange_01");
}

bool cbIntersect(glm::vec3 _bp, float _br, glm::vec3 _rp, glm::vec2 _rb, glm::vec3& _bv)
{
	bool xinv = false;
	bool yinv = false;
	float tx = _bp.x;
	float ty = _bp.y;
	float ox = _rp.x - _rb.x / 2.0f;
	float oy = _rp.y - _rb.y / 2.0f;

	if(tx < ox)
	{
		tx = ox;
		xinv = true;
	}
	if(tx > ox + _rb.x)
	{
		tx = ox + _rb.x;
		xinv = true;
	}
	if(ty < oy)
	{
		ty = oy;
		yinv = true;
	}
	if(ty > oy + _rb.y)
	{
		ty = oy + _rb.y;
		yinv = true;
	}

	bool result = ((_bp.x - tx) * (_bp.x - tx) + (_bp.y - ty) * (_bp.y - ty )) < _br * _br;

	if(result)
	{
		if(xinv)
			_bv.x *= -1;
		if(yinv)
			_bv.y *= -1;
	}

	return result;

}

void fbIntersect(glm::vec3& _bp, glm::vec2 _bd, glm::vec3& _bv)
{
	if(_bp.x + _bd.x / 2.0 > 1.0)
	{
		_bp.x = 1.0f - _bd.x / 2.0f;
		_bv.x *= -1;
	}
	if(_bp.x - _bd.x / 2.0 < -1.0)
	{
		_bp.x = -1.0f + _bd.x / 2.0f;
		_bv.x *= -1;
	}
	if(_bp.y + _bd.y / 2.0 > 1.0)
	{
		_bp.y = 1.0f - _bd.y / 2.0f;
		_bv.y *= -1;
	}
	if(_bp.y - _bd.y / 2.0 < -1.0)
	{
		_bp.y = -1.0f + _bd.y / 2.0f;
		_bv.y *= -1;
	}
}

void rbIntersect(glm::vec3& _bp, glm::vec2 _bd, glm::vec3& _bv, std::vector<glm::vec3>& _r, glm::vec2 _rd)
{
		for(size_t i = 0; i < _r.size(); i++)
		{
			if(cbIntersect(_bp + _bv, _bd.x, _r[i], _rd, _bv))
			{
				//_bv.y *= -1;

				std::vector<glm::vec3>::iterator it = _r.begin() + i;
				_r.erase(it, it+1);

				break;
			}
		}
}

void Game::run()
{
	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;

	std::shared_ptr<ICoreSystem> systemPtr(system);

	IGraphics::ptr graphics = systemPtr->getGraphics();
	if (!graphics->init())
	{
		printf("Failed to initialize renderer\n");
		return;
	}

	graphics->loadResources(systemPtr->getRootDir() / "resources");

	SoundManager* sounds = systemPtr->getSounds();
	sounds->initialize();

	IInput::ptr input = systemPtr->getInput();

	double previousTime = systemPtr->getTime();
	double currentTime = previousTime;
	double deltaTime = 0.f;

	sounds->load("Bubble3");

	std::cout << "Starting to run" << std::endl;
	std::cout << "Texture name: \n" << map->getTextureName() << std::endl;


	glm::vec3 bVector = glm::vec3(0.0,-0.75,0);
	glm::vec3 b2Vector = glm::vec3(0.0,0.75,0);
	glm::vec2 bDim = glm::vec2(0.06125 * 0.6,0.06125);
	glm::vec3 bVelocity = glm::vec3(0.0125, 0.0125, 0);
	glm::vec3 rVector = glm::vec3(-1.0,0,0);
	glm::vec2 rDim = glm::vec2(0.125, 0.125);

	glm::vec3 fakeVector = glm::vec3(0,1 - 0.06125,0);
	glm::vec3 b2Velocity = -bVelocity;
	std::vector<glm::vec3> blocks;
	for(int y=0; y < 3; y++)
	{
		for(int x=0; x < 13; x++ )
		{
			blocks.push_back( glm::vec3(x*rDim.x-0.75, y*rDim.y, 0)); 
		}
	}
	bool rExist = true;

	while(!systemPtr->windowIsClosing())
	{
		systemPtr->pollEvents();

		previousTime = currentTime;
		currentTime = systemPtr->getTime();
		deltaTime = currentTime - previousTime;

		std::vector<IInput::Event> events = input->getEvents(true);
		for (auto event : events)
		{
			switch (event.type)
			{
			case IInput::Event::Type::KEY:
				std::cout << "Key event (" << (int)event.keyEvent.key << ")" << std::endl;
				break;

			case IInput::Event::Type::MOUSE_BUTTON:
				break;

			case IInput::Event::Type::MOUSE_MOVE:
				break;
			}
		}

		float dt = (float)(deltaTime * 1000.f);
		float bspeed = 0.075f;
		float pspeed = 0.001f;
		sounds->play("Bubble3");

		graphics->addRectangle(glm::vec3(0.f, 0.f, 1.0f), glm::vec2(2.f), 0.f, map->getTextureName());

		graphics->addRectangle(glm::vec3(-1.f, 0.f, 0.95f), glm::vec2(0.05, 2.f), 0.f, map->getFrameTextureName());
		graphics->addRectangle(glm::vec3(1.f, 0.f, 0.95f), glm::vec2(0.05, 2.f), 0.f, map->getFrameTextureName());
		graphics->addRectangle(glm::vec3(0.f, -1.f, 0.95f), glm::vec2(2.f, 0.05), 0.f, map->getFrameTextureName());
		graphics->addRectangle(glm::vec3(0.f, 1.f, 0.95f), glm::vec2(2.f, 0.05), 0.f, map->getFrameTextureName());

		b2Vector = b2Vector + b2Velocity * bspeed * dt;

		fbIntersect(bVector, bDim, bVelocity);
		fbIntersect(b2Vector, bDim, b2Velocity);

		if(fakeVector.x < bVector.x)
			fakeVector += glm::vec3(1,0,0) * pspeed * dt;
		else if(fakeVector.x > bVector.x)
			fakeVector -= glm::vec3(1,0,0) * pspeed * dt;

		rbIntersect(bVector, bDim, bVelocity, blocks, rDim);
		rbIntersect(b2Vector, bDim, b2Velocity, blocks, rDim);

		static float x,y;
		x+=-0.001f;
		y+=-0.001f;

		//graphics->addRectangle(testVector, glm::vec2(0.25f, 0.125f), 0.f, "blocks/dark_magenta_01");
		graphics->addRectangle(fakeVector, glm::vec2(0.25f, 0.125f), 0.f,"blocks/pink_01");

		for(size_t i = 0; i < blocks.size(); i++)
			graphics->addRectangle(blocks[i], rDim, 10.f, "blocks/orange_01");

		//graphics->addRectangle(glm::vec3(0, 0, 1.f), glm::vec2(2,2), 10.f, "background");
		graphics->addRectangle(bVector, bDim, 1.0f, "player/ball");
		graphics->addRectangle(b2Vector, bDim, 1.0f, "player/ball");
		const static float FRAMES_PER_SECOND = 60.f;
		const static float FRAME_TIME = 1.f / FRAMES_PER_SECOND;

		//if (deltaTime < FRAME_TIME)
		//{
		//	//std::chrono::milliseconds dura((unsigned int)((FRAME_TIME - deltaTime) * 1000.f));
		//	//std::this_thread::sleep_for(dura);
		//}

		graphics->drawFrame();
	}

	graphics->destroy();
}

