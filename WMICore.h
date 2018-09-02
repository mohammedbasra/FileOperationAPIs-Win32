#pragma once
# pragma comment(lib, "wbemuuid.lib")

#include<Wbemidl.h>
#include<comdef.h>

#define WMI_SERVICE		L"winmgmt"

//#define TRACE USERMSG
//#define ERRTRACE ERRMSG

#define SAFE_OBJ_DELETE		SECURE_DELETE
#define ERRMSG				printf
#define USERMSG				printf
#define TRACE				printf

#define ERR_SUCCESS 0
#define ERR_EXCEPTION_OCCURED 1

#define MAX_NAME_SIZE 64

enum EM_SERVICE_STATUS
{
	EM_SERVICE_UNKNOWN =0,
	EM_SERVICE_STOPPED = 1,
	EM_SERVICE_START_PENDING,
	EM_SERVICE_STOP_PENDING,
	EM_SERVICE_RUNNING,
	EM_SERVICE_CONTINUE_PENDING,
	EM_SERVICE_PAUSE_PENDING,
	EM_SERVICE_PAUSED 

};

class CWMI_Core
{
	CWMI_Core(void);
	static CWMI_Core				*static_instance;
	
public:
	static HRESULT					m_hres;
	
	static CWMI_Core		*GetWMIInstance();

	static HRESULT			DestroyWMIInstance();

	static BOOL				IsConnect();
	
	HRESULT		ConnectServer( 
								/* [in] */ const BSTR strNetworkResource=L"root\\cimv2",
								/* [in] */ const BSTR strUser=NULL,
								/* [in] */ const BSTR strPassword=NULL,
								/* [in] */ const BSTR strDomain = NULL,
								/* [in] */ const BSTR sUser = NULL
							 );
	HRESULT		ExecuteQuery( 
								/* [in] */ const BSTR strQueryLanguage,
								/* [in] */ const BSTR strQuery,
								/* [in] */ long lFlags,
								/* [in] */ IEnumWbemClassObject **pTrEnum
								
							);

	HRESULT			ExecNotifyQuery(const BSTR strQueryLanguage,const BSTR strQuery,long lFlags,IEnumWbemClassObject **pTrEnum);

	HRESULT			ExecMethod(const BSTR strObjectPath,const BSTR strMethodName,IWbemClassObject *pInParams,IWbemClassObject **ppOutParams);

	HRESULT			ExecNotifyQueryAsync(const BSTR strQueryLanguage,const BSTR strQuery,long lFlags,IWbemObjectSink *pStubsink);

	DWORD			GetProcessOwner(CString &Username,CString &Domainname);

	HRESULT			GetObject(VARIANT vtProp,IWbemClassObject **pRefClassObject);

	/*****************************************************************/
	
	DWORD						GetServiceStatus(SC_HANDLE hService);

	EM_SERVICE_STATUS			CheckWMIEnvironment();

	SC_HANDLE					OpenSCManager();

	SC_HANDLE					OpenService(SC_HANDLE hSCManager, LPWSTR serviceName);

	/*****************************************************************/
	
	BOOL		Release1();
	
private:
	BOOL		Init();
	static BOOL		m_bInit;
	

	
private:
	IWbemLocator*			m_pLoc;
	IWbemServices*			m_pSvc;	
	IWbemClassObject		*m_pclsObj;
	SEC_WINNT_AUTH_IDENTITY ClientID;
	DWORD					m_dwErr;
	static BOOL				m_Connect;
	LPWSTR					FileBuf;
	CRITICAL_SECTION		m_Sync_cs;
public:
	virtual ~CWMI_Core(void);
};
/*.............................*/
/*	Display ErrorLog	*/

static void DisplayErrorInfo(DWORD status)
{
	char  szBuf[1024]={0};
	SYSTEMTIME time;

	GetLocalTime(&time);

	LPVOID lpMsgBuf;
	memset(szBuf,0,(512*sizeof(TCHAR)));

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		status,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	TRACE("----------------------Error Message------------------------------------------------\n");
	TRACE("***************************************************************\n");
	TRACE("Year:%d\nDate:%d\nMonth:%d\nTime:%d-hr_%d-min-%dsec\n",time.wYear,time.wDay,time.wMonth,time.wHour,time.wMinute,time.wSecond);
	TRACE("***************************************************************\n");

	sprintf_s(szBuf, 
		"failed with error %d: %ws\n", 
		status , lpMsgBuf); 
	TRACE(szBuf);

	TRACE("------------------------------------------------------------------------------------\n");

//	MessageBox(NULL,szBuf,_T("Network share Browser"),MB_OK);
}

