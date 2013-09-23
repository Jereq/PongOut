#pragma once
#include <string>

template<typename T>
class Asset
{
protected:
	Asset()
		: name("EMPTY"), refCount(0)
	{
	}
	virtual ~Asset(){}

public:
	T* getData()const
	{
		refCount++;
		return data;
	};

	std::string	getName()const
	{
		return name;
	}

	virtual bool clear() = 0;

	bool initialize(T* _data, std::string _name)
	{
		if(m_data == NULL)
			return false;

		data = _data;
		name = _name;

		return true;
	}

	void dereference()
	{
		refCount--;
	}

	unsigned int getRefCount()const
	{
		return refCount;
	}

protected:
	T*			data;
	std::string	name;

private:
	unsigned int refCount;
};
