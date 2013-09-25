#ifndef __CORESYSTEMWINDOWS_H
#define __CORESYSTEMWINDOWS_H

#include "../icoresystem.h"
#include "GraphicsWindows.h"

class CoreSystemWindows : public ICoreSystem
{
public:
	CoreSystemWindows();
	~CoreSystemWindows();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	void setHwnd(HWND _hWnd);
protected:
	virtual bool	isNewKeyPress(unsigned short _key);
	virtual bool	isKeyPress(unsigned short _key);

private:
	virtual boost::filesystem::path getRootDir() const;
	virtual double getTime() const;
	virtual bool windowIsClosing() const;
	virtual void pollEvents();

	virtual IGraphics* getGraphics();

	GraphicsWindows*	graphics;
	HWND				hWnd;

	void registerRAW();

	//timer
	INT64	frequency;	
	float	ticksPerMs;
	INT64	startTime;
	float	frameTime;	
};

#endif