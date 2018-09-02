
#include "stdafx.h"
#include "Processutil.h"

//Dependency for checking IsProcessRunning() function
#include <tlhelp32.h>

namespace OSD_Util
{

CProcess::CProcess(void)
{
	gv_ShutdownInProgress = FALSE;
}

CProcess::~CProcess(void)
{
}

HANDLE CProcess::getHandle()
{
	return m_hProcess;
}

DWORD CProcess::getProcessID()
{
	return m_ProcessId;	
}

DWORD CProcess::executeFile(CString path, CString executeFileName, CString &strRemarks,int timeout, BOOL showUI,DWORD dwnewprocessGroup /*=0*/)
{
	PROCESS_INFORMATION pinfo;
	STARTUPINFO  info;
	int ret;
	//int retVal = FAILURE;
	LPTSTR currentDirectory = NULL;
	DWORD errorCode = ERROR_SUCCESS;

	 INFOLOG(L"@@@@@@@@ Inside execute Method @@@@@@@@ \n");

	 if(timeout != INFINITE)
	 {
		 timeout = timeout*1000*60;
	 }

	if ( gv_ShutdownInProgress )
	{
		INFOLOG(L"execute : Shutdown is in Progress. So returning without executing the process %s .\n", executeFileName);
		return ERROR_RETRY;
	}

	if (path)
	{
		currentDirectory = (LPTSTR)calloc(sizeof(TCHAR),BUFSIZE);
		errorCode = GetCurrentDirectory(BUFSIZE, currentDirectory);

		if( errorCode == 0 )
		{
			errorCode = GetLastError();
			INFOLOG(L"execute: Failed to get the CurrentWorkingDirectory \n");
			printErrorMsgForID(errorCode);
			return errorCode;
		}

		if(errorCode > BUFSIZE)
		{
			INFOLOG(L"execute : CurrentWorkingDirectory Buffer overflow. Need to allocate the length for %d chars \n", errorCode );
			strRemarks= WriteErrorMsgForID(errorCode);
			return errorCode;
		}

		if ( !executeFileName.IsEmpty())
		{

			if(!SetCurrentDirectory(path))
			{
				errorCode = GetLastError();
				INFOLOG(L"execute : Failed to set the WorkingDirectory %s \n ", path.GetBuffer() );
				printErrorMsgForID(errorCode);
				free(currentDirectory);
				strRemarks= WriteErrorMsgForID(errorCode);
				return errorCode;
			}
		}
		else
		{
			strRemarks = WriteErrorMsgForID(errorCode);
			INFOLOG(L"execute : failed to FileName Empty  - %d\n", errorCode );
			return errorCode;
		}
	}

	memset(&pinfo,0,sizeof(pinfo));
	memset(&info,0,sizeof(info));
	info.cb = sizeof(STARTUPINFO);

	if(showUI)
	{
		info.dwFlags      =   STARTF_USESHOWWINDOW;
		info.wShowWindow  =   SW_SHOWNORMAL;
	}

	if(dwnewprocessGroup != 0)
	{
		dwnewprocessGroup |= CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW;
	}
	else
	{
		dwnewprocessGroup = CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW;
	}

	ret = CreateProcess (NULL,
			executeFileName.GetBuffer(),
			NULL,
			NULL,
			FALSE,
			dwnewprocessGroup,
			NULL,
			path,
			&info,
			&pinfo);
	if(!ret) 
	{
		errorCode = GetLastError();
		INFOLOG(L"execute : Failed to Create the Process for the application \n");
		strRemarks = WriteErrorMsgForID(errorCode);
	}
	else
	{
		INFOLOG(L"execute : Process successfully created for application %s \n", executeFileName);
		m_hProcess = pinfo.hProcess;
		m_ProcessId = pinfo.dwProcessId;	
		if (timeout == 0)
		{
			errorCode = ERROR_SUCCESS;
			strRemarks = WriteErrorMsgForID(errorCode);
			goto CleanUp;
		}
		ret = WaitForSingleObject(pinfo.hProcess,timeout);
		if(ret == WAIT_FAILED)
		{
			DWORD hr = GetLastError();
			INFOLOG(L"execute : WaitForSingleObject Failed with error code %d \n", hr);
		}
		else if(ret != WAIT_OBJECT_0) 
		{
			errorCode = WAIT_TIMEOUT;
			INFOLOG(L"execute : Terminating the Process for application %s as time is exceeded %d Minutes \n", executeFileName, timeout);
			strRemarks = WriteErrorMsgForID(errorCode);
			if(TerminateProcess (pinfo.hProcess, 0))
			{
				INFOLOG(L"execute : The process has been successfully terminated ..! \n");
			}
			else
			{
				INFOLOG(L"execute : The process termination is failed with error code : %d \n",GetLastError());
			}
		}
		else
		{
			if (!GetExitCodeProcess (pinfo.hProcess, &errorCode))
			{
				errorCode = GetLastError();
				INFOLOG(L"execute : Unable to find the exit code for the application and the error code is : %d \n", errorCode);
				strRemarks = WriteErrorMsgForID(errorCode);
			}
			else
			{
				strRemarks = WriteErrorMsgForID(errorCode);
				INFOLOG(L"execute : Exit code for the application : %d \n", errorCode);
			}

		}
	}
CleanUp:
	CloseHandle(pinfo.hThread);
	CloseHandle(pinfo.hProcess);
	if ( path && currentDirectory != NULL )
	{
		SetCurrentDirectory(currentDirectory);
		free(currentDirectory);
	}
#ifndef MASTER_AGENT	
	INFOLOG(L"execute : Remarks from the CreateProcess is %s for the executable %s \n",strRemarks,executeFileName); 
#endif
	INFOLOG(L"@@@@@@@@ End Of execute Method @@@@@@@@ \n");
	return errorCode;

}
/****************************************************************************************************************
Method Name : execute
Purpose     : to execute an executable using createProcess .
Input       : 1. path			 --> The root directory of the executable
			  2. executeFileName --> filename to execute 
			  3. remarks         --> error message of this method
			  4. timeout         --> time to wait for single object 
			  5. showUI    	 --> if GUI is needed then true(for script configuration) else false
Return		: errocode of the method .
*****************************************************************************************************************/

DWORD CProcess::execute(CString path, CString executeFileName, CString &remarks,int timeout, BOOL showUI,DWORD dwnewprocessGroup/*=0*/)
{
	DWORD errorCode = ERROR_SUCCESS;
	errorCode = executeFile(path, executeFileName, remarks, timeout,  showUI,dwnewprocessGroup);
	if ( errorCode == ERROR_SUCCESS_REBOOT_REQUIRED || errorCode == ERROR_SUCCESS_REBOOT_INITIATED || errorCode == ERROR_SUCCESS )
	{
		errorCode= ERROR_SUCCESS;
	}
	return errorCode;
}

HANDLE CProcess::GetProcessHandle(DWORD i_ProcessId)
{
	try
	{
		this->m_hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,i_ProcessId);
		
		return this->m_hProcess;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CProcess::GetProcessHandle() %d]>std Exception caught (%S)\n",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CProcess::GetProcessHandle() %d]>Def Exception caught\n",__LINE__);
	}
	return NULL;
}

