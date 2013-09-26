#pragma once
#include "Sound/SoundManager.h"
#include "IGraphics.h"
#include <boost/filesystem.hpp>

class ICoreSystem
{
public:
	virtual ~ICoreSystem() {};

	virtual boost::filesystem::path getRootDir() const =0;
	virtual double getTime() const =0;
	virtual bool windowIsClosing() const =0;
	virtual void pollEvents() =0;

	virtual SoundManager* getSounds();
	virtual IGraphics* getGraphics() =0;

	virtual bool	isKeyPress(unsigned short _key)=0;
	virtual bool	isNewKeyPress(unsigned short _key)=0;

protected:
	SoundManager* soundManager;
	ICoreSystem();
};
