#ifndef __GRAPHICSWINDOWS_H
#define __GRAPHICSWINDOWS_H

#include "D3D.h"

class GraphicsWindows
{
public:
	GraphicsWindows(HWND _hWnd);
	~GraphicsWindows();
	
protected:

private:
	virtual bool initialize(...);
	D3D*	d3d;
	HWND	hWnd;
};

#endif