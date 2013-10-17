#pragma once
#include <cstdint>
#include <vector>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <iterator>
#include <map>

#include "CommonTypes.h"

class msgBase
{	
public:	
	typedef boost::shared_ptr<msgBase> ptr;

	enum class MsgType : std::uint16_t
	{
		CHAT = 1,
		CONNECTSUCCESS,
		LOGINREQUEST,
		LOGOUTREQUEST,
		CREATEUSERREQUEST,
		REMINDUSER,
		FRIENDLISTREQUEST,
		FRIENDLISTRESPONSE,
		INTERNALMESSAGE,
		GAMEMESSAGE,
		ACKNOWLEDGELAST,
	};

	struct header
	{
		MsgType type;
		std::uint16_t length;
	};

	msgBase(MsgType _type);
	virtual ~msgBase(void);
	
	header getHeader();
	virtual ptr interpretPacket(const std::deque<char>& _buffer) = 0;
	virtual std::vector<char> getData() = 0;

	std::string getType();

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

template<typename inIter>
	class implPack<CommonTypes::Block, inIter>
	{
	public:
		static void _pack(const CommonTypes::Block& _block, inIter _dest)
		{
			pack(_block.health, _dest);
			pack(_block.id, _dest);
			pack(_block.x, _dest);
			pack(_block.y, _dest);
			pack(_block.z, _dest);
			pack(_block.textures, _dest);
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

	std::map<MsgType, std::string> typeMap;




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

template<typename outIter>
	class implUnpack<CommonTypes::Block, outIter>
	{
	public:
		static outIter _unpack(CommonTypes::Block& _block, outIter _source)
		{
			_source = unpack(_block.health, _source);
			_source = unpack(_block.id, _source);
			_source = unpack(_block.x, _source);
			_source = unpack(_block.y, _source);
			_source = unpack(_block.z, _source);
			_source = unpack(_block.textures, _source);

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

