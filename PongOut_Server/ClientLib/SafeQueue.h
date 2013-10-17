#pragma once

#include "stdafx.h"
#include <queue>
#include <mutex>

template <class T>
class SafeQueue
{
private:
	std::mutex mutex;
public:
	typedef std::queue<T> queueType;
	std::queue<T> baseQueue;

	void push(const T& _in)
	{
		std::lock_guard<std::mutex> guard(mutex);
		baseQueue.push(_in);
	}

	T pop()
	{
		std::lock_guard<std::mutex> guard(mutex);
		T tmp = baseQueue.back();
		baseQueue.pop();
		return tmp;
	}

	std::unique_lock<std::mutex> getLock()
	{
		return std::unique_lock<std::mutex>(mutex);
	}
};