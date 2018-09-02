/*
 * ErrorCode.h 
 * 
 * Copyright(C) 
 */

#pragma once
#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

namespace OS_Manager
{
/////////////////////////////////
//Macros
/////////////////////////////////
#define OSD_IMAGE_CREATION_APPLICATION		0
#define OSD_DEPLOYMENT_APPLICATION			1

//Utilites
#define OSD_ERROR_STRING_EXTN                "_STR"
#define OSD_CONVERT_TO_ERROR_STR(__INPUT__)  #__INPUT__ OSD_ERROR_STRING_EXTN
#define OSD_ERROR_STRING(__INPUT__)          OSD_CONVERT_TO_ERROR_STR(__INPUT__)

#define OSD_SET_INTERNAL_ERR(Res,Module,Error)		{\
	                                                 Res.RetCode   = Error;\
	                                                 Res.IntModule = Module;\
													 Res.IntLineNum = __LINE__;\
										            }

#define OSD_SET_EXTERNAL_ERR(Res,Module,Error)		{\
	                                                 Res.SystemRetCode = Error;\
	                                                 Res.ExtModule     = Module;\
													 Res.ExtLineNum = __LINE__;\
										            }


#define OSD_MODULE_EXT_ERROR(Res,Module)		        {\
														if(Res.SystemRetCode != 0){\
														   Res.IntModule = Module;\
														   Res.IntLineNum = __LINE__;\
														}\
														}
										            

//#define POSD_SET_INTERNAL_ERR(Res,Module,Error)		{\
//	                                                 Res->RetCode   = Error;\
//	                                                 Res->IntModule = Module;\
//													 Res->IntLineNum = __LINE__;\
//										            }
//
//#define POSD_SET_EXTERNAL_ERR(Res,Module,Error)		{\
//	                                                 Res->SystemRetCode = Error;\
//	                                                 Res->ExtModule     = Module;\
//													 Res->ExtLineNum = __LINE__;\\

#define POSD_SET_INTERNAL_ERR(Res,Module,FileAccess,Error)		{\
													 Res->RetCode   = Error;\
	                                                 Res->IntModule = Module;\
													 Res->IntLineNum = __LINE__;\
													 Res->FileAccessType = FileAccess;\
										            }

#define POSD_SET_EXTERNAL_ERR(Res,Module,FileAccess,Error)		{\
	                                                 Res->SystemRetCode = Error;\
	                                                 Res->ExtModule     = Module;\
													 Res->ExtLineNum = __LINE__;\
													 Res->FileAccessType = FileAccess;\
										            }



/*
#define OSD_SET_INTERNAL_ERR(Res,Module,Error)		{\
	                                                 Res.RetCode   = Error;\
	                                                 Res.IntModule = Module;\
												     Res.SystemRetCode = ERROR_SUCCESS;\
	                                                 Res.ExtModule     = OSD_EXTMODULE_UNKNOWN;
										            }

#define OSD_SET_EXTERNAL_ERR(Res,Module,Error)		{\
	                                                 Res.SystemRetCode = Error;\
	                                                 Res.ExtModule     = Module;\
													 Res.RetCode   = OSD_ERROR_SUCCESS;\
	                                                 Res.IntModule = OSD_MODULE_UNKNOWN;\
										            }
*/

//Console status strings
#define OSD_STATUS_SUCC_STR					    _T("Success")
#define OSD_STATUS_FAIL_STR					    _T("Failed")

//Internal Error strings
#define OSD_ERROR_SUCCESS_STR					_T("Success")
#define OSD_ERROR_COMMON_STR					_T("Internal error occured. Contact Support to resolve this error")

#define OSD_ERROR_IN_IMAGE_CREATION_STR			_T("Error occurred while creating image. Contact Support to resolve this error")
#define OSD_ERROR_IN_DEPLOYMENT_STR				_T("Error occurred while deploying image. Contact Support to resolve this error")

/*NOTE: This is for internal usage to display the error string*/
//For internal usage alone we can convert macro into string.
#define OSD_ERROR_INVALID_PARAMETER_STR			_T("Invalid Parameter")
#define OSD_ERROR_CALL_FAILURE_STR				_T("Call failure")
#define OSD_ERROR_NOT_SUPPORTED_STR				_T("Call not supported")
#define OSD_ERROR_MEM_ALLOC_FAILURE_STR			_T("Memory alloc failed")
#define OSD_ERROR_THREAD_CREATION_FAILED_STR	_T("Thread creation failed")
#define OSD_ERROR_SEMAPHORE_STR					_T("Semaphore error")
#define OSD_ERROR_MUTEX_STR						_T("Mutex error")
#define OSD_ERROR_CRITICAL_SECTION_STR			_T("Critical section error")
#define OSD_ERROR_EXCEPTION_CAUGHT_STR			_T("Exception caught")
#define OSD_ERROR_READ_FAILED_STR				_T("Read failed")
#define OSD_ERROR_WRITE_FAILED_STR				_T("Write failed")
#define OSD_ERROR_REQ_START_FAILED_STR			_T("Start request Failed")
#define OSD_ERROR_REQ_STOP_FAILED_STR			_T("Stop request Failed")
#define OSD_ERROR_REQ_UNABLE_TO_PROCESS_STR		_T("Unable to process request")
#define OSD_ERROR_REQ_NOT_SUPPORTED_STR			_T("Request not supported")
#define OSD_ERROR_REQ_DUPLICATE_STR				_T("Duplicate request")
#define OSD_ERROR_REQ_INVALID_STR				_T("Invalid request")
#define OSD_ERROR_DATA_STRUCTURE_FAIL_STR		_T("Data structure Fail")
#define OSD_ERROR_DATA_STRUCTURE_DUP_STR		_T("Data structure Dupicate")
#define OSD_ERROR_INVALID_INPUT_STR				_T("Invalid Input")
#define OSD_ERROR_SYSTEM_ERROR_STR				_T("System error occured")

#define OSD_ERROR_MBR_INVALID_STR                   _T("Unable to match MBR. It might be a extended/logical partion disk, current version not supported this feature")
#define OSD_ERROR_BOOTSECTOR_INVALID_STR            _T("Unable to read bootsector")
#define OSD_ERROR_DISK_ENUM_FAILED_STR              _T("Disk enumeration failed")
#define OSD_ERROR_NO_DISK_AVAILABLE_STR             _T("No disk available")
#define OSD_ERROR_DISKIO_INVALID_STR				_T("DiskIO invalid")
#define OSD_ERROR_INVALID_NET_PATH_STR				_T("Invalid Net Path")
#define OSD_ERROR_FILE_ACCESS_ERROR_STR				_T("File Access Error")
#define OSD_ERROR_INVALID_PATH_STR					_T("Invalid Path")
#define OSD_ERROR_DISK_SIZE_NOT_MATCHING_STR		_T("Deployment failed since the target disk size is lesser than source disk size or the disk is incompatible. Shrink partitions, create image again and retry deployment.")

