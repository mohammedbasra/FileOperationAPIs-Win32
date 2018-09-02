#pragma once

namespace OS_Manager
{

///////////////////////////////
//	Macros, Defines
///////////////////////////////
#define OSD_SYS_BIOS                L"SELECT * FROM Win32_BIOS"
#define OSD_SYS_CSPRODUCT           L"SELECT * FROM Win32_ComputerSystemProduct"
#define OSD_SYS_OS					L"SELECT * FROM Win32_OperatingSystem"
#define OSD_SYS_CS					L"SELECT * FROM Win32_ComputerSystem"

#define OSD_WMI_UUID				L"UUID"
#define OSD_WMI_SERIAL_NUMBER		L"SerialNumber"
#define OSD_WMI_OS_NAME				L"Caption"
#define OSD_WMI_OS_DRIVE			L"SystemDrive"
#define OSD_WMI_MODEL				L"Model"

#define OSD_WMI_LOCAL_NAMESPACE     L"ROOT\\CIMV2"

#define SYS_SYSTEM					TEXT("WINDOWS\\System")
#define SYS_SYSTEM32				TEXT("WINDOWS\\System32")
#define SYS_SYSWOW64				TEXT("WINDOWS\\SysWOW64")

#define SECURE_BOOT_REG_PATH   "SYSTEM\\CurrentControlSet\\Control\\SecureBoot\\State"
#define SECURE_BOOT_REG_KEY    "UEFISecureBootEnabled"

#define OS_REG_PATH				"SOFTWARE1\\Microsoft\\Windows NT\\CurrentVersion"
#define OS_ARCH_PATH            "SYSTEM1\\ControlSet001\\Control\\Session Manager\\Environment"
#define TCPIP_REG_PATH			"SYSTEM1\\ControlSet001\\Services\\Tcpip\\Parameters"
#define OS_TYPE_REG_PATH		"SYSTEM1\\ControlSet001\\Control\\ProductOptions"
#define DOMAIN_REG_PATH			"SOFTWARE1\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\DataStore\\Machine\\0"

#define OS_NAME_KEY				"ProductName"
#define OS_PRODUCT_TYPE_KEY		"ProductType"
#define OS_ARCHITECTURE_KEY		"PROCESSOR_ARCHITECTURE"
#define OS_MAJOR_VERSION_KEY    "CurrentMajorVersionNumber" 
#define OS_MINOR_VERSION_KEY    "CurrentMinorVersionNumber" 

#define OS_ARCHITECTURE_32BIT	L"x86"
#define OS_ARCHITECTURE_64BIT	L"AMD64"

#define DNS_NAME_KEY			"Domain"
#define HOST_NAME_KEY			"HostName"
#define DOMAIN_NAME_KEY			"szName"

#define SOFTWARE_SUBKEY			L"SOFTWARE1"
#define SYSTEM_SUBKEY			L"SYSTEM1"

#define SOFTWARE_PATH			":\\Windows\\System32\\config\\SOFTWARE"
#define SYSTEM_PATH				":\\Windows\\System32\\config\\SYSTEM"

#define PRODUCT_TYPE_WORKSTATION			     "WINNT"
#define PRODUCT_TYPE_ADVANCED_SERVER			 "SERVERNT"
#define PRODUCT_TYPE_DOMAIN_CONTROLLER		     "LANMANNT"


#define COMPDETAIL_BUFFERSIZE 256

#define MAC_ADDRESS_RETRY_COUNT		10
#define MAC_ADDRESS_TIME_OUT		5000

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

typedef enum BCDBootStatusPolicy
{
	/*0001*/ BOOTPOLICY_IGNORE_ALL_FAILURES,
	/*0002*/ BOOTPOLICY_DISPLAY_ALL_FAILURES

}BCD_BOOT_POLICY_OPTIONS;

#define	OS_MICROSOFT_WINDOWS_10							16
#define	OS_MICROSOFT_WINDOWS_SERVER_2016				15
#define	OS_MICROSOFT_WINDOWS_81							14
#define	OS_MICROSOFT_WINDOWS_SERVER_2012_R2				13
#define	OS_MICROSOFT_WINDOWS_8							12
#define	OS_MICROSOFT_WINDOWS_SERVER_2012				11
#define	OS_MICROSOFT_WINDOWS_7							10
#define	OS_MICROSOFT_WINDOWS_SERVER_2008_R2				 9
#define	OS_MICROSOFT_WINDOWS_SERVER_2008				 8
#define	OS_MICROSOFT_WINDOWS_VISTA						 7
#define	OS_MICROSOFT_WINDOWS_SERVER_2003_R2				 6
#define	OS_MICROSOFT_WINDOWS_HOME_SERVER				 5
#define	OS_MICROSOFT_WINDOWS_SERVER_2003				 4
#define	OS_MICROSOFT_WINDOWS_XP_PROFESSIONAL_x64		 3
#define	OS_MICROSOFT_WINDOWS_XP							 2
#define	OS_MICROSOFT_WINDOWS_2000						 1
#define	OS_MICROSOFT_WINDOWS_UNKNOWN					 0


//////////////////////////////
//	Structures
/////////////////////////////

struct RawSMBIOSData
{
	BYTE    Used20CallingMethod;
	BYTE    SMBIOSMajorVersion;
	BYTE    SMBIOSMinorVersion;
	BYTE    DmiRevision;
	DWORD   Length;
	BYTE    SMBIOSTableData[1];
};

typedef struct 
{
	UINT8   Type;
	UINT8   Length;
	UINT16  Handle;
}SMBIOS_HEADER_STRUCTURE;

typedef struct{
	BYTE	Type;
	BYTE	Length;
	WORD	Handle;
	BYTE	Manufacturer;
	BYTE	ProductName;
	BYTE	Version;
	BYTE	SerialNumber;
	BYTE	UUID[16];
	BYTE	WakeUpTime;
	BYTE	SKUNumber;
	BYTE	Family;
}SMBIOS_SYS_INFO;


typedef struct UserAccntDetail{
	CAtlString UserName;
	CAtlString UserSID;
	CAtlString GroupName;
	CAtlString DomainName;
	CAtlString DomainDNSName;
	BOOL       IsDomainUser;
	INT64	   Group;
	UserAccntDetail& operator=(const UserAccntDetail& i_data)
	{
		Group = i_data.Group;
		UserName = i_data.UserName;
		GroupName = i_data.GroupName;
		UserSID  = i_data.UserSID;
		DomainName = i_data.DomainName;
		DomainDNSName = i_data.DomainDNSName;
		
		return *this;
	}
}USER_ACCOUNT_DETAIL;

typedef std::vector<USER_ACCOUNT_DETAIL>	USER_ACCOUNT_DETAIL_LIST;
typedef USER_ACCOUNT_DETAIL_LIST::iterator	USER_ACCOUNT_DETAIL_LIST_ITR;

typedef BOOL (WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

BOOL IsWow64();

/////////////////////////////
//	Class
/////////////////////////////

class CCommonDetails
{
private:
	//CParser *pParserObj;
	INT64 m_Timestamp;  //this is initialized in configfactory
	INT64 m_OSType;
	INT64 m_OSIntValue;
    CAtlString m_ComputerName;
	CAtlString m_ComputerFullName;
    CAtlString m_ComputerDomainName;
	BOOL m_IsDomain;
	CAtlString m_ComputerDNSName;
	CAtlString m_ProcessorName;
    CAtlString m_MACAddress;
	CAtlString m_UUID;
    CAtlString m_SerialNumber;
    CAtlString m_IPAddress;
	CAtlString m_OSName;
	CAtlString m_OSDrive;
	CAtlString m_BIOSMode;
	DWORD m_IsSecureBoot;
	INT64 m_ProcessorArchitecture;
	INT64 m_OSArchitecture;
	INT64 m_OSMajorVersion;
	INT64 m_OSMinorVersion;
	INT64 m_ProductType;
	INT64 m_SuiteMask;
	INT64 m_BuildNumber;
	INT64 m_RamSize;
	CAtlString m_ManufacturerName;
	CAtlString m_Model;
	BOOL m_IsOfflineImgCreation;
	USER_ACCOUNT_DETAIL_LIST m_UserAccountList;

	BOOL m_COMStatus;

	OSD_RESULT fetchBIOSMode();
	OSD_RESULT fetchSecureBootOption();

	OSD_RESULT fetchWMI_ModelName();
	OSD_RESULT fetchProcessorName();
	OSD_RESULT fetchRAMSize();
	OSD_RESULT fetchComputerName();
	OSD_RESULT fetchComputerFullName();
	OSD_RESULT fetchComputerDomainName();
	OSD_RESULT fetchWMI_UUID();
	OSD_RESULT fetchWMI_SerialNumber();
	OSD_RESULT DumpSMBiosTable(BYTE *TableData);
	OSD_RESULT fetchSMB_SerialNumberUUID();
	OSD_RESULT fetchSerialNumberUUID();
	OSD_RESULT fetchUserAccountDetails();
	OSD_RESULT fetchUsersinGroup(LPWSTR GroupName);
	OSD_RESULT fetchOSName();
	OSD_RESULT GetOSNameString(LPTSTR pszOS, LPTSTR pszDriveLetter = NULL);
	OSD_RESULT fetchOSDrive();
	OSD_RESULT fetchIPMACAddress();
	OSD_RESULT fetchSystemOSInfo();
	OSD_RESULT WMIQuery(CString cstrNetworkResource,	//Namespace to connect
						CString cstrQueryClass,			//Query class	
						CString cstrQuery,				//Query name
						CString &ctsrQueryValue			//result value
						);
public:

	CAtlString getComputerName();
	CAtlString getComputerFullName();
	CAtlString getComputerDomainName();
	BOOL getIsDomain();
	CAtlString getComputerDNSName();
	CAtlString getMACAddress();
	CAtlString getUUID();
	CAtlString getSerialNumber();
	CAtlString getIPAddress();
	CAtlString getOSName();
	CAtlString getOSDrive();
	INT64 getOSType();
	CAtlString getProcessorName();
	INT64 getRamSize();
	CAtlString getModelName();
	CAtlString getManufacturerName();

	CAtlString getBIOSMode();
	VOID setBIOSMode(CAtlString i_BIOSMode);

	BOOL getIsSecureBoot();
	VOID setIsSecureBoot(BOOL i_IsSecureBoot);

	INT64 getOSIntValue();
	VOID setOSIntValue(INT64 i_OSIntValue);

	INT64 getProcessorArchitecture();
	INT64 getOSArchitecture();
	INT64 getOSMajorVersion();
	INT64 getOSMinorVersion();
	INT64 getProductType();
	INT64 getSuiteMask();
	INT64 getBuildNumber();

	USER_ACCOUNT_DETAIL_LIST getUserAccountList();
	OSD_RESULT OSDriveQuery(CAtlString i_DriveLetter, BOOL& IsOSDrive, CAtlString& i_OSName);

	CCommonDetails();//datamodel choice);
	~CCommonDetails(void);
	OSD_RESULT populateComuterDetails();
	OSD_RESULT populateBIOSDetails();
	OSD_RESULT populateofflineDetails(CAtlString OSDrive);

	BOOL getCOMStatus();
	void setCOMStatus(BOOL status);
	INT64 getTimeStamp();
	void setTimeStamp(INT64 i_TimeStamp);
	void setMacAddress(CString i_MAC_Address);

	void setOfflineImgCreation(BOOL i_IsOfflineImgCreation);
	BOOL getOfflineImgCreation();

};

class CCommonEditBCD 
{
public:
	CCommonEditBCD();
	~CCommonEditBCD();
	OSD_RESULT EditBootStatusPolicy(CAtlString BCDLocation);
	OSD_RESULT EditBootStatusPolicy(BCD_BOOT_POLICY_OPTIONS BootPolicyOption);
};
}//OS_Manager

class CommonDetails
{
public:
	CommonDetails();
	~CommonDetails();
};

