#ifndef __SCREENSTATE_H
#define __SCREENSTATE_H

#include <Graphics/IGraphics.h>
#include <CoreSystem/ICoreSystem.h>
#include <Input/IInput.h>
#include <string>
#include <Server.h>

class ScreenState
{
public:
	typedef std::shared_ptr<ScreenState> ptr;

	ScreenState(const std::string _screenName);
	virtual ~ScreenState();

	virtual void onInput(const std::vector<IInput::Event> _events);
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);
	virtual bool initialize(std::shared_ptr<ICoreSystem> _iCoreSystem, Server::ptr _server);
	virtual bool onExit();
	virtual bool onEntry();

	virtual std::string getText(const std::string& _elemId) const =0;

	bool isInitialized() const;
	bool isActive() const;
	bool isPopup() const;
	std::string getScreenName() const;

protected:
	std::string getBackgroundName() const;
	bool		initialized;
	bool		active;
	bool		popup;

//private:
	std::string screenName;
	std::string backgroundName;
	std::shared_ptr<ICoreSystem>	iCoreSystem;
	Server::ptr server;
};

#endif
