#pragma once

#include "../ICoreSystem.h"
#include "RendererLinux.h"

class CoreSystemLinux : public ICoreSystem
{
private:
	boost::filesystem::path rootDir;

	RendererLinux renderer;

public:
	CoreSystemLinux(const boost::filesystem::path& _rootDir);

	virtual boost::filesystem::path getRootDir() const override;
	virtual double getTime() const override;
	virtual bool windowIsClosing() const override;
	virtual void pollEvents() override;

	virtual RendererLinux* getRenderer() override;
};