#define OSD_ERROR_BITMAP_ERROR_STR					_T("Bitmap Error")
#define OSD_ERROR_BITMAP_INVALID_STR				_T("Bitmap Invalid")
#define OSD_ERROR_BITMAP_CANT_STORE_STR				_T("Can not store Bitmap")
#define OSD_ERROR_FILESYSTEM_ERROR_STR				_T("Filesystem Error")
#define OSD_ERROR_FILESYSTEM_INVALID_STR			_T("Invalid Filesystem")
#define OSD_ERROR_HEADER_INVALID_STR				_T("Invalid Header")
#define OSD_ERROR_HEADER_ENCODE_FAILED_STR			_T("Failed to encode Header")
#define OSD_ERROR_HEADER_SAVE_FAILED_STR			_T("Failed to save Header")
#define OSD_ERROR_HEADER_INVLD_POSITION_STR			_T("Header position invalid")
#define OSD_ERROR_HEADER_MAP_FAILED_STR				_T("Failed to map header")
#define OSD_ERROR_IOBUFFER_FAILED_STR				_T("IO Buffer Failed")
#define OSD_ERROR_EXITHANDLER_FAILED_STR			_T("Exit Handler failed")
#define OSD_ERROR_NULL_POINTER_STR					_T("NULL Pointer Error")
#define OSD_ERROR_JSON_INVALID_KEY_STR				_T("Invalid Json Key")
#define OSD_ERROR_JSON_INVALID_VALUE_STR			_T("Invalid Json Value")
#define OSD_ERROR_JSON_INVALID_FILE_STR				_T("Invalid Json File")
#define OSD_ERROR_PARSER_CREATION_FAILED_STR		_T("Parser creation Failed")
#define OSD_ERROR_DATA_MODEL_CREATE_FAILED_STR		_T("Failed to create Datamodel")
#define OSD_ERROR_DATA_MODEL_NULL_STR				_T("Null DataModel")
#define OSD_ERROR_DATA_MODEL_CONVERT_FAILED_STR		_T("Failed to convert datamodel")
#define OSD_ERROR_UNABLE_TO_REG_SIGNAL_HANDLER_STR	_T("Unable to register signal handler")
#define OSD_ERROR_INVALID_NET_INTERFACE_STR			_T("Invalid Net Interface")
#define OSD_ERROR_AGENT_RES_ID_URL_COSTRC_FAILED_STR	_T("Agent Resourse ID URL construction Failed")
#define OSD_ERROR_AGENT_RESOURCE_ID_UNAVAILABLE_STR		_T("Agent Resourse ID Unavailable")
#define OSD_ERROR_NET_URL_NOT_AVAILABLE_STR			_T("Net URL Not available")
#define OSD_ERROR_CONFIG_NOT_INITIALIZE_PROPERLY_STR	_T("Configuration not initialized properly")
#define OSD_ERROR_SERIALIZER_FAILED_STR				_T("Serialiser failed")
#define OSD_ERROR_REQ_ALREADY_STOPPED_STR			_T("Request already stopped")
#define OSD_ERROR_POINT_OF_CONTACT_EMPTY_STR		_T("Point of contact empty")
#define OSD_ERROR_NET_DOWNLOAD_PATH_CREATION_FAILED_STR	_T("Net Download path creation failed")
#define OSD_ERROR_POST_DEPLOY_OSDRIVE_NOT_AVAILABLE_STR	_T("Post deployment OS Drive Not available")
#define OSD_ERROR_COMPRESSION_MODULE_CREATION_FAILED_STR	_T("Compression module creation failed")
#define OSD_ERROR_IN_DECOMPRESSION_STR				_T("Error in Decompression")
#define OSD_ERROR_IN_COMPRESSION_STR				_T("Error in Compression")
#define OSD_ERROR_NET_RECEIVE_ERROR_STR				_T("Net Receive Error")
#define OSD_ERROR_NET_MULTICAST_OBJECT_FAILED_STR	_T("Net multicast object Failed")
#define OSD_ERROR_NET_MULTICAST_INIT_FAILED_STR		_T("Net Multicast Initialization Failed")
#define OSD_ERROR_NET_MULTICAST_INIT_SUCC_STR		_T("Net Multicast Initialization Success")
#define OSD_ERROR_NET_MULTICAST_ALREADY_INITIALIZED_STR		_T("Net Multicast already Initialized")
#define OSD_ERROR_NET_MULTICAST_GROUP_JOIN_SUCC_STR		_T("Net multicast group join success")
#define OSD_ERROR_NET_MULTICAST_GROUP_JOIN_FAILED_STR	_T("Net multicast group join Failed")
#define OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_SUCC_STR	_T("Net multicast group Leave Success")
#define OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_FAILED_STR	_T("Net multicast group Leave Failed")
#define OSD_ERROR_NET_MULTICAST_INVALID_PACKET_STR		_T("Net multicast invalid packet")
#define OSD_ERROR_NET_MULTICAST_DATA_NOT_RECEIVED_STR	_T("Net multicast data not received")
#define OSD_ERROR_NET_MULTICAST_READY_TO_RECEIVE_STR	_T("Net multicast ready to receive")
#define OSD_ERROR_NET_MULTICAST_KILL_FAILED_STR			_T("Net multicast kill Failed")
#define OSD_ERROR_NET_MULTICAST_KILL_SUCC_STR			_T("Net multicast kill Success")
#define OSD_ERROR_FAILED_TO_GET_SYSTEMINFO_STR			_T("Failed to get systeminfo")
#define OSD_ERROR_IN_ADDING_DRIVER_STR					_T("Error in adding drivers")
#define OSD_ERROR_DEST_SIZE_LESS_THAN_SOURCE_SIZE_STR	_T("Deployment failed - Image size is incompatible with the disk size of target computer")
#define OSD_ERROR_DEST_SIZE_NOT_ENOUGH_TO_EXPAND_STR	_T("Destination size not enough to expand")
#define OSD_ERROR_DISK_ADJUSTMENT_FAILED_STR				_T("Deployment is success. Disk Adjustment has error")	
#define OSD_ERROR_HIDDEN_VOLUME_RESIZE_IGNORED_STR			_T("Disk resize is ignored because of last partition is hidden")
//#define OSD_ERROR_OS_NOT_COMPATIBLE_STR					_T("The OS you are trying to deploy is not compatible with this hardware")
#define OSD_ERROR_OS_NOT_COMPATIBLE_STR					_T("You are trying to deploy 64 bit OS. Please use 64 bit WinPE to deploy 64 bit OS")

#define OSD_ERROR_NO_ENOUGH_SPACE_STR				_T("Not enough storage is available in given path")
#define OSD_ERROR_REQ_DISK_SELECTION_INAVLID_STR	_T("Requested disk selection failed")
#define OSD_ERROR_DISK_SELECTION_FAILED_STR			_T("Disk selection failed")			
#define OSD_ERROR_DISK_NOT_SELECTED_STR				_T("Disk not selected")
#define OSD_ERROR_MULTIPLE_DISK_SELECTED_STR		_T("Multiple disk selected")
#define OSD_ERROR_VOLUME_INVALID_STR				_T("Invalid volume")
#define OSD_ERROR_VOLUME_NOT_SELECTED_STR			_T("Volume not selected")
#define OSD_ERROR_VOLUME_SELECTION_FAILED_STR		_T("Volume selection failed")

#define OSD_ERROR_NET_MODULE_NOT_INITIALIZED_STR    _T("Network module not initialized")
#define OSD_ERROR_NET_SESSION_INVALID_STR			_T("Network module session invalid")
#define OSD_ERROR_NET_PROXY_CREDENTIAL_INVALID_STR	_T("Network proxy credential invalid")
#define OSD_ERROR_NET_CONNECTION_INVALID_STR		_T("Network module connection invalid")
#define OSD_ERROR_NET_CONNECTION_CLOSED_STR			_T("Network module connection already closed")
#define OSD_ERROR_NET_SEND_FAILED_STR				_T("Network data send failure")
#define OSD_ERROR_NET_SOCKSHUTDOWN_FAILED_STR		_T("Network module shutdown failed")
#define OSD_ERROR_NET_MODULE_INIT_FAILED_STR		_T("Network module init failed")

#define OSD_ERROR_UNABLE_TO_FIND_OS_DRIVE_STR		_T("Unable to find OS drive info in image")
#define OSD_ERROR_UNABLE_POST_DEPLOY_DISK_ENUM_FAILED_STR	_T("Post deployment disk enum failed")
#define OSD_ERROR_POST_DEPLOYMENT_FAILED_STR				_T("Post deployment failed")

#define OSD_ERROR_DECOMPRESSION_SIZE_EXCEEDED_STR   _T("Error occurred while decompressing image for deployment. Contact Support to resolve this error")
#define OSD_ERROR_CLUSTER_SEQUENCE_ERROR_STR        _T("Disk used cluster sequence mapping failed")
#define OSD_ERROR_UNABLE_TO_DECOMPRESS_INVALID_CONTENT_STR _T("Incompatible image - Deployment failed. Upgrade to latest server build, create new  image and initiate deployment")

#define OSD_NET_CONNECTION_FAILED_STR				_T("Unable to connect server")
#define OSD_NET_CONNECTION_SUCC_STR					_T("Server conncted successfully")
#define OSD_NET_CONNECTION_INPROGRESS_STR			_T("Server connection in progress")
#define OSD_NET_CONNECTION_RECONNECT_STR			_T("Unable to connect server. Reconnecting...")
#define OSD_ERROR_UNKNOWN_STR						_T("Unknown error")

