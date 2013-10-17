#ifndef __TMLREADER_H
#define __TMLREADER_H

#include <boost/filesystem/fstream.hpp>
#include <string>
#include <vector>

struct TMLNODE
{
	std::string name;
	std::vector<std::pair<std::string, std::string>> members;
	std::vector<TMLNODE> nodes;

	void addChild(TMLNODE& n)
	{ nodes.push_back(n); }

	void addMember(const std::string key, const std::string value)
	{
		members.push_back(std::pair<std::string,std::string>(key, value));
	}
};

class TMLReader
{
private:
	TMLNODE root;
	std::string getValueFromKey(std::string nodeName, std::string key, TMLNODE& n);
	std::string getValueFromKey(std::string key, TMLNODE& n);
	//void addNode(NODE& _node, std::istream& _is);



public:
	TMLReader();
	~TMLReader();
	bool readFile(boost::filesystem::path _file);
	std::string getValueFromKey(std::string nodeName, std::string key);
	std::string getValueFromKey(std::string key);
	TMLNODE getRoot()const;
};

#endif