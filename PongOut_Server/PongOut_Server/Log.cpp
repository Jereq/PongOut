#include "stdafx.h"
#include "Log.h"

#include <boost/bind.hpp>
#include <chrono>
#include <sstream>

using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace std;

boost::shared_ptr<Log> Log::ptr;

boost::shared_ptr<Log> Log::getInstance()
{
	if (!ptr)
	{
		ptr = boost::shared_ptr<Log>(new Log());
		ptr->runThread = true;
		ptr->logThread = thread(boost::bind(&Log::printFromQueue, ptr));
	}
	return ptr;
}

Log::Log(void)
{
	enumMap.insert(pair<LogType, string>(LogType::LOG_INFO, "INFO"));
	enumMap.insert(pair<LogType, string>(LogType::LOG_ERROR, "ERROR"));
	currentLogFile = boost::shared_ptr<Log::logFile>();
	debugOn = true;
}


Log::~Log(void)
{
	int i = 42;
}

void Log::addLog( LogType _type, int _prio, std::string _msg, char* _file, int _line )
{
	boost::shared_ptr<Log> p = getInstance();
	lock_guard<std::mutex> lock(p->queueLock);

	message m;
	m.type = _type;
	m.prio = _prio;
	m.msg = _msg;
	m.file = _file;
	m.line = _line;

	p->msgQueue.push(m);
}

void Log::setPrioLevel( int _prio )
{
	boost::shared_ptr<Log> p = getInstance();
	p->prio = _prio;
}

void Log::setDebugOn( bool _debug )
{
	boost::shared_ptr<Log> p = getInstance();
	p->debugOn = _debug;
}

void Log::printFromQueue()
{
	while (runThread)
	{
		if (msgQueue.size() > 0)
		{
			ptime ct = second_clock::local_time();

			while (!msgQueue.empty())
			{
				if (prio > msgQueue.front().prio)
				{
					ostringstream outStr;
					message m = msgQueue.front();

					outStr << ct.time_of_day() <<  " ## " << enumMap.at(msgQueue.front().type) <<  " : " << m.msg;

					if (debugOn)
					{
						outStr << endl << "Line: " << m.line << endl << "File: " << m.file << endl << "======================================================================================";
					}

					cout << outStr.str() << endl;
					writeToLogFile(outStr.str());
				}				
				lock_guard<std::mutex> lock(queueLock);
				msgQueue.pop();
			}
		}

		chrono::milliseconds pause(100);
		this_thread::sleep_for(pause);
	}
}

void Log::writeToLogFile( std::string _msg )
{
	date currDate(day_clock::local_day_ymd());

	if (currentLogFile == boost::shared_ptr<Log::logFile>() || currentLogFile->dateCreated != currDate)
	{		
		currentLogFile = createLogFile();
	}

	_msg.append("\n");
	const char* buffer = _msg.c_str();

	currentLogFile->file = _fsopen(currentLogFile->name.c_str(), "a", _SH_DENYRW);
	fprintf(currentLogFile->file, buffer);
	fclose(currentLogFile->file);
}

boost::shared_ptr<Log::logFile> Log::createLogFile()
{
	boost::shared_ptr<Log::logFile> res = boost::shared_ptr<Log::logFile>(new Log::logFile());

	date currDate(day_clock::local_day_ymd());
	string name = to_iso_extended_string(currDate) + "_pongOutServer.log";

	res->file = nullptr;
	res->dateCreated = currDate;
	res->name = name;

	return res;
}

void Log::destroy()
{	
	ptr->runThread = false;
	ptr->logThread.join();
	ptr.reset();
}

