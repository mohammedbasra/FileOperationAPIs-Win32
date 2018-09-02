#include "stdafx.h"
#include "WMICore.h"
/*******************************************************************************************************/
//									   Constructor 
/*******************************************************************************************************/
CWMI_Core *CWMI_Core::static_instance = NULL;
HRESULT	CWMI_Core::m_hres = S_OK;
BOOL CWMI_Core::m_Connect = FALSE;
BOOL CWMI_Core::m_bInit = FALSE;
CWMI_Core::CWMI_Core(void)
{
	m_dwErr = 0;
	m_pLoc = NULL;
	m_pSvc = NULL;
	m_pclsObj = NULL;
//	m_pEnumerator = NULL;
	FileBuf = NULL; 
	m_Connect = FALSE;
	InitializeCriticalSection(&m_Sync_cs);
}
/*******************************************************************************************************/
//										Destructor
/*******************************************************************************************************/
CWMI_Core::~CWMI_Core(void)
{
	if(FileBuf != NULL)
	{
		delete [] FileBuf;
		FileBuf = NULL;
	}
	Release1();
}
BOOL CWMI_Core::Release1()
{
	try
	{
		if(m_pLoc != NULL)
		{
			m_pLoc->Release();
			m_pLoc = NULL;
		}
		if(m_pSvc != NULL)
		{
			m_pSvc->Release();
			m_pSvc = NULL;

		}
		if(m_pclsObj != NULL)
		{
			m_pclsObj ->Release();
			m_pclsObj = NULL;
		}
		//DeleteCriticalSection(&m_Sync_cs);
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}

}
/*
Description:	this fuction Calls CoinitializeSecurity & CocreateInstance 
				Which it is Initializeation for this Class.It must be Called Only once.

*/
CWMI_Core *CWMI_Core::GetWMIInstance()
{
	if(static_instance == NULL)
	{
		static_instance = new CWMI_Core();
		m_hres = static_instance->ConnectServer() ;
	}
	return static_instance;
	
}
HRESULT	CWMI_Core::DestroyWMIInstance()
{
	try
	{
		if(static_instance != NULL)
		{
			SAFE_OBJ_DELETE(static_instance);
			static_instance = NULL; 
			m_Connect = FALSE;			
		}
		return ERR_SUCCESS;
	}
	catch(...)
	{
		INFOLOG(L"Exception caught in CWMI_Core::DestroyWMIInstance\n");
		return ERR_EXCEPTION_OCCURED;
	}
}
BOOL CWMI_Core::Init()
{
	HRESULT		hres = NULL;
	try
	{
		if(!m_bInit)
		{
			hres =  CoInitializeSecurity(
										NULL, 
										-1,                          // COM authentication
										NULL,                        // Authentication services
										NULL,                        // Reserved
										RPC_C_AUTHN_LEVEL_NONE,   // Default authentication 
										RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
										NULL,                        // Authentication info
										EOAC_DYNAMIC_CLOAKING,      // Additional capabilities 
										NULL                         // Reserved
									);
			if (FAILED(hres))
			{
				INFOLOG(L"CWMI_Core::Init in CoInitializeSecurity:\n");
				DisplayErrorInfo(hres);
				return FALSE;                    // Program has failed.
			}
			m_bInit = TRUE;
		}
		if(m_pLoc != NULL)
		{
			m_pLoc->Release();
			m_pLoc = NULL;
		}
		hres = CoCreateInstance(
			CLSID_WbemLocator,             
			0, 
			CLSCTX_INPROC_SERVER|CLSCTX_REMOTE_SERVER, 
			IID_IWbemLocator, (LPVOID *) &m_pLoc);

		if (FAILED(hres))
		{

			INFOLOG(L"CWMI_Core::Init in CoCreateInstance:\n");
			DisplayErrorInfo(hres);
			return FALSE;                 // Program has failed.
		}
		
		
		return TRUE;
	}
	catch(...)
	{
		INFOLOG(L"Exception caught in  wmiCore:Init");
		return FALSE;
	}
}

/************************************************************************************************/
//  						Function to get the status of WMI Service
/************************************************************************************************/

// NOTES:
// -------
// SERVICE_STOPPED                        0x00000001
// SERVICE_START_PENDING                  0x00000002
// SERVICE_STOP_PENDING                   0x00000003
// SERVICE_RUNNING                        0x00000004
// SERVICE_CONTINUE_PENDING               0x00000005
// SERVICE_PAUSE_PENDING                  0x00000006
// SERVICE_PAUSED                         0x00000007

DWORD CWMI_Core::GetServiceStatus(SC_HANDLE hService)
{
	SERVICE_STATUS ss;
	if ( !QueryServiceStatus( hService, &ss ) )
	{
		INFOLOG(L"QueryServiceStatus Failed(%x)\n",GetLastError());
		return 1000;
	}
	return ss.dwCurrentState;
}

