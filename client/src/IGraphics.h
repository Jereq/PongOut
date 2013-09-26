#pragma once

/**
 *  NOTE: File starts here!
 */

#include <boost/filesystem.hpp>
#include <glm/glm.hpp>
#include <string>

/**
 *  Interface to provide platform independent access to platform dependent 2D graphics.
 */
class IGraphics
{
public:
	typedef std::shared_ptr<IGraphics> ptr;

	/**
	 *  destructor.
	 */
	virtual ~IGraphics() {};

	/**
	 *  Initialize the graphics system. Init should not be called on a previous initialized object,
	 *  unless it has been destroyed in between.
	 *  <br>
	 *  The object must be in an initialized state before any other function may be called on the object.
	 *
	 *  @return false if a problem was encountered during initialization, otherwise true.
	 */
	virtual bool init() =0;
	/**
	 *  Destroy the graphics system. Destroy should be called on an initialized
	 *  object when it is no longer needed.
	 *  <br>
	 *  Calling destroy on an object that is not initialized does nothing.
	 */
	virtual void destroy() =0;

	/**
	 *  Load graphic resources from the given folder. The folder must contain a file named resources.txt
	 *  containing a list of resources in the following format:
	 *  <br>
	 *  {resourceType}:{resourceName} {resourcePath}
	 *  <br>
	 *  where {resourceType} is "texture" for textures. Any unknown resource type is ignored.
	 *  Each resource should be on its own line. Lines containing anything else will be ignored.
	 *
	 *  @param _resourceDir the directory to load resources from
	 *  @return false if a problem was encountered, otherwise true.
	 */
	virtual bool loadResources(const boost::filesystem::path& _resourceDir) =0;

	/**
	 *  Add a rectangle to be drawn this frame at the specified position.
	 *  <br>
	 *  This function draws nothing on its own. Added rectangles will be drawn the next time drawFrame is called,
	 *  after which the added rectangles will be removed. Therefore, to get a persistent rectangle,
	 *  addRectangle must be called every frame for each rectangle.
	 *
	 *  @param _center the center of the rectangle to be drawn, with (-1,-1) in the lower left corner of the screen
	 *  	and (1, 1) in the upper right corner.
	 *  @param _size the size of the rectangle to be drawn, with (2, 2) being the size of the window.
	 *  @param _rotation the rotation in degrees clock-wise around the center.
	 *  @param _id the resource id of the texture to apply to the rectangle.
	 */
	virtual void addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string _id) =0;

	/**
	 *  Draw the current frame, using any queued objects.
	 */
	virtual void drawFrame() =0;
};
