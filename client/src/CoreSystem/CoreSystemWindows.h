#ifndef __CORESYSTEMWINDOWS_H
#define __CORESYSTEMWINDOWS_H

#include "ICoreSystem.h"
#include <Graphics/GraphicsWindows.h>
#include <Input/InputWindows.h>

class CoreSystemWindows : public ICoreSystem
{
public:
	CoreSystemWindows(const boost::filesystem::path& _rootDir);
	~CoreSystemWindows();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
protected:

private:
	virtual double getTime() const;
	virtual bool windowIsClosing() const;
	virtual void pollEvents();

	virtual IGraphics::ptr getGraphics() override;
	virtual IInput::ptr getInput() override;

	GraphicsWindows::ptr	graphics;
	InputWindows::ptr		input;

	void registerRAW();
	//timer
	INT64	frequency;	
	float	ticksPerMs;
	INT64	startTime;
	float	frameTime;	
};

#endif