/************************************************************************************************/
//  						Function to open Service Control Manager
/************************************************************************************************/
SC_HANDLE  CWMI_Core::OpenSCManager()
{
	SC_HANDLE hSCManager = ::OpenSCManager (NULL,SERVICES_ACTIVE_DATABASE,SC_MANAGER_ALL_ACCESS);
	if ( NULL == hSCManager )

	{
		INFOLOG(L"OpenSCManager Failed : %x\n",GetLastError());
		return NULL;
	}
	return hSCManager;
}

/************************************************************************************************/
//  								Function to open WMI service
/************************************************************************************************/
SC_HANDLE CWMI_Core::OpenService(SC_HANDLE hSCManager, LPWSTR serviceName)
{
	SC_HANDLE hService = ::OpenServiceW(hSCManager,serviceName,SC_MANAGER_ALL_ACCESS);
	if ( hService == NULL )
	{
		INFOLOG(L"OpenService Failed : %x\n", GetLastError());
		return NULL;
	}
	return hService;
}


EM_SERVICE_STATUS CWMI_Core::CheckWMIEnvironment()
{
	EM_SERVICE_STATUS emstatus = EM_SERVICE_UNKNOWN;
	try
	{		
		SC_HANDLE hSCManager = OpenSCManager();
		SC_HANDLE hService = OpenService(hSCManager, WMI_SERVICE);

		emstatus = (EM_SERVICE_STATUS)GetServiceStatus(hService);
		if(emstatus == EM_SERVICE_RUNNING)
		{
			INFOLOG(L"SERVICE	: WORKING\n");
		}
		else
		{
			INFOLOG(L"Service status = %d\n", emstatus);
		}

		if ( NULL !=  hService ) 
		{
			CloseServiceHandle(hService);
			hService = NULL;
		}
		if ( NULL != hSCManager )
		{
			CloseServiceHandle(hSCManager);
			hSCManager = NULL;
		}
		return emstatus;
	}
	catch(...)
	{
		return emstatus;
	}
}