/*NOTE: This is for common error string to support all the platforms
*       Not used now.
*/
#define OSD_SYSERROR_SUCCESS_STR						_T("System Error Success")
#define OSD_SYSERROR_INVALID_FUNCTION_STR				_T("Invalid System Function")
#define OSD_SYSERROR_FILE_NOT_FOUND_STR					_T("The system cannot find the file specified")
#define OSD_SYSERROR_PATH_NOT_FOUND_STR					_T("The system cannot find the path specified")
#define OSD_SYSERROR_TOO_MANY_OPEN_FILES_STR			_T("The system cannot open the file")
#define OSD_SYSERROR_ACCESS_DENIED_STR					_T("Access is denied")
#define OSD_SYSERROR_INVALID_IO_HANDLE_STR				_T("The handle is invalid")
#define OSD_SYSERROR_INVALID_STORAGE_STR				_T("The storage control blocks were destroyed")
#define OSD_SYSERROR_NOT_ENOUGH_MEMORY_STR				_T("Not enough storage is available")
#define OSD_SYSERROR_BAD_OS_ENVIRONMENT_STR				_T("The environment is incorrect")
#define OSD_SYSERROR_INVALID_FILE_FORMAT_STR			_T("Program with an incorrect format")
#define OSD_SYSERROR_INVALID_ACCESS_STR					_T("The access code is invalid")
#define OSD_SYSERROR_INVALID_DATA_STR					_T("Invalid data")
#define OSD_SYSERROR_OUTOFMEMORY_STR					_T("Not enough storage is available")
#define OSD_SYSERROR_INVALID_DRIVE_STR					_T("The system cannot find the drive specified")
#define OSD_SYSERROR_CURRENT_DIRECTORY_STR				_T("The directory cannot be removed")
#define OSD_SYSERROR_NOT_SAME_DEVICE_STR				_T("The system cannot move the file to a different disk drive")
#define OSD_SYSERROR_NO_MORE_FILES_STR					_T("There are no more files")
#define OSD_SYSERROR_WRITE_PROTECT_STR					_T("The media is write protected")
#define OSD_SYSERROR_NO_SUCH_DEVICE_STR					_T("The system cannot find the device specified")
#define OSD_SYSERROR_DEVICE_NOT_READY_STR				_T("The device is not ready")
#define OSD_SYSERROR_INVALID_IO_CMD_STR					_T("The device does not recognize the command")
#define OSD_SYSERROR_CRC_STR							_T("CRC error")
#define OSD_SYSERROR_INVALID_SEEK_STR					_T("Seek operation failed")
#define OSD_SYSERROR_DISK_NOT_FOUND_STR					_T("The specified disk not found")
#define OSD_SYSERROR_SECTOR_NOT_FOUND_STR				_T("The drive cannot find the sector")
#define OSD_SYSERROR_WRITE_FAULT_OCCURED_STR			_T("The system cannot write to the specified device")
#define OSD_SYSERROR_READ_FAULT_OCCURED_STR				_T("The system cannot read from the specified device")
#define OSD_SYSERROR_DEVICE_NOT_FUNCTIONING_STR			_T("A device attached to the system is not functioning")
#define OSD_SYSERROR_SHARING_VIOLATION_STR				_T("The process cannot access the file because it is being used by another process")
#define OSD_SYSERROR_LOCK_VIOLATION_STR					_T("The process cannot access the file because another process has locked a portion of the file")
#define OSD_SYSERROR_WRONG_DISK_STR						_T("The wrong diskette is in the drive")
#define OSD_SYSERROR_SHARING_OPERATION_STR				_T("Too many files opened for sharing")
#define OSD_SYSERROR_HANDLE_EOF_STR						_T("Reached the end of the file")
#define OSD_SYSERROR_DISK_FULL_STR						_T("The disk is full")
#define OSD_SYSERROR_REQ_NOT_SUPPORTED_STR				_T("The request is not supported")
#define OSD_SYSERROR_CANT_FIND_NETWORK_STR				_T("Cannot find the network path")
#define OSD_SYSERROR_DUP_NETWORK_NAME_STR				_T("Duplicate name exists on the network")
#define OSD_SYSERROR_INVALID_NETWORK_PATH_STR			_T("The network path was not found")
#define OSD_SYSERROR_NETWORK_BUSY_STR					_T("The network is busy")
#define OSD_SYSERROR_NETWORK_RES_NOT_EXIST_STR			_T("The specified network resource or device is no longer available")
#define OSD_SYSERROR_NETWORK_CMD_ERROR_STR				_T("Network cmd limit exceeed")
#define OSD_SYSERROR_NETWORK_HW_ERROR_STR				_T("A network adapter hardware error occurred")
#define OSD_SYSERROR_NETWORK_SERVER_ERROR_STR			_T("The specified server cannot perform the requested operation")
#define OSD_SYSERROR_NETWORK_UNKNOWN_ERROR_STR			_T("An unexpected network error occurred")
#define OSD_SYSERROR_REMOTE_ERROR_STR					_T("The remote adapter is not compatible")
#define OSD_SYSERROR_NETWORK_NAME_DELETED_STR			_T("The specified network name is no longer available")
#define OSD_SYSERROR_NETWORK_ACCESS_DENIED_STR			_T("Network access is denied")
#define OSD_SYSERROR_BAD_NETWORK_DEVICE_STR				_T("The network resource type is not correct")
#define OSD_SYSERROR_BAD_NETWORK_NAME_STR				_T("The network name cannot be found")
#define OSD_SYSERROR_TOO_MANY_NETWORK_ADAPTER_STR		_T("The name limit for the local computer network adapter card was exceeded")
#define OSD_SYSERROR_NETWORK_OPERATION_INVALID_STR		_T("The network BIOS session limit was exceeded")
#define OSD_SYSERROR_SHARING_PAUSED_STR					_T("The remote server has been paused or is in the process of being started")
#define OSD_SYSERROR_TOO_MANY_CONNECTIONS_STR			_T("Too many remote connections")
#define OSD_SYSERROR_IO_OPERATION_PAUSED_STR			_T("IO operation paused")
#define OSD_SYSERROR_FILE_EXISTS_STR					_T("The file exists")
#define OSD_SYSERROR_CANT_CREATE_DIR_OR_FILE_STR		_T("The directory or file cannot be created")
#define OSD_SYSERROR_ALREADY_ASSIGNED_STR				_T("The local device name is already in use")
#define OSD_SYSERROR_INVALID_PASSWORD_STR				_T("The specified network password is not correct")
#define OSD_SYSERROR_INVALID_PARAMETER_STR				_T("The parameter is incorrect")
#define OSD_SYSERROR_NET_WRITE_FAULT_STR				_T("A write fault occurred on the network")
#define OSD_SYSERROR_UNABLE_TO_START_PROCESS_STR		_T("The system cannot start another process at this time")
#define OSD_SYSERROR_DISK_CHANGE_STR					_T("The program stopped because an alternate diskette was not inserted")
#define OSD_SYSERROR_DRIVE_LOCKED_STR					_T("The disk is in use or locked by another process")
#define OSD_SYSERROR_BROKEN_PIPE_STR					_T("The pipe has been ended")
#define OSD_SYSERROR_OPEN_FAILED_STR					_T("The system cannot open the device or file specified")
#define OSD_SYSERROR_BUFFER_OVERFLOW_STR				_T("The file name is too long")
#define OSD_SYSERROR_INVALID_NAME_STR					_T("The filename, directory name, or volume label syntax is incorrect")
#define OSD_SYSERROR_UNKNOWN_STR						_T("Unknown system error")
#define OSD_ERROR_TARGET_DISK_NOT_DETECTED				_T("Unable to detect target hard disk. Ensure that the required hard disk drivers are available in the created WinPE media to detect hard disk.")

#define OSD_ERROR_CONFIG_IS_NOT_AVAILABLE_STR				_T("Client configuration data fetching failed")
#define OSD_ERROR_UNABLE_TO_CREATE_CLIENT_CONFIG_STR		_T("Client configuration data processing failed")

