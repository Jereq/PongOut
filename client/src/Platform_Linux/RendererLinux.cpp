#include "RendererLinux.h"

RendererLinux::RendererLinux(const boost::filesystem::path& _rootDir)
	: g(_rootDir)
{

}

bool RendererLinux::init()
{
	return g.init();
}

void RendererLinux::destroy()
{
	g.destroy();
}

bool RendererLinux::loadImage(const boost::filesystem::path& _imagePath)
{
	return g.loadImage(_imagePath);
}

void RendererLinux::draw()
{
	g.draw();
}

bool RendererLinux::windowIsClosing() const
{
	return g.windowClosing();
}
