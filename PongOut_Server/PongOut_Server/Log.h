#pragma once
#include <boost/shared_ptr.hpp>
#include <queue>
#include <cstdint>
#include <thread>
#include <boost/enable_shared_from_this.hpp>
#include <map>
#include <mutex>
#include <cstdio>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

class Log : public boost::enable_shared_from_this<Log>
{
public:
	enum class LogType : std::uint8_t
	{
		LOG_INFO,
		LOG_ERROR,
		LOG_DEBUG,
	};

	static boost::shared_ptr<Log> getInstance();
	
	~Log(void);

	static void addLog(LogType _type, int _prio, std::string _msg);
	static void destroy();
	static void setPrioLevel(int _prio);

private:

	struct message
	{
		LogType type;
		int prio;
		std::string msg;
	};

	struct logFile
	{
		FILE* file;
		boost::gregorian::date dateCreated;
		std::string name;
	};

	Log(void);

	int prio;
	static boost::shared_ptr<Log> ptr;
	std::mutex queueLock;
	std::queue<message> msgQueue;
	std::thread logThread;
	std::map<LogType, std::string> enumMap;
	bool runThread;

	boost::shared_ptr<logFile> currentLogFile;

	void printFromQueue();
	void writeToLogFile(std::string _msg);
	boost::shared_ptr<logFile> createLogFile(); 
};

