/*
 * Common data types
 * 
 * Copyright(C) 
 */

#pragma once
#ifndef __DATA_TYPES__
#define __DATA_TYPES__

////////////////////////////////////////
//Own datatype conversion
////////////////////////////////////////
/*
typedef char				OSD_CHAR;
typedef unsigned char       BYTE;
typedef int					OSD_INT;
typedef unsigned int		OSD_UINT;
typedef int					WORD;
typedef unsigned int		OSD_UWORD;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned short      OSD_SWORD;
typedef float               OSD_FLOAT;

typedef long int			OSD_LINT,OSD_LWORD;
typedef unsigned long		OSD_LUINT,OSD_LUWORD;

typedef char				*OSD_PCHAR;
typedef int					*OSD_PINT;
typedef unsigned int		*OSD_PUINT;
typedef void				OSD_VOID;
typedef void				*OSD_PVOID;
typedef unsigned int        *PUINT;

typedef long int			*OSD_PLINT,*OSD_PLWORD;
typedef unsigned long int	*OSD_PULINT,*OSD_PULWORD;

typedef long long			OSD_LLWORD,OSD_LLINT;
typedef unsigned long long  OSD_ULLWORD,ULONGLONG;

typedef OSD_PVOID           HANDLE;

*/

////////////////////////////////////////
//Utilites for internal usage
////////////////////////////////////////

#if defined(_WIN32) || defined (WIN32) || defined (_WINDOWS)
#define OSD_CTRL_C_EVENT			CTRL_C_EVENT
#define OSD_CTRL_BREAK_EVENT		CTRL_BREAK_EVENT
#define OSD_CTRL_CLOSE_EVENT		CTRL_CLOSE_EVENT
#define OSD_CTRL_LOGOFF_EVENT		CTRL_LOGOFF_EVENT
#define OSD_SHUTDOWN_EVENT			CTRL_SHUTDOWN_EVENT
#else
//Otherwise
#endif

#define ISNULL(__ptr__)    (NULL == __ptr__)
#define ISNOTNULL(__ptr__) (NULL != __ptr__)
#define OSD_UNUSED(__VAR__)    ((VOID)(__VAR__))

#define SECURE_DELETE(__X__)    if(ISNOTNULL(__X__)){\
	                                delete __X__;\
                                    __X__ = NULL;\
								}

#define SECURE_FREE(__X__)    if(ISNOTNULL(__X__)){\
	                                free(__X__);\
                                    __X__ = NULL;\
								}

#define SECURE_ARRAY_DELETE(__X__)    if(ISNOTNULL(__X__)){\
	                                delete []__X__;\
                                    __X__ = NULL;\
								}

/*
//It has platform dependency
#define SECURE_CLOSE_HANDLE(__X__)    if(INVALID_HANDLE_VALUE == __X__){\
	                                CloseHandle(__X__);\
                                    __X__ = INVALID_HANDLE_VALUE;\
								}
*/

#define OSD_TRUE            1
#define OSD_FALSE			0


//This must be converted as per the platform API
typedef enum tagInternalFilePosition
{
	OSD_FILE_BEGIN          = 0,
	OSD_FILE_CURRENT        = 1,
	OSD_FILE_END            = 2
}InternalFilePosition_Enum;

#endif /*__DATA_TYPES__*/