#define OSD_ERROR_IN_VOLUME_SHRINK_STR						_T("Error occured while shrinking volume")	
#define OSD_ERROR_IN_VOLUME_EXTEND_STR						_T("Error occured while extending volume")
#define OSD_ERROR_IN_VOLUME_RESIZE_STR						_T("Error occured while resizing volume")	

#define OSD_ERROR_DATAMODEL_PARSE_ERROR_STR				_T("Error occurred while processing server request. Contact Support to resolve this error.") 
#define OSD_ERROR_DATAMODEL_SERIALIZE_ERROR_STR			_T("Error occurred while communicating server. Contact Support to resolve this error.") 

#define  OSD_ERROR_COPY_FAILED_STR						_T("Unable to copy WinPE files. Reinstall the existing WinPE tool or try using another WinPE tool.")
#define  OSD_ERROR_DISM_INITIALIZATION_STR				_T("Unable to initiate WinPE - Dism initialization failed. Restart the system and try again or Reinstall the WinPE tool.")
#define  OSD_ERROR_MOUNT_STR							_T("Unable to mount wim file in the WinPE path. Reinstall the existing WinPE tool or try using another WinPE tool.")
#define  OSD_ERROR_DISM_SESSION_CREATION_STR			_T("Unable to initiate WinPE - Dism session failed. Try again after some time or try using another WinPE tool.")
#define  OSD_ERROR_UNMOUNT_STR							_T("Unable to unmount wim file in the server path. Try again after some time or try using another WinPE tool.")
#define  OSD_ERROR_DLL_FUNC_ADDRESS_INVALID_STR			_T("Unable to initiate WinPE - Corrupted files found in the server path.")
#define  OSD_ERROR_DLL_NOT_FOUND_STR					_T("Unable to initiate WinPE - Dll Failure.  Reinstall the existing WinPE tool or try using another WinPE tool.")
#define  OSD_ERROR_ERROR_IN_FINDING_WINVER_STR			_T("Unable to Find WinPE tool in the given path. Ensure that the WinPE tool is installed properly.")
#define  OSD_ERROR_IN_DRIVE_FORMAT_STR 					_T("Unable to format drive - The drive is either not accessible or corrupt.") 
#define  OSD_ERROR_IN_USB_COPY_STR						_T("Unable to copy WinPE files to the USB drive - Ensure that the USB drive is inserted properly.")

/////////////////////////////////
//Enumerations
/////////////////////////////////

