/*
 * ErrorCode.cpp
 * 
 * Copyright(C) 
 */


#include "stdafx.h"
namespace OS_Manager
{
/////////////////////////////////
//Global Variables
/////////////////////////////////
static const LPTSTR gpCFileName = L"ErrorCode.cpp";
INT CErrorCode::m_ApplType = -1;
/////////////////////////////////
//Function definitions
/////////////////////////////////

CAtlString GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	return GetLastErrorAsString(errorMessageID);
}

CAtlString GetLastErrorAsString(DWORD &errorMessageID)
{
	CAtlString result;
	result.Empty();

	if (errorMessageID)
	{
		LPCTSTR lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorMessageID,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		if (bufLen)
		{
			LPCTSTR lpMsgStr = (LPCTSTR)lpMsgBuf;
			std::string resultstr(lpMsgStr, lpMsgStr+bufLen);

			result = resultstr.c_str();
			result.Format(L"%s",result.GetBuffer());
			return result;
		}
		else
		{
			ERRORLOG(L"[CDeviceInstall::GetLastErrorAsString() %d] Failed to get last error message as string. returning code",__LINE__);
			result.Format(L"%d",errorMessageID);
		}
	}
	return result;
}


//To convert the external error code into internal errors
#ifdef WIN32
OSD_ERROR_CODE_ENUM CErrorCode::ConvertToIntnlErr(LONGLONG i_ExtError)
{
	//OSD_ERROR_CODE_ENUM InternalErr;
	try
	{
		switch(i_ExtError)
		{
			case ERROR_SUCCESS:
			{

			}break;
			case ERROR_INVALID_FUNCTION:
			{

			}break;
			case ERROR_FILE_NOT_FOUND:
			{

			}break;
			case ERROR_PATH_NOT_FOUND:
			{

			}break;
			case ERROR_TOO_MANY_OPEN_FILES:
			{

			}break;
			case ERROR_ACCESS_DENIED:
			{

			}break;
			case ERROR_INVALID_HANDLE:
			{

			}break;
			case ERROR_ARENA_TRASHED:
			{

			}break;
			case ERROR_NOT_ENOUGH_MEMORY:
			{

			}break;
			case ERROR_INVALID_BLOCK:
			{

			}break;
			case ERROR_BAD_ENVIRONMENT:
			{

			}break;
			case ERROR_BAD_FORMAT:
			{

			}break;
			case ERROR_INVALID_ACCESS:
			{

			}break;
			case ERROR_INVALID_DATA:
			{

			}break;
			case ERROR_OUTOFMEMORY:
			{

			}break;
			case ERROR_INVALID_DRIVE:
			{

			}break;
			case ERROR_CURRENT_DIRECTORY:
			{

			}break;
			case ERROR_NOT_SAME_DEVICE:
			{

			}break;
			case ERROR_NO_MORE_FILES:
			{

			}break;
			case ERROR_WRITE_PROTECT:
			{

			}break;
			case ERROR_BAD_UNIT:
			{

			}break;
			case ERROR_NOT_READY:
			{

			}break;
			case ERROR_BAD_COMMAND:
			{

			}break;
			case ERROR_CRC:
			{

			}break;
			case ERROR_BAD_LENGTH:
			{

			}break;
			case ERROR_SEEK:
			{

			}break;
			case ERROR_NOT_DOS_DISK:
			{

			}break;
			case ERROR_SECTOR_NOT_FOUND:
			{

			}break;
			case ERROR_WRITE_FAULT:
			{

			}break;
			case ERROR_READ_FAULT:
			{

			}break;
			case ERROR_GEN_FAILURE:
			{

			}break;
			case ERROR_SHARING_VIOLATION:
			{

			}break;
			case ERROR_LOCK_VIOLATION:
			{

			}break;
			case ERROR_WRONG_DISK:
			{

			}break;
			case ERROR_SHARING_BUFFER_EXCEEDED:
			{

			}break;
			case ERROR_HANDLE_EOF:
			{

			}break;
			case ERROR_HANDLE_DISK_FULL:
			{

			}break;
			case ERROR_NOT_SUPPORTED:
			{

			}break;
			case ERROR_REM_NOT_LIST:
			{

			}break;
			case ERROR_DUP_NAME:
			{

			}break;
			case ERROR_BAD_NETPATH:
			{

			}break;
			case ERROR_NETWORK_BUSY:
			{

			}break;
			case ERROR_DEV_NOT_EXIST:
			{

			}break;
			case ERROR_TOO_MANY_CMDS:
			{

			}break;
			case ERROR_ADAP_HDW_ERR:
			{

			}break;
			case ERROR_BAD_NET_RESP:
			{

			}break;
			case ERROR_UNEXP_NET_ERR:
			{

			}break;
			case ERROR_BAD_REM_ADAP:
			{

			}break;
			case ERROR_NETNAME_DELETED:
			{

			}break;
			case ERROR_NETWORK_ACCESS_DENIED:
			{

			}break;
			case ERROR_BAD_DEV_TYPE:
			{

			}break;
			case ERROR_BAD_NET_NAME:
			{

			}break;
			case ERROR_TOO_MANY_NAMES:
			{

			}break;
			case ERROR_TOO_MANY_SESS:
			{

			}break;
			case ERROR_SHARING_PAUSED:
			{

			}break;
			case ERROR_REQ_NOT_ACCEP:
			{

			}break;
			case ERROR_REDIR_PAUSED:
			{

			}break;
			case ERROR_FILE_EXISTS:
			{

			}break;
			case ERROR_CANNOT_MAKE:
			{

			}break;
			case ERROR_ALREADY_ASSIGNED:
			{

			}break;
			case ERROR_INVALID_PASSWORD:
			{

			}break;
			case ERROR_INVALID_PARAMETER:
			{

			}break;
			case ERROR_NET_WRITE_FAULT:
			{

			}break;
			case ERROR_NO_PROC_SLOTS:
			{

			}break;
			case ERROR_TOO_MANY_SEMAPHORES:
			{

			}break;
			case ERROR_EXCL_SEM_ALREADY_OWNED:
			{

			}break;
			case ERROR_SEM_IS_SET:
			{

			}break;
			case ERROR_TOO_MANY_SEM_REQUESTS:
			{

			}break;
			case ERROR_INVALID_AT_INTERRUPT_TIME:
			{

			}break;
			case ERROR_SEM_OWNER_DIED:
			{

			}break;
			case ERROR_SEM_USER_LIMIT:
			{

			}break;
			case ERROR_DISK_CHANGE:
			{

			}break;
			case ERROR_DRIVE_LOCKED:
			{

			}break;
			case ERROR_BROKEN_PIPE:
			{

			}break;
			case ERROR_OPEN_FAILED:
			{

			}break;
			case ERROR_BUFFER_OVERFLOW:
			{

			}break;
			case ERROR_DISK_FULL:
			{

			}break;
			case ERROR_NO_MORE_SEARCH_HANDLES:
			{

			}break;
			case ERROR_INVALID_TARGET_HANDLE:
			{

			}break;
			case ERROR_INVALID_CATEGORY:
			{

			}break;
			case ERROR_BAD_DRIVER_LEVEL:
			{

			}break;
			case ERROR_NO_VOLUME_LABEL:
			{

			}break;
			case ERROR_INVALID_NAME:
			{

			}break;
			default:
			{

			}break;
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

	throw std::exception("Error code mapping exception");
}

CString CErrorCode::ConvertToUsrMsg(LONGLONG   i_SystemRetCode)
{
	try
	{
		if(i_SystemRetCode == 0){
			//No error message has been recorded
			return OSD_SYSERROR_SUCCESS_STR; 
		}

		LPTSTR ErrStringBuffer = NULL;
		DWORD Size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									 NULL, 
									 (DWORD)i_SystemRetCode, 
									 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
									 (LPTSTR)&ErrStringBuffer, 
									 0, 
									 NULL);

		if(ISNULL(ErrStringBuffer))
		{
			INFOLOG(L"[%s %d]>SysErr:ConvertToUsrMsg Failed LError(0x%x) inputErr(0x%x)\n",
				     gpCFileName,
					 __LINE__, 
					 ::GetLastError(),
					 i_SystemRetCode);

			return OSD_SYSERROR_UNKNOWN_STR;
		}

		//Generate output string
		CString OutString(ErrStringBuffer,Size);

		//Free the buffer.
		LocalFree(ErrStringBuffer);
		
		return OutString;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}

	throw std::exception("ConvertToUsrMsg exception");
}
#else
OSD_ERROR_CODE_ENUM CErrorCode::ConvertToIntnlErr(LONGLONG i_ExtError)
{
	throw std::exception("Error code mapping:Invalid platform choosed");
}

CString CErrorCode::ConvertToUsrMsg(LONGLONG   i_SystemRetCode)
{
	throw std::exception("Error code mapping:Invalid platform choosed");
}
#endif

CString CErrorCode::ConvertToUsrMsg(OSD_ERROR_CODE_ENUM i_RetCode)
{
	try
	{
#ifdef OSD_ENABLE_INTERNAL_ERROR_MSG
		switch(i_RetCode)
		{
			case OSD_ERROR_SUCCESS:
			{
				return OSD_ERROR_SUCCESS_STR;
			}
			case OSD_ERROR_CALL_FAILURE:
			{
				return OSD_ERROR_CALL_FAILURE_STR;
			}
			case OSD_ERROR_NOT_SUPPORTED:
			{
				return OSD_ERROR_NOT_SUPPORTED_STR;
			}
			case OSD_ERROR_MEM_ALLOC_FAILURE:
			{
				return OSD_ERROR_MEM_ALLOC_FAILURE_STR;
			}
			case OSD_ERROR_THREAD_CREATION_FAILED:
			{
				return OSD_ERROR_THREAD_CREATION_FAILED_STR;
			}
			case OSD_ERROR_SEMAPHORE:
			{
				return OSD_ERROR_SEMAPHORE_STR;
			}
			case OSD_ERROR_MUTEX:
			{
				return OSD_ERROR_MUTEX_STR;
			}
			case OSD_ERROR_CRITICAL_SECTION:
			{
				return OSD_ERROR_CRITICAL_SECTION_STR;
			}
			case OSD_ERROR_EXCEPTION_CAUGHT:
			{
				return OSD_ERROR_EXCEPTION_CAUGHT_STR;
			}
			case OSD_ERROR_READ_FAILED:
			{
				return OSD_ERROR_READ_FAILED_STR;
			}
			case OSD_ERROR_WRITE_FAILED:
			{
				return OSD_ERROR_WRITE_FAILED_STR;
			}
			case OSD_ERROR_REQ_UNABLE_TO_PROCESS:
			{
				return OSD_ERROR_REQ_UNABLE_TO_PROCESS_STR;
			}
			case OSD_ERROR_REQ_NOT_SUPPORTED:
			{
				return OSD_ERROR_REQ_NOT_SUPPORTED_STR;
			}
			case OSD_ERROR_REQ_DUPLICATE:
			{
				return OSD_ERROR_REQ_DUPLICATE_STR;
			}
			case OSD_ERROR_REQ_INVALID:
			{
				return OSD_ERROR_REQ_INVALID_STR;
			}
			case OSD_ERROR_SYSTEM_ERROR:
			{
				return OSD_ERROR_SYSTEM_ERROR_STR;
			}
			case OSD_ERROR_MBR_INVALID:
			{
				return OSD_ERROR_MBR_INVALID_STR;
			}
			case OSD_ERROR_BOOTSECTOR_INVALID:
			{
				return OSD_ERROR_BOOTSECTOR_INVALID_STR;
			}
			case OSD_ERROR_DISK_ENUM_FAILED:
			{
				return OSD_ERROR_DISK_ENUM_FAILED_STR;
			}
			case OSD_ERROR_NO_DISK_AVAILABLE:
			{
				return OSD_ERROR_NO_DISK_AVAILABLE_STR;
			}
			case OSD_ERROR_DISKIO_INVALID:
			{
				return OSD_ERROR_DISKIO_INVALID_STR;
			}
			case OSD_ERROR_DISK_SIZE_NOT_MATCHING:
			{
				return OSD_ERROR_DISK_SIZE_NOT_MATCHING_STR;
			}
			case OSD_ERROR_NO_ENOUGH_SPACE:
			{
				return OSD_ERROR_NO_ENOUGH_SPACE_STR;
			}
			case OSD_ERROR_REQ_DISK_SELECTION_INAVLID:
			{
				return OSD_ERROR_REQ_DISK_SELECTION_INAVLID_STR;
			}
			case OSD_ERROR_DISK_SELECTION_FAILED:
			{
				return OSD_ERROR_DISK_SELECTION_FAILED_STR;
			}
			case OSD_ERROR_DISK_NOT_SELECTED:
			{
				return OSD_ERROR_DISK_NOT_SELECTED_STR;
			}
			case OSD_ERROR_MULTIPLE_DISK_SELECTED:
			{
				return OSD_ERROR_MULTIPLE_DISK_SELECTED_STR;
			}
			case OSD_ERROR_VOLUME_INVALID:
			{
				return OSD_ERROR_VOLUME_INVALID_STR;
			}
			case OSD_ERROR_VOLUME_NOT_SELECTED:
			{
				return OSD_ERROR_VOLUME_NOT_SELECTED_STR;
			}
			case OSD_ERROR_VOLUME_SELECTION_FAILED:
			{
				return OSD_ERROR_VOLUME_SELECTION_FAILED_STR;
			}
			case OSD_ERROR_NET_MODULE_NOT_INITIALIZED:
			{
				return OSD_ERROR_NET_MODULE_NOT_INITIALIZED_STR;
			}
			case OSD_ERROR_NET_SESSION_INVALID:
			{
				return OSD_ERROR_NET_SESSION_INVALID_STR;
			}
			case OSD_ERROR_NET_PROXY_CREDENTIAL_INVALID:
			{
				return OSD_ERROR_NET_PROXY_CREDENTIAL_INVALID_STR;
			}
			case OSD_ERROR_NET_CONNECTION_FAILED:
			{
				return OSD_ERROR_NET_CONNECTION_INVALID_STR;
			}
			case OSD_ERROR_NET_CONNECTION_CLOSED:
			{
				return OSD_ERROR_NET_CONNECTION_CLOSED_STR;
			}
			case OSD_ERROR_NET_SEND_FAILED:
			{
				return OSD_ERROR_NET_SEND_FAILED_STR;
			}
			case OSD_ERROR_NET_SOCKSHUTDOWN_FAILED:
			{
				return OSD_ERROR_NET_SOCKSHUTDOWN_FAILED_STR;
			}
			case OSD_ERROR_NET_MODULE_INIT_FAILED:
			{
				return OSD_ERROR_NET_MODULE_INIT_FAILED_STR;
			}
			case OSD_ERROR_UNABLE_TO_FIND_OS_DRIVE:
			{
				return OSD_ERROR_UNABLE_TO_FIND_OS_DRIVE_STR;
			}
			case OSD_ERROR_UNABLE_POST_DEPLOY_DISK_ENUM_FAILED:
			{
				return OSD_ERROR_UNABLE_POST_DEPLOY_DISK_ENUM_FAILED_STR;
			}
			case OSD_ERROR_POST_DEPLOYMENT_FAILED:
			{
				return OSD_ERROR_POST_DEPLOYMENT_FAILED_STR;
			}
			case OSD_ERROR_DECOMPRESSION_SIZE_EXCEEDED:
			{
				return OSD_ERROR_DECOMPRESSION_SIZE_EXCEEDED_STR;
			}
			case OSD_ERROR_CLUSTER_SEQUENCE_ERROR:
			{
				return OSD_ERROR_CLUSTER_SEQUENCE_ERROR_STR;
			}
			case OSD_ERROR_UNABLE_TO_DECOMPRESS_INVALID_CONTENT:
			{
				return OSD_ERROR_UNABLE_TO_DECOMPRESS_INVALID_CONTENT_STR;
			}
			case OSD_ERROR_COPY_FAILED:
			{
				return OSD_ERROR_COPY_FAILED_STR;
			}
			case OSD_ERROR_DISM_INITIALIZATION:
			{
				return OSD_ERROR_DISM_INITIALIZATION_STR;
			}
			case OSD_ERROR_MOUNT:
			{
				return OSD_ERROR_MOUNT_STR;
			}
			case OSD_ERROR_DISM_SESSION_CREATION:
			{
				return OSD_ERROR_DISM_SESSION_CREATION_STR;
			}
			case OSD_ERROR_UNMOUNT:
			{
				return OSD_ERROR_UNMOUNT_STR;
			}
			case OSD_ERROR_DLL_FUNC_ADDRESS_INVALID:
			{
				return OSD_ERROR_DLL_FUNC_ADDRESS_INVALID_STR;
			}
			case OSD_ERROR_DLL_NOT_FOUND:
			{
				return OSD_ERROR_DLL_NOT_FOUND_STR;
			}
			case OSD_ERROR_ERROR_IN_FINDING_WINVER:
			{
				return OSD_ERROR_ERROR_IN_FINDING_WINVER_STR;
			}
			case OSD_ERROR_IN_DRIVE_FORMAT:
			{
				return OSD_ERROR_IN_DRIVE_FORMAT_STR;
			}
			case OSD_ERROR_IN_USB_COPY:
			{
				return OSD_ERROR_IN_USB_COPY_STR;
			}
			case OSD_ERROR_INVALID_PARAMETER:
			{
				return OSD_ERROR_INVALID_PARAMETER_STR;
			}
			case OSD_ERROR_REQ_START_FAILED:
			{
				return OSD_ERROR_REQ_START_FAILED_STR;
			}
			case OSD_ERROR_REQ_STOP_FAILED:
				{
					return OSD_ERROR_REQ_STOP_FAILED_STR;
				}
			case OSD_ERROR_DATA_STRUCTURE_FAIL:
				{
					return OSD_ERROR_DATA_STRUCTURE_FAIL_STR;
				}
			case OSD_ERROR_DATA_STRUCTURE_DUP:
				{
					return OSD_ERROR_DATA_STRUCTURE_DUP_STR;
				}
			case OSD_ERROR_INVALID_INPUT:
				{
					return OSD_ERROR_INVALID_INPUT_STR;
				}
			case OSD_ERROR_INVALID_NET_PATH:
				{
					return OSD_ERROR_INVALID_NET_PATH_STR;
				}
			case OSD_ERROR_FILE_ACCESS_ERROR:
				{
					return OSD_ERROR_FILE_ACCESS_ERROR_STR;
				}
			case OSD_ERROR_INVALID_PATH:
				{
					return OSD_ERROR_INVALID_PATH_STR;
				}
			case OSD_ERROR_BITMAP_ERROR:
				{
					return OSD_ERROR_BITMAP_ERROR_STR;
				}
			case OSD_ERROR_BITMAP_INVALID:
				{
					return OSD_ERROR_BITMAP_INVALID_STR;
				}
			case OSD_ERROR_BITMAP_CANT_STORE:
				{
					return OSD_ERROR_BITMAP_CANT_STORE_STR;
				}
			case OSD_ERROR_FILESYSTEM_ERROR:
				{
					return OSD_ERROR_FILESYSTEM_ERROR_STR;
				}
			case OSD_ERROR_FILESYSTEM_INVALID:
				{
					return OSD_ERROR_FILESYSTEM_INVALID_STR;
				}
			case OSD_ERROR_HEADER_INVALID:
				{
					return OSD_ERROR_HEADER_INVALID_STR;
				}
			case OSD_ERROR_HEADER_ENCODE_FAILED:
				{
					return OSD_ERROR_HEADER_ENCODE_FAILED_STR;
				}
			case OSD_ERROR_HEADER_SAVE_FAILED:
				{
					return OSD_ERROR_HEADER_SAVE_FAILED_STR;
				}
			case OSD_ERROR_HEADER_INVLD_POSITION:
				{
					return OSD_ERROR_HEADER_INVLD_POSITION_STR;
				}
			case OSD_ERROR_HEADER_MAP_FAILED:
				{
					return OSD_ERROR_HEADER_MAP_FAILED_STR;
				}
			case OSD_ERROR_IOBUFFER_FAILED:
				{
					return OSD_ERROR_IOBUFFER_FAILED_STR;
				}
			case OSD_ERROR_EXITHANDLER_FAILED:
				{
					return OSD_ERROR_EXITHANDLER_FAILED_STR;
				}
			case OSD_ERROR_NET_CONNECTION_INVALID:
				{
					return OSD_ERROR_NET_CONNECTION_INVALID_STR;
				}
			case OSD_ERROR_NULL_POINTER:
				{
					return OSD_ERROR_NULL_POINTER_STR;
				}
			case OSD_ERROR_JSON_INVALID_KEY:
				{
					return OSD_ERROR_JSON_INVALID_KEY_STR;
				}
			case OSD_ERROR_JSON_INVALID_VALUE:
				{
					return OSD_ERROR_JSON_INVALID_VALUE_STR;
				}
			case OSD_ERROR_JSON_INVALID_FILE:
				{
					return OSD_ERROR_JSON_INVALID_FILE_STR;
				}
			case OSD_ERROR_PARSER_CREATION_FAILED:
				{
					return OSD_ERROR_PARSER_CREATION_FAILED_STR;
				}
			case OSD_ERROR_DATA_MODEL_CREATE_FAILED:
				{
					return OSD_ERROR_DATA_MODEL_CREATE_FAILED_STR;
				}
			case OSD_ERROR_DATA_MODEL_NULL:
				{
					return OSD_ERROR_DATA_MODEL_NULL_STR;
				}
			case OSD_ERROR_DATA_MODEL_CONVERT_FAILED:
				{
					return OSD_ERROR_DATA_MODEL_CONVERT_FAILED_STR;
				}
			case OSD_ERROR_UNABLE_TO_REG_SIGNAL_HANDLER:
				{
					return OSD_ERROR_UNABLE_TO_REG_SIGNAL_HANDLER_STR;
				}
			case OSD_ERROR_INVALID_NET_INTERFACE:
				{
					return OSD_ERROR_INVALID_NET_INTERFACE_STR;
				}
			case OSD_ERROR_AGENT_RES_ID_URL_COSTRC_FAILED:
				{
					return OSD_ERROR_AGENT_RES_ID_URL_COSTRC_FAILED_STR;
				}
			case OSD_ERROR_AGENT_RESOURCE_ID_UNAVAILABLE:
				{
					return OSD_ERROR_AGENT_RESOURCE_ID_UNAVAILABLE_STR;
				}
			case OSD_ERROR_NET_URL_NOT_AVAILABLE:
				{
					return OSD_ERROR_NET_URL_NOT_AVAILABLE_STR;
				}
			case OSD_ERROR_CONFIG_NOT_INITIALIZE_PROPERLY:
				{
					return OSD_ERROR_CONFIG_NOT_INITIALIZE_PROPERLY_STR;
				}
			case OSD_ERROR_SERIALIZER_FAILED:
				{
					return OSD_ERROR_SERIALIZER_FAILED_STR;
				}
			case OSD_ERROR_REQ_ALREADY_STOPPED:
				{
					return OSD_ERROR_REQ_ALREADY_STOPPED_STR;
				}
			case OSD_ERROR_POINT_OF_CONTACT_EMPTY:
				{
					return OSD_ERROR_POINT_OF_CONTACT_EMPTY_STR;
				}
			case OSD_ERROR_NET_DOWNLOAD_PATH_CREATION_FAILED:
				{
					return OSD_ERROR_NET_DOWNLOAD_PATH_CREATION_FAILED_STR;
				}
			case OSD_ERROR_POST_DEPLOY_OSDRIVE_NOT_AVAILABLE:
				{
					return OSD_ERROR_POST_DEPLOY_OSDRIVE_NOT_AVAILABLE_STR;
				}
			case OSD_ERROR_COMPRESSION_MODULE_CREATION_FAILED:
				{
					return OSD_ERROR_COMPRESSION_MODULE_CREATION_FAILED_STR;
				}
			case OSD_ERROR_IN_DECOMPRESSION:
				{
					return OSD_ERROR_IN_DECOMPRESSION_STR;
				}
			case OSD_ERROR_IN_COMPRESSION:
				{
					return OSD_ERROR_IN_COMPRESSION_STR;
				}
			case OSD_ERROR_NET_RECEIVE_ERROR:
				{
					return OSD_ERROR_NET_RECEIVE_ERROR_STR;
				}
			case OSD_ERROR_NET_MULTICAST_OBJECT_FAILED:
				{
					return OSD_ERROR_NET_MULTICAST_OBJECT_FAILED_STR;
				}
			case OSD_ERROR_NET_MULTICAST_INIT_FAILED:
				{
					return OSD_ERROR_NET_MULTICAST_INIT_FAILED_STR;
				}
			case OSD_ERROR_NET_MULTICAST_INIT_SUCC:
				{
					return OSD_ERROR_NET_MULTICAST_INIT_SUCC_STR;
				}
			case OSD_ERROR_NET_MULTICAST_ALREADY_INITIALIZED:
				{
					return OSD_ERROR_NET_MULTICAST_ALREADY_INITIALIZED_STR;
				}
			case OSD_ERROR_NET_MULTICAST_GROUP_JOIN_SUCC:
				{
					return OSD_ERROR_NET_MULTICAST_GROUP_JOIN_SUCC_STR;
				}
			case OSD_ERROR_NET_MULTICAST_GROUP_JOIN_FAILED:
				{
					return OSD_ERROR_NET_MULTICAST_GROUP_JOIN_FAILED_STR;
				}
			case OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_SUCC:
				{
					return OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_SUCC_STR;
				}
			case OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_FAILED:
				{
					return OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_FAILED_STR;
				}
			case OSD_ERROR_NET_MULTICAST_INVALID_PACKET:
				{
					return OSD_ERROR_NET_MULTICAST_INVALID_PACKET_STR;
				}
			case OSD_ERROR_NET_MULTICAST_DATA_NOT_RECEIVED:
				{
					return OSD_ERROR_NET_MULTICAST_DATA_NOT_RECEIVED_STR;
				}
			case OSD_ERROR_NET_MULTICAST_READY_TO_RECEIVE:
				{
					return OSD_ERROR_NET_MULTICAST_READY_TO_RECEIVE_STR;
				}
			case OSD_ERROR_NET_MULTICAST_KILL_FAILED:
				{
					return OSD_ERROR_NET_MULTICAST_KILL_FAILED_STR;
				}
			case OSD_ERROR_NET_MULTICAST_KILL_SUCC:
				{
					return OSD_ERROR_NET_MULTICAST_KILL_SUCC_STR;
				}
			case OSD_ERROR_FAILED_TO_GET_SYSTEMINFO:
				{
					return OSD_ERROR_FAILED_TO_GET_SYSTEMINFO_STR;
				}
			case OSD_ERROR_IN_ADDING_DRIVER:
				{
					return OSD_ERROR_IN_ADDING_DRIVER_STR;
				}
			case OSD_ERROR_DEST_SIZE_LESS_THAN_SOURCE_SIZE:
				{
					return OSD_ERROR_DEST_SIZE_LESS_THAN_SOURCE_SIZE_STR;
				}
			case OSD_ERROR_DEST_SIZE_NOT_ENOUGH_TO_EXPAND:
				{
					return OSD_ERROR_DEST_SIZE_NOT_ENOUGH_TO_EXPAND_STR;
				}
			case OSD_ERROR_DISK_ADJUSTMENT_FAILED:
				{
					return OSD_ERROR_DISK_ADJUSTMENT_FAILED_STR;
				}
			case OSD_ERROR_HIDDEN_VOLUME_RESIZE_IGNORED:
				{
					return OSD_ERROR_HIDDEN_VOLUME_RESIZE_IGNORED_STR;
				}
			case OSD_ERROR_OS_NOT_COMPATIBLE:
				{
					return OSD_ERROR_OS_NOT_COMPATIBLE_STR;
				}
			case OSD_ERROR_CONFIG_IS_NOT_AVAILABLE:
				{
					return OSD_ERROR_CONFIG_IS_NOT_AVAILABLE_STR;
				}
			case OSD_ERROR_UNABLE_TO_CREATE_CLIENT_CONFIG:
				{
					return OSD_ERROR_UNABLE_TO_CREATE_CLIENT_CONFIG_STR;
				}
			case OSD_ERROR_IN_VOLUME_SHRINK:
				{
					return OSD_ERROR_IN_VOLUME_SHRINK_STR;
				}
			case OSD_ERROR_IN_VOLUME_EXTEND:
				{
					return OSD_ERROR_IN_VOLUME_EXTEND_STR;
				}
			case OSD_ERROR_IN_VOLUME_RESIZE:
				{
					return OSD_ERROR_IN_VOLUME_RESIZE_STR;
				}
			case OSD_ERROR_DATAMODEL_AGENT_INSTALL_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_AGENT_INSTALL_FAILED_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_COMP_DETAILS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_AGENT_RESOURCE_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_AGENT_REGISTRATION_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_NET_PREF_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_NET_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_COMP_NAME_REQ_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_COMP_NAME_RESP_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DEPLOYMENT_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DEPLOYMENT_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DEPLOYMENT_TEMPLATES_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVER_DB_RESP_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVER_DB_STATUS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVER_REQ_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVERS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVERS_SCAN_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVERS_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVERS_STATUS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_ERROR_DM_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_ERROR_REPORT_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_CREATION_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_COM_DETAILS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_STATUS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_PROGRESS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_LICENSE_VALIDATION_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_MACHINE_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_OSD_REQ_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_PART_DETAILS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_PATCHER_STATUS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_PE_DRVIER_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_PE_SHARE_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_PE_TOOL_DETAILS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_POST_DEPLOY_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_SECURITY_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_SERVER_INFO_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_SERVER_REQ_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_SHARE_REPO_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_SW_DEPLOY_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_TARGET_DISK_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_USER_ACCOUNTS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_USER_ACCOUNT_SETTINGS_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_VIEW_LOG_PARSE_ERROR:
			case OSD_ERROR_DATAMODEL_STORAGE_DETAILS_PARSE_ERROR:
				{
					return OSD_ERROR_DATAMODEL_PARSE_ERROR_STR;
				}
			case OSD_ERROR_DATAMODEL_AGENT_INSTALL_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_AGENT_INSTALL_FAILED_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_COMP_DETAILS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_AGENT_RESOURCE_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_AGENT_REGISTRATION_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_NET_PREF_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_NET_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_COMP_NAME_REQ_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_COMP_NAME_RESP_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DEPLOYMENT_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DEPLOYMENT_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DEPLOYMENT_TEMPLATES_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVER_DB_RESP_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVER_DB_STATUS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVER_REQ_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVERS_SERIALIZE_ERROR:	
			case OSD_ERROR_DATAMODEL_DRIVERS_SCAN_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVERS_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_DRIVERS_STATUS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_ERROR_DM_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_ERROR_REPORT_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_CREATION_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_COM_DETAILS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_IMAGE_STATUS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_PROGRESS_STATUS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_LICENSE_VALIDATION_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_MACHINE_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_OSD_REQ_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_PART_DETAILS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_PATCHER_STATUS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_PE_DRIVER_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_PE_SHARE_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_PE_TOOL_DETAILS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_POST_DEPLOY_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_SECURITY_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_SERVER_INFO_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_SERVER_REQ_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_SHARE_REPO_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_SW_DEPLOY_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_TARGET_DISK_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_USER_ACCOUNTS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_USER_ACCOUNT_SETTINGS_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_VIEW_LOG_SERIALIZE_ERROR:
			case OSD_ERROR_DATAMODEL_STORAGE_DETAILS_SERIALIZE_ERROR:
				{
					return OSD_ERROR_DATAMODEL_SERIALIZE_ERROR_STR;
				}
			case OSD_ERROR_UNKNOWN:
			default:
			{
				if(CErrorCode::m_ApplType == OSD_IMAGE_CREATION_APPLICATION)
				{
					return OSD_ERROR_IN_IMAGE_CREATION_STR;
				}
				else if(CErrorCode::m_ApplType == OSD_DEPLOYMENT_APPLICATION)
				{
					return OSD_ERROR_IN_DEPLOYMENT_STR;
				}
				else
				{
					return OSD_ERROR_COMMON_STR;
				}
				
			}
		}
#else /*OSD_ENABLE_INTERNAL_ERROR_MSG*/
		if(i_RetCode == OSD_ERROR_NONE)
		{
			return OSD_ERROR_SUCCESS_STR;
		}
		else
		{
			return OSD_ERROR_COMMON_STR;
		}
#endif /*!OSD_ENABLE_INTERNAL_ERROR_MSG*/
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}

	throw std::exception("Error conversion exception");
}

}/*OS_Manager*/