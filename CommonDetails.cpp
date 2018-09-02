
#include "stdafx.h"
#include "CommonDetails.h"

////////////////////////////////
// Built-in Headers
////////////////////////////////
#include <LM.h>
#include <Wbemidl.h>
#include <comdef.h>
#include <Iphlpapi.h>
#include <strsafe.h>
#include <Sddl.h>
#include "..\include\CommonDetails.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")		//ip mac
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Advapi32.lib")

/////////////////////////////////
// Global Variables
/////////////////////////////////
static const LPTSTR gpCFileName = _T("CommonDetails.cpp");

/////////////////////////////////
// Function definitions
/////////////////////////////////
namespace OS_Manager
{

CCommonDetails::CCommonDetails()//datamodel choice)
{
		//pParserObj = pParserObj->create(choice);

		m_UUID.Empty();
		m_ComputerFullName.Empty();
		m_ComputerDomainName.Empty();
		m_IsDomain = false;
		m_ComputerDNSName.Empty();
		m_ComputerName.Empty();
		m_MACAddress.Empty();
		m_SerialNumber.Empty();
		m_IPAddress.Empty();
		m_OSName.Empty();
		m_COMStatus = false;
		m_Timestamp = 0;
		m_UserAccountList.clear();
		m_ProcessorArchitecture = 0;
		m_OSArchitecture = 0;
		m_OSMajorVersion = 0;
		m_OSMinorVersion = 0;
		m_ProductType = 0;
		m_SuiteMask = 0;
		m_BuildNumber = 0;
		m_ManufacturerName.Empty();
		m_Model.Empty();
		m_IsOfflineImgCreation = FALSE;
		m_IsSecureBoot = 0;
		m_BIOSMode.Empty();
}

CCommonDetails::~CCommonDetails(void)
{
}

CAtlString CCommonDetails::getModelName()
{
	return m_Model;
}

INT64 CCommonDetails::getTimeStamp()
{
	return m_Timestamp;
}
void CCommonDetails::setTimeStamp(INT64 i_TimeStamp)
{
	m_Timestamp = i_TimeStamp;
}

BOOL CCommonDetails::getCOMStatus()
{
	return m_COMStatus;
}

void CCommonDetails::setCOMStatus(BOOL status)
{
	m_COMStatus = status;
}

CString CCommonDetails::getComputerName()
{
	return m_ComputerName;
}
CString CCommonDetails::getComputerFullName()
{
	return m_ComputerFullName;
}
CString CCommonDetails::getComputerDomainName()
{
	return m_ComputerDomainName;
}
BOOL CCommonDetails::getIsDomain()
{
	return m_IsDomain;
}
CString CCommonDetails::getComputerDNSName()
{
	return m_ComputerDNSName;
}
CString CCommonDetails::getMACAddress()
{
	return m_MACAddress;
}

void CCommonDetails::setMacAddress(CString i_MAC_Address)
{
	m_MACAddress = i_MAC_Address;
}

CString CCommonDetails::getUUID()
{
	return m_UUID;
}
CString CCommonDetails::getSerialNumber()
{
	return m_SerialNumber;
}
CString CCommonDetails::getIPAddress()
{
	return m_IPAddress;
}
CString CCommonDetails::getOSName()
{
	return m_OSName;
}

CString CCommonDetails::getOSDrive()
{
	this->fetchOSDrive();
	return m_OSDrive;
}

INT64 CCommonDetails::getProcessorArchitecture()
{
	return this->m_ProcessorArchitecture;
}
INT64 CCommonDetails::getOSArchitecture()
{
	return this->m_OSArchitecture;
}
INT64 CCommonDetails::getOSMajorVersion()
{
	return this->m_OSMajorVersion;
}
INT64 CCommonDetails::getOSMinorVersion()
{
	return this->m_OSMinorVersion;
}
INT64 CCommonDetails::getProductType()
{
	return this->m_ProductType;
}
INT64 CCommonDetails::getSuiteMask()
{
	return this->m_SuiteMask;
}
INT64 CCommonDetails::getBuildNumber()
{
	return this->m_BuildNumber;
}

void CCommonDetails::setOfflineImgCreation(BOOL i_IsOfflineImgCreation)
{
	this->m_IsOfflineImgCreation = i_IsOfflineImgCreation;
}
BOOL CCommonDetails::getOfflineImgCreation()
{
	return this->m_IsOfflineImgCreation;
}

INT64 CCommonDetails::getOSType()
{
	return this->m_OSType;
}

CAtlString CCommonDetails::getProcessorName()
{
	return this->m_ProcessorName;
}

INT64 CCommonDetails::getRamSize()
{
	return this->m_RamSize;
}

USER_ACCOUNT_DETAIL_LIST CCommonDetails::getUserAccountList()
{
	return m_UserAccountList;
}

CAtlString CCommonDetails::getManufacturerName()
{
	return m_ManufacturerName;
}

CAtlString CCommonDetails:: getBIOSMode()
{
	return m_BIOSMode;
}
VOID CCommonDetails:: setBIOSMode(CAtlString i_BIOSMode)
{
	m_BIOSMode = i_BIOSMode;
}

BOOL CCommonDetails:: getIsSecureBoot()
{
	return m_IsSecureBoot;
}
VOID CCommonDetails:: setIsSecureBoot(BOOL i_IsSecureBoot)
{
	i_IsSecureBoot = i_IsSecureBoot;
}

INT64 CCommonDetails::getOSIntValue()
{
	TCHAR szOSName[COMPDETAIL_BUFFERSIZE];
	GetOSNameString(szOSName);
	return m_OSIntValue;
}
VOID CCommonDetails::setOSIntValue(INT64 i_OSIntValue)
{
	m_OSIntValue = i_OSIntValue;
}

OSD_RESULT CCommonDetails::OSDriveQuery(CAtlString i_DriveLetter, BOOL& IsOSDrive, CAtlString& io_OSName)
{
	OSD_RESULT Result;
	try
	{
		std::vector<CAtlString> sysFilePaths; 
		sysFilePaths.push_back(SYS_SYSTEM);
		sysFilePaths.push_back(SYS_SYSTEM32);
		sysFilePaths.push_back(SYS_SYSWOW64);

		INT OSCheck[3] = {0};

		for( INT index = 0; index < sysFilePaths.size(); index++)
		{
			CAtlString FileFullPath;
			FileFullPath = i_DriveLetter;
			FileFullPath.Append(sysFilePaths[index].GetBuffer());

			if(INVALID_FILE_ATTRIBUTES != GetFileAttributes(FileFullPath.GetBuffer()))
			{
				//_tprintf(_T("\n %s    Directory Found !! "),FullPathtoChk);
				OSCheck[index] = 1;
			}
			else
			{
				//_tprintf(_T("\n %s    Directory not Found !! "),FullPathtoChk);
				OSCheck[index] = 0;
			}
		}

		if( OSCheck[0] && OSCheck[1] || OSCheck[2] )
		{
			IsOSDrive = TRUE;
		}
		else
		{
			IsOSDrive = FALSE;
		}
		/*else if( OSCheck[0] || OSCheck[1] )
		{
			IsOSDrive = FALSE;
		}
		else
		{
			IsOSDrive = TRUE;
		}*/

		if(IsOSDrive)
		{
			TCHAR szOSName[COMPDETAIL_BUFFERSIZE];
			Result = GetOSNameString(szOSName);
			if(OSD_ERROR_SUCCESS != Result.RetCode)
			{
				wprintf(TEXT("[%s : %d] Failed to get OS name (%d)\n"), 
						gpCFileName, 
						__LINE__, 
						Result.RetCode); 
				OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
				return Result;
			}
			io_OSName = szOSName;
		}

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);
		return Result;
		
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return Result;
}

OSD_RESULT CCommonDetails::WMIQuery(CString cstrNetworkResource, CString cstrQueryClass, CString cstrQueryProperty, CString &ctsrQueryValue)
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);

		CWMI_Core *WmiObj = CWMI_Core::GetWMIInstance();
		IEnumWbemClassObject *classEnum;
		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;
		HRESULT hres;

		hres = WmiObj->ConnectServer();

		if (FAILED(hres))
		{
			INFOLOG(TEXT("[%s : %d] Failed to create connection to WMI namespace (0x%X)\n"), 
						gpCFileName, 
						__LINE__, 
						hres); 

			WmiObj->DestroyWMIInstance();

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SYSTEM_ERROR);

			OSD_SET_EXTERNAL_ERR(result, OSD_EXTMODULE_WMI, hres);
			return result;                
		}

		hres = WmiObj->ExecuteQuery(_bstr_t(L"WQL"),_bstr_t(cstrQueryClass),NULL,&classEnum);

		if (FAILED(hres))
		{
			INFOLOG(TEXT("[%s : %d] Failed to execute query (0x%X)\n"), 
						gpCFileName, 
						__LINE__, 
						hres); 

			WmiObj->DestroyWMIInstance();

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SYSTEM_ERROR);

			OSD_SET_EXTERNAL_ERR(result, OSD_EXTMODULE_WMI, hres);
			return result;              
		}

		while (classEnum)
		{
			HRESULT hr = classEnum->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if(0 == uReturn)
			{	
				break;
			}

			VARIANT vtProp;
			VariantInit(&vtProp);
			CString cstrProperty;

			// Get the value of the Name property
			hr = pclsObj->Get(cstrQueryProperty, 0, &vtProp, 0, 0);
			if(WBEM_S_NO_ERROR == hr)
			{	
				cstrProperty.Format(_T("%s"), (LPCTSTR)vtProp.bstrVal);
				ctsrQueryValue = cstrProperty;
			}
			else
			{
				INFOLOG(TEXT("[%s : %d] Invalid property name (0x%X)\n"), 
						gpCFileName, 
						__LINE__, 
						hr); 

				OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SYSTEM_ERROR);

				OSD_SET_EXTERNAL_ERR(result, OSD_EXTMODULE_WMI, hr);
			}
			VariantClear(&vtProp);	
		}

		WmiObj->DestroyWMIInstance();

		return result;
	}
		
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails::fetchProcessorName()
{
	OSD_RESULT result;
	try
	{
		int CPUInfo[4] = {-1};
		char CPUBrandString[0x40];	
		__cpuid(CPUInfo, 0x80000000);
		unsigned int nExIds = CPUInfo[0];

		memset(CPUBrandString, 0, sizeof(CPUBrandString));

		// Get the information associated with each extended ID.
		for (unsigned int i=0x80000000; i<=nExIds; ++i)
		{
			__cpuid(CPUInfo, i);
			// Interpret CPU brand string.
			if  (i == 0x80000002)
				memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
			else if  (i == 0x80000003)
				memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
			else if  (i == 0x80000004)
				memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
		}

		m_ProcessorName = CPUBrandString;

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);
		return result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);
	}
	OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	return result;
}

