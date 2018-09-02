/*
 * CRegistry.cpp
 * 
 * Copyright(C) 
 */

#include "stdafx.h"
#include "Registry.h"

namespace OS_Manager
{
/////////////////////////////////
//Global Variables
/////////////////////////////////
static const CAtlString gpCFileName = L"Registry.cpp";

/////////////////////////////////
//FileAccess class implementation
/////////////////////////////////

CRegistry::CRegistry()
{
	   
}

CRegistry::~CRegistry()
{
	   
}

BOOL CRegistry::RegistryIsAvailable(CAtlString csKeyParent,
							        CAtlString lpszSubKeyName)
{
	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			return FALSE;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKeyName, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS == Result.SystemRetCode)
		{
			oExtReg.Close();

			return TRUE;
		}
		return FALSE;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	throw std::exception("RegistryIsAvailable exception caught");
}

BOOL CRegistry::RegistryCreateKey(CAtlString csKeyParent,
							        CAtlString lpszSubKeyName)
{
	try
	{
		HKEY handleHkey, hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			return FALSE;
		}

		LONG loadKey = RegCreateKeyEx( hKeyParent, 
											lpszSubKeyName, 
											0, 
											NULL, 
											REG_OPTION_NON_VOLATILE, 
											KEY_WRITE | KEY_SET_VALUE, 
											NULL, 
											&handleHkey, 
											NULL );

		if (loadKey != ERROR_SUCCESS)
		{
			return FALSE;
		}

		RegCloseKey(handleHkey);
		return TRUE;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	return FALSE;
}

OSD_RESULT CRegistry::RegistrySetValue(
						CAtlString csKeyParent,
						CAtlString lpszSubKeyName,
						CAtlString pszValueName,
						DWORD dwType,
						const PVOID pValue,
						ULONG nBytes)
{
	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKeyName, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.SetValue(pszValueName,
											dwType,
											pValue,
											nBytes);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			oExtReg.Close();

			return Result;
		}
		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_NET,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CRegistry::RegistrySetKeyValue(
									CAtlString csKeyParent,
									CAtlString lpszSubKeyName,
									CAtlString lpszKeyName,
									CAtlString lpszValue,
									CAtlString lpszValueName)
{

	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKeyName, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.SetKeyValue(lpszKeyName,
											lpszValue,
											lpszValueName);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			oExtReg.Close();
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			return Result;
		}
		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_NET,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CRegistry::RegistryQueryValue(
						CAtlString csKeyParent,
						CAtlString lpszSubKeyName,
						CAtlString pszValueName,
						DWORD* pdwType,
						void* pData,
						ULONG* pnBytes)
{
	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKeyName, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.QueryValue(pszValueName,
											pdwType,
											pData,
											pnBytes);

		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			oExtReg.Close();
			return Result;
		}
		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_NET,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CRegistry::RegistryQueryStringValue(
							CAtlString csKeyParent,
							CAtlString lpszSubKeyName,
							CAtlString pszValueName,
							CAtlString &o_outString)
{
	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKeyName, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		//Static allocation.
		TCHAR stringout[MAX_PATH]={0};
		DWORD Size = MAX_PATH;
		
		Result.SystemRetCode = oExtReg.QueryStringValue(pszValueName,stringout,&Size);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			oExtReg.Close();
			return Result;
		}
		o_outString = stringout;
		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_NET,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CRegistry::RegistryDeleteSubKey(
											CAtlString csKeyParent,
											CAtlString lpszSubKey,
											CAtlString csKeyName)
{
	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKey, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.DeleteSubKey(csKeyName);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			oExtReg.Close();
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			return Result;
		}
		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_NET,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CRegistry::RegistryRecurseDeleteKey(CAtlString csKeyParent,CAtlString lpszSubKey,CAtlString Key)
{
	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent =(HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKey, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.RecurseDeleteKey(Key);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			oExtReg.Close();
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			return Result;
		}
		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_NET,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CRegistry::RegistryDeleteValue(CAtlString csKeyParent,CAtlString lpszSubKey,CAtlString lpszValue)
{
	OSD_RESULT Result;
	try
	{
		HKEY hKeyParent = (HKEY)this->ConvertParentKey(csKeyParent);
		if(ISNULL(hKeyParent))
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.Open(hKeyParent, lpszSubKey, KEY_ALL_ACCESS);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_INVALID_PARAMETER);
			
			return Result;
		}

		Result.SystemRetCode = oExtReg.DeleteValue(lpszValue);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			oExtReg.Close();
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			return Result;
		}
		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%S %d]>std Exception caught (%S)\n",__FUNCTION__,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>Unknown Exception caught\n",__FUNCTION__,__LINE__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_NET,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}

OSD_RESULT CRegistry::RegDeleteMountedDevice(CAtlString i_SubKeyName)
{
	OSD_RESULT Result;
	try
	{
		Result.SystemRetCode = oExtReg.Open(REG_MOUNTED_DEVICES_PARENT, REG_MOUNTED_DEVICES_SUB_KEY, KEY_ALL_ACCESS);

		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_SYSTEM_ERROR);
			OSD_SET_EXTERNAL_ERR(Result,OSD_EXTMODULE_REGISTRY,Result.SystemRetCode);
			return Result;
		}

		Result.SystemRetCode = oExtReg.DeleteSubKey(i_SubKeyName);
		if(ERROR_SUCCESS != Result.SystemRetCode)
		{
			INFOLOG(_T("[%s:%d] Unable to del Registry Mounteddevices Lerror(0x%x)\n"),gpCFileName,__LINE__,GetLastError());

			//return FALSE;
		}

		oExtReg.Close();
		OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_NONE);
		return Result;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}
	oExtReg.Close();
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_REGISTRY,OSD_ERROR_EXCEPTION_CAUGHT);
	return Result;
}


