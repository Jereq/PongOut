#ifndef __CORESYSTEMWINDOWS_H
#define __CORESYSTEMWINDOWS_H

#include "../ICoreSystem.h"
#include "GraphicsWindows.h"
#include "Input/InputWindows.h"

class CoreSystemWindows : public ICoreSystem
{
public:
	CoreSystemWindows(const boost::filesystem::path& _rootDir);
	~CoreSystemWindows();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
protected:
	virtual bool	isNewKeyPress(unsigned short _key);
	virtual bool	isKeyPress(unsigned short _key);

private:
	virtual double getTime() const;
	virtual bool windowIsClosing() const;
	virtual void pollEvents();

	virtual SoundManager* getSounds();
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
