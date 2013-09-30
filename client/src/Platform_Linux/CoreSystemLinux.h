#pragma once

#include "../ICoreSystem.h"
#include "GraphicsLinux.h"

class CoreSystemLinux : public ICoreSystem
{
private:
	boost::filesystem::path rootDir;

	GraphicsLinux graphics;

public:
	CoreSystemLinux(const boost::filesystem::path& _rootDir);

	virtual boost::filesystem::path getRootDir() const override;
	virtual double getTime() const override;
	virtual bool windowIsClosing() const override;
	virtual void pollEvents() override;

	virtual bool isKeyPress(unsigned short _key) override;
	virtual bool isNewKeyPress(unsigned short _key) override;

	virtual GraphicsLinux* getGraphics() override;
};