// Checks, if an application with this name is running or not
//
BOOL CProcess::IsProcessRunning(CString i_ExeName)
{
	PROCESSENTRY32* processInfo = NULL;
	HANDLE hSnapShot            = INVALID_HANDLE_VALUE;
	BOOL bRunning               = FALSE;
	try
	{
		DWORD dwOwnPID = GetProcessId(GetCurrentProcess());
		hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		processInfo=new PROCESSENTRY32;
		processInfo->dwSize=sizeof(PROCESSENTRY32);
		
		while(Process32Next(hSnapShot,processInfo)!=FALSE)
		{
			if (!_tcscmp(processInfo->szExeFile,i_ExeName.GetBuffer()))
			{
				if (processInfo->th32ProcessID != dwOwnPID)
				{
					this->m_ProcessId = processInfo->th32ProcessID;
					GetProcessHandle(this->m_ProcessId);
					bRunning=TRUE;
					break;
				}
			}
		}	
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CProcess::IsProcessRunning() %d]>std Exception caught (%S)\n",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CProcess::IsProcessRunning() %d]>Def Exception caught\n",__LINE__);
	}
	if(INVALID_HANDLE_VALUE != hSnapShot)
	{
		CloseHandle(hSnapShot);
		hSnapShot = INVALID_HANDLE_VALUE;
	}
	SECURE_DELETE(processInfo);

	return bRunning;
}

}//namespace
