#pragma once
#include <cstdint>
#include <vector>
#include <deque>
#include <boost/uuid/uuid.hpp>
#include <boost/shared_ptr.hpp>
#include <iterator>

class msgBase
{	
public:	
	typedef boost::shared_ptr<msgBase> ptr;

	enum class MsgType : std::uint16_t
	{
		CHAT = 1,
		LOGIN,
		LOGOUT,
		CREATEUSER,
		REMINDUSER,
		GETFRIENDS,
	};

	struct header
	{
		MsgType type;
		std::uint16_t length;
	};

	struct userData
	{
		std::string userName, password;
		boost::uuids::uuid uuid;

		userData() {}
		userData(std::string _userName, std::string _password, boost::uuids::uuid _uuid)
		{
			userName = _userName;
			password = _password;
			uuid = _uuid;
		}
	};	

	msgBase(MsgType _type);
	virtual ~msgBase(void);
	
	header getHeader();
	virtual ptr interpretPacket(const std::deque<char>& _buffer) = 0;
	virtual std::vector<char> getData() = 0;
	
protected:
	header msgHeader;
	
#pragma region pack

private:

template<typename T, typename inIter>
	class implPack
	{
	public:
		static void _pack(const T& _msg, inIter _dest)
		{

			std::copy((char*) &_msg, (char*) &_msg + sizeof(T), _dest);
		}
	};

template<typename inIter>
	class implPack<std::string, inIter>
	{
	public:
		static void _pack(const std::string& _str, inIter _dest)
		{
			std::uint16_t strLen = _str.length();

			std::copy((char*) &strLen, (char*) &strLen + sizeof(strLen), _dest);
			std::copy(_str.begin(), _str.end(), _dest);
		}
	};

template<typename inIter>
	class implPack<boost::uuids::uuid, inIter>
	{
	public:
		static void _pack(const boost::uuids::uuid& _uuid, inIter _dest)
		{
			std::copy(_uuid.begin(), _uuid.end(), _dest);
		}
	};

public:

template<typename T, typename inIter>
	static void pack(const T& _msg, inIter _dest)
	{
		return implPack<T, inIter>::_pack(_msg, _dest);
	}

#pragma endregion pack

#pragma region unpack

private:
template<typename T, typename outIter>
	class implUnpack
	{
	public:
		static outIter _unpack(T& _msg, outIter _source)
		{
			std::copy(_source, _source + sizeof(T), (char*)&_msg);
			return _source + sizeof(T);
		}
	};

template<typename outIter>
	class implUnpack<std::string, outIter>
	{
	public:
		static outIter _unpack(std::string& _msg, outIter _source)
		{
			std::uint16_t len;
			_source = unpack(len, _source);
			_msg = std::string(_source, _source + len);
			return _source + len;
		}
	};

template<typename outIter>
	class implUnpack<boost::uuids::uuid, outIter>
	{
	public:
		static outIter _unpack(boost::uuids::uuid& _uuid, outIter _source)
		{
			std::copy(_source, _source + boost::uuids::uuid::static_size(), _uuid.begin());
			return _source + boost::uuids::uuid::static_size();
		}
	};

public:
template<typename T, typename outIter>
	static outIter unpack(T& _msg, outIter _source)
	{
		return implUnpack<T, outIter>::_unpack(_msg, _source);
	}

#pragma endregion unpack
	
	friend class PacketHandler;
};

