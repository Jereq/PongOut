#pragma once
#include <boost/shared_ptr.hpp>
#include <queue>
#include <cstdint>
#include <thread>
#include <boost/enable_shared_from_this.hpp>
#include <map>

class Log : public boost::enable_shared_from_this<Log>
{
public:

	enum class LogType : std::uint8_t
	{
		INFO,
		ERROR,
		DEBUG,
	};

	static boost::shared_ptr<Log> getInstance();
	
	~Log(void);

	static void addLog(LogType _type, std::string _msg);
	static void destroy();

private:

	typedef std::pair<LogType, std::string> msg;
	Log(void);
	static boost::shared_ptr<Log> ptr;
	std::mutex queueLock;
	std::queue<msg> msgQueue;
	std::thread logThread;
	std::map<LogType, std::string> enumMap;
	bool runThread;


	void printFromQueue();
};