OSD_RESULT CCommonDetails::fetchRAMSize()
{
	OSD_RESULT result;
	try
	{
		MEMORYSTATUSEX memstruct;

		memstruct.dwLength = sizeof(memstruct);

		if(!GlobalMemoryStatusEx(&memstruct))
		{
			DWORD dwLastErr = ::GetLastError();
			INFOLOG(TEXT("[%s : %d] Failed to retrieve the system physical memory information (%d)\n"), 
						gpCFileName, 
						__LINE__, 
						dwLastErr);

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(result, OSD_EXTMODULE_MEMORY, dwLastErr);
			return result;
		}

		m_RamSize = memstruct.ullTotalPhys;

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);
		return result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);
	}
	OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	return result;
}

OSD_RESULT CCommonDetails::fetchComputerName()
{
	OSD_RESULT result;
	try
	{
		TCHAR buffer[CHAR_MAX] = TEXT("");
		DWORD dwsize = sizeof(buffer);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);

		if(GetComputerNameEx(ComputerNameDnsHostname,buffer, &dwsize))
		{
			CCommonDetails::m_ComputerName = buffer;
		}
		else
		{
			INFOLOG(TEXT("[%s : %d] Failed to get Computer Name (%d)\n"), 
							gpCFileName,
							__LINE__,
							GetLastError());

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
		}
		return result;
	}

	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

//OSD_RESULT CCommonDetails::fetchUserAccountDetails()
//{
//	OSD_RESULT result;
//	try
//	{
//		LPUSER_INFO_1 pBuf = NULL;
//		LPUSER_INFO_1 pTmpBuf;
//		DWORD dwLevel = 1;
//		DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
//		DWORD dwEntriesRead = 0;
//		DWORD dwTotalEntries = 0;
//		DWORD dwResumeHandle = 0;
//		DWORD i;
//		DWORD dwTotalCount = 0;
//		NET_API_STATUS nStatus;
//		LPTSTR pszServerName = NULL;
//
//		m_UserAccountList.clear();
//
//		do 
//		{
//			nStatus = NetUserEnum((LPCWSTR) pszServerName,
//									dwLevel,
//									FILTER_NORMAL_ACCOUNT, // global users
//									(LPBYTE*)&pBuf,
//									dwPrefMaxLen,
//									&dwEntriesRead,
//									&dwTotalEntries,
//									&dwResumeHandle);
//			if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
//			{
//				if ((pTmpBuf = pBuf) != NULL)
//				{
//					for (i = 0; (i < dwEntriesRead); i++)
//					{
//						//assert(pTmpBuf != NULL);
//
//						if (pTmpBuf == NULL)
//						{
//							INFOLOG(TEXT("[%s : %d]>>%S() : An access violation has occurred"), 
//									gpCFileName, 
//									__LINE__, 
//									__FUNCTION__);
//							break;
//						}
//
//						USER_ACCOUNT_DETAIL temp_userActDetails;
//
//						temp_userActDetails.UserName = pTmpBuf->usri1_name;
//						temp_userActDetails.Group = pTmpBuf->usri1_priv;
//
//						DWORD dwLevel = 4;
//						LPUSER_INFO_4 pUserBuf = NULL;
//
//						nStatus = NetUserGetInfo((LPCWSTR) pszServerName,
//										pTmpBuf->usri1_name,
//										dwLevel,
//										(LPBYTE*)&pUserBuf);
//
//						if(nStatus == NERR_Success)
//						{
//							LPTSTR pUserSID = NULL;
//							if(ConvertSidToStringSid(pUserBuf->usri4_user_sid,&pUserSID))
//							{
//								temp_userActDetails.UserSID = pUserSID;
//							}
//							else
//							{
//								temp_userActDetails.UserSID.Empty();
//								INFOLOG(TEXT("[%s : %d]>>%S() : A system error has occurred in ConvertSidToStringSid : Result [%d] Error [%d]"), 
//																gpCFileName, __LINE__, __FUNCTION__, nStatus, ::GetLastError());
//							}
//							LocalFree(pUserSID);
//						}
//						else
//						{
//							temp_userActDetails.UserSID.Empty();
//							INFOLOG(TEXT("[%s : %d]>>%S() : A system error has occurred in LPUSER_INFO_4 : Result [%d] Error [%d]"), 
//															gpCFileName, __LINE__, __FUNCTION__, nStatus, ::GetLastError());
//						}
//
//						if (pUserBuf != NULL)
//							NetApiBufferFree(pUserBuf);
//
//						m_UserAccountList.push_back(temp_userActDetails);
//
//						pTmpBuf++;
//						dwTotalCount++;
//					}
//				}
//			}
//	
//			else
//			{
//				INFOLOG(TEXT("[%s : %d]>>%S() : A system error has occurred: %d"), 
//								gpCFileName, 
//								__LINE__, 
//								__FUNCTION__,
//								nStatus);
//			}
//			if (pBuf != NULL)
//			{
//				NetApiBufferFree(pBuf);
//				pBuf = NULL;
//			}
//		}while (nStatus == ERROR_MORE_DATA); 
//
//		if (pBuf != NULL)
//			NetApiBufferFree(pBuf);
//
//		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);
//		return result;
//	}
//	catch(std::exception &exc)
//	{
//		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
//					gpCFileName,
//					__LINE__, 
//					__FUNCTION__, 
//					exc.what());
//
//		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
//	}
//	catch(...)
//	{
//		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
//					gpCFileName, 
//					__LINE__, 
//					__FUNCTION__);
//
//		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
//	}
//	return result;
//}