typedef enum tagErrorCode
{
	/*0000*/ OSD_ERROR_SUCCESS          = 0,
	/*0000*/ OSD_ERROR_NONE             = OSD_ERROR_SUCCESS,
	/*0001*/ OSD_ERROR_CALL_FAILURE,
	/*0002*/ OSD_ERROR_INVALID_PARAMETER,
	/*0003*/ OSD_ERROR_NOT_SUPPORTED,
	/*0004*/ OSD_ERROR_MEM_ALLOC_FAILURE,
	/*0005*/ OSD_ERROR_THREAD_CREATION_FAILED,
	/*0006*/ OSD_ERROR_SEMAPHORE,
	/*0007*/ OSD_ERROR_MUTEX,
	/*0008*/ OSD_ERROR_CRITICAL_SECTION,
	/*0009*/ OSD_ERROR_EXCEPTION_CAUGHT,
	/*0010*/ OSD_ERROR_READ_FAILED,
	/*0011*/ OSD_ERROR_WRITE_FAILED,
	/*0012*/ OSD_ERROR_REQ_START_FAILED,
	/*0013*/ OSD_ERROR_REQ_STOP_FAILED,
	/*0014*/ OSD_ERROR_REQ_UNABLE_TO_PROCESS,
	/*0015*/ OSD_ERROR_REQ_NOT_SUPPORTED,
	/*0016*/ OSD_ERROR_REQ_DUPLICATE,
	/*0017*/ OSD_ERROR_REQ_INVALID,
	/*0018*/ OSD_ERROR_DATA_STRUCTURE_FAIL,
	/*0019*/ OSD_ERROR_DATA_STRUCTURE_DUP,
	/*0020*/ OSD_ERROR_INVALID_INPUT,
	/*0021*/ OSD_ERROR_REQ_DISK_SELECTION_INAVLID,
	/*0022*/ OSD_ERROR_DISK_SELECTION_FAILED,
	/*0023*/ OSD_ERROR_DISK_NOT_SELECTED,
	/*0024*/ OSD_ERROR_MULTIPLE_DISK_SELECTED,
	/*0025*/ OSD_ERROR_VOLUME_INVALID,
	/*0026*/ OSD_ERROR_VOLUME_NOT_SELECTED,
	/*0027*/ OSD_ERROR_VOLUME_SELECTION_FAILED,
	/*0028*/ OSD_ERROR_MBR_INVALID,
	/*0029*/ OSD_ERROR_BOOTSECTOR_INVALID,
	/*0030*/ OSD_ERROR_DISK_ENUM_FAILED,
	/*0031*/ OSD_ERROR_NO_DISK_AVAILABLE,
	/*0032*/ OSD_ERROR_DISKIO_INVALID,
	/*0033*/ OSD_ERROR_INVALID_NET_PATH,
	/*0034*/ OSD_ERROR_FILE_ACCESS_ERROR,
	/*0035*/ OSD_ERROR_INVALID_PATH,
	/*0036*/ OSD_ERROR_BITMAP_ERROR,
	/*0037*/ OSD_ERROR_BITMAP_INVALID,
	/*0038*/ OSD_ERROR_BITMAP_CANT_STORE,
	/*0039*/ OSD_ERROR_FILESYSTEM_ERROR,
	/*0040*/ OSD_ERROR_FILESYSTEM_INVALID,
	/*0041*/ OSD_ERROR_HEADER_INVALID,
	/*0042*/ OSD_ERROR_HEADER_ENCODE_FAILED,
	/*0043*/ OSD_ERROR_HEADER_SAVE_FAILED,
	/*0044*/ OSD_ERROR_HEADER_INVLD_POSITION,
	/*0045*/ OSD_ERROR_HEADER_MAP_FAILED,
	/*0046*/ OSD_ERROR_IOBUFFER_FAILED,
	/*0047*/ OSD_ERROR_EXITHANDLER_FAILED,
	/*0048*/ OSD_ERROR_DISK_SIZE_NOT_MATCHING,
	/*0049*/ OSD_ERROR_NO_ENOUGH_SPACE,
	/*0050*/ OSD_ERROR_NET_MODULE_NOT_INITIALIZED,
	/*0051*/ OSD_ERROR_NET_SESSION_INVALID,
	/*0052*/ OSD_ERROR_NET_PROXY_CREDENTIAL_INVALID,
	/*0053*/ OSD_ERROR_NET_CONNECTION_FAILED,
	/*0054*/ OSD_ERROR_NET_CONNECTION_INVALID,
	/*0055*/ OSD_ERROR_NET_CONNECTION_CLOSED,
	/*0056*/ OSD_ERROR_NET_SEND_FAILED,
	/*0056*/ OSD_ERROR_NET_SOCKSHUTDOWN_FAILED,
	/*0057*/ OSD_ERROR_NET_MODULE_INIT_FAILED,

	/*0058*/ OSD_ERROR_NULL_POINTER,
	/*0059*/ OSD_ERROR_JSON_INVALID_KEY,
	/*0060*/ OSD_ERROR_JSON_INVALID_VALUE,
	/*0061*/ OSD_ERROR_JSON_INVALID_FILE,
	/*0062*/ OSD_ERROR_PARSER_CREATION_FAILED,
	/*0063*/ OSD_ERROR_DATA_MODEL_CREATE_FAILED,
	/*0064*/ OSD_ERROR_DATA_MODEL_NULL,
	/*0065*/ OSD_ERROR_DATA_MODEL_CONVERT_FAILED,
	/*0066*/ OSD_ERROR_UNABLE_TO_REG_SIGNAL_HANDLER,
	/*0067*/ OSD_ERROR_INVALID_NET_INTERFACE,
	/*0068*/ OSD_ERROR_AGENT_RES_ID_URL_COSTRC_FAILED,
	/*0069*/ OSD_ERROR_AGENT_RESOURCE_ID_UNAVAILABLE,
	/*0070*/ OSD_ERROR_NET_URL_NOT_AVAILABLE,
	/*0071*/ OSD_ERROR_CONFIG_NOT_INITIALIZE_PROPERLY,
	/*0072*/ OSD_ERROR_SERIALIZER_FAILED,
	/*0073*/ OSD_ERROR_REQ_ALREADY_STOPPED,
	/*0074*/ OSD_ERROR_POINT_OF_CONTACT_EMPTY,
	/*0075*/ OSD_ERROR_UNABLE_TO_FIND_OS_DRIVE,
	/*0077*/ OSD_ERROR_UNABLE_POST_DEPLOY_DISK_ENUM_FAILED,
	/*0078*/ OSD_ERROR_POST_DEPLOYMENT_FAILED,
	/*0079*/ OSD_ERROR_NET_DOWNLOAD_PATH_CREATION_FAILED,
	/*0080*/ OSD_ERROR_POST_DEPLOY_OSDRIVE_NOT_AVAILABLE,
	/*0081*/ OSD_ERROR_COMPRESSION_MODULE_CREATION_FAILED,
	/*0082*/ OSD_ERROR_IN_DECOMPRESSION,
	/*0083*/ OSD_ERROR_IN_COMPRESSION,
	/*0084*/ OSD_ERROR_DECOMPRESSION_SIZE_EXCEEDED,
	/*0085*/ OSD_ERROR_CLUSTER_SEQUENCE_ERROR,
	/*0086*/ OSD_ERROR_NET_RECEIVE_ERROR,
	/*0087*/ OSD_ERROR_NET_MULTICAST_OBJECT_FAILED,
	/*0088*/ OSD_ERROR_NET_MULTICAST_INIT_FAILED,
	/*0089*/ OSD_ERROR_NET_MULTICAST_INIT_SUCC,
	/*0090*/ OSD_ERROR_NET_MULTICAST_ALREADY_INITIALIZED,
	/*0091*/ OSD_ERROR_NET_MULTICAST_GROUP_JOIN_SUCC,
	/*0092*/ OSD_ERROR_NET_MULTICAST_GROUP_JOIN_FAILED,
	/*0093*/ OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_SUCC,
	/*0094*/ OSD_ERROR_NET_MULTICAST_GROUP_LEAVE_FAILED,
	/*0095*/ OSD_ERROR_NET_MULTICAST_INVALID_PACKET,
	/*0096*/ OSD_ERROR_NET_MULTICAST_DATA_NOT_RECEIVED,
	/*0097*/ OSD_ERROR_NET_MULTICAST_READY_TO_RECEIVE,
	/*0098*/ OSD_ERROR_NET_MULTICAST_KILL_FAILED,
	/*0099*/ OSD_ERROR_NET_MULTICAST_KILL_SUCC,
	/*0100*/ OSD_ERROR_UNABLE_TO_DECOMPRESS_INVALID_CONTENT,
	/*0101*/ OSD_ERROR_FAILED_TO_GET_SYSTEMINFO,
	/*0102*/ OSD_ERROR_COPY_FAILED,
	/*0103*/ OSD_ERROR_DISM_INITIALIZATION,
	/*0104*/ OSD_ERROR_MOUNT,
	/*0105*/ OSD_ERROR_DISM_SESSION_CREATION,
	/*0106*/ OSD_ERROR_UNMOUNT,
	/*0107*/ OSD_ERROR_DLL_FUNC_ADDRESS_INVALID,
	/*0108*/ OSD_ERROR_DLL_NOT_FOUND,
	/*0109*/ OSD_ERROR_ERROR_IN_FINDING_WINVER,
	/*0110*/ OSD_ERROR_IN_ADDING_DRIVER,
	/*0111*/ OSD_ERROR_IN_DRIVE_FORMAT,
    /*0112*/ OSD_ERROR_DEST_SIZE_LESS_THAN_SOURCE_SIZE,
	/*0113*/ OSD_ERROR_DEST_SIZE_NOT_ENOUGH_TO_EXPAND,
	/*0114*/ OSD_ERROR_DISK_ADJUSTMENT_FAILED,
	/*0115*/ OSD_ERROR_FAILED_TO_GET_BOOT_TYPE,
	/*0116*/ OSD_ERROR_UNABLE_TO_FIND_BOOT_DRIVE,
	/*0117*/ OSD_ERROR_UNABLE_TO_COPY_BOOT_FILES,
	/*0118*/ OSD_ERROR_UNABLE_TO_ASSIGN_DRIVE_LETTER,
	/*0119*/ OSD_ERROR_HIDDEN_VOLUME_RESIZE_IGNORED,
	/*0120*/ OSD_ERROR_FAILED_TO_GET_REGISTRY_VALUE,
	/*0121*/ OSD_ERROR_OS_NOT_COMPATIBLE,
	/*0122*/ OSD_ERROR_CONFIG_IS_NOT_AVAILABLE,
	/*0123*/ OSD_ERROR_UNABLE_TO_CREATE_CLIENT_CONFIG,
	/*0124*/ OSD_ERROR_INVALID_PROCESS_ID,
	/*0125*/ OSD_ERROR_IN_USB_COPY,
	/*0126*/ OSD_ERROR_AGENT_REINSTALL_FAILED,
	/*0127*/ OSD_ERROR_AGENT_DOWNLOAD_FAILED,
	/*0128*/ OSD_ERROR_IN_VOLUME_SHRINK,
	/*0129*/ OSD_ERROR_IN_VOLUME_EXTEND,
	/*0130*/ OSD_ERROR_IN_VOLUME_RESIZE,
	/*0131*/ OSD_ERROR_IN_DISKPART_COMMANDS_EXC,

	//Do: You need to add above this
	//!!!CAUTION: Don't change this enumeration order
	/*0200*/ OSD_ERROR_SYSTEM_ERROR                = 200,
	/*0201*/ OSD_ERROR_UNKNOWN,

	/*0202*/ OSD_ERROR_DATAMODEL_AGENT_INSTALL_PARSE_ERROR,
	/*0203*/ OSD_ERROR_DATAMODEL_AGENT_INSTALL_SERIALIZE_ERROR,

	/*0204*/ OSD_ERROR_DATAMODEL_AGENT_INSTALL_FAILED_PARSE_ERROR,
	/*0205*/ OSD_ERROR_DATAMODEL_AGENT_INSTALL_FAILED_SERIALIZE_ERROR,

	/*0206*/ OSD_ERROR_DATAMODEL_COMP_DETAILS_PARSE_ERROR,
	/*0207*/ OSD_ERROR_DATAMODEL_COMP_DETAILS_SERIALIZE_ERROR,

	/*0208*/ OSD_ERROR_DATAMODEL_AGENT_RESOURCE_PARSE_ERROR,
	/*0209*/ OSD_ERROR_DATAMODEL_AGENT_RESOURCE_SERIALIZE_ERROR,

	/*0210*/ OSD_ERROR_DATAMODEL_AGENT_REGISTRATION_PARSE_ERROR,
	/*0211*/ OSD_ERROR_DATAMODEL_AGENT_REGISTRATION_SERIALIZE_ERROR,

	/*0212*/ OSD_ERROR_DATAMODEL_NET_PREF_PARSE_ERROR,
	/*0213*/ OSD_ERROR_DATAMODEL_NET_PREF_SERIALIZE_ERROR,

	/*0214*/ OSD_ERROR_DATAMODEL_NET_SETTINGS_PARSE_ERROR,
	/*0215*/ OSD_ERROR_DATAMODEL_NET_SETTINGS_SERIALIZE_ERROR,

	/*0216*/ OSD_ERROR_DATAMODEL_COMP_NAME_REQ_PARSE_ERROR,
	/*0217*/ OSD_ERROR_DATAMODEL_COMP_NAME_REQ_SERIALIZE_ERROR,

	/*0218*/ OSD_ERROR_DATAMODEL_COMP_NAME_RESP_PARSE_ERROR,
	/*0219*/ OSD_ERROR_DATAMODEL_COMP_NAME_RESP_SERIALIZE_ERROR,

	/*0220*/ OSD_ERROR_DATAMODEL_DEPLOYMENT_PARSE_ERROR,
	/*0221*/ OSD_ERROR_DATAMODEL_DEPLOYMENT_SERIALIZE_ERROR,

	/*0222*/ OSD_ERROR_DATAMODEL_DEPLOYMENT_SETTINGS_PARSE_ERROR,
	/*0223*/ OSD_ERROR_DATAMODEL_DEPLOYMENT_SETTINGS_SERIALIZE_ERROR,

	/*0224*/ OSD_ERROR_DATAMODEL_DEPLOYMENT_TEMPLATES_PARSE_ERROR,
	/*0225*/ OSD_ERROR_DATAMODEL_DEPLOYMENT_TEMPLATES_SERIALIZE_ERROR,

	/*0226*/ OSD_ERROR_DATAMODEL_DRIVER_DB_RESP_PARSE_ERROR,
	/*0227*/ OSD_ERROR_DATAMODEL_DRIVER_DB_RESP_SERIALIZE_ERROR,

	/*0228*/ OSD_ERROR_DATAMODEL_DRIVER_DB_STATUS_PARSE_ERROR,
	/*0229*/ OSD_ERROR_DATAMODEL_DRIVER_DB_STATUS_SERIALIZE_ERROR,

	/*0230*/ OSD_ERROR_DATAMODEL_DRIVER_REQ_PARSE_ERROR,
	/*0231*/ OSD_ERROR_DATAMODEL_DRIVER_REQ_SERIALIZE_ERROR,

	/*0232*/ OSD_ERROR_DATAMODEL_DRIVERS_PARSE_ERROR,
	/*0233*/ OSD_ERROR_DATAMODEL_DRIVERS_SERIALIZE_ERROR,

	/*0234*/ OSD_ERROR_DATAMODEL_DRIVERS_SCAN_PARSE_ERROR,
	/*0235*/ OSD_ERROR_DATAMODEL_DRIVERS_SCAN_SERIALIZE_ERROR,

	/*0236*/ OSD_ERROR_DATAMODEL_DRIVERS_SETTINGS_PARSE_ERROR,
	/*0237*/ OSD_ERROR_DATAMODEL_DRIVERS_SETTINGS_SERIALIZE_ERROR,

	/*0238*/ OSD_ERROR_DATAMODEL_DRIVERS_STATUS_PARSE_ERROR,
	/*0239*/ OSD_ERROR_DATAMODEL_DRIVERS_STATUS_SERIALIZE_ERROR,

	/*0240*/ OSD_ERROR_DATAMODEL_ERROR_DM_PARSE_ERROR,
	/*0241*/ OSD_ERROR_DATAMODEL_ERROR_DM_SERIALIZE_ERROR,

	/*0242*/ OSD_ERROR_DATAMODEL_ERROR_REPORT_PARSE_ERROR,
	/*0243*/ OSD_ERROR_DATAMODEL_ERROR_REPORT_SERIALIZE_ERROR,

	/*0244*/ OSD_ERROR_DATAMODEL_IMAGE_CREATION_PARSE_ERROR,
	/*0245*/ OSD_ERROR_DATAMODEL_IMAGE_CREATION_SERIALIZE_ERROR,

	/*0246*/ OSD_ERROR_DATAMODEL_IMAGE_COM_DETAILS_PARSE_ERROR,
	/*0247*/ OSD_ERROR_DATAMODEL_IMAGE_COM_DETAILS_SERIALIZE_ERROR,

	/*0248*/ OSD_ERROR_DATAMODEL_IMAGE_SETTINGS_PARSE_ERROR,
	/*0249*/ OSD_ERROR_DATAMODEL_IMAGE_SETTINGS_SERIALIZE_ERROR,

	/*0250*/ OSD_ERROR_DATAMODEL_IMAGE_STATUS_PARSE_ERROR,
	/*0251*/ OSD_ERROR_DATAMODEL_IMAGE_STATUS_SERIALIZE_ERROR,

	/*0252*/ OSD_ERROR_DATAMODEL_PROGRESS_PARSE_ERROR,
	/*0253*/ OSD_ERROR_DATAMODEL_PROGRESS_STATUS_SERIALIZE_ERROR,

	/*0254*/ OSD_ERROR_DATAMODEL_LICENSE_VALIDATION_PARSE_ERROR,
	/*0255*/ OSD_ERROR_DATAMODEL_LICENSE_VALIDATION_SERIALIZE_ERROR,

	/*0256*/ OSD_ERROR_DATAMODEL_MACHINE_SETTINGS_PARSE_ERROR,
	/*0257*/ OSD_ERROR_DATAMODEL_MACHINE_SETTINGS_SERIALIZE_ERROR,

	/*0258*/ OSD_ERROR_DATAMODEL_OSD_REQ_PARSE_ERROR,
	/*0259*/ OSD_ERROR_DATAMODEL_OSD_REQ_SERIALIZE_ERROR,

	/*0260*/ OSD_ERROR_DATAMODEL_PART_DETAILS_PARSE_ERROR,
	/*0261*/ OSD_ERROR_DATAMODEL_PART_DETAILS_SERIALIZE_ERROR,

	/*0262*/ OSD_ERROR_DATAMODEL_PATCHER_STATUS_PARSE_ERROR,
	/*0263*/ OSD_ERROR_DATAMODEL_PATCHER_STATUS_SERIALIZE_ERROR,

	/*0264*/ OSD_ERROR_DATAMODEL_PE_DRVIER_PARSE_ERROR,
	/*0265*/ OSD_ERROR_DATAMODEL_PE_DRIVER_SERIALIZE_ERROR,

	/*0266*/ OSD_ERROR_DATAMODEL_PE_SHARE_PARSE_ERROR,
	/*0267*/ OSD_ERROR_DATAMODEL_PE_SHARE_SERIALIZE_ERROR,

	/*0268*/ OSD_ERROR_DATAMODEL_PE_TOOL_DETAILS_PARSE_ERROR,
	/*0269*/ OSD_ERROR_DATAMODEL_PE_TOOL_DETAILS_SERIALIZE_ERROR,

	/*0270*/ OSD_ERROR_DATAMODEL_POST_DEPLOY_PARSE_ERROR,
	/*0271*/ OSD_ERROR_DATAMODEL_POST_DEPLOY_SERIALIZE_ERROR,

	/*0272*/ OSD_ERROR_DATAMODEL_SECURITY_SETTINGS_PARSE_ERROR,
	/*0273*/ OSD_ERROR_DATAMODEL_SECURITY_SETTINGS_SERIALIZE_ERROR,

	/*0274*/ OSD_ERROR_DATAMODEL_SERVER_INFO_PARSE_ERROR,
	/*0275*/ OSD_ERROR_DATAMODEL_SERVER_INFO_SERIALIZE_ERROR,

	/*0276*/ OSD_ERROR_DATAMODEL_SERVER_REQ_PARSE_ERROR,
	/*0277*/ OSD_ERROR_DATAMODEL_SERVER_REQ_SERIALIZE_ERROR,

	/*0278*/ OSD_ERROR_DATAMODEL_SHARE_REPO_PARSE_ERROR,
	/*0279*/ OSD_ERROR_DATAMODEL_SHARE_REPO_SERIALIZE_ERROR,

	/*0280*/ OSD_ERROR_DATAMODEL_SW_DEPLOY_PARSE_ERROR,
	/*0281*/ OSD_ERROR_DATAMODEL_SW_DEPLOY_SERIALIZE_ERROR,

	/*0282*/ OSD_ERROR_DATAMODEL_TARGET_DISK_SETTINGS_PARSE_ERROR,
	/*0283*/ OSD_ERROR_DATAMODEL_TARGET_DISK_SETTINGS_SERIALIZE_ERROR,

	/*0284*/ OSD_ERROR_DATAMODEL_USER_ACCOUNTS_PARSE_ERROR,
	/*0285*/ OSD_ERROR_DATAMODEL_USER_ACCOUNTS_SERIALIZE_ERROR,

	/*0286*/ OSD_ERROR_DATAMODEL_USER_ACCOUNT_SETTINGS_PARSE_ERROR,
	/*0287*/ OSD_ERROR_DATAMODEL_USER_ACCOUNT_SETTINGS_SERIALIZE_ERROR,

	/*0288*/ OSD_ERROR_DATAMODEL_VIEW_LOG_PARSE_ERROR,
	/*0289*/ OSD_ERROR_DATAMODEL_VIEW_LOG_SERIALIZE_ERROR,

	/*0290*/ OSD_ERROR_DATAMODEL_STORAGE_DETAILS_PARSE_ERROR,
	/*0291*/ OSD_ERROR_DATAMODEL_STORAGE_DETAILS_SERIALIZE_ERROR,

	//Dont add here

}OSD_ERROR_CODE_ENUM;

