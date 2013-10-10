#pragma once

#include "ICoreSystem.h"
#include <Graphics/GraphicsLinux.h>
#include <Input/InputLinux.h>

class CoreSystemLinux : public ICoreSystem
{
private:
	GLFWwindow* window;

	GraphicsLinux::ptr graphics;
	InputLinux::ptr input;

	static void errorCallback(int _error, const char* _description);

	bool openWindow();

public:

	CoreSystemLinux(const boost::filesystem::path& _rootDir);
	~CoreSystemLinux();

	virtual double getTime() const override;
	virtual bool windowIsClosing() const override;
	virtual void pollEvents() override;

	virtual IGraphics::ptr getGraphics() override;
	virtual IInput::ptr getInput() override;
};
