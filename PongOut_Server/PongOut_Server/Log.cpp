#include "stdafx.h"
#include "Log.h"

#include <boost/bind.hpp>
#include <chrono>
#include <boost/date_time/posix_time/posix_time.hpp>

boost::shared_ptr<Log> Log::ptr;

boost::shared_ptr<Log> Log::getInstance()
{
	if (!ptr)
	{
		ptr = boost::shared_ptr<Log>(new Log());
		ptr->runThread = true;
		ptr->logThread = std::thread(boost::bind(&Log::printFromQueue, ptr));
	}
	return ptr;
}

Log::Log(void)
{
	enumMap.insert(std::pair<LogType, std::string>(LogType::INFO, "INFO"));
	enumMap.insert(std::pair<LogType, std::string>(LogType::ERROR, "ERROR"));
	enumMap.insert(std::pair<LogType, std::string>(LogType::DEBUG, "DEBUG"));
}


Log::~Log(void)
{
	int i = 42;
}

void Log::addLog( LogType _type, std::string _msg )
{
	boost::shared_ptr<Log> p = getInstance();
	std::lock_guard<std::mutex> lock(p->queueLock);
	p->msgQueue.push(msg(_type, _msg));
}

void Log::printFromQueue()
{
	while (runThread)
	{
		if (msgQueue.size() > 0)
		{
			boost::posix_time::ptime ct = boost::posix_time::second_clock::local_time();

			for (unsigned int i = 0; i < msgQueue.size(); i++)
			{
				std::cout << ct.time_of_day() << " ## " <<  enumMap.at(msgQueue.front().first) << " : " << msgQueue.front().second << std::endl;
				std::lock_guard<std::mutex> lock(queueLock);
				msgQueue.pop();
			}
		}

		std::chrono::milliseconds pause(1000);
		std::this_thread::sleep_for(pause);
	}
	int dummy = 42;
}

void Log::destroy()
{	
	ptr->runThread = false;
	ptr->logThread.join();
	ptr.reset();
}
