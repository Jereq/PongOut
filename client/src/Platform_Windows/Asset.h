#ifndef __ASSET_H
#define __ASSET_H

template<typename T>
class Asset
{
public:

protected:
	Asset(T* _data, int _id)
		: data(_data), id(_id)
	{}

	virtual ~Asset()
	{}

	virtual void clean() = 0;	//if special cleanup operationens are needed for data - define in derived class

	int	getId()const
	{ return id; }

private:
	T*	data;
	int id;
};

#endif