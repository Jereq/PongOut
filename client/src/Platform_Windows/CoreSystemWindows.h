#ifndef __CORESYSTEMWINDOWS_H
#define __CORESYSTEMWINDOWS_H

#include "../icoresystem.h"
#include "GraphicsWindows.h"

class CoreSystemWindows : public ICoreSystem
{
public:
	CoreSystemWindows(const boost::filesystem::path& _rootDir);
	~CoreSystemWindows();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
protected:


private:
	virtual boost::filesystem::path getRootDir() const;
	virtual double getTime() const;
	virtual bool windowIsClosing() const;
	virtual void pollEvents();

	virtual IGraphics::ptr getGraphics() override;

	IGraphics::ptr	graphics;
};

#endif