/*
	if WMI server connection established it will return true or otherwise false
*/
BOOL CWMI_Core::IsConnect()
{
	return m_Connect;
}
/*
Description:	The ConnectServer method connects to Windows Management Instrumentation (WMI) 
				on the computer specified in the strNetworkResource parameter.Do not specify strUser, strPassword, or strAuthority 
				when making a connection to a local namespace(root\\cimv2).And Set the ProxyBlanket
*/
HRESULT	CWMI_Core::ConnectServer(const BSTR strNetworkResource,const BSTR strUser,const BSTR strPassword,const BSTR Domain,const BSTR sUser)
{
	HRESULT		hres = S_OK;


	try
	{
		if(Init()==TRUE)
		{
			if(m_pSvc != NULL)
			{
				m_pSvc->Release();
				m_pSvc = NULL;
			}
			hres = m_pLoc->ConnectServer(strNetworkResource,strUser,strPassword,
				_bstr_t(L"MS_409"),NULL,NULL,0,&m_pSvc);
			if (FAILED(hres))
			{
				INFOLOG(L"CWMI_Core::ConnectServer in ConnectServer: errorcode - 0x%X\n",hres);
				return hres;                // Program has failed.
			}
			if(strUser != NULL)
			{
				ClientID.Domain		= (unsigned short*)Domain;
				ClientID.Password	= (unsigned short *)strPassword;
				ClientID.User		= (unsigned short *)strUser;
				ClientID.UserLength = (unsigned long)wcslen(strUser);
				ClientID.DomainLength = (unsigned long)wcslen(Domain);
				ClientID.PasswordLength =(unsigned long) wcslen(strPassword);
				ClientID.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
				hres = CoSetProxyBlanket(
					m_pSvc,                        // Indicates the proxy to set
					RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
					RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
					NULL,                        // Server principal name 
					RPC_C_AUTHN_LEVEL_PKT,      // RPC_C_AUTHN_LEVEL_xxx 
					RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
					&ClientID,                        // client identity
					EOAC_DYNAMIC_CLOAKING                    // proxy capabilities 
					);
			}
			else
			{
				hres = CoSetProxyBlanket(
					m_pSvc,                        // Indicates the proxy to set
					RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
					RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
					NULL,                        // Server principal name 
					RPC_C_AUTHN_LEVEL_PKT,      // RPC_C_AUTHN_LEVEL_xxx 
					RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
					NULL,                        // client identity
					EOAC_DYNAMIC_CLOAKING                    // proxy capabilities 
					);
			}

			if (FAILED(hres))
			{
				INFOLOG(L"CWMI_Core::ConnectServer in CoSetProxyBlanket: errorcode - 0x%X\n",hres);
				return hres;               // Program has failed.
			}
			m_Connect = TRUE;
			return hres;
		}
		return S_FALSE;
	}
	catch(...)
	{
		INFOLOG(L"Exception caught in CWMI_Core::ConnectServer");
		return S_FALSE;
	}
}
HRESULT	CWMI_Core::ExecuteQuery(const BSTR strQueryLanguage,const BSTR strQuery,long lFlags,IEnumWbemClassObject **pTrEnum)
{
	HRESULT hres = NULL;
	try
	{
		EnterCriticalSection(&m_Sync_cs);
		
		hres = m_pSvc->ExecQuery(strQueryLanguage,strQuery,
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
			NULL,
			pTrEnum);
		if (FAILED(hres))
		{
			INFOLOG(L"CWMI_Core::ExecuteQuery in ExecuteQuery: errorcode - 0x%X\n",hres);
			LeaveCriticalSection(&m_Sync_cs);
			return hres;               // Program has failed.
		}
		hres = CoSetProxyBlanket(*pTrEnum,                        // Indicates the proxy to set
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
			NULL,                        // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT,      // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
			NULL,
			//&ClientID,                        // client identity
			EOAC_DYNAMIC_CLOAKING                    // proxy capabilities 
			);
		if(FAILED(hres))
		{
			INFOLOG(L"CWMI_Core::ExecuteQuery in CoSetProxyBlanket:\n");
			DisplayErrorInfo(hres);
			LeaveCriticalSection(&m_Sync_cs);
			return hres;
		}
		LeaveCriticalSection(&m_Sync_cs);
		
		return hres;
	}
	catch(...)
	{
		INFOLOG(L"Exception caught in CWMI_Core::ExecuteQuery\n");
		LeaveCriticalSection(&m_Sync_cs);
		return S_FALSE;
	}
}
HRESULT	CWMI_Core::ExecNotifyQueryAsync(const BSTR strQueryLanguage,const BSTR strQuery,long lFlags,IWbemObjectSink *pStubsink)
{
	HRESULT hres = NULL;
	IWbemObjectSink   *IWbs = NULL;
	try
	{
		//EnterCriticalSection(&m_Sync_cs);
		hres = m_pSvc->ExecNotificationQueryAsync(strQueryLanguage,strQuery,lFlags,NULL,pStubsink);
		if (FAILED(hres))
		{
			INFOLOG(L"CWMI_Core::ExecuteQuery in ExecNotificationQueryAsync:\n");
			DisplayErrorInfo(hres);
			return hres;               // Program has failed.
		}
		return ERR_SUCCESS;
	}
	catch(...)
	{
		INFOLOG(L"Exception caught in CWMI_Core::ExecNotifyQueryAsync \n");
		return ERR_EXCEPTION_OCCURED;
	}

}
HRESULT	CWMI_Core::ExecNotifyQuery(const BSTR strQueryLanguage,const BSTR strQuery,long lFlags,IEnumWbemClassObject **pTrEnum)
{
	HRESULT hres = NULL;
	IWbemObjectSink   *IWbs = NULL;
	try
	{
		//EnterCriticalSection(&m_Sync_cs);
		hres = m_pSvc->ExecNotificationQuery(strQueryLanguage,strQuery,
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
			NULL,

			pTrEnum);
		if (FAILED(hres))
		{
			INFOLOG(L"CWMI_Core::ExecuteQuery in ExecQuery:\n");
			DisplayErrorInfo(hres);
			return hres;               // Program has failed.
		}
		hres = CoSetProxyBlanket(
			*pTrEnum,                        // Indicates the proxy to set
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
			NULL,                        // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT,      // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
			NULL,                        // client identity
			EOAC_DYNAMIC_CLOAKING                    // proxy capabilities 
			);
		if(FAILED(hres))
		{
			INFOLOG(L"CWMI_Core::ExecuteQuery  failed in CoSetProxyBlanket:\n");
			DisplayErrorInfo(hres);
			return hres;
		}
		
		//LeaveCriticalSection(&m_Sync_cs);
		return hres;
	}
	catch(...)
	{
		INFOLOG(L"Exception caught in CWMI_Core::ExecuteQuery \n");
		//LeaveCriticalSection(&m_Sync_cs);
		return S_FALSE;
	}
}
HRESULT	CWMI_Core::GetObject(VARIANT vtProp,IWbemClassObject **pRefClassObject)
{
	HRESULT hres = S_OK;
	try
	{
		hres = m_pSvc->GetObject(vtProp.bstrVal,0,0,pRefClassObject,0);
		if (FAILED(hres))
		{
			INFOLOG(L"CWMI_Core::GetObject() -> objectPath - %S failed due to errCode - %d\n",vtProp.bstrVal,hres);
		}
		return hres;
	}
	catch(...)
	{
		return hres;
	}
}
HRESULT	CWMI_Core::ExecMethod(const BSTR strObjectPath,const BSTR strMethodName,IWbemClassObject *pInParams,IWbemClassObject **ppOutParams)
{
	HRESULT hres = NULL;
	IWbemObjectSink   *IWbs = NULL;
	try
	{
		hres = m_pSvc->ExecMethod(strObjectPath,strMethodName,0,NULL,NULL,ppOutParams,NULL);
			
		if (FAILED(hres))
		{
			INFOLOG(L"CWMI_Core::ExecMethod() -> objectPath - %S MethodName - %S failed due to errCode - %d\n",strObjectPath,strMethodName,hres);
			return hres;               // Program has failed.
		}
		return hres;
	}
	catch(...)
	{
		INFOLOG(L"Exception caught in  CWMI_Core::ExecMethod\n");
		return S_FALSE;
	}
}