OSD_RESULT CCommonDetails::fetchUsersinGroup(LPWSTR GroupName)
{
	OSD_RESULT Result;
	try
	{
		LPLOCALGROUP_MEMBERS_INFO_2 pBuf = NULL;
		LPLOCALGROUP_MEMBERS_INFO_2 pTmpBuf;
		DWORD dwLevel = 2;
		DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
		DWORD dwEntriesRead = 0;
		DWORD dwTotalEntries = 0;
		DWORD_PTR dwResumeHandle = 0;
		DWORD i;
		DWORD dwTotalCount = 0;
		NET_API_STATUS nStatus;
		LPTSTR pszServerName = NULL;

		do 
		{
			nStatus = NetLocalGroupGetMembers((LPCWSTR) pszServerName,
							(LPCWSTR) GroupName,
                            dwLevel,
                            (LPBYTE*)&pBuf,
                            dwPrefMaxLen,
                            &dwEntriesRead,
                            &dwTotalEntries,
                            &dwResumeHandle);

			if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
			{
				if ((pTmpBuf = pBuf) != NULL)
				{
					for (i = 0; (i < dwEntriesRead); i++)
					{
						//assert(pTmpBuf != NULL);

						if (pTmpBuf == NULL)
						{
							INFOLOG(TEXT("[CCommonDetails::fetchUsersinGroup() %d]: An access violation has occurred"),__LINE__);
							break;
						}

						DWORD LeftIndex, RightIndex;
						CAtlString UserFullName;
						USER_ACCOUNT_DETAIL temp_userActDetails;

						UserFullName = pTmpBuf->lgrmi2_domainandname;
						LeftIndex = UserFullName.Find(L"\\");
						RightIndex = UserFullName.GetLength() - (LeftIndex + 1) ;

						temp_userActDetails.UserName = UserFullName.Right(RightIndex);
						temp_userActDetails.DomainName = UserFullName.Left(LeftIndex);
						temp_userActDetails.GroupName = GroupName;
						
						LPTSTR pUserSID = NULL;
						if(ConvertSidToStringSid(pTmpBuf->lgrmi2_sid,&pUserSID))
						{
							temp_userActDetails.UserSID = pUserSID;
						}
						else
						{
							temp_userActDetails.UserSID.Empty();
							INFOLOG(TEXT("[CCommonDetails::fetchUsersinGroup() %d]: A system error has occurred : Error [%d]"), 
															 __LINE__, ::GetLastError());
						}

						BOOL IsLocalUser, IsDomainUser;

						IsLocalUser  = (0 == temp_userActDetails.DomainName.CompareNoCase(this->m_ComputerName));
						IsDomainUser = (0 == temp_userActDetails.DomainName.CompareNoCase(this->m_ComputerDomainName));
						
						if(IsLocalUser || IsDomainUser)
						{
							temp_userActDetails.IsDomainUser = IsDomainUser;
							temp_userActDetails.DomainDNSName = this->m_ComputerDNSName;
							this->m_UserAccountList.push_back(temp_userActDetails);
						}

						pTmpBuf++;
						dwTotalCount++;
					}
				}
			}
	
			else
			{
				INFOLOG(TEXT("[CCommonDetails::fetchUsersinGroup() %d]: A system error has occurred: %d"), 
								 __LINE__,nStatus);
			}
			if (pBuf != NULL)
			{
				NetApiBufferFree(pBuf);
				pBuf = NULL;
			}

		}while (nStatus == ERROR_MORE_DATA); 

		if (pBuf != NULL)
			NetApiBufferFree(pBuf);

		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEINSTALL_PCH,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CCommonDetails::fetchUsersinGroup() %d] Standard Exception caught : %s",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CCommonDetails::fetchUsersinGroup() %d] Unknown Exception caught",__LINE__);
	}

	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEINSTALL_PCH,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CCommonDetails::fetchUserAccountDetails()
{
	OSD_RESULT Result;
	try
	{
		LPLOCALGROUP_INFO_0 pBuf = NULL;
	    LPLOCALGROUP_INFO_0 pTmpBuf;
	    DWORD dwLevel = 0;
		DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
		DWORD dwEntriesRead = 0;
		DWORD dwTotalEntries = 0;
		DWORD_PTR dwResumeHandle = 0;
		DWORD i;
		DWORD dwTotalCount = 0;
		NET_API_STATUS nStatus;
		LPTSTR pszServerName = NULL;

		m_UserAccountList.clear();

		this->fetchComputerName();
		this->fetchComputerDomainName();

		do 
		{
			 nStatus = NetLocalGroupEnum((LPCWSTR) pszServerName,
                            dwLevel,
                            (LPBYTE*)&pBuf,
                            dwPrefMaxLen,
                            &dwEntriesRead,
                            &dwTotalEntries,
                            &dwResumeHandle);

			if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
			{
				if ((pTmpBuf = pBuf) != NULL)
				{
					for (i = 0; (i < dwEntriesRead); i++)
					{
						//assert(pTmpBuf != NULL);

						if (pTmpBuf == NULL)
						{
							INFOLOG(TEXT("[CCommonDetails::fetchUserAccountDetails() %d]: An access violation has occurred"),__LINE__);
							break;
						}

						Result = this->fetchUsersinGroup(pTmpBuf->lgrpi0_name);
						
						pTmpBuf++;
						dwTotalCount++;
					}
				}
			}
	
			else
			{
				INFOLOG(TEXT("[CCommonDetails::fetchUserAccountDetails() %d]: A system error has occurred: %d"), 
								 __LINE__,nStatus);
			}
			if (pBuf != NULL)
			{
				NetApiBufferFree(pBuf);
				pBuf = NULL;
			}

		}while (nStatus == ERROR_MORE_DATA); 

		if (pBuf != NULL)
			NetApiBufferFree(pBuf);

		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEINSTALL_PCH,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CCommonDetails::fetchUserAccountDetails() %d] Standard Exception caught : %s",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CCommonDetails::fetchUserAccountDetails() %d] Unknown Exception caught",__LINE__);
	}

	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEINSTALL_PCH,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CCommonDetails::fetchComputerFullName()
{
	OSD_RESULT result;
	try
	{
		TCHAR buffer[CHAR_MAX] = TEXT("");
		DWORD dwsize = sizeof(buffer);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

		if(GetComputerNameEx(ComputerNameDnsFullyQualified,buffer, &dwsize))
		{
			CCommonDetails::m_ComputerFullName = buffer;
		}
		else
		{
			INFOLOG(TEXT("[%s : %d] Failed to get Computer Full Name (%d)\n"), 
							gpCFileName,
							__LINE__,
							GetLastError());

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
		}
		return result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails::fetchComputerDomainName()
{
	OSD_RESULT result;
	try
	{
		NET_API_STATUS netStatus;
		LPWSTR tempNameSpace;
		NETSETUP_JOIN_STATUS ntSetupStatus;

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);

		netStatus = NetGetJoinInformation( NULL, &tempNameSpace, &ntSetupStatus );
		if( netStatus == NERR_Success )
		{
			CCommonDetails::m_ComputerDomainName = tempNameSpace;

			NET_API_STATUS netBufferStatus;
			netBufferStatus = NetApiBufferFree(tempNameSpace);

			if(NERR_Success != netBufferStatus)
			{
				INFOLOG(TEXT("[%s : %d] Failed to free buffer (%d)\n"), 
							gpCFileName,
							__LINE__,
							netBufferStatus);
			}

			if(ntSetupStatus == NetSetupUnknownStatus)
			{
				m_IsDomain = FALSE;
			}
			else if(ntSetupStatus == NetSetupUnjoined)
			{
				m_IsDomain = FALSE;
			}
			else if(ntSetupStatus == NetSetupWorkgroupName)
			{
				m_IsDomain = FALSE;
			}
			else if(ntSetupStatus == NetSetupDomainName)
			{
				m_IsDomain = TRUE;

				TCHAR buffer[CHAR_MAX] = TEXT("");
				DWORD dwsize = sizeof(buffer);

				if(GetComputerNameEx(ComputerNameDnsDomain,buffer, &dwsize))
				{
					CCommonDetails::m_ComputerDNSName = buffer;
				}
				else
				{
					INFOLOG(TEXT("[%s : %d] Failed to get Computer DNS Domain Name (%d)\n"), 
									gpCFileName,
									__LINE__,
									GetLastError());

					OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
				}
			}
			else
			{
			   m_IsDomain = FALSE;
			}
		} 
		else
		{
			INFOLOG(TEXT("[%s : %d] Failed to get Computer Domain Name (%d)\n"), 
									gpCFileName,
									__LINE__,
									GetLastError());
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
		}
		return result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails::fetchWMI_UUID()
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

		result = CCommonDetails::WMIQuery(
					OSD_WMI_LOCAL_NAMESPACE,
					OSD_SYS_CSPRODUCT,
					OSD_WMI_UUID,
					m_UUID);

		if(OSD_ERROR_NONE != result.RetCode)
		{
			INFOLOG(TEXT("[%s : %d] Failed to get UUID (%d)\n"), 
						gpCFileName, 
						__LINE__, 
						result.RetCode);

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_CALL_FAILURE);
		}

		if(m_UUID.IsEmpty())
		{

		}

		return result;
	}

	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;	
}