typedef enum tagSystemErrorCode
{
	/*0000*/ OSD_SYSERROR_SUCCESS            = 0,
	/*0000*/ OSD_SYSERROR_NONE               = OSD_SYSERROR_SUCCESS,
	/*0001*/ OSD_SYSERROR_INVALID_FUNCTION,
	/*0002*/ OSD_SYSERROR_FILE_NOT_FOUND,
	/*0003*/ OSD_SYSERROR_PATH_NOT_FOUND,
	/*0004*/ OSD_SYSERROR_TOO_MANY_OPEN_FILES,
	/*0005*/ OSD_SYSERROR_ACCESS_DENIED,
	/*0006*/ OSD_SYSERROR_INVALID_IO_HANDLE,
	/*0007*/ OSD_SYSERROR_INVALID_STORAGE,
	/*0008*/ OSD_SYSERROR_NOT_ENOUGH_MEMORY,
	/*0009*/ OSD_SYSERROR_BAD_OS_ENVIRONMENT,
	/*0010*/ OSD_SYSERROR_INVALID_FILE_FORMAT,
	/*0011*/ OSD_SYSERROR_INVALID_ACCESS,
	/*0012*/ OSD_SYSERROR_INVALID_DATA,
	/*0013*/ OSD_SYSERROR_OUTOFMEMORY,
	/*0014*/ OSD_SYSERROR_INVALID_DRIVE,
	/*0015*/ OSD_SYSERROR_CURRENT_DIRECTORY,
	/*0016*/ OSD_SYSERROR_NOT_SAME_DEVICE,
	/*0017*/ OSD_SYSERROR_NO_MORE_FILES,
	/*0018*/ OSD_SYSERROR_WRITE_PROTECT,
	/*0019*/ OSD_SYSERROR_NO_SUCH_DEVICE,
	/*0020*/ OSD_SYSERROR_DEVICE_NOT_READY,
	/*0021*/ OSD_SYSERROR_INVALID_IO_CMD,
	/*0022*/ OSD_SYSERROR_CRC,
	/*0023*/ OSD_SYSERROR_INVALID_SEEK,
	/*0024*/ OSD_SYSERROR_DISK_NOT_FOUND,
	/*0025*/ OSD_SYSERROR_SECTOR_NOT_FOUND,
	/*0026*/ OSD_SYSERROR_WRITE_FAULT_OCCURED,
	/*0027*/ OSD_SYSERROR_READ_FAULT_OCCURED,
	/*0028*/ OSD_SYSERROR_DEVICE_NOT_FUNCTIONING,
	/*0029*/ OSD_SYSERROR_SHARING_VIOLATION,
	/*0030*/ OSD_SYSERROR_LOCK_VIOLATION,
	/*0031*/ OSD_SYSERROR_WRONG_DISK,
	/*0032*/ OSD_SYSERROR_SHARING_OPERATION,
	/*0033*/ OSD_SYSERROR_HANDLE_EOF,
	/*0034*/ OSD_SYSERROR_DISK_FULL,
	/*0035*/ OSD_SYSERROR_REQ_NOT_SUPPORTED,
	/*0036*/ OSD_SYSERROR_CANT_FIND_NETWORK,
	/*0037*/ OSD_SYSERROR_DUP_NETWORK_NAME,
	/*0038*/ OSD_SYSERROR_INVALID_NETWORK_PATH,
	/*0039*/ OSD_SYSERROR_NETWORK_BUSY,
	/*0040*/ OSD_SYSERROR_NETWORK_RES_NOT_EXIST,
	/*0041*/ OSD_SYSERROR_NETWORK_CMD_ERROR,
	/*0042*/ OSD_SYSERROR_NETWORK_HW_ERROR,
	/*0043*/ OSD_SYSERROR_NETWORK_SERVER_ERROR,
	/*0044*/ OSD_SYSERROR_NETWORK_UNKNOWN_ERROR,
	/*0045*/ OSD_SYSERROR_REMOTE_ERROR,
	/*0046*/ OSD_SYSERROR_NETWORK_NAME_DELETED,
	/*0047*/ OSD_SYSERROR_NETWORK_ACCESS_DENIED,
	/*0048*/ OSD_SYSERROR_BAD_NETWORK_DEVICE,
	/*0049*/ OSD_SYSERROR_BAD_NETWORK_NAME,
	/*0050*/ OSD_SYSERROR_TOO_MANY_NETWORK_ADAPTER,
	/*0051*/ OSD_SYSERROR_NETWORK_OPERATION_INVALID,
	/*0052*/ OSD_SYSERROR_SHARING_PAUSED,
	/*0053*/ OSD_SYSERROR_TOO_MANY_CONNECTIONS,
	/*0054*/ OSD_SYSERROR_IO_OPERATION_PAUSED,
	/*0055*/ OSD_SYSERROR_FILE_EXISTS,
	/*0056*/ OSD_SYSERROR_CANT_CREATE_DIR_OR_FILE,
	/*0057*/ OSD_SYSERROR_ALREADY_ASSIGNED,
	/*0058*/ OSD_SYSERROR_INVALID_PASSWORD,
	/*0059*/ OSD_SYSERROR_INVALID_PARAMETER,
	/*0060*/ OSD_SYSERROR_NET_WRITE_FAULT,
	/*0061*/ OSD_SYSERROR_UNABLE_TO_START_PROCESS,
	/*0062*/ OSD_SYSERROR_DISK_CHANGE,
	/*0063*/ OSD_SYSERROR_DRIVE_LOCKED,
	/*0064*/ OSD_SYSERROR_BROKEN_PIPE,
	/*0065*/ OSD_SYSERROR_OPEN_FAILED,
	/*0066*/ OSD_SYSERROR_BUFFER_OVERFLOW,
	/*0067*/ OSD_SYSERROR_INVALID_NAME,
	/*0068*/ OSD_SYSERROR_UNKNOWN

}OSD_SYSERROR_CODE_ENUM;

