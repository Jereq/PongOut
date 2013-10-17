#include "stdafx.h"

#include "TMLReader.h"
#include <iostream>

TMLReader::TMLReader()
{
}

TMLReader::~TMLReader()
{

}

static void addNode(TMLNODE& _node, std::istream& _is)
{
	while(true)
	{
		std::string line;

		std::getline(_is,line);

		int ksep = line.find_first_not_of('\t');
		int kend = line.find_first_of(' ');

		if(line.substr(ksep, kend) == "/" + _node.name)
			break;

		//std::string lin = line.substr(ksep, kend-ksep);

		if(kend == -1)	//we've found a new child node
		{
			TMLNODE n;
			n.name = line.substr(ksep, kend-ksep);
			TMLNODE* _n = &n;
			addNode(*_n, _is);
			_node.addChild(n);	
		}
		else
		{
			int vsep = line.find_first_not_of(' ', kend);
			int vend = line.find_last_not_of(' ');

			std::string key, value;
			key = line.substr(ksep, kend-ksep);
			value = line.substr(vsep, vend);

			_node.members.push_back(std::pair<std::string,std::string>(key, value));
		}
	}
}

std::string TMLReader::getValueFromKey(std::string key, TMLNODE& n)
{
	for(std::pair<std::string,std::string> m : n.members)
	{
		if(key == m.first)
			return m.second;
	}
	for(TMLNODE& _n : n.nodes)
	{
		std::string value = getValueFromKey(key, _n);

		if(value != "")
			return value;
	}

	return "";
}

std::string TMLReader::getValueFromKey(std::string nodeName, std::string key, TMLNODE& n)
{
	for(TMLNODE& _n : n.nodes)
	{
		if(nodeName == _n.name)
			return getValueFromKey(key, _n);
	}

	return "";
}

bool TMLReader::readFile(boost::filesystem::path _file)
{
	boost::filesystem::fstream file(_file);	
	if(file == NULL)
	{
		return false;
	}

	root.name = "Root";

	while(true)
	{
		std::string line;

		if(!std::getline(file,line))
			break;

		int ksep = line.find_first_not_of('\t');
		int kend = line.find_first_of(' ');

		if(kend == -1)
		{
			addNode(root, file);
		}
	}
	file.close();

	return true;
}

std::string TMLReader::getValueFromKey(std::string nodeName, std::string key)
{
	return getValueFromKey(nodeName, key, root);
}

std::string TMLReader::getValueFromKey(std::string key)
{
	return getValueFromKey(key, root);
}

TMLNODE TMLReader::getRoot()const
{
	return root;
}