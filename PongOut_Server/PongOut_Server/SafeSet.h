#pragma once
#include "stdafx.h"

#include <set>
#include <mutex>

template <class T>
class SafeSet
{
private:
	std::mutex mutex;

public:
	typedef std::set<T> setType;
	std::set<T> baseSet;

	typename std::set<T>::_Pairib insert(const T& _in)
	{
		std::lock_guard<std::mutex> guard(mutex);
		return baseSet.insert(_in);
	}

	typename std::set<T>::size_type erase(const T& _in)
	{
		std::lock_guard<std::mutex> guard(mutex);
		return baseSet.erase(_in);
	}

	std::unique_lock<std::mutex> getLock()
	{
		return std::unique_lock<std::mutex>(mutex);
	}
};