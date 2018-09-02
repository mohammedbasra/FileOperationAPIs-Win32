/*
 * File Access.cpp
 * 
 * Copyright(C) 
 */
#include "stdafx.h"
#include "../OSManagerSplitter/stdafx.h"
#include "../../OSManagerSplitter/include/FileAccess.h"
#include "../../OSManagerSplitter/include/Local_FileAccess.h"
#include<Windows.h>
#include<stdio.h>

namespace OS_Manager
{
/////////////////////////////////
//Global Variables
/////////////////////////////////
static const CString gpCFileName = _T("Local_FileAccess.cpp");

CDiskIO* CDiskIO::m_oDiskIO = NULL;

/////////////////////////////////
//FileAccess class implementation
/////////////////////////////////

IFileAccess :: IFileAccess(FILE_DATA_SOURCE_ENUM i_DataSource)
{
	this->m_DataSource = i_DataSource;
	m_PollState        = TRUE;
	m_ExtDataHandler   = NULL;
}

IFileAccess::~IFileAccess()
{
	m_PollState        = FALSE;
}

IFileAccess* IFileAccess::CreateInstance(FILE_DATA_SOURCE_ENUM i_DataSource)
{
    try
	{
	    switch(i_DataSource)
		{
		  case FILE_ACCESS_LOCAL:
		  case FILE_ACCESS_NETWORK_DRIVE:
		  case FILE_ACCESS_DATA_BUFFER:
		  {
			 return CLocal_FileAccess::CreateInstance(i_DataSource);
		  }  
		  default:
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

BOOL IFileAccess::CalculateSize(DWORD i_Offset, LARGE_INTEGER &o_Size, DWORD i_SectorSize)
{
	try
	{
	    o_Size.QuadPart = (i_Offset * i_SectorSize);

		return TRUE;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}

	return FALSE;
}

OSD_RESULT IFileAccess::StopPendingFileAccess()
{
	OSD_RESULT Result;
	try
	{
		m_PollState  = FALSE;

		//Result = StopPendingRequest(); 

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
	OSD_SET_INTERNAL_ERR(Result,OSD_MODULE_FILEACCESS,OSD_ERROR_EXCEPTION_CAUGHT);

	return Result;
}
} /*OS_Manager*/

 
