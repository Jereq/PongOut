#include "stdafx.h"
#include "SQLManager.h"
#include "CreateUser.h"
#include "VerifyUser2.h"
#include "AddFriend.h"
#include "DeleteUser.h"
#include "GetFriends.h"
#include "RemoveFriend.h"

#include <atlstr.h>

SQLManager::SQLManager(void)
{
}


SQLManager::~SQLManager(void)
{
}

long SQLManager::createUser(std::string _userName, std::string _userPassword)
{
	HRESULT hr = CoInitialize(NULL); 

	CCreateUser c;

	CString username = _userName.c_str();
	CString userPassword = _userPassword.c_str();	

	wcscpy_s(c.m_userName, (LPCTSTR)username);
	wcscpy_s(c.m_userPassword, (LPCTSTR)userPassword);


	hr = c.OpenAll();
	ATLASSERT( SUCCEEDED (hr) );
	CoUninitialize(); 
	return c.m_RETURN_VALUE;
}

long SQLManager::verifyUser( std::string _userName, std::string _userPassword )
{
	HRESULT hr = CoInitialize(NULL); 

	 CVerifyUser2 vu;

	CString username = _userName.c_str();
	CString userPassword = _userPassword.c_str();	

	wcscpy_s(vu.m_userName, (LPCTSTR)username);
	wcscpy_s(vu.m_userPassword, (LPCTSTR)userPassword);


	hr = vu.OpenAll();
	ATLASSERT( SUCCEEDED (hr) );
	CoUninitialize(); 
	return vu.m_RETURN_VALUE;
}

long SQLManager::addFriend( long _myID, long _friendID )
{
	HRESULT hr = CoInitialize(NULL); 

	CAddFriend af;

	af.m_myID = _myID;
	af.m_friendID = _friendID;

	hr = af.OpenAll();
	ATLASSERT( SUCCEEDED (hr) );
	CoUninitialize(); 
	return af.m_RETURN_VALUE;
}

long SQLManager::deleteUser( long _id )
{
	HRESULT hr = CoInitialize(NULL); 

	CDeleteUser du;

	du.m_userID = _id;

	hr = du.OpenAll();
	ATLASSERT( SUCCEEDED (hr) );
	CoUninitialize(); 
	return du.m_RETURN_VALUE;
}

long SQLManager::removeFriend( long _myID, long _friendID )
{
	HRESULT hr = CoInitialize(NULL); 

	CRemoveFriend rf;

	rf.m_myID = _myID;
	rf.m_friendID = _friendID;

	hr = rf.OpenAll();
	ATLASSERT( SUCCEEDED (hr) );
	CoUninitialize(); 
	return rf.m_RETURN_VALUE;
}

std::vector<int> SQLManager::getFriends( long _myID )
{
	HRESULT hr = CoInitialize(NULL);
	CGetFriends gf;
	std::vector<int> res;

	gf.m_myID = _myID;

	hr = gf.OpenAll();
	ATLASSERT( SUCCEEDED (hr) );

	hr = gf.MoveFirst();

	while (SUCCEEDED(hr) && hr != DB_S_ENDOFROWSET)
	{
		res.push_back(gf.m_ID);
		hr = gf.MoveNext();
	}

	gf.Close();
	gf.ReleaseCommand();
	CoUninitialize(); 

	return res;
}
