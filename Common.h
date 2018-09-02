/*
 * Common.h 
 * 
 * Copyright(C) 
 */

#ifndef __COMMON_H__
#define __COMMON_H__

/////////////////////////////////////////
// default values
////////////////////////////////////////

#define DRIVE_LETTER_MAX_SIZE   8
#define MAXIMUM_VOLUME_COUNT    128

//#define MAX_IO_SIZE             (100*1024*1024) //(400*1024*1024)
//
//#define MAX_CLUSTER_SEQUENCE     (ULONGLONG(MAX_IO_SIZE / 4096)) //102400   //76800 // //(25600)
//
//#define OSD_BUFFER_SIZE				MAX_IO_SIZE

//Added for user input based memory usage
#define IO_BUFFER_SIZE_LOW			(50*1024*1024)
#define IO_BUFFER_SIZE_MEDIUM		(100*1024*1024)
#define IO_BUFFER_SIZE_HIGH			(400*1024*1024)
#define MULTICAST_MAX_BUFFER_SIZE	IO_BUFFER_SIZE_HIGH

#define IO_SYNCHRONOUS    0
#define IO_ASYNCHRONOUS   1
#define IO_MODE           IO_SYNCHRONOUS

#define OSD_ENABLE_INTERNAL_ERROR_MSG

#define OSMANAGER_APPLICATION_CONSOLE	0
#define OSMANAGER_APPLICATION_GUI		1
#define OSMANAGER_APPLICATION_NO_GUI    2  
#define OSMANAGER_APPLICATION			OSMANAGER_APPLICATION_CONSOLE


/*Currently disabled to support all volumes even there is no drive letter*/
//#define OSD_VOLUME_DRIVE_BASED_CREATION

/*#define OSD_DISKENUM_WITH_CLUSTER_SIZE*/

//Size units & its bytes
#define OSD_KB_SIZE					(1024)
#define OSD_MB_SIZE					(1024 * OSD_KB_SIZE)
#define OSD_GB_SIZE					(1024 * OSD_MB_SIZE)
/*Note: To avoid compilation warning TB value is hard-coded: "Warning: integral constant overflow"*/
#define OSD_TB_SIZE					(1099511627776) //(1024 * (OSD_GB_SIZE ))
#define OSD_BYTE_STRING_SIZE		256


#define OSD_CONVERT_BYTES_TO_MB(__BYTES_DATA__) (ULONGLONG(ULONGLONG(__BYTES_DATA__) / ULONGLONG(OSD_MB_SIZE)))
#define OSD_CONVERT_BYTES_TO_GB(__BYTES_DATA__) (ULONGLONG(ULONGLONG(__BYTES_DATA__) / ULONGLONG(OSD_GB_SIZE)))

/////////////////////////////////////////
// File Access Enumerations
////////////////////////////////////////

typedef enum tagFileAccessType
{
	/*-1*/ FILE_ACCESS_DEFAULT = -1,
	/*0*/ FILE_ACCESS_SRC,
	/*1*/ FILE_ACCESS_DEST,
	///*2*/ FILE_ACCESS_HEADER

}FILE_ACCESS_TYPE_ENUM;

typedef enum tagFileFlagEnum
{
    DISKIO_FILE_READ_ONLY,
	DISKIO_CREATE_ALWAYS,
	DISKIO_OPEN_EXISTING

}OS_MANAGER_FILE_FLAG_ENUM;

typedef enum tagIOModeEnum
{
    DISKIO_SYNCHRONOUS   = 0,
	DISKIO_ASYNCHRONOUS

}OS_MANAGER_IO_MODE_ENUM;

typedef enum tagOSDApplicationType
{
	OSD_APPLICATION_COSOLE,
	OSD_APPLICATION_GUI,
	OSD_APPLICATION_NO_GUI,

}OSD_APPLICATION_TYPE_ENUM;

/////////////////////////////////////////
// Structure &Enumerations
////////////////////////////////////////



#endif /*__COMMON_H__*/