#pragma once

#include "Graphics.h"
#include "../IRenderer.h"

class RendererLinux : public IRenderer
{
	Graphics g;

public:
	RendererLinux(const boost::filesystem::path& _rootDir);

	virtual bool init() override;
	virtual void destroy() override;

	virtual bool loadImage(const boost::filesystem::path& _imagePath) override;
	virtual void draw() override;

	bool windowIsClosing() const;
};