OSD_RESULT CCommonDetails::fetchWMI_ModelName()
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

		result = CCommonDetails::WMIQuery(
					OSD_WMI_LOCAL_NAMESPACE,
					OSD_SYS_CS,
					OSD_WMI_MODEL,
					m_Model);

		if(OSD_ERROR_NONE != result.RetCode)
		{
			INFOLOG(TEXT("[%s : %d] Failed to get Serial Number (%d)\n"), 
						gpCFileName, 
						__LINE__, 
						result.RetCode);

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_CALL_FAILURE);
		}
		return result;
	}

	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails::fetchWMI_SerialNumber()
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

		result = CCommonDetails::WMIQuery(
					OSD_WMI_LOCAL_NAMESPACE,
					OSD_SYS_BIOS,
					OSD_WMI_SERIAL_NUMBER,
					m_SerialNumber);

		if(OSD_ERROR_NONE != result.RetCode)
		{
			INFOLOG(TEXT("[%s : %d] Failed to get Serial Number (%d)\n"), 
						gpCFileName, 
						__LINE__, 
						result.RetCode);

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_CALL_FAILURE);
		}
		return result;
	}

	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails::DumpSMBiosTable(BYTE *TableData)
{
	OSD_RESULT Result;
	try
	{
		while (((SMBIOS_HEADER_STRUCTURE*)TableData)->Handle != 0xffff && ((SMBIOS_HEADER_STRUCTURE*)TableData)->Length != 0) 
		{
			if(1 == ((SMBIOS_HEADER_STRUCTURE*)TableData)->Type)
			{
				CString compUUID;
				SMBIOS_SYS_INFO *SysInfo = (SMBIOS_SYS_INFO*)((SMBIOS_HEADER_STRUCTURE*)TableData);

				char *pValue = (char*)(SysInfo);
				int StructLen = sizeof(BYTE)*(SysInfo->Length);
				pValue += StructLen;

				compUUID.Format(TEXT("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
					SysInfo->UUID[3],SysInfo->UUID[2],
					SysInfo->UUID[1],SysInfo->UUID[0],
					SysInfo->UUID[5],SysInfo->UUID[4],
					SysInfo->UUID[7],SysInfo->UUID[6],
					SysInfo->UUID[8],SysInfo->UUID[9],
					SysInfo->UUID[10],SysInfo->UUID[11],
					SysInfo->UUID[12],SysInfo->UUID[13],
					SysInfo->UUID[14],SysInfo->UUID[15]);

				CCommonDetails::m_UUID = compUUID;
				if(CCommonDetails::m_UUID.IsEmpty())
				{
					INFOLOG(TEXT("[%s : %d] Failed to get UUID\n"), 
							gpCFileName,
							__LINE__);
				}

				for(int i = 0; i<SysInfo->SerialNumber; i++)
				{
					string strValue = pValue;
					if(i == SysInfo->SerialNumber-1)
					{
						CString cSerial(CA2T(strValue.c_str()));
						CCommonDetails::m_SerialNumber = cSerial;
						if(CCommonDetails::m_SerialNumber.IsEmpty())
						{
							INFOLOG(TEXT("[%s : %d] Failed to get Serial Number\n"), 
									gpCFileName,
									__LINE__);
						}
						break;
					}
					pValue += (strValue.length()+1);
				}

				pValue = (char*)(SysInfo);
				StructLen = sizeof(BYTE)*(SysInfo->Length);
				pValue += StructLen;
				for(int i = 0; i<SysInfo->ProductName; i++)
				{
					string strValue = pValue;
					if(i == SysInfo->ProductName-1)
					{
						CString cSerial(CA2T(strValue.c_str()));
						m_Model = cSerial;
						if(m_Model.IsEmpty())
						{
							INFOLOG(TEXT("[%s : %d] Failed to get Model Name\n"), 
									gpCFileName,
									__LINE__);
						}
						break;
					}
					pValue += (strValue.length()+1);
				}

				pValue = (char*)(SysInfo);
				StructLen = sizeof(BYTE)*(SysInfo->Length);
				pValue += StructLen;
				for(int i = 0; i<SysInfo->Manufacturer; i++)
				{
					string strValue = pValue;
					if(i == SysInfo->Manufacturer-1)
					{
						CString cSerial(CA2T(strValue.c_str()));
						m_ManufacturerName = cSerial;
						if(m_ManufacturerName.IsEmpty())
						{
							INFOLOG(TEXT("[%s : %d] Failed to get Manufacturer Name\n"), 
									gpCFileName,
									__LINE__);
						}
						break;
					}
					pValue += (strValue.length()+1);
				}

				OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

				return Result;
			}

			TableData += ((SMBIOS_HEADER_STRUCTURE*)TableData)->Length;
			while (*(UINT16*)TableData != 0) {
				TableData++;
			}
			TableData += sizeof (UINT16);
		}

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);

		return Result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return Result;
}

OSD_RESULT CCommonDetails::fetchSMB_SerialNumberUUID()
{
	OSD_RESULT Result;
	try
	{
		UINT nBufferSize = GetSystemFirmwareTable ('RSMB', NULL, NULL, 0);
		if(0 != nBufferSize)
		{
			DWORD retval = 0;
			BYTE *pBufferData = new BYTE[nBufferSize];
			if(nBufferSize != GetSystemFirmwareTable('RSMB',NULL,pBufferData,nBufferSize) && nBufferSize > GetSystemFirmwareTable('RSMB',NULL,pBufferData,nBufferSize))
			{
				SECURE_DELETE(pBufferData);
				INFOLOG(TEXT("[%s : %d] Failed to get system firmware Table (%d)\n"), 
							gpCFileName,
							__LINE__,
							GetLastError());

				OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);

				return Result;
			}
			Result = DumpSMBiosTable(((RawSMBIOSData*)pBufferData)->SMBIOSTableData);
			if(OSD_ERROR_SUCCESS != Result.RetCode)
			{
				SECURE_DELETE(pBufferData);
				INFOLOG(TEXT("[%s : %d] Failed to get system information(SMBIOS)\n"), 
							gpCFileName,
							__LINE__);
				OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_REQ_INVALID);

				return Result;
			}

			SECURE_DELETE(pBufferData);
			OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

			return Result;
		}
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return Result;

}

