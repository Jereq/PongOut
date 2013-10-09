#include "ResourceLoader.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

#include <sstream>

ResourceLoader::ErrorCode ResourceLoader::getResources(std::vector<Resource>& _ret, const boost::filesystem::path& _resourceDir)
{
	if (!fs::exists(_resourceDir) || !fs::is_directory(_resourceDir))
	{
		return ErrorCode::DIRECTORY_NOT_FOUND;
	}

	fs::path resourceFile = _resourceDir / "resources.txt";
	if (!fs::exists(resourceFile) || !fs::is_regular_file(resourceFile))
	{
		return ErrorCode::RESOURCE_FILE_NOT_FOUND;
	}

	ErrorCode res = ErrorCode::SUCCESS;

	int rowNr = 0;
	fs::fstream f(resourceFile);
	while (!f.eof())
	{
		rowNr++;

		std::string line;
		std::getline(f, line);

		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		std::istringstream ss(line);
		std::string type;
		std::getline(ss, type, ':');

		if (ss.eof())
		{
			res = ErrorCode::INVALID_FORMAT;
			continue;
		}

		if (type.empty())
		{
			res = ErrorCode::INVALID_FORMAT;
			continue;
		}

		std::string resourceName;
		std::string resourcePath;
		ss >> resourceName >> resourcePath;

		if (resourceName.empty() || resourcePath.empty())
		{
			res = ErrorCode::INVALID_FORMAT;
			continue;
		}

		Resource res = { type, resourceName, _resourceDir / resourcePath };

		_ret.push_back(res);
	}

	return res;
}

ResourceLoader::ErrorCode ResourceLoader::getResources(std::vector<Resource>& _ret, const boost::filesystem::path& _resourceDir, const std::string& _type)
{
	if (_type.empty())
	{
		return ErrorCode::INVALID_ARGUMENT;
	}

	std::vector<Resource> result;
	ErrorCode res = getResources(result, _resourceDir);

	if (res != ErrorCode::SUCCESS)
	{
		return res;
	}

	for (auto val : result)
	{
		if (val.type == _type)
		{
			_ret.push_back(val);
		}
	}

	return ErrorCode::SUCCESS;
}

