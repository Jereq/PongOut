#pragma once
#include <cstdint>
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <boost/shared_ptr.hpp>
#include <iterator>

class msgBase
{
public:

	typedef boost::shared_ptr<msgBase> ptr;

	enum class MsgType : std::uint16_t
	{
		CHAT,
		LOGIN,
		LOGOUT,
		CREATEUSER,
		REMINDUSER,
	};

	struct header
	{
		MsgType type;
		std::uint16_t length;
	};

	

	msgBase(MsgType _type);
	virtual ~msgBase(void);
	
	header getHeader();
	virtual ptr createPacket(const std::vector<char>& _buffer) = 0;
	virtual std::vector<char> getData() = 0;
	
protected:
	header msgHeader;
	
#pragma region pack

template<class T>
	void pack(const T& _msg, std::back_insert_iterator<std::vector<char>> _dest)
	{
		std::copy((char*) &_msg, (char*) &_msg + sizeof(T), _dest);
	}

template<>
	void pack<std::string>(const std::string& _str, std::back_insert_iterator<std::vector<char>> _dest)
	{
		std::uint16_t strLen = _str.length();

		std::copy((char*) &strLen, (char*) &strLen + sizeof(strLen), _dest);
		std::copy(_str.begin(), _str.end(), _dest);
	}

template<>
	void pack<boost::uuids::uuid>(const boost::uuids::uuid& _uuid, std::back_insert_iterator<std::vector<char>> _dest)
	{
		std::copy(_uuid.begin(), _uuid.end(), _dest);
	}
#pragma endregion pack

#pragma region unpack

template<class T>
	std::vector<char>::const_iterator unpack(T& _msg, std::vector<char>::const_iterator _source)
	{
		std::copy(_source, _source + sizeof(T), (char*)&_msg);
		return _source + sizeof(T);
	}

template<>
	std::vector<char>::const_iterator unpack<std::string>(std::string& _msg, std::vector<char>::const_iterator _source)
	{
		std::uint16_t len;
		_source = unpack(len, _source);
		_msg = std::string(_source, _source + len);
		return _source + len;
	}

template<>
	std::vector<char>::const_iterator unpack<boost::uuids::uuid>(boost::uuids::uuid& _uuid, std::vector<char>::const_iterator _source)
	{
		std::copy(_source, _source + boost::uuids::uuid::static_size(), _uuid.begin());
		return _source + boost::uuids::uuid::static_size();
	}

#pragma endregion unpack
	

};