OSD_RESULT CCommonDetails::fetchSerialNumberUUID()
{
	OSD_RESULT Result;

	Result = fetchSMB_SerialNumberUUID();
	if(OSD_ERROR_SUCCESS != Result.RetCode)
	{
		if(m_UUID.IsEmpty())
		{
			Result = fetchWMI_UUID();
			if(OSD_ERROR_SUCCESS != Result.RetCode)
			{
				INFOLOG(TEXT("[%s : %d] Failed to Collect UUID through WMI\n"), 
								gpCFileName, 
								__LINE__);
				OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, Result.RetCode);
				
				return Result;
			}
		}

		if(m_SerialNumber.IsEmpty())
		{
			Result = fetchWMI_SerialNumber();
			if(OSD_ERROR_SUCCESS != Result.RetCode)
			{
				INFOLOG(TEXT("[%s : %d] Failed to Collect UUID through WMI\n"), 
								gpCFileName, 
								__LINE__);
				OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, Result.RetCode);

				return Result;
			}
		}
	}

	OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

	return Result;
}

// OSType : 1 = Server, 2 = Workstation
//
OSD_RESULT CCommonDetails::GetOSNameString(LPTSTR pszOS, LPTSTR pszDriveLetter)
{
	   OSD_RESULT Result;
	   OSVERSIONINFOEX osvi;
	   SYSTEM_INFO si;
	   PGNSI pGNSI;
	   PGPI pGPI;
	   BOOL bOsVersionInfoEx;
	   DWORD dwType;
	   CAtlString pszKernelPath;

	   if(ISNULL(pszDriveLetter))
	   {
		   pszKernelPath = _T("kernel32.dll");
	   }
	   else
	   {
		   pszKernelPath = pszDriveLetter;
		   pszKernelPath.Append(_T("WINDOWS\\System32\\"));
		   pszKernelPath.Append(_T("kernel32.dll"));
	   }

	   ZeroMemory(&si, sizeof(SYSTEM_INFO));
	   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	   bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	   if( ! bOsVersionInfoEx )
	   {
		   INFOLOG(TEXT("[%s : %d] Failed to get OS INFO\n"), 
								gpCFileName, 
								__LINE__);

		   OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
		   return Result;
	   }

	   pGNSI = (PGNSI) GetProcAddress(
		   GetModuleHandle(pszKernelPath.GetBuffer()),		//TEXT("kernel32.dll")
		  "GetNativeSystemInfo");

	   if(NULL != pGNSI)
	   {
			pGNSI(&si);
	   }
	   else 
	   {
			GetSystemInfo(&si);
	   }

	   if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
			osvi.dwMajorVersion > 4 )
	   {
		  StringCchCopy(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Microsoft "));

		  // Identify product.

		  if ( osvi.dwMajorVersion == 10 )
		  {
			  if( osvi.dwMinorVersion == 0 )
			 {
				if( osvi.wProductType == VER_NT_WORKSTATION )
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows 10 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_10;
					m_OSType = 2;

				}
				else 
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows Server 2016 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2016;
					m_OSType = 1;
				}
			 }

			 pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(pszKernelPath.GetBuffer()),			//TEXT("kernel32.dll") 
				"GetProductInfo");

			 if(NULL == pGPI)
			 {
				 INFOLOG(TEXT("[%s : %d] Failed to get product info\n"), 
								gpCFileName, 
								__LINE__);
				 OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
				 return Result;
			 }

			 pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			 switch( dwType )
			 {
				case PRODUCT_ULTIMATE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Ultimate Edition" ));
				   break;
				case PRODUCT_PROFESSIONAL:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Professional" ));
				   break;
				case PRODUCT_HOME_PREMIUM:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Home Premium Edition" ));
				   break;
				case PRODUCT_HOME_BASIC:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Home Basic Edition" ));
				   break;
				case PRODUCT_ENTERPRISE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition" ));
				   break;
				case PRODUCT_BUSINESS:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Business Edition" ));
				   break;
				case PRODUCT_STARTER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Starter Edition" ));
				   break;
				case PRODUCT_CLUSTER_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Cluster Server Edition" ));
				   break;
				case PRODUCT_DATACENTER_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Datacenter Edition" ));
				   break;
				case PRODUCT_DATACENTER_SERVER_CORE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Datacenter Edition (core installation)" ));
				   break;
				case PRODUCT_ENTERPRISE_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition" ));
				   break;
				case PRODUCT_ENTERPRISE_SERVER_CORE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition (core installation)" ));
				   break;
				case PRODUCT_ENTERPRISE_SERVER_IA64:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
				   break;
				case PRODUCT_SMALLBUSINESS_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Small Business Server" ));
				   break;
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Small Business Server Premium Edition" ));
				   break;
				case PRODUCT_STANDARD_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Standard Edition" ));
				   break;
				case PRODUCT_STANDARD_SERVER_CORE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Standard Edition (core installation)" ));
				   break;
				case PRODUCT_WEB_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Web Server Edition" ));
				   break;
			 }

		  }

		  if ( osvi.dwMajorVersion == 6 )
		  {
			 if( osvi.dwMinorVersion == 0 )
			 {
				if( osvi.wProductType == VER_NT_WORKSTATION )
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows Vista "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_VISTA;
					m_OSType = 2;
				}
				else 
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows Server 2008 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2008;
					m_OSType = 1;
				}
			 }

			 if ( osvi.dwMinorVersion == 1 || osvi.dwMinorVersion == 2 || osvi.dwMinorVersion == 3 )
			 {
				if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 1)
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows 7 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_7;
					m_OSType = 2;
				}
				else if ( osvi.wProductType != VER_NT_WORKSTATION && osvi.dwMinorVersion == 1)
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows Server 2008 R2 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2008_R2;
					m_OSType = 1;
				}
				else if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 2)
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows 8 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_8;
					m_OSType = 2;
				}
				else if ( osvi.wProductType != VER_NT_WORKSTATION && osvi.dwMinorVersion == 2)
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows Server 2012 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2012;
					m_OSType = 1;
				}
				else if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 3)
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows 8.1 "));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_81;
					m_OSType = 2;
				}
				else 
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows Server 2012 R2 " ));
					m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2012_R2;
					m_OSType = 1;
				}
			 }

			 pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(pszKernelPath.GetBuffer()),			//TEXT("kernel32.dll") 
				"GetProductInfo");

			 if(NULL == pGPI)
			 {
				 INFOLOG(TEXT("[%s : %d] Failed to get product info\n"), 
								gpCFileName, 
								__LINE__);
				 OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
				 return Result;
			 }

			 pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			 switch( dwType )
			 {
				case PRODUCT_ULTIMATE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Ultimate Edition" ));
				   break;
				case PRODUCT_PROFESSIONAL:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Professional" ));
				   break;
				case PRODUCT_HOME_PREMIUM:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Home Premium Edition" ));
				   break;
				case PRODUCT_HOME_BASIC:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Home Basic Edition" ));
				   break;
				case PRODUCT_ENTERPRISE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition" ));
				   break;
				case PRODUCT_BUSINESS:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Business Edition" ));
				   break;
				case PRODUCT_STARTER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Starter Edition" ));
				   break;
				case PRODUCT_CLUSTER_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Cluster Server Edition" ));
				   break;
				case PRODUCT_DATACENTER_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Datacenter Edition" ));
				   break;
				case PRODUCT_DATACENTER_SERVER_CORE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Datacenter Edition (core installation)" ));
				   break;
				case PRODUCT_ENTERPRISE_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition" ));
				   break;
				case PRODUCT_ENTERPRISE_SERVER_CORE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition (core installation)" ));
				   break;
				case PRODUCT_ENTERPRISE_SERVER_IA64:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
				   break;
				case PRODUCT_SMALLBUSINESS_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Small Business Server" ));
				   break;
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Small Business Server Premium Edition" ));
				   break;
				case PRODUCT_STANDARD_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Standard Edition" ));
				   break;
				case PRODUCT_STANDARD_SERVER_CORE:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Standard Edition (core installation)" ));
				   break;
				case PRODUCT_WEB_SERVER:
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Web Server Edition" ));
				   break;
			 }
		  }

		  if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		  {
			 if( GetSystemMetrics(SM_SERVERR2) )
			 {
				StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Windows Server 2003 R2, "));
				m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2003_R2;
				m_OSType = 1;
			 }
			 else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
			 {
				StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Windows Storage Server 2003"));
				m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2003;
				m_OSType = 1;
			 }
			 else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
			 {
				StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Windows Home Server"));
				m_OSIntValue = OS_MICROSOFT_WINDOWS_HOME_SERVER;
				m_OSType = 1;
			 }
			 else if( osvi.wProductType == VER_NT_WORKSTATION &&
					  si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			 {
				StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Windows XP Professional x64 Edition"));
				m_OSIntValue = OS_MICROSOFT_WINDOWS_XP_PROFESSIONAL_x64;
				m_OSType = 2;
			 }
			 else
			 {
				StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows Server 2003, "));
				m_OSIntValue = OS_MICROSOFT_WINDOWS_SERVER_2003;
				m_OSType = 1;
			 }

			 if ( osvi.wProductType != VER_NT_WORKSTATION )
			 {
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
					}
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
					}
				}

				else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Datacenter x64 Edition" ));
					}
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Enterprise x64 Edition" ));
					}
					else 
					{	
						StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Standard x64 Edition" ));
					}
				}

				else
				{
					if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Compute Cluster Edition" ));
					}
					else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Datacenter Edition" ));
					}
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Enterprise Edition" ));
					}
					else if( osvi.wSuiteMask & VER_SUITE_BLADE )
					{
					   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Web Edition" ));
					}
					else 
					{
						StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Standard Edition" ));
					}
				}
			 }
		  }

		  if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		  {
			 StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows XP "));
			 m_OSIntValue = OS_MICROSOFT_WINDOWS_XP;
			 m_OSType = 2;
			 if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
			 {
				StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Home Edition" ));
			 }
			 else 
			 {
				 StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Professional" ));
			 }
		  }

		  if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		  {
			 StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT("Windows 2000 "));
			 m_OSIntValue = OS_MICROSOFT_WINDOWS_2000;
			 m_OSType = 2;
			 if ( osvi.wProductType == VER_NT_WORKSTATION )
			 {
				StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Professional" ));
			 }
			 else 
			 {
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
				{
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Datacenter Server" ));
				}
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
				{
				   StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Advanced Server" ));
				}
				else 
				{
					StringCchCat(pszOS, COMPDETAIL_BUFFERSIZE, TEXT( "Server" ));
				}
			 }
		  }

		  OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);
			
		  return Result;
	   }

	   else
	   {  
		  INFOLOG(TEXT("[%s : %d] Failed to get OS name\n"), 
						gpCFileName, 
						__LINE__);

		  OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_UNKNOWN);
		  m_OSIntValue = OS_MICROSOFT_WINDOWS_UNKNOWN;

		  return Result;
	   }
}

