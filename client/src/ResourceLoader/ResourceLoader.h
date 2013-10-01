#pragma once

/**
 *  COPYLEFT NOTICE: You may use this however you want, at your responsibility.
 */

#include <boost/filesystem.hpp>

#include <vector>

/**
 *  Class containing utility functions for dealing with resources.
 */
class ResourceLoader
{
public:
	/**
	 *  Error codes returned by functions in ResourceLoader.
	 */
	enum class ErrorCode
	{
		/**
		 *  Everything finished successfully
		 */
		SUCCESS,
		/**
		 *  The directory path specified does not exist or is not a directory
		 */
		DIRECTORY_NOT_FOUND,
		/**
		 *  The directory specified does not contain a 'resources.txt'
		 */
		RESOURCE_FILE_NOT_FOUND,
		/**
		 *  The resource file contained invalid format
		 */
		INVALID_FORMAT,
		/**
		 *  An invalid argument was encountered.
		 */
		INVALID_ARGUMENT,
	};

	/**
	 *  A structure describing i single resource
	 */
	struct Resource
	{
		/**
		 *  The type of the resource, for example 'texture' or 'sound'
		 */
		std::string type;
		/**
		 *  The resource name, for example 'player/ball'
		 */
		std::string name;
		/**
		 *  The full path to the resource, for example 'C:/.../textures/ball_prototype.png'
		 */
		boost::filesystem::path path;
	};

	/**
	 *  Get the resources from a specified directory. The directory must contain a file named 'resources.txt',
	 *  containing descriptions of the resources, on per line, in the format '<type>:<name>    <path>'.
	 *  Empty lines and lines starting with '#' will be ignored.
	 *
	 *	@param _ret The vector to store retrieved resources in. Resources will be appended to any existing.
	 *	@param _resourceDir A path to a directory containing resources and the file 'resources.txt'.
	 *
	 *	@return An error code stating the success or failure of the operation. If SUCCESS, _ret will contain any read
	 *		resources. If DIRECTORY_NOT_FOUND or RESOURCE_FILE_NOT_FOUND, no resources could be found and _ret is not modified.
	 *		If INVALID_FORMAT, one or more lines contained invalid input and was ignored. Any successfully read resources
	 *		*IS* appended to _ret.
	 */
	static ErrorCode getResources(std::vector<Resource>& _ret, const boost::filesystem::path& _resourceDir);

	/**
	 *  Get the resources from a specified directory. The directory must contain a file named 'resources.txt',
	 *  containing descriptions of the resources, on per line, in the format '<type>:<name>    <path>'.
	 *  Empty lines and lines starting with '#' will be ignored.
	 *
	 *	@param _ret The vector to store retrieved resources in. Resources will be appended to any existing.
	 *	@param _resourceDir A path to a directory containing resources and the file 'resources.txt'.
	 *	@param _type The type of resources to retrieve. Must not be empty.
	 *
	 *	@return An error code stating the success or failure of the operation. If SUCCESS, _ret will contain any read
	 *		resources. If DIRECTORY_NOT_FOUND, RESOURCE_FILE_NOT_FOUND or INVALID_ARGUMENT, no resources could be found
	 *		and _ret is not modified. If INVALID_FORMAT, one or more lines contained invalid input and was ignored.
	 *		Any successfully read resources *IS* appended to _ret.
	 */
	static ErrorCode getResources(std::vector<Resource>& _ret, const boost::filesystem::path& _resourceDir, const std::string& _type);
};

