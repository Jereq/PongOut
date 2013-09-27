#pragma once

/**
 *  File description: Contains code. You have been warned.
 */

#include "IGraphics.h"

#include <boost/filesystem.hpp>

/**
 *  Interface to provide platform independent access to platform dependent system facilities.
 */
class ICoreSystem
{
private:
	boost::filesystem::path rootDir;

protected:
	static std::shared_ptr<ICoreSystem> instance;

	/**
	 *  constructor.
	 *
	 *  @param _rootDir the path to the directory where the game is located
	 */
	ICoreSystem(const boost::filesystem::path& _rootDir);

public:
	typedef std::weak_ptr<ICoreSystem>  ptr;

	/**
	 *  Initialize an instance of the platforms implementation.
	 *
	 *  @param _argc the the number of command line arguments passed to the main function
	 *  @param _argv the command line arguments passed to the main function
	 *  @return true on success, otherwise false
	 */
	static bool init(int _argc, char** _argv);

	/**
	 *  Get an instance of the platforms implementation. The system must be
	 *  initialized manually before a valid instance can be returned.
	 *
	 *  @return a pointer to the instance, empty pointer if not initialized
	 */
	static ptr getInstance();

	/**
	 *  Free the instance correctly.
	 */
	static void destroy();

	/**
	 *  destructor.
	 */
	virtual ~ICoreSystem() {};

	/**
	 *  Get the root directory of the game. This should be the root location of game assets.
	 */
	virtual boost::filesystem::path getRootDir() const;
	/**
	 *  Get the time since the game launched, in seconds.
	 *
	 *  @return the time in seconds since the application started.
	 */
	virtual double getTime() const =0;
	/**
	 *  Get whether the OS requests that the window is closed, for example because the user
	 *  closed the window (clicked the X).
	 *
	 *  @return true if the system requests that the window should close, false otherwise.
	 */
	virtual bool windowIsClosing() const =0;
	/**
	 *  Poll system events.
	 *  <br>
	 *  Most window systems requires its applications to regularly check for system events,
	 *  which is the task for pollEvents.
	 */
	virtual void pollEvents() =0;

	/**
	 *  Get an interface that can be used to render graphics.
	 *  <br>
	 *  The interface may or may not be initialized.
	 *
	 *  @return a pointer to a graphics object. Will not return nullptr.
	 */
	virtual IGraphics::ptr getGraphics() =0;
};
