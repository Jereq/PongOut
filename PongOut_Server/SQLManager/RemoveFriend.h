// RemoveFriend.h : Declaration of the CRemoveFriend

#pragma once

// code generated on den 9 oktober 2013, 10:51

class CRemoveFriendAccessor
{
public:




	LONG m_RETURN_VALUE;
	LONG m_myID;
	LONG m_friendID;

	void GetRowsetProperties(CDBPropSet* pPropSet)
	{
		pPropSet->AddProperty(DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
	}

	HRESULT OpenDataSource()
	{
		CDataSource _db;
		HRESULT hr;
//#error Security Issue: The connection string may contain a password
// The connection string below may contain plain text passwords and/or
// other sensitive information. Please remove the #error after reviewing
// the connection string for any security related issues. You may want to
// store the password in some other form or use a different user authentication.
		hr = _db.OpenFromInitializationString(L"Provider=SQLOLEDB.1;Password=galenpannan87(/;Persist Security Info=True;User ID=Skola;Initial Catalog=PongOut;Data Source=85.24.168.163;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=OSSIAN-PC;Use Encryption for Data=False;Tag with column collation when possible=False");
		if (FAILED(hr))
		{
#ifdef _DEBUG
			AtlTraceErrorRecords(hr);
#endif
			return hr;
		}
		return m_session.Open(_db);
	}

	void CloseDataSource()
	{
		m_session.Close();
	}

	operator const CSession&()
	{
		return m_session;
	}

	CSession m_session;

	DEFINE_COMMAND_EX(CRemoveFriendAccessor, L"{ ? = CALL dbo.RemoveFriend(?,?) }")


	BEGIN_PARAM_MAP(CRemoveFriendAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_myID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_friendID)
	END_PARAM_MAP()
};

class CRemoveFriend : public CCommand<CAccessor<CRemoveFriendAccessor>, CNoRowset >
{
public:
	HRESULT OpenAll()
	{
		HRESULT hr;
		hr = OpenDataSource();
		if (FAILED(hr))
			return hr;
		__if_exists(GetRowsetProperties)
		{
			CDBPropSet propset(DBPROPSET_ROWSET);
			__if_exists(HasBookmark)
			{
				if( HasBookmark() )
					propset.AddProperty(DBPROP_IRowsetLocate, true);
			}
			GetRowsetProperties(&propset);
			return OpenRowset(&propset);
		}
		__if_not_exists(GetRowsetProperties)
		{
			__if_exists(HasBookmark)
			{
				if( HasBookmark() )
				{
					CDBPropSet propset(DBPROPSET_ROWSET);
					propset.AddProperty(DBPROP_IRowsetLocate, true);
					return OpenRowset(&propset);
				}
			}
		}
		return OpenRowset();
	}

	HRESULT OpenRowset(DBPROPSET *pPropSet = NULL)
	{
		HRESULT hr = Open(m_session, NULL, pPropSet);
#ifdef _DEBUG
		if(FAILED(hr))
			AtlTraceErrorRecords(hr);
#endif
		return hr;
	}

	void CloseAll()
	{
		Close();
		ReleaseCommand();
		CloseDataSource();
	}
};


