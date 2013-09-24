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
		REQUESTFRIENDLIST,
		RESPONSEFRIENDLIST,
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

template<typename inIter, typename fm, typename sm>
	class implPack<std::pair<fm, sm>, inIter>
	{
	public:
		static void _pack(const std::pair<fm, sm>& _pair, inIter _dest)
		{
			pack(_pair.first, _dest);
			pack(_pair.second, _dest);
		}
	};

template<typename inIter, typename vecType>
	class implPack<std::vector<vecType>, inIter>
	{
	public:
		static void _pack(const std::vector<vecType>& _vector, inIter _dest)
		{
			pack(static_cast<std::uint16_t>(_vector.size()), _dest);

			for (auto a : _vector)
			{
				pack(a, _dest);
			}
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

template<typename outIter, typename fm, typename sm>
	class implUnpack<std::pair<fm, sm>, outIter>
	{
	public:
		static outIter _unpack(std::pair<fm, sm>& _pair, outIter _source)
		{
			_source = unpack(_pair.first, _source);
			_source = unpack(_pair.second, _source);
			return _source;
		}
	};

template<typename outIter, typename vecType>
	class implUnpack<std::vector<vecType>, outIter>
	{
	public:
		static outIter _unpack(std::vector<vecType>& _vector, outIter _source)
		{
			std::uint16_t len;
			_source = unpack(len, _source);
			_vector.resize(len);

			for (int i = 0; i < len; i++)
			{
				_source = unpack(_vector[i], _source);
			}
			
			return _source;
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