OSD_RESULT CCommonDetails::fetchOSName()
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

		TCHAR szOSName[COMPDETAIL_BUFFERSIZE];

		result = GetOSNameString(szOSName);
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			result = CCommonDetails::WMIQuery(
						OSD_WMI_LOCAL_NAMESPACE,
						OSD_SYS_OS,
						OSD_WMI_OS_NAME,
						m_OSName);

			if(OSD_ERROR_NONE != result.RetCode)
			{
				INFOLOG(TEXT("[%s : %d] Failed to get OS name (%d)\n"), 
							gpCFileName, 
							__LINE__, 
							result.RetCode);

				OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_CALL_FAILURE);
			}

			if(m_OSName.IsEmpty())
			{
				OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_CALL_FAILURE);
			}

			return result;
		}

		CString csOSName;
		csOSName.Format(L"%s",szOSName);

		CCommonDetails::m_OSName = csOSName;

		return result;
	}

	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails::fetchOSDrive()
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);

		errno_t error;
		char *sysDrive;
		size_t len;
		error = _dupenv_s(&sysDrive, &len,"SystemDrive");
		if (sysDrive == NULL || 0 != error) 
		{
			result = CCommonDetails::WMIQuery(
					OSD_WMI_LOCAL_NAMESPACE,
					OSD_SYS_OS,
					OSD_WMI_OS_DRIVE,
					m_OSDrive);

			if(OSD_ERROR_NONE != result.RetCode)
			{
				INFOLOG(TEXT("[%s : %d] Failed to get OS name (%d)\n"), 
							gpCFileName, 
							__LINE__, 
							result.RetCode);

				OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_CALL_FAILURE);
			}
		} 
		else {
			m_OSDrive = sysDrive;
		}

		return result;
	}

	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails::fetchIPMACAddress()
{
	OSD_RESULT result;
	try
	{
		PIP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		CString mac_addr;
		mac_addr.Empty();

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);

		AdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
		if(NULL == AdapterInfo) 
		{
			INFOLOG(TEXT("[%s : %d] Failed to allocate memory\n"), 
						gpCFileName, 
						__LINE__);

			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_MEM_ALLOC_FAILURE);
		}

		if(ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(AdapterInfo, &dwBufLen)) 
		{
			AdapterInfo = (IP_ADAPTER_INFO *) malloc(dwBufLen);
			if (NULL == AdapterInfo) {
				INFOLOG(TEXT("[%s : %d] Failed to allocate memory\n"), 
						gpCFileName, 
						__LINE__);

				OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_MEM_ALLOC_FAILURE);
			 }
		 }

		std::string macinfo("0.0.0.0");
		if(NO_ERROR == GetAdaptersInfo(AdapterInfo, &dwBufLen)) 
		{

		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
		
		do 
		{
			mac_addr.Format(TEXT("%02X:%02X:%02X:%02X:%02X:%02X"),
			pAdapterInfo->Address[0], pAdapterInfo->Address[1],
			pAdapterInfo->Address[2], pAdapterInfo->Address[3],
			pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

			if(0 != macinfo.compare(pAdapterInfo->IpAddressList.IpAddress.String))
			{
				// multi ip address possibilities
				CCommonDetails::m_IPAddress = pAdapterInfo->IpAddressList.IpAddress.String;
				CCommonDetails::m_MACAddress = mac_addr;
			}
			pAdapterInfo = pAdapterInfo->Next;    

		}while(pAdapterInfo);  

		SECURE_FREE(AdapterInfo);
		}	
		return result;
	}

	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	#if defined(_WIN64)

	bIsWow64 = TRUE;  // 64-bit programs run only on Win64

	#elif defined(_WIN32)

    LPFN_ISWOW64PROCESS fnIsWow64Process;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)
                       GetProcAddress(GetModuleHandle(TEXT("kernel32")),
                                      "IsWow64Process");

    if (NULL != fnIsWow64Process)
    {
        printf("Default method failed. trying alternate method.");

        if (!fnIsWow64Process(GetCurrentProcess(),
                              &bIsWow64))
        {
            ERRORLOG(L"Unable to detect windows architecture.");
        }
    }
	#endif

    return bIsWow64;
}
OSD_RESULT CCommonDetails::fetchSystemOSInfo()
{
	OSD_RESULT Result;
	try
	{
		SYSTEM_INFO siSysInfo; 

		GetSystemInfo(&siSysInfo); 
	
		m_ProcessorArchitecture = siSysInfo.wProcessorArchitecture;

		if(IsWow64())
		{
			m_OSArchitecture = 64;
		}
		else
		{
			m_OSArchitecture = 32;
		}

		OSVERSIONINFOEX siOSVerInfo;
		ZeroMemory(&siOSVerInfo, sizeof(OSVERSIONINFOEX));
		siOSVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if(GetVersionEx((OSVERSIONINFO*)&siOSVerInfo))
		{
			m_OSMajorVersion = siOSVerInfo.dwMajorVersion;
			m_OSMinorVersion = siOSVerInfo.dwMinorVersion;
			m_ProductType = siOSVerInfo.wProductType;
			m_SuiteMask = siOSVerInfo.wSuiteMask;
			m_BuildNumber = siOSVerInfo.dwBuildNumber;

			OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);
		}
		else
		{
			INFOLOG(TEXT("[%s : %d]>>%S() : Failed to get OS version information : Error(%d) \n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__,
					GetLastError());
			OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_FAILED_TO_GET_SYSTEMINFO);
		}

		return Result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught \n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);
	}

	OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

