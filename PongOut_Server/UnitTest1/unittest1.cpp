#include "stdafx.h"
#include "CppUnitTest.h"

#include <msgBase.h>
#include <Chat.h>
#include <vector>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/shared_ptr.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(TestMsgBase)
	{
	public:
		
		TEST_METHOD(PackHeader)
		{
			std::vector<char> res;
			std::back_insert_iterator<std::vector<char>> it(res);

			msgBase::header h;
			h.length = 100;
			h.type = msgBase::MsgType::CHAT;

			msgBase::pack(h, it);

			std::vector<char> facit(4); 
			facit[0] = 1;
			facit[1] = 0;
			facit[2] = 100;
			facit[3] = 0;

			Assert::AreEqual(facit.size(), res.size(), L"Size mismatch", LINE_INFO());
			Assert::IsTrue(facit == res, L"Data mismatch", LINE_INFO());
		}

		TEST_METHOD(PackUUID)
		{
			boost::uuids::uuid u = boost::uuids::random_generator()();
			std::vector<char> res;
			std::back_insert_iterator<std::vector<char>> it(res);

			msgBase::pack(u, it);

			std::vector<char> facit(u.begin(), u.end());

			Assert::AreEqual(facit.size(), res.size(), L"Size mismatch", LINE_INFO());
			Assert::IsTrue(facit == res, L"Data mismatch", LINE_INFO());
		}

		TEST_METHOD(PackString)
		{
			std::string s = "abcdef";
			char expected[] = "\6\0abcdef";
			std::vector<char> res;
			std::back_insert_iterator<std::vector<char>> it(res);

			msgBase::pack(s, it);

			std::vector<char> facit(expected, expected + sizeof(expected) - 1);

			Assert::AreEqual(facit.size(), res.size(), L"Size mismatch", LINE_INFO());
			Assert::IsTrue(facit == res, L"Data mismatch", LINE_INFO());
		}

		TEST_METHOD(PackStdPair)
		{
			std::pair<int, int> p(10, 10);			
			std::vector<char> res;
			std::back_insert_iterator<std::vector<char>> it(res);

			msgBase::pack(p, it);

			std::vector<char> facit;
			std::back_insert_iterator<std::vector<char>> fit(facit);

			msgBase::pack(10, fit);
			msgBase::pack(10, fit);

			Assert::AreEqual(facit.size(), res.size(), L"Size mismatch", LINE_INFO());

			std::wstringstream wss;

			for (unsigned int i = 0; i < facit.size(); i++)
			{
				wss << "data mismatch at index: " << i << " is: " << (int)res[i] << " should be: " << (int)facit[i];
				Assert::AreEqual(facit[i], res[i], wss.str().c_str(), LINE_INFO());
			}			
		}

		TEST_METHOD(PackStdVector)
		{
			std::vector<int> vec;
			std::vector<char> res, facit;
			std::back_insert_iterator<std::vector<char>> it(res);
			std::back_insert_iterator<std::vector<char>> fit(facit);

			msgBase::pack(static_cast<std::uint16_t>(100), fit);

			for (int i = 0; i < 100; i ++)
			{
				vec.push_back(i);
				msgBase::pack(i, fit);
			}

			msgBase::pack(vec, it);

			Assert::AreEqual(facit.size(), res.size(), L"Size mismatch", LINE_INFO());
		}

		TEST_METHOD(UnPackHeader)
		{
			std::vector<char> sourceVector;
			std::back_insert_iterator<std::vector<char>> sit(sourceVector);

			msgBase::header sourceHeader;
			sourceHeader.length = 100;
			sourceHeader.type = msgBase::MsgType::CHAT;

			msgBase::pack(sourceHeader, sit);

			std::vector<char>::const_iterator rit = sourceVector.cbegin();
			msgBase::header res;

			rit = msgBase::unpack(res, rit);

			Assert::IsTrue(sourceHeader.length == res.length, L"Header length mismatch", LINE_INFO());
			Assert::IsTrue(sourceHeader.type == res.type, L"Header type mismatch", LINE_INFO());
		}

		TEST_METHOD(UnPackUUID)
		{
			boost::uuids::uuid sourceUUID = boost::uuids::random_generator()();
			std::vector<char> sourceVector;
			std::back_insert_iterator<std::vector<char>> sit(sourceVector);

			msgBase::pack(sourceUUID, sit);

			std::vector<char>::const_iterator rit = sourceVector.cbegin();
			boost::uuids::uuid res;

			rit = msgBase::unpack(res, rit);

			Assert::IsTrue(sourceUUID == res, L"UUID mismatch", LINE_INFO());
		}

		TEST_METHOD(UnPackString)
		{
			std::string sourceString = "abcdef";
			std::vector<char> sourceVector;
			std::back_insert_iterator<std::vector<char>> sit(sourceVector);

			msgBase::pack(sourceString, sit);

			std::vector<char>::const_iterator rit = sourceVector.cbegin();
			std::string res;

			rit = msgBase::unpack(res, rit);

			Assert::AreEqual(sourceString.size(), res.size(), L"String length mismatch", LINE_INFO());
			Assert::AreEqual(sourceString, res, L"Strings not equal", LINE_INFO());
		}

		TEST_METHOD(UnPackStdPair)
		{
			std::pair<int, int> sourcePair(10, 10);			
			std::vector<char> sourceVector;
			std::back_insert_iterator<std::vector<char>> sit(sourceVector);

			msgBase::pack(sourcePair, sit);

			std::vector<char>::const_iterator rit = sourceVector.cbegin();
			std::pair<int, int> res;

			rit = msgBase::unpack(res, rit);

			Assert::AreEqual(sourcePair.first, res.first, L"", LINE_INFO());
			Assert::AreEqual(sourcePair.second, res.second, L"", LINE_INFO());
		}

		TEST_METHOD(UnPackStdVector)
		{
			std::vector<int> source;

			for (int i = 0; i < 100; i++)
			{
				source.push_back(i);
			}

			std::vector<char> sourceVector;
			std::back_insert_iterator<std::vector<char>> sit(sourceVector);

			msgBase::pack(source, sit);

			std::vector<char>::const_iterator rit = sourceVector.cbegin();
			std::vector<int> res;

			rit = msgBase::unpack(res, rit);

			Assert::AreEqual(source.size(), res.size(), L"Size mismatch", LINE_INFO());
		}
	};
}