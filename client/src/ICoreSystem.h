#pragma once

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

	virtual IGraphics* getGraphics() =0;
};
