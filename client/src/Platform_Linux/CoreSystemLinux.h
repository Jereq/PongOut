#pragma once

#include "../ICoreSystem.h"
#include "GraphicsLinux.h"

class CoreSystemLinux : public ICoreSystem
{
private:
	GraphicsLinux::ptr graphics;

	static void errorCallback(int _error, const char* _description);

public:

	CoreSystemLinux(const boost::filesystem::path& _rootDir);
	~CoreSystemLinux();

	virtual double getTime() const override;
	virtual bool windowIsClosing() const override;
	virtual void pollEvents() override;

	virtual IGraphics::ptr getGraphics() override;
};