typedef enum tagInternalModules
{
	/*0000*/ OSD_MODULE_UNKNOWN      = 0,
	/*0001*/ OSD_MODULE_WRAPPER,
	/*0002*/ OSD_MODULE_DISKMANAGER,
	/*0003*/ OSD_MODULE_DISKIO,
	/*0004*/ OSD_MODULE_DEVICEIO,
	/*0005*/ OSD_MODULE_NETSHARE,
	/*0006*/ OSD_MODULE_REGISTRY,
	/*0007*/ OSD_MODULE_MBR,
	/*0008*/ OSD_MODULE_GPT,
	/*0009*/ OSD_MODULE_BOOTSECTOR,
	/*0010*/ OSD_MODULE_VOLUME,
	/*0011*/ OSD_MODULE_FILESYSTEM,
	/*0012*/ OSD_MODULE_FILEACCESS,
	/*0013*/ OSD_MODULE_CHECKLIST,
	/*0014*/ OSD_MODULE_HEADER,
	/*0015*/ OSD_MODULE_CONSOLE,
	/*0016*/ OSD_MODULE_RESPONSE_HANDLELR,
	/*0017*/ OSD_MODULE_NET,
	/*0018*/ OSD_MODULE_CONFIG,

    /*0019*/ OSD_MODULE_IMAGE_CREATION,
	/*0020*/ OSD_MODULE_LIBJSON,
	/*0021*/ OSD_MODULE_COMPUTER_DETAILS,
	/*0022*/ OSD_MODULE_AGENT_INSTALLATION_FAILED,
	/*0023*/ OSD_MODULE_AGENT_INSTALLATION_SUCCESS,
	/*0024*/ OSD_MODULE_COLLECT_COMPUTER_DETAILS,
	/*0025*/ OSD_MODULE_FETCH_COMPUTER_DETAILS_SUCCESS,
	/*0026*/ OSD_MODULE_IMAGE_CREATION_BLOCKED,
	/*0027*/ OSD_MODULE_IMAGE_CREATION_FAILED,
	/*0028*/ OSD_MODULE_IMAGE_CREATION_PROGRESS,
	/*0029*/ OSD_MODULE_PARTITION_DETAILS_FAILED,	
	/*0030*/ OSD_MODULE_SERVER_MANAGER,
	/*0031*/ OSD_MODULE_AGENT_RESOURCE_STATE_DM,
	/*0032*/ OSD_MODULE_STORAGE_DETAILS,
	/*0033*/ OSD_MODULE_COMMON_DETAILS,
	/*0034*/ OSD_MODULE_ERROR_REPORT,
	/*0035*/ OSD_MODULE_PEBUILDER,
	/*0036*/ OSD_MODULE_OSD_REQUEST_DM,
	/*0037*/ OSD_MODULE_NETWORK_SETTINGS_DM,
	/*0038*/ OSD_MODULE_MACHINE_SETTINGS_DM,
	/*0039*/ OSD_MODULE_USER_ACCOUNTS_DM,
	/*0040*/ OSD_MODULE_POST_DEPLOY_SETTINGS_DM,
	/*0041*/ OSD_MODULE_DRIVER_SETTINGS_DM,
	/*0042*/ OSD_MODULE_NETPREFERENCE_SETTINGS_DM,
	/*0043*/ OSD_MODULE_SECURITY_ATTR_SETTINGS_DM,
	/*0044*/ OSD_MODULE_IMAGE_SETTINGS_DM,
	/*0045*/ OSD_MODULE_DEPLOYMENT_DM,
	/*0046*/ OSD_MODULE_DEPLOYMENT_TEMPLATES_DM,
	/*0047*/ OSD_MODULE_DEPLOYMENT_SETTINGS_DM,
	/*0048*/ OSD_MODULE_TARGET_DISK_SETTINGS_DM,
	/*0048*/ OSD_MODULE_USERACCOUNT_SETTINGS_DM,
	/*0049*/ OSD_MODULE_CONNECTION_MANAGER_PATCHER,
	/*0050*/ OSD_MODULE_DRIVER_STATUS_DM,
	/*0051*/ OSD_MODULE_PATCHER_STATUS_DM,
	/*0052*/ OSD_MODULE_SOFTWARE_DEPLOYMENT_DM,
	/*0053*/ OSD_MODULE_DEVICEINSTALL_PCH,
	/*0054*/ OSD_MODULE_AGENT_INSTALLER,
	/*0055*/ OSD_MODULE_VIEW_LOG_STATUS,
	/*0056*/ OSD_MODULE_MULTICAST,
	/*0057*/ OSD_MODULE_ZMQ,
	/*0058*/ OSD_MODULE_DRIVER_SCAN_DETAILS_DM,
	/*0059*/ OSD_MODULE_DRIVER_REQUEST_DM,
	/*0060*/ OSD_MODULE_INF_PARSER,
	/*0061*/ OSD_MODULE_SERVER_DB_RESPONSE_DM,
	/*0062*/ OSD_MODULE_SERVER_DB_REPLY,
	/*0063*/ OSD_MODULE_LICENSE_VALIDATION_DM,
	/*0064*/ OSD_MODULE_COPYDIR,
	/*0065*/ OSD_MODULE_PXE_COPY,
	/*0066*/ OSD_MODULE_DISM_PROCESS,
	/*0067*/ OSD_MODULE_WINAPI_PROCESS,
	/*0068*/ OSD_MODULE_MEDIA_CREATION,
	/*0069*/ OSD_MODULE_USB_MEDIA,
	/*0070*/ OSD_MODULE_PXE_MEDIA,
	/*0071*/ OSD_MODULE_ISO_MEDIA,
	/*0072*/ OSD_MODULE_PE_WRAPPER,
	/*0073*/ OSD_MODULE_SERVER_INFO_DM,
	/*0074*/ OSD_MODULE_IMAGE_HEADER_DM,
	/*0075*/ OSD_MODULE_POST_DEPLOYMENT,
	/*0076*/ OSD_MODULE_DEVICE_INSTALL,
	/*0077*/ OSD_MODULE_SERVER_REQUEST,
	/*0078*/ OSD_MODULE_COMPUTER_NAME_REQ,
	/*0079*/ OSD_MODULE_COMPUTER_NAME_RES,
	/*0080*/ OSD_MODULE_IMAGED_COMPUTER_DETAILS_DM,
	/*0081*/ OSD_MODULE_COMPUTER_CONFIG,
	/*0082*/ OSD_MODULE_WINPE_DETAILS_DM,
	/*0083*/ OSD_MODULE_INSTALLER_SETUP

}OSD_INTERNAL_MODULE_ENUM;