DWORD GetDWORDRegKey(HKEY hKey,LPSTR subKey, LPSTR valueName, DWORD &nValue)
{
 	DWORD retVal = S_OK;
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);

		retVal = RegOpenKeyExA( hKey, subKey, 0, KEY_QUERY_VALUE, &hKey );

	if( retVal != ERROR_SUCCESS )
	{
		INFOLOG(L"GetDWORDRegKey: Error while opening registry key for serverName %d \n", retVal );
		return retVal;
	}
    DWORD nError = ::RegQueryValueExA(hKey,
										valueName,
										0,
										NULL,
										reinterpret_cast<LPBYTE>(&nResult),
										&dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        nValue = nResult;
    }
    return nError;
}
OSD_RESULT CCommonDetails::fetchSecureBootOption()
{
	OSD_RESULT Result;

	try
	{
		DWORD Value, reterror;

		reterror = GetDWORDRegKey(HKEY_LOCAL_MACHINE, SECURE_BOOT_REG_PATH, SECURE_BOOT_REG_KEY ,Value);
		if(reterror == ERROR_SUCCESS)
		{
			m_IsSecureBoot = Value;
		}
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught \n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);
	}

	OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CCommonDetails::fetchBIOSMode()
{
	OSD_RESULT Result;

	try
	{
		DWORD LastError, RetError;
		LPCWSTR Namestr = L"";
		LPCWSTR Guidstr = L"{00000000-0000-0000-0000-000000000000}";
		PVOID Databuf = new char[BUFSIZ];

		RetError = GetFirmwareEnvironmentVariable(Namestr,Guidstr,Databuf,BUFSIZ);
		LastError = GetLastError();
		if(RetError == 0)
		{
			if(LastError == ERROR_NOACCESS)
			{
				m_BIOSMode = L"UEFI	BIOS";

				INFOLOG(TEXT("[%s : %d]>>%S() : Boot mode - UEFI BIOS \n"), 
						gpCFileName, 
						__LINE__, 
						__FUNCTION__);

			}
			else if(LastError == ERROR_INVALID_FUNCTION)
			{
				m_BIOSMode = L"Legacy BIOS";

				INFOLOG(TEXT("[%s : %d]>>%S() : Boot mode - Legacy BIOS \n"), 
						gpCFileName, 
						__LINE__, 
						__FUNCTION__);
				
			}
			else
			{
				OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_FAILED_TO_GET_BOOT_TYPE);
				return Result;
			}
			
		}
		SECURE_DELETE(Databuf);
		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_SUCCESS);
		return Result;
	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught \n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);
	}

	OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CCommonDetails::populateComuterDetails()
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);

		result = fetchComputerName();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchComputerFullName();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		/*! Loop until both mac address and IP address is obtained !*/
		/*for(INT iCount = 0; iCount < MAC_ADDRESS_RETRY_COUNT; iCount++)
		{*/
			result = fetchIPMACAddress();
			if(OSD_ERROR_SUCCESS != result.RetCode)
			{
				OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
			}
			/*if(!this->m_IPAddress.IsEmpty() && !this->m_MACAddress.IsEmpty())
			{
				break;
			}
			else
			{
				Sleep(MAC_ADDRESS_TIME_OUT);
			}*/
		//}

		result = fetchProcessorName();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchRAMSize();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchSystemOSInfo();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchComputerDomainName();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchUserAccountDetails();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchBIOSMode();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchSecureBootOption();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		//initializes COM
		if(S_OK != CoInitialize(NULL)) 
		{
			INFOLOG(TEXT("[%s : %d] COM initialization Failed / COM already running\n"), 
						gpCFileName, 
						__LINE__);
			CoUninitialize();
			setCOMStatus(false);
		}
		else
		{
			setCOMStatus(true);
		}

		result = fetchSerialNumberUUID();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchOSName();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchOSDrive();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		//Uninitialize COM
		if(getCOMStatus())
		{
			CoUninitialize();
			setCOMStatus(false);
		}
		return result;

	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails:: populateBIOSDetails()
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);
		
		result = fetchBIOSMode();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		result = fetchSecureBootOption();
		if(OSD_ERROR_SUCCESS != result.RetCode)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, result.RetCode);
		}

		return result;

	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}

OSD_RESULT CCommonDetails:: populateofflineDetails(CAtlString OSDrive)
{
	OSD_RESULT result;
	try
	{
		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_NONE);

		HANDLE proccessHandle = GetCurrentProcess();     
		DWORD typeOfAccess = TOKEN_ADJUST_PRIVILEGES;   
		HANDLE tokenHandle;

		CRegistry RegObj;
		if ( OpenProcessToken(proccessHandle, typeOfAccess, &tokenHandle) )
		{
			// Enabling RESTORE and BACKUP privileges
			if ( !RegObj.SetPrivilege(tokenHandle, SE_RESTORE_NAME, TRUE))
			{
				INFOLOG(L"[ %d]>%S() Error set Privilege failed.",__LINE__,__FUNCTION__);
			}
        
			if ( !RegObj.SetPrivilege(tokenHandle, SE_BACKUP_NAME, TRUE))
			{
				INFOLOG(L"[ %d]>%S() Error set Privilege failed.",__LINE__,__FUNCTION__);
			}

		}
		else
		{
			INFOLOG(L"[ %d]>%S() Error getting the access token.",__LINE__,__FUNCTION__);
		}

		HKEY hKey = HKEY_LOCAL_MACHINE;

		CAtlString pFile = OSDrive;
		pFile += SOFTWARE_PATH;

		LONG loadKey;	
		DWORD Retcode, dwValue;
		LPSTR Value;

		loadKey = RegLoadKey(hKey, SOFTWARE_SUBKEY, pFile);
		if(loadKey != ERROR_SUCCESS)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS,OSD_ERROR_FAILED_TO_GET_REGISTRY_VALUE);
			return result;
		}

		Retcode = RegObj.ReadRegExpandSZEx(HKEY_LOCAL_MACHINE, DOMAIN_REG_PATH, DOMAIN_NAME_KEY ,&Value);
		if(Retcode == ERROR_SUCCESS)
		{
			CAtlString DomainName = Value;
			DWORD Index = DomainName.Find(L"\\");

			if( Index != -1 )
			{
				this->m_ComputerDomainName = DomainName.Left(Index);
				this->m_IsDomain = TRUE;
			}

		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get Domain Name Failed Error %d \n",__LINE__,Retcode);
		}

		Retcode = RegObj.ReadRegExpandSZEx(HKEY_LOCAL_MACHINE, OS_REG_PATH, OS_NAME_KEY ,&Value);
		if(Retcode == ERROR_SUCCESS)
		{
			this->m_OSName = Value;
		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get OS Name Failed Error %d \n",__LINE__,Retcode);
		}

		Retcode = GetDWORDRegKey(HKEY_LOCAL_MACHINE, OS_REG_PATH, OS_MAJOR_VERSION_KEY ,dwValue);
		if(Retcode == ERROR_SUCCESS)
		{
			this->m_OSMajorVersion = dwValue;
		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get OS Major Version Failed Error %d\n",__LINE__,Retcode);
		}

		Retcode = GetDWORDRegKey(HKEY_LOCAL_MACHINE, OS_REG_PATH, OS_MINOR_VERSION_KEY ,dwValue);
		if(Retcode == ERROR_SUCCESS)
		{
			this->m_OSMinorVersion = dwValue;
		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get OS Minor Version Failed Error  %d\n",__LINE__,Retcode);
		}

		RegUnLoadKey(hKey, LPCWSTR(SOFTWARE_SUBKEY));

		pFile = OSDrive;
		pFile += SYSTEM_PATH;

		loadKey = RegLoadKey(hKey, SYSTEM_SUBKEY, pFile);
		if(loadKey != ERROR_SUCCESS)
		{
			OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS,OSD_ERROR_FAILED_TO_GET_REGISTRY_VALUE);
			return result;
		}

		Retcode = RegObj.ReadRegExpandSZEx(HKEY_LOCAL_MACHINE, OS_ARCH_PATH, OS_ARCHITECTURE_KEY ,&Value);
		if(Retcode == ERROR_SUCCESS)
		{
			CString OSArch = Value;
			this->m_OSArchitecture =  (OSArch.CompareNoCase(OS_ARCHITECTURE_64BIT)==0) ? 64 : 32;
		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get OS Architecture Failed Error %d \n",__LINE__,Retcode);
		}

		Retcode = RegObj.ReadRegExpandSZEx(HKEY_LOCAL_MACHINE, OS_TYPE_REG_PATH, OS_PRODUCT_TYPE_KEY ,&Value);
		if(Retcode == ERROR_SUCCESS)
		{
			CAtlStringA OSType = Value;
			if(!OSType.IsEmpty())
			{
				bool IsWorkStation = (OSType.CompareNoCase(PRODUCT_TYPE_WORKSTATION) == 0);  
				this->m_OSType = IsWorkStation ? 2 : 1 ;
			}
		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get OS Product Type Failed Error %d \n",__LINE__,Retcode);
		}

		Retcode = RegObj.ReadRegExpandSZEx(HKEY_LOCAL_MACHINE, TCPIP_REG_PATH, HOST_NAME_KEY ,&Value);
		if(Retcode == ERROR_SUCCESS)
		{
			this->m_ComputerName= Value;
		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get Computer Name Failed Error %d \n",__LINE__,Retcode);
		}

		Retcode = RegObj.ReadRegExpandSZEx(HKEY_LOCAL_MACHINE, TCPIP_REG_PATH, DNS_NAME_KEY ,&Value);
		if(Retcode == ERROR_SUCCESS)
		{			
			this->m_ComputerDNSName = Value;
			this->m_ComputerFullName = this->m_ComputerName;

			if(!this->m_ComputerDNSName.IsEmpty())
			{
				this->m_ComputerFullName.Append(L".");
				this->m_ComputerFullName.Append(this->m_ComputerDNSName);
				this->m_IsDomain = TRUE;
			}
		}
		else
		{
			INFOLOG(L"[CWrapper::populateofflineDetails() %d]> Get DNS Name Failed Error %d \n",__LINE__,Retcode);
		}

		RegUnLoadKey(hKey, LPCWSTR(SYSTEM_SUBKEY));

		return result;

	}
	catch(std::exception &exc)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : std exception caught (%s)\n"), 
					gpCFileName,
					__LINE__, 
					__FUNCTION__, 
					exc.what());

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	catch(...)
	{
		INFOLOG(TEXT("[%s : %d]>>%S() : def exception caught (%s)\n"), 
					gpCFileName, 
					__LINE__, 
					__FUNCTION__);

		OSD_SET_INTERNAL_ERR(result, OSD_MODULE_COMMON_DETAILS, OSD_ERROR_EXCEPTION_CAUGHT);
	}
	return result;
}