BOOL CRegistry::SetPrivilege( HANDLE hToken, LPCWSTR nameOfPrivilege, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tknPrivileges;
    LUID luid;

	// lookup privilege on local system privilege to lookup receives LUID of privilege
    if ( !LookupPrivilegeValue( NULL, nameOfPrivilege, &luid))            
    {
        INFOLOG(L"[%d]>%S() LookupPrivilegeValue error: %d",__LINE__,__FUNCTION__, GetLastError());
        return FALSE;
    }

    tknPrivileges.PrivilegeCount = 1;
    tknPrivileges.Privileges[0].Luid = luid;
    if ( bEnablePrivilege )
	{
        tknPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
    else
	{
        tknPrivileges.Privileges[0].Attributes = 0;
	}

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tknPrivileges,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        INFOLOG(L"[%s %d]>%S() AdjustTokenPrivileges error: %d",__LINE__,__FUNCTION__, GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        INFOLOG(L"[%s %d]>%S() The token does not have the specified privilege.",__LINE__,__FUNCTION__);
        return FALSE;
    }

    return TRUE;
}

DWORD CRegistry::ReadRegExpandSZEx(HKEY hKey, LPSTR subKey, LPSTR valueName, LPSTR *valueData ) 
{
	DWORD
	dwBufLen = 0,
	type = REG_EXPAND_SZ,
	retVal = S_OK;

	*valueData = NULL;

	retVal = RegOpenKeyExA( hKey, subKey, 0, KEY_QUERY_VALUE, &hKey );

	if( retVal != ERROR_SUCCESS )
	{
		INFOLOG(L"ReadRegSZEx: Error while opening registry key for serverName %d \n", retVal );
		return retVal;
	}

	retVal = RegQueryValueExA( hKey, valueName, NULL, &type, (LPBYTE)*valueData, &dwBufLen );
	if ( retVal == ERROR_SUCCESS )
	{
		*valueData = (LPSTR)calloc(1, dwBufLen + 5);
		retVal = RegQueryValueExA( hKey, valueName, NULL, &type, (LPBYTE)*valueData, &dwBufLen );
		if ( retVal != ERROR_SUCCESS )
		{
			ERRMSG( "ReadRegSZEx: Error while reading Expand SZ value! %d \n", retVal );
		}
	}
	RegCloseKey( hKey );
	return retVal;
}





DWORD CRegistry::ConvertDataType( const CAtlString i_Type)
{
	try
	{
		if( i_Type.IsEmpty()) return 0L;

		if( !i_Type.Compare(L"REG_DWORD") )
		{
			return REG_DWORD;
		} 
		else if( !i_Type.Compare(L"REG_SZ") )
		{
			return REG_SZ;
		} 
		else if( !i_Type.Compare(L"REG_MULTI_SZ") )
		{
			return REG_MULTI_SZ;
		} 
		else if( !i_Type.Compare(L"REG_EXPAND_SZ") )
		{
			return REG_EXPAND_SZ;
		} 
		else if( !i_Type.Compare(L"REG_BINARY") )
		{
			return REG_BINARY;
		}
		else return 0L;
		/* Should add other data types also */
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}
	return NULL;
}

HANDLE CRegistry::ConvertParentKey( const CAtlString i_Parent)
{
	try
	{
		if ( !i_Parent ) return NULL;

		if( (!i_Parent.Compare(OSD_REG_PARENT_HKEY_CLASSES_ROOT)) || 
			(!i_Parent.Compare(OSD_REG_PARENT_SHORT_HKEY_CLASSES_ROOT) ))
		{
			return HKEY_CLASSES_ROOT;
		}
		
		else if( (!i_Parent.Compare(OSD_REG_PARENT_HKEY_CURRENT_CONFIG)) || 
			    (!i_Parent.Compare(OSD_REG_PARENT_SHORT_HKEY_CURRENT_CONFIG) ))
		{
			return HKEY_CURRENT_CONFIG;
		}
		else if( (!i_Parent.Compare(OSD_REG_PARENT_HKEY_CURRENT_USER)) || 
			     (!i_Parent.Compare(OSD_REG_PARENT_SHORT_HKEY_CURRENT_USER) ))
		{
			return HKEY_CURRENT_USER;
		}
		else if( (!i_Parent.Compare(OSD_REG_PARENT_HKEY_LOCAL_MACHINE)) || 
			     (!i_Parent.Compare(OSD_REG_PARENT_SHORT_HKEY_LOCAL_MACHINE) ))
		{
			return HKEY_LOCAL_MACHINE;
		}
		else if( (!i_Parent.Compare(OSD_REG_PARENT_HKEY_USERS)) || 
			     (!i_Parent.Compare(OSD_REG_PARENT_SHORT_HKEY_USERS) ))
		{
			return HKEY_USERS;
		}
		else 
		{
			return NULL;
		}
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}
	return NULL;
}


}