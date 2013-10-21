#pragma once

#include "stdafx.h"
#include <map>
#include <mutex>

template <class T, class S>
class SafeMap
{
private:
	std::mutex mutex;

public:
	typedef std::map<T, S> mapType;

	std::map<T, S> baseMap;

	void insert(const std::pair<T, S>& _pair)
	{
		std::lock_guard<std::mutex> guard(mutex);
		baseMap.insert(_pair);
	}

	typename mapType::size_type erase(const T& _key)
	{
		std::lock_guard<std::mutex> guard(mutex);
		return baseMap.erase(_key);
	}

	S& at(const T& _key)
	{
		std::lock_guard<std::mutex> guard(mutex);
		return baseMap.at(_key);
	}

	std::unique_lock<std::mutex> getLock()
	{
		return std::unique_lock<std::mutex>(mutex);
	}
};