CCommonEditBCD::CCommonEditBCD()
{
}

CCommonEditBCD::~CCommonEditBCD()
{
}

OSD_RESULT CCommonEditBCD::EditBootStatusPolicy(CAtlString BCDLocation)
{
OSD_RESULT Result;
	try
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si,sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		ZeroMemory(&pi,sizeof(pi));

		CAtlString cmdArgs;
		cmdArgs.Append(_T("bcdedit.exe"));
		cmdArgs.Append(_T(" /set {default} bootstatuspolicy ignoreallfailures"));
		cmdArgs.Append(_T(" /store \""));
		cmdArgs.Append(BCDLocation.GetBuffer());
		cmdArgs.Append(_T("\""));
		
		TCHAR WinDir[MAX_PATH] = {0};
		if(!GetWindowsDirectory(WinDir,MAX_PATH))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_DEVICEIO,::GetLastError());

			return Result;
		}

		CAtlString cmdExePath;
		cmdExePath.Format(_T("%s\\system32\\bcdedit.exe"),WinDir);

		BOOL Status = CreateProcess( cmdExePath.GetBuffer(),
										 cmdArgs.GetBuffer(),
										 NULL, 
										 NULL, 
										 TRUE, 
										 0, 
										 NULL, 
										 NULL, 
										 &si, &pi);

		if(FALSE == Status)
		{
			INFOLOG(L"[CDeviceIO::EditBCDfile() %d]> Failed to set BCD",__LINE__);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_DEVICEIO,::GetLastError());

			return Result;
		}
		
		//Wait for the diskpart complete
		WaitForSingleObject( pi.hProcess, INFINITE );

		DWORD retcode;
		GetExitCodeProcess(pi.hProcess,LPDWORD(&retcode));
		if(retcode != ERROR_SUCCESS)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_DEVICEIO,::GetLastError());

			//return Result;
		}

		if(NULL != pi.hProcess){
			CloseHandle( pi.hProcess );
			pi.hProcess = NULL;
		}
		if(NULL != pi.hThread){
			CloseHandle( pi.hThread );
			pi.hThread = NULL;
		}

		INFOLOG(L"[%s %d]>CreateImage::BCD Edit BootPolicy Success\n",gpCFileName,__LINE__);
					
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_NONE);

		return Result;
	}	
	catch(std::exception &exc)
	{
	    INFOLOG(L"[CDeviceIO::EditBCDfile() %d]>std Exception caught (%S)\n",__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CDeviceIO::EditBCDfile() %d]>Unknown Exception caught \n",__LINE__);
	}
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CCommonEditBCD::EditBootStatusPolicy(BCD_BOOT_POLICY_OPTIONS BootPolicyOption)
{
	OSD_RESULT Result;
	try
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si,sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		ZeroMemory(&pi,sizeof(pi));

		CAtlString cmdArgs;
		cmdArgs.Append(_T("bcdedit.exe"));
		cmdArgs.Append(_T(" "));
		switch(BootPolicyOption)
		{
		case BOOTPOLICY_IGNORE_ALL_FAILURES:
			cmdArgs.Append(_T("/set {current} bootstatuspolicy ignoreallfailures"));
			break;
		case BOOTPOLICY_DISPLAY_ALL_FAILURES:
			cmdArgs.Append(_T("/set {current} bootstatuspolicy displayallfailures"));
		}
		
		TCHAR WinDir[MAX_PATH] = {0};
		if(!GetWindowsDirectory(WinDir,MAX_PATH))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_DEVICEIO,::GetLastError());

			return Result;
		}

		CAtlString cmdExePath;
		cmdExePath.Format(_T("%s\\system32\\bcdedit.exe"),WinDir);

		BOOL Status = CreateProcess( cmdExePath.GetBuffer(),
										 cmdArgs.GetBuffer(),
										 NULL, 
										 NULL, 
										 TRUE, 
										 0, 
										 NULL, 
										 NULL, 
										 &si, &pi);

		if(FALSE == Status)
		{
			INFOLOG(L"[CDeviceIO::EditBCDfile() %d]> Failed to set BCD",__LINE__);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_DEVICEIO,::GetLastError());

			return Result;
		}
		
		//Wait for the diskpart complete
		WaitForSingleObject( pi.hProcess, INFINITE );

		DWORD retcode;
		GetExitCodeProcess(pi.hProcess,LPDWORD(&retcode));
		if(retcode != ERROR_SUCCESS)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_DEVICEIO,::GetLastError());

			//return Result;
		}

		if(NULL != pi.hProcess){
			CloseHandle( pi.hProcess );
			pi.hProcess = NULL;
		}
		if(NULL != pi.hThread){
			CloseHandle( pi.hThread );
			pi.hThread = NULL;
		}

		INFOLOG(L"[%s %d]>CreateImage::BCD Edit BootPolicy Success\n",gpCFileName,__LINE__);
					
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_NONE);

		return Result;
	}	
	catch(std::exception &exc)
	{
	    INFOLOG(L"[CDeviceIO::EditBCDfile() %d]>std Exception caught (%S)\n",__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CDeviceIO::EditBCDfile() %d]>Unknown Exception caught \n",__LINE__);
	}
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_DEVICEIO,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;

}

}//OS_Manager


CommonDetails::CommonDetails()
{
}


CommonDetails::~CommonDetails()
{
}
