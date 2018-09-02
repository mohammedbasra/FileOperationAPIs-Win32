/*
 *  CRegistry.h 
 * 
 * Copyright(C) 
 */

#ifndef __REGISTRY_H__
#define __REGISTRY_H__

namespace OS_Manager
{
/////////////////////////////////////////
//CRegistry class default values
/////////////////////////////////////////

#define OSD_REG_PARENT_HKEY_CLASSES_ROOT      L"HKEY_CLASSES_ROOT"
#define OSD_REG_PARENT_HKEY_CURRENT_CONFIG    L"HKEY_CURRENT_CONFIG"
#define OSD_REG_PARENT_HKEY_CURRENT_USER      L"HKEY_CURRENT_USER"
#define OSD_REG_PARENT_HKEY_LOCAL_MACHINE     L"HKEY_LOCAL_MACHINE"
#define OSD_REG_PARENT_HKEY_USERS			  L"HKEY_USERS"

#define OSD_REG_PARENT_SHORT_HKEY_CLASSES_ROOT      L"HKCR"
#define OSD_REG_PARENT_SHORT_HKEY_CURRENT_CONFIG    L"HKCC"
#define OSD_REG_PARENT_SHORT_HKEY_CURRENT_USER      L"HKCU"
#define OSD_REG_PARENT_SHORT_HKEY_LOCAL_MACHINE     L"HKEY_LOCAL_MACHINE"
#define OSD_REG_PARENT_SHORT_HKEY_USERS				L"HKEY_USERS"


#define REG_MOUNTED_DEVICES_PARENT		HKEY_LOCAL_MACHINE
#define REG_MOUNTED_DEVICES_SUB_KEY		_T("SYSTEM")
#define REG_MOUNTED_DEVICES_KEY			_T("MountedDevices")

#define OSD_REG_OFFLINE_DETAILS			L"Software\\OfflineDetails"
#define OSD_REG_IS_OFFLINE_KEY			L"IsOfflineImage"
#define OSD_REG_OS_DRIVE_KEY			L"OSDrive"


#define OSD_REG_START_VALUE				L"Start"
#define OSD_REG_ZERO_VALUE				L"0"
#define OSD_REG_START_OVERRIDE			L"\\StartOverride"

#define OSD_REG_CONTROLSET				L"SYSTEM1\\controlset"

#define OSD_REG_SERVICES_PCIIDE			L"\\services\\pciide"
#define OSD_REG_SERVICES_ATAPI			L"\\services\\atapi"
#define OSD_REG_SERVICES_MSAHCI			L"\\services\\msahci"
#define OSD_REG_SERVICES_STORAHCI		L"\\services\\storahci"
#define OSD_REG_SERVICES_IASTOR			L"\\services\\iastor"
#define OSD_REG_SERVICES_IASTORV		L"\\services\\iastorv"


/////////////////////////////////////////
//CRegistry class declaration
/////////////////////////////////////////

class CRegistry
{
public:
    CRegistry();
	~CRegistry();
	CRegKey oExtReg;
	
private:
public:

	BOOL RegistryIsAvailable(CAtlString hKeyParent,
							 CAtlString lpszSubKeyName);

	BOOL RegistryCreateKey(CAtlString hKeyParent,
							 CAtlString lpszSubKeyName);
							 
	OSD_RESULT RegistrySetValue(
							CAtlString csKeyParent,
							CAtlString lpszSubKeyName,
							CAtlString pszValueName,
							DWORD dwType,
							const PVOID pValue,
							ULONG nBytes);

	OSD_RESULT RegistrySetKeyValue(
							CAtlString csKeyParent,
							CAtlString lpszSubKeyName,
							CAtlString lpszKeyName,
							CAtlString lpszValue,
							CAtlString lpszValueName = L"");

	OSD_RESULT RegistryQueryValue(
							CAtlString csKeyParent,
							CAtlString lpszSubKeyName,
							CAtlString pszValueName,
							DWORD* pdwType,
							void* pData,
							ULONG* pnBytes);

	OSD_RESULT RegistryQueryStringValue(
							CAtlString csKeyParent,
							CAtlString lpszSubKeyName,
							CAtlString pszValueName,
							CAtlString &o_outString);

	OSD_RESULT RegistryDeleteSubKey(CAtlString csKeyParent,CAtlString lpszSubKey,CAtlString csKeyName);
	OSD_RESULT RegistryRecurseDeleteKey(CAtlString csKeyParent,CAtlString lpszSubKey,CAtlString Key);
	OSD_RESULT RegistryDeleteValue(CAtlString csKeyParent,CAtlString lpszSubKey,CAtlString lpszValue);

	HANDLE ConvertParentKey( const CAtlString i_Parent);
	DWORD ConvertDataType( const CAtlString i_Type);

	OSD_RESULT RegDeleteMountedDevice(CAtlString i_SubKeyName=REG_MOUNTED_DEVICES_KEY);

	BOOL SetPrivilege( HANDLE hToken, LPCWSTR nameOfPrivilege, BOOL bEnablePrivilege);
	DWORD ReadRegExpandSZEx(HKEY hKey, LPSTR subKey, LPSTR valueName, LPSTR *valueData );
};
}
#endif /*__REGISTRY_H__*/