typedef enum tagExternalModules
{
	/*0000*/ OSD_EXTMODULE_UNKNOWN      = 0,
	/*0000*/ OSD_EXTMODULE_DEVICEIO,
	/*0001*/ OSD_EXTMODULE_WMI,
	/*0002*/ OSD_EXTMODULE_REGISTRY,
	/*0003*/ OSD_EXTMODULE_NETSHARE,
	/*0004*/ OSD_EXTMODULE_FILEACCESS,
	/*0005*/ OSD_EXTMODULE_MEMORY,
	/*0006*/ OSD_EXTMODULE_NET,
	/*0007*/ OSD_EXTMODULE_THREAD,
	/*0008*/ OSD_EXTMODULE_JSON,
	/*0009*/ OSD_EXTMODULE_PEBUILDER,
	/*0010*/ OSD_EXTMODULE_COMPUTERCONFIG

}OSD_EXTERNAL_MODULE_ENUM;

/////////////////////////////////
//Structure declaration
/////////////////////////////////

typedef struct tagResultStruct
{
	tagResultStruct()
	{
		RetCode        = OSD_ERROR_CALL_FAILURE;
		IntModule      = OSD_MODULE_UNKNOWN;
		IntLineNum     = 0;
		SystemRetCode  = 0;
		ExtModule      = OSD_EXTMODULE_UNKNOWN;
		ExtLineNum     = 0;
		FileAccessType = -1;
		ResString.Empty();
		PostString.Empty();
	}
	void resetAll()
	{
		RetCode        = OSD_ERROR_CALL_FAILURE;
		IntModule      = OSD_MODULE_UNKNOWN;
		IntLineNum     = 0;
		SystemRetCode  = 0;
		ExtModule      = OSD_EXTMODULE_UNKNOWN;
		ExtLineNum     = 0;
		FileAccessType = -1;
		ResString.Empty();
		PostString.Empty();
	}

	OSD_ERROR_CODE_ENUM           RetCode;
	OSD_INTERNAL_MODULE_ENUM      IntModule;
	DWORD                         IntLineNum;

	LONGLONG                      SystemRetCode;
	OSD_EXTERNAL_MODULE_ENUM      ExtModule;
	DWORD                         ExtLineNum;
	INT							  FileAccessType;
	CAtlString ResString;
	CAtlString PostString;

}OSD_RESULT,*POSD_RESULT;

typedef struct fileHeader {
	DWORD counter = 0;
	DWORD CRC = 0;
	LONGLONG payLoad = 0;
	//CString fragmentName;
}FILE_HEADER,*PFILE_HEADER;
/////////////////////////////////
//Class declaration
/////////////////////////////////

class CErrorCode
{
public:
	static INT m_ApplType;
	static OSD_ERROR_CODE_ENUM ConvertToIntnlErr(LONGLONG i_ExtError);
	static CAtlString ConvertToUsrMsg(OSD_ERROR_CODE_ENUM i_RetCode);
	static CAtlString ConvertToUsrMsg(LONGLONG   i_SystemRetCode);
};

CAtlString GetLastErrorAsString();
CAtlString GetLastErrorAsString(DWORD &errorMessageID);

}/*OS_Manager*/

#endif /*__ERROR_CODE_H__*/