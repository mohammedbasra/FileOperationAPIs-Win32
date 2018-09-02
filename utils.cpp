/*
 * Utils
 * 
 * Copyright(C) 
 */

#include "stdafx.h"
#include <psapi.h>
#include <fstream>

#pragma comment(lib,"Psapi.lib")

/////////////////////////////////
//Global Variables
/////////////////////////////////
static const LPTSTR gpCFileName = L"utils.cpp";


BYTE* VirtualMemAlloc(DWORD i_Size)
{
	try
	{
		BYTE *Buffer =(BYTE *) ::VirtualAlloc(NULL,							// system selects address
											i_Size,							// size of allocation
											MEM_RESERVE| MEM_COMMIT,				// allocate reserved pages
											PAGE_READWRITE);						// protection = no access

		return Buffer;
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

BYTE* VirtualReMemAlloc(BYTE *i_PrevBuffer, DWORD i_NewSize)
{
	try
	{
		if(!::VirtualFree(i_PrevBuffer,0,MEM_RELEASE))
		{
		    return NULL;
		}

		BYTE *Buffer =(BYTE *) ::VirtualAlloc(NULL,							// system selects address
											i_NewSize,							// size of allocation
											MEM_RESERVE| MEM_COMMIT,				// allocate reserved pages
											PAGE_READWRITE);						// protection = no access

		return Buffer;
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

BOOL VirtualMemFree(BYTE *i_PrevBuffer)
{ 
	try
	{
		if(ISNULL(i_PrevBuffer))
		{
			return TRUE;
		}
		return ::VirtualFree(i_PrevBuffer,0,MEM_RELEASE);
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

void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;
	//Commented because of unused
	//CONSOLE_FONT_INFO		ftinfo;

    GetConsoleCursorInfo(out, &cursorInfo);
	
	
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
	//SetConsoleTextAttribute(out,225);	
}

void clear()
{   //system("cls") alternative
    COORD topLeft={0,0};
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console,&screen);
    FillConsoleOutputCharacterA(console,' ',screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
                               screen.dwSize.X * screen.dwSize.Y, topLeft,&written);
    SetConsoleCursorPosition(console,topLeft);
}

#define mydelay 100

void delay(int m)
{
    clock_t wait = m+ clock();
    while (wait > clock());
}


int RotateCursor()  
{  
   while(1)
   {
           INFOLOG(L"\\\b");
           delay(mydelay);
           INFOLOG(L"|\b");
           delay(mydelay);
           INFOLOG(L"/\b");
           delay(mydelay);
           INFOLOG(L"_\b");
           delay(mydelay);
   }

   return 0;  
}

///////////////////////////////////////////////////////////////////////////
// Prints the error message for the error code obtained from GetLastError().
// IN:  -empty-
// OUT: Prints the error message.
void GetLastErrorMessage()
{
	DWORD tc;
	LPVOID lpMsgBuf = NULL;

	tc = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, 
						NULL,
						GetLastError(), 
						MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
					    ( LPTSTR ) &lpMsgBuf, 
						0, 
						NULL);
	INFOLOG( L" Error Message: %s\n", lpMsgBuf );
	LocalFree( lpMsgBuf );
}

void printToCoordinates(int x, int y, string text)
{
    INFOLOG(L"\033[%d;%dH%s\n", x, x, text);
}

CString GetSizeUnitString( ULONGLONG i_lluBytes )        
{
	CString csOutput = _T("");
	TCHAR   tcTempOutput[OSD_BYTE_STRING_SIZE]={0};

    if( i_lluBytes >= OSD_TB_SIZE ) {
		_sntprintf_s(tcTempOutput,OSD_BYTE_STRING_SIZE,_T("%.2f TB"), long double(i_lluBytes/OSD_TB_SIZE));   
	}
    else if( i_lluBytes >= OSD_GB_SIZE && i_lluBytes < OSD_TB_SIZE ) {
        _sntprintf_s(tcTempOutput,OSD_BYTE_STRING_SIZE, _T("%.2f GB"), long double(i_lluBytes/OSD_GB_SIZE));
	}
    else if( i_lluBytes >= OSD_MB_SIZE && i_lluBytes < OSD_GB_SIZE ) {
       _sntprintf_s(tcTempOutput,OSD_BYTE_STRING_SIZE,_T("%.2f MB"), long double(i_lluBytes/OSD_MB_SIZE)); 
	}
    else if( i_lluBytes >= OSD_KB_SIZE && i_lluBytes < OSD_MB_SIZE ) {
        _sntprintf_s(tcTempOutput,OSD_BYTE_STRING_SIZE,_T("%.2f KB"), long double(i_lluBytes/OSD_KB_SIZE));
	}
    else if ( i_lluBytes < OSD_KB_SIZE) {
        _sntprintf_s(tcTempOutput,OSD_BYTE_STRING_SIZE,_T("%.2f B"), long double(i_lluBytes));
	}
    else {
        _sntprintf_s(tcTempOutput,OSD_BYTE_STRING_SIZE,_T("%.2f B"), long double(i_lluBytes));
	}

	csOutput.Append(tcTempOutput);
	
    return csOutput;
 }


BOOL ConvertANCIToWideChar(CHAR *in_Src, WCHAR *out_Dst, INT in_MaxLen)
{
    /* locals */
    INT lv_Len;

  // do NOT decrease maxlen for the eos
  if (in_MaxLen <= 0)
    return FALSE;

  /* let windows find out the meaning of ansi
    * - the SrcLen=-1 triggers WCTMB to add a eos to Dst and fails if MaxLen is too small.
    * - if SrcLen is specified then no eos is added
    * - if (SrcLen+1) is specified then the eos IS added
   */
  lv_Len = MultiByteToWideChar(CP_ACP, 0, in_Src, -1, out_Dst, in_MaxLen);

  // validate
  if (lv_Len < 0)
    lv_Len = 0;

  /* ensure eos, watch out for a full buffersize
   * - if the buffer is full without an eos then clear the output like WCTMB does
   *   in case of too small outputbuffer
   *   unfortunately there is no way to let WCTMB return shortened strings,
   *   if the outputbuffer is too small then it fails completely
  */
  if (lv_Len < in_MaxLen)
    out_Dst[lv_Len] = 0;
  else if (out_Dst[in_MaxLen-1])
    out_Dst[0] = 0;

  // done
  return TRUE;
}

/***************************/
/* unicode-ansi conversion */
/***************************/
BOOL ConvertWideCharToANCI(WCHAR *in_Src, CHAR *out_Dst, INT in_MaxLen)
{
    /* locals */
    INT  lv_Len;
    BOOL lv_UsedDefault;
  
  /*do NOT decrease maxlen for the eos*/
  if (in_MaxLen <= 0)
    return FALSE;

   /* let windows find out the meaning of ansi
    * - the SrcLen=-1 triggers WCTMB to add a eos to Dst and fails if MaxLen is too small.
    * - if SrcLen is specified then no eos is added
    * - if (SrcLen+1) is specified then the eos IS added
   */

  /*ANSI code page*/
  lv_Len = WideCharToMultiByte(
     CP_ACP, 0, in_Src, -1, out_Dst, in_MaxLen, 0, &lv_UsedDefault);


  /*validate*/
  if (lv_Len < 0)
    lv_Len = 0;

  /* ensure eos, watch out for a full buffersize
   * - if the buffer is full without an eos then clear the output like WCTMB does
   *   in case of too small outputbuffer
   *   unfortunately there is no way to let WCTMB return shortened strings,
   *   if the outputbuffer is too small then it fails completely
  */
  if (lv_Len < in_MaxLen)
    out_Dst[lv_Len] = 0;
  else if (out_Dst[in_MaxLen-1])
    out_Dst[0] = 0;

  // return whether invalid chars were present
  return !lv_UsedDefault;
}

/***************************/
/* Get Root Directory */
/***************************/
BOOL GetRootDirectoryFromPath(LPTSTR io_Root)
{
	try
	{
		return PathStripToRoot(io_Root);
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>std Exception caught (%S)\n",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>Def Exception caught\n",gpCFileName,__LINE__);
	}
	return FALSE;
}

CString WriteErrorMsgForID( DWORD msgID )
{	
	DWORD tc;
	LPVOID lpMsgBuf = NULL;
	CString lpMsgBuffer;
	
	tc = FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM  | FORMAT_MESSAGE_ALLOCATE_BUFFER |FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK, 
			NULL, 
			msgID, 
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
			( LPWSTR ) &lpMsgBuf, 
			0, 
			NULL);

	if(lpMsgBuf != NULL)
	{
		lpMsgBuffer = (LPTSTR)lpMsgBuf;
		lpMsgBuffer.Trim();
		LocalFree(lpMsgBuf);
	}
	else
	{
		lpMsgBuffer = _T("Unknown error code");
		LocalFree(lpMsgBuf);
	}
	if ( msgID == ERROR_SUCCESS )
	{
		INFOLOG(L"Message : %s \n",lpMsgBuffer.GetBuffer());
	}
	else
	{
		INFOLOG(L"Error Code : %d , Error Message : %s \n",msgID, lpMsgBuffer.GetBuffer());
	}

	return lpMsgBuffer;
}



///////////////////////////////////////////////////////////////////////////
// Prints the error message for the error code passed to the function.
// IN:  Error value(msgID).
// OUT: Prints the error message for the error value.
void printErrorMsgForID( DWORD msgID )
{
	CString lpMsgBuf = _T("");
	lpMsgBuf = WriteErrorMsgForID( msgID );
}

DWORD GetOSDAgent(CString &strAgentDir)
{
	OS_Manager::CRegistry reg;
	LPTSTR pOSDAgentDir = NULL;
	DWORD dwType = REG_SZ;
	ULONG ulSize = MAX_PATH;

	DWORD ErrCode = ERROR_SUCCESS;
	try
	{
		strAgentDir = _T("");

		pOSDAgentDir = new TCHAR[MAX_PATH];

		OS_Manager::OSD_RESULT osdres = reg.RegistryQueryValue(REG_HKLM,REG_AGENT,REG_AGENT_INSTALLDIR,&dwType,pOSDAgentDir,&ulSize);
		if(osdres.RetCode != OS_Manager::OSD_ERROR_NONE)
		{
			INFOLOG(L"Error : Unable to get registry value %d",osdres.RetCode);
			return osdres.RetCode;
		}

		strAgentDir = pOSDAgentDir;

		SECURE_ARRAY_DELETE(pOSDAgentDir);

		return ErrCode;
	}
	catch(...)
	{
		ErrCode = ERROR_INVALID_DATA;
		return ErrCode;

	}
}

DWORD ChangeToAnsiStr( LPWSTR wideString, LPSTR *ansiString )
{
	int len;
	len = WideCharToMultiByte( CP_ACP, 0, wideString, -1, 0, 0, 0, 0 );
	*ansiString = ( LPSTR )calloc( 1, ( len + 1 )* sizeof(char) );
	len = WideCharToMultiByte( CP_ACP, 0, wideString, -1, *ansiString, len, 0, 0 );
	if (!len)
	{
		GetLastErrorMessage();
		return ERROR_INVALID_DATA;
	}
	return ERROR_SUCCESS;
}

DWORD ChangeToWideStr( LPSTR ansiString, LPWSTR *wideString )
{
	int reqLen = 0;      

	if(ansiString == NULL)
	{
		INFOLOG(L"ChangeToWideStr: AnsiString is empty! \n" );
		return ERROR_INVALID_DATA;
	}

	//edit : Changed CP_ACP to CP_UTF8
	reqLen = MultiByteToWideChar( CP_UTF8, 0, ansiString, -1, 0, 0 );
	*wideString = ( LPWSTR )calloc(1, (reqLen)*sizeof(WCHAR));
	reqLen = MultiByteToWideChar( CP_UTF8, 0, ansiString, -1, *wideString, reqLen );

	if (!reqLen)
	{
		GetLastErrorMessage();
		return ERROR_INVALID_DATA;
	}
	return ERROR_SUCCESS; 
}

DWORD ChangeWideStrToUtf8Str(LPWSTR wideString,LPSTR * utf8)
{
	int len;		
	len = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, NULL, 0, 0, 0);
	*utf8 = ( LPSTR )calloc( 1, ( len + 1 )* sizeof(char) );
	len = WideCharToMultiByte( CP_UTF8, 0, wideString, -1, *utf8, len, 0, 0 );
	if (!len)
	{		
		GetLastErrorMessage();
		return ERROR_INVALID_DATA;
	}
	return ERROR_SUCCESS;

}
DWORD ChangeUtf8ToWideStr(const char * utf8,LPWSTR *wideString)
{
	int reqLen = 0;      
	if(utf8 == NULL)
	{
		INFOLOG(L"ChangeUtf8ToWideStr: utf8 is empty! \n" );
		return ERROR_INVALID_DATA;
	}
	else
		INFOLOG(L"UTF String Received:%S\n",(char*)utf8);

	reqLen = MultiByteToWideChar( CP_UTF8, 0, utf8, -1, 0, 0 );
	*wideString = ( LPWSTR )calloc(1, (reqLen)*sizeof(WCHAR));
	reqLen = MultiByteToWideChar( CP_UTF8, 0, utf8, -1, *wideString, reqLen );

	if (!reqLen)
	{
		GetLastErrorMessage();
		return ERROR_INVALID_DATA;
	}
	return ERROR_SUCCESS;
}

DWORD ChangeAnsiToUTF8Str(LPSTR ansiString, LPSTR *utf8String)
{
	LPWSTR wideString = NULL;
	if(ansiString == NULL)
	{
		INFOLOG(L"ChangeAnsiToUTF8Str: ansiString is empty! \n" );
		return ERROR_INVALID_DATA;
	}
	if(ChangeToWideStr(ansiString,&wideString) == ERROR_SUCCESS)
	{
		return ChangeWideStrToUtf8Str(wideString,utf8String);
	}
	return ERROR_INVALID_DATA;
}

CStringA ChangeWideStrToUtf8Str(CStringW inStr)
{
	CStringA returnStr;
	LPSTR utfStr = NULL;
	if(ChangeWideStrToUtf8Str(inStr.GetBuffer(),&utfStr) ==ERROR_SUCCESS)
	{
		returnStr = utfStr;
		SECURE_FREE(utfStr);
	}
	else
	{
		returnStr = "";
	}
	return returnStr;
}

CStringW ChangeUtf8ToWideStr(CStringA inStr)
{
	
	CStringW returnStr;
	LPWSTR wideStr = NULL;
	if(ChangeUtf8ToWideStr((const char *)inStr.GetBuffer(),&wideStr) ==ERROR_SUCCESS)
	{
		returnStr = wideStr;
		SECURE_FREE(wideStr);
	}
	else
	{
		returnStr = L"";
	}
	return returnStr;
}

CStringA ChangeAnsiToUTF8Str(CStringA instr)
{
	CStringA returnStr;
	LPSTR utfStr = NULL;
	if(ChangeAnsiToUTF8Str(instr.GetBuffer(),&utfStr) ==ERROR_SUCCESS)
	{
		returnStr = utfStr;
		SECURE_FREE(utfStr);
	}
	else
	{
		returnStr = "";
	}
	return returnStr;
}


CStringA ChangeToAnsiStr(CStringW instr)
{
	CStringA returnStr;
	LPSTR ansistr = NULL;
	if(ChangeToAnsiStr(instr.GetBuffer(),&ansistr) ==ERROR_SUCCESS)
	{
		returnStr = ansistr;
		SECURE_FREE(ansistr);
	}
	else
	{
		returnStr = "";
	}
	return returnStr;
}

BOOL ParseSharePath(CString i_Path, 
	                CString &o_HostName, 
					CString &o_FolderName, 
					CString &o_SubFolder, 
					CString &o_FileName,
					BOOL    i_IsFile)
{
	try
	{
		o_HostName.Empty();
		o_FolderName.Empty();
		o_SubFolder.Empty();

		if(i_Path.IsEmpty())
		{
			return FALSE;	
		}

		INT pos = 0,
			curpos = (INT)_tcslen(L"\\\\");

		pos = curpos;
		while(TRUE)
		{
			TCHAR TempString[MAX_PATH] = {0};
			pos = i_Path.Find(L"\\",curpos);
			if(-1 != pos)
			{
				//Always first pattern will be a hostname
				if(o_HostName.IsEmpty()){
					_tcsncpy_s(TempString,(i_Path.GetBuffer() + curpos),(pos-curpos));
					TempString[pos - curpos] = TCHAR('\0');
					o_HostName = TempString;
				}
				else if(o_FolderName.IsEmpty()) //Always second pattern will be a hostname
				{
					_tcsncpy_s(TempString,(i_Path.GetBuffer() + curpos),(pos-curpos));
					TempString[pos - curpos] = TCHAR('\0');
					o_FolderName = TempString;
				}
				else if(i_IsFile) //If it is a file & after host name & share folder, it is sub folder. Finally it holds only last sub folder
				{
					_tcsncpy_s(TempString,(i_Path.GetBuffer() + curpos),(pos-curpos));
					TempString[pos - curpos] = TCHAR('\0');
					o_SubFolder = TempString;
				}
			}
			else
			{
				//At last it is sub folder or file name
				_tcsncpy_s(TempString,(i_Path.GetBuffer() + curpos),(i_Path.GetLength()-curpos));
				TempString[i_Path.GetLength() - curpos] = TCHAR('\0');

				if(i_IsFile)
				{
					o_FileName = TempString;
				}
				else
				{
					o_SubFolder = TempString;
				}

				break;
			}
			curpos = pos + 1;
		}
		return TRUE;
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n","test",__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n","test",__LINE__, __FUNCTION__);
	}
	return FALSE;
}

BOOL GetDiskFreeSpaceDetails(CString i_Path,
	                         ULARGE_INTEGER &o_FreeBytesAvailable,
	                         ULARGE_INTEGER &o_TotalNumberOfBytes,
							 ULARGE_INTEGER &o_TotalNumberOfFreeBytes)
{
	BOOL Status = FALSE;
	try
	{
		
		Status = GetDiskFreeSpaceEx(i_Path.GetBuffer(),
										&o_FreeBytesAvailable,
										&o_TotalNumberOfBytes,
										&o_TotalNumberOfFreeBytes
										);

		return Status;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%s %d]>GetDriveInfo::std Exception caught (%S)",_T("getdiskfree.cpp"),__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>GetDriveInfo::Def Exception caught",_T("getdiskfree.cpp"),__LINE__);
	}
	return FALSE;
}

BOOL IsSpaceAvailable(ULONGLONG i_AvailableSize, ULONGLONG i_RequiredSize)
{
	//If want we can put > instead of >=. 
	return ((i_AvailableSize >= i_RequiredSize) ? (TRUE) : (FALSE));
}

void WriteEventLogEntry(CString srcname, PWSTR pszMessage, WORD wType)
{
    HANDLE hEventSource = NULL;
    LPCWSTR lpszStrings[2] = { NULL, NULL };

    hEventSource = RegisterEventSource(NULL, srcname);
    if (hEventSource)
    {
		lpszStrings[0] = srcname.GetBuffer();
        lpszStrings[1] = pszMessage;

        ReportEvent(hEventSource,  // Event log handle
            wType,                 // Event type
            0,                     // Event category
            0,                     // Event identifier
            NULL,                  // No security identifier
            2,                     // Size of lpszStrings array
            0,                     // No binary data
            lpszStrings,           // Array of strings
            NULL                   // No binary data
            );

        DeregisterEventSource(hEventSource);
    }
}

INT64 OSD_atoi(CStringA i_String)
{
	return std::stoll(i_String.GetBuffer(),NULL,10);
}

//eg. D:\\folder1\\folder2\\ - Valid
//eg. D:\\folder1\\ - valid
//eg. D:\\folder1 - Not valid
BOOL CreateDirectoryRecursive(CString i_Path)
{
	try
	{
		if(i_Path.IsEmpty())
		{
			return FALSE;
		}

		TCHAR *path = i_Path.GetBuffer();

		TCHAR folder[MAX_PATH];
		TCHAR *end = NULL;
		ZeroMemory(folder, MAX_PATH * sizeof(TCHAR));

		end = _tcschr(path, _T('\\'));

		while(end != NULL)
		{
			_tcsncpy_s(folder, path, end - path + 1);
			if(!CreateDirectoryW(folder, NULL))
			{
				DWORD err = GetLastError();

				if(err != ERROR_ALREADY_EXISTS)
				{
					// No need to through error
				}
			}
			end = _tcschr(++end, _T('\\'));
		}
		return TRUE;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>CreateDirectoryRecursive::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>CreateDirectoryRecursive::Def Exception caught",gpCFileName,__LINE__);
	}
	return FALSE;
}

VOID LogCurrentProcessMemroyUsage( )
{
	PrintMemoryInfo(GetCurrentProcessID());
}

DWORD GetCurrentProcessID( )
{
	return GetCurrentProcessId();
}

VOID PrintMemoryInfo( DWORD processID )
{
	try
	{
		HANDLE hProcess;
		PROCESS_MEMORY_COUNTERS pmc;

		// Print information about the memory usage of the process.
		hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
										PROCESS_VM_READ,
										FALSE, processID );

		INFOLOG(L"*********************************MEMORY USAGE******************************************");
		if (NULL == hProcess){
			INFOLOG(L"[%S %d]>PROCESS ID NULL",gpCFileName,__LINE__);
			return;
		}
		TCHAR ProcessName[2048] = {0};
		GetProcessImageFileName(hProcess,ProcessName,sizeof(ProcessName) - 1);

		//GetModuleFileNameEx(hProcess, 0, name, sizeof(name) - 1);
		//GetModuleFileName(GetModuleHandle(NULL), ProcessName, sizeof(ProcessName));

		if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
		{
			INFOLOG( L"\t PROCESS NAME: %s\n\t CURRENT WORKING SET SIZE: %s\n\t PEAK WORKING SET SIZE: %s", 
				ProcessName, 
				GetSizeUnitString(pmc.WorkingSetSize),
				GetSizeUnitString(pmc.PeakWorkingSetSize));
		}

		CloseHandle( hProcess );

		INFOLOG(L"***************************************************************************");
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>PrintMemoryInfo::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>PrintMemoryInfo::Def Exception caught",gpCFileName,__LINE__);
	}
	
}

double GetPercentageValue(ULONGLONG i_TotalValue, double i_PercentNumber)
{
	try
	{
		if(i_TotalValue <= 0 )
		{
			return i_TotalValue;
		}
		return double( double(i_TotalValue * double(i_PercentNumber)) / 100 );
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>GetPercentageValue::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>GetPercentageValue::Def Exception caught",gpCFileName,__LINE__);
	}
	return i_TotalValue;
}

double GetNumberIntoPercent(ULONGLONG i_TotalValue, ULONGLONG i_currentVolue)
{
	try
	{
		return double( double(i_currentVolue / double(double(i_TotalValue) * 100) ));
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>GetNumberIntoPercent::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>GetNumberIntoPercent::Def Exception caught",gpCFileName,__LINE__);
	}
	return i_TotalValue;
}

// Converts a wstring into a GUID
BOOL OSD_WString2Guid(CAtlString src, GUID & o_resGUId)
{
	try
	{
		// Check if this is a GUID
		GUID result = {0};
		HRESULT hr = ::CLSIDFromString(T2OLE(const_cast<TCHAR*>(src.GetBuffer())), &result);
		if (FAILED(hr))
		{
			INFOLOG(L"[%S %d]>WString2Guid ERROR: The string '%s' is not formatted as a GUID",gpCFileName,__LINE__,src.GetBuffer());

			throw(E_INVALIDARG);
		}

		return FALSE;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>WString2Guid::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>WString2Guid::Def Exception caught",gpCFileName,__LINE__);
	}
    return FALSE;
}

CString GetFolderName(CString Directory)
{
	Directory.MakeReverse();
	int x = Directory.Find(L"\\");
	Directory = Directory.Mid(0,x);
	Directory.MakeReverse();
	return Directory;
}

DWORD DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories)
{
  bool            bSubdirectory = false;       // Flag, indicating whether
                                               // subdirectories have been found
  HANDLE          hFile;                       // Handle to directory
  std::string     strFilePath;                 // Filepath
  std::string     strPattern;                  // Pattern
  WIN32_FIND_DATAA FileInformation;             // File information


  strPattern = refcstrRootDirectory + "\\*.*";
  hFile = ::FindFirstFileA(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bDeleteSubdirectories)
          {
            // Delete subdirectory
            int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
            if(iRC)
              return iRC;
          }
          else
            bSubdirectory = true;
        }
        else
        {
          // Set file attributes
          if(::SetFileAttributesA(strFilePath.c_str(),
                                 FILE_ATTRIBUTE_NORMAL) == FALSE)
            return ::GetLastError();

          // Delete file
          if(::DeleteFileA(strFilePath.c_str()) == FALSE)
            return ::GetLastError();
        }
      }
    } while(::FindNextFileA(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
    else
    {
      if(!bSubdirectory)
      {
        // Set directory attributes
        if(::SetFileAttributesA(refcstrRootDirectory.c_str(),
                               FILE_ATTRIBUTE_NORMAL) == FALSE)
          return ::GetLastError();

        // Delete directory
        if(::RemoveDirectoryA(refcstrRootDirectory.c_str()) == FALSE)
          return ::GetLastError();
      }
    }
  }

  return 0;
}


HANDLE GetNativeCmdLog(CAtlString FileName)
{
	try
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE; 

		HANDLE hLog = CreateFile(FileName.GetBuffer(),
									FILE_APPEND_DATA,
									FILE_SHARE_WRITE | FILE_SHARE_READ,
									&sa,
									OPEN_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									NULL );

		DWORD dwFileSize, MaxLimit;

		MaxLimit = DWORD(ONE_MEGA_BYTE); 
		dwFileSize = GetFileSize(hLog,NULL);

		if(dwFileSize < MaxLimit)
		{
			return hLog;
		}

		if(hLog != NULL)
			CloseHandle(hLog);

		DeleteFile(FileName.GetBuffer());

		hLog = CreateFile(FileName.GetBuffer(),
									FILE_APPEND_DATA,
									FILE_SHARE_WRITE | FILE_SHARE_READ,
									&sa,
									OPEN_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									NULL );
		return hLog;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>GetNativeCmdLog::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>GetNativeCmdLog::Def Exception caught",gpCFileName,__LINE__);
	}
    return NULL;
}

BOOL OSDPECommonUtils :: AddApplication()
{
	try
	{
		CAtlString Startnet, MountDir, SrcFilePath, DestFilePath, Osd32bitPath, Osd64bitPath, OsdAppPath;

		MountDir = m_PEDirctory + L"\\mount";
		Startnet = MountDir + L"\\Windows\\System32\\Startnet.cmd";
		
		Osd32bitPath = "..\\webapps\\DesktopCentral\\agent\\32bit\\osd";
		Osd64bitPath = "..\\webapps\\DesktopCentral\\agent\\64bit\\osd";

		OsdAppPath = m_Is32bit ? Osd32bitPath : Osd64bitPath;

	//Delete Old Files
		std::string OldPath = CAtlStringA(MountDir).GetBuffer();
		OldPath += "\\Program Files\\osd";

		DeleteDirectory(OldPath,TRUE);

	//Copy OSD Image Files
		SrcFilePath = OsdAppPath + L"\\images";
		DestFilePath = MountDir + L"\\Program Files\\osd\\images";

		BOOL bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy OSD Application Files	
		SrcFilePath = OsdAppPath + L"\\bin\\OSDA.exe";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\OSDA.exe";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		SrcFilePath = OsdAppPath + L"\\bin\\OSDMonitor.exe";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\OSDMonitor.exe";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		SrcFilePath = OsdAppPath + L"\\bin\\OSDConfig.json";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\OSDConfig.json";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		SrcFilePath = OsdAppPath + L"\\bin\\OSManagerDepends.dll";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\OSManagerDepends.dll";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		SrcFilePath = OsdAppPath + L"\\bin\\gdiplus.dll";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\gdiplus.dll";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy 32 bit OSD Patcher Files	
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\32bit";
		bflag = CreateDirectory(DestFilePath,NULL);
		if(!bflag)
		{
			//return FALSE;
		}

		SrcFilePath = Osd32bitPath + L"\\bin\\OSManagerPatcher.exe";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\32bit\\OSManagerPatcher.exe";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		SrcFilePath = Osd32bitPath + L"\\bin\\OSManagerDepends.dll";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\32bit\\OSManagerDepends.dll";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		SrcFilePath = Osd32bitPath + L"\\bin\\dpinst.exe";
		DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\32bit\\dpinst.exe";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy 64 bit OSD Patcher Files	
		if(!m_Is32bit)
		{
			DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\64bit";
			bflag = CreateDirectory(DestFilePath,NULL);
			if(!bflag)
			{
				//return FALSE;
			}

			SrcFilePath = Osd64bitPath + L"\\bin\\OSManagerPatcher.exe";
			DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\64bit\\OSManagerPatcher.exe";

			bflag = CopyMediaDir(SrcFilePath,DestFilePath);
			if(!bflag)
			{
				return FALSE;
			}

			SrcFilePath = Osd64bitPath + L"\\bin\\OSManagerDepends.dll";
			DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\64bit\\OSManagerDepends.dll";

			bflag = CopyMediaDir(SrcFilePath,DestFilePath);
			if(!bflag)
			{
				return FALSE;
			}

			SrcFilePath = Osd64bitPath + L"\\bin\\dpinst.exe";
			DestFilePath = MountDir + L"\\Program Files\\osd\\bin\\64bit\\dpinst.exe";

			bflag = CopyMediaDir(SrcFilePath,DestFilePath);
			if(!bflag)
			{
				return FALSE;
			}

		}

	//Startnet command to run OSDA.exe
		CAtlStringA commands;
		CHAR TempBuffer[MAX_PATH] = {0};

		commands.Append("\n");
		commands.Append("wpeinit\n");
		commands.Append("\"wpeutil.exe\" DisableFirewall\n");
		commands.Append("start\n"); 
		commands.Append("cd \"..\\..\\Program Files\\osd\\bin\""); 
		//commands.Append("\n start \"\" \"winvnc.exe\"");    //vnc command
		commands.Append("\n\"OSDA.exe\"");

		ofstream ofile(Startnet.GetBuffer(), ios::out);
		ofile.write(commands.GetBuffer(),commands.GetLength());
		ofile.close();

		return TRUE;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>AddApplication::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>AddApplication::Def Exception caught",gpCFileName,__LINE__);
	}
	return FALSE;
}

BOOL OSDPECommonUtils :: CopyMediaDir(CString SrcDir, CString DestDir)
{
	try
	{
		SrcDir  += L'\0';
		DestDir += L'\0';
		SHFILEOPSTRUCT SHDirStruct;
	    ZeroMemory(&SHDirStruct, sizeof(SHFILEOPSTRUCT));
        SHDirStruct.hwnd   = NULL;
        SHDirStruct.wFunc  = FO_COPY;
        SHDirStruct.pFrom  = SrcDir;
        SHDirStruct.pTo    = DestDir;
        SHDirStruct.fFlags = FOF_MULTIDESTFILES|FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_SILENT;

		if(SHFileOperation(&SHDirStruct) != 0)
		{
			return FALSE;
		}
		return TRUE;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>CopyMediaDir::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>CopyMediaDir::Def Exception caught",gpCFileName,__LINE__);
	}
	return FALSE;
}

BOOL OSDPECommonUtils :: CreateWinPEDirAIK() //(CString WinPEDir, CString WinArch)
{
	try
	{
		BOOL bflag;

	//Create Mount Directory
		CString DestFilePath,SrcFilePath;
		CString MountDir = m_PEDirctory + L"\\media";
		bflag = CreateDirectory(m_PEDirctory,NULL);
		bflag = CreateDirectory(MountDir,NULL);
		MountDir = m_PEDirctory + L"\\media\\sources";
		bflag = CreateDirectory(MountDir,NULL);
		MountDir = m_PEDirctory + L"\\mount";
		bflag = CreateDirectory(MountDir,NULL);
		if(!bflag)
		{
			if(GetFileAttributes(MountDir) == INVALID_FILE_ATTRIBUTES)
			{
				return FALSE;
			}
		}

	//Creating the path for boot files from AIK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files\\Windows AIK\\Tools\\PETools\\"+ m_OSD_PE.m_PEArch;
		SrcFilePath = m_PEToolPath + L"PETools\\";
		SrcFilePath = SrcFilePath + m_PEArch;
		SrcFilePath = SrcFilePath + L"\\boot";

		//SrcFilePath.Format(L"%sPETools\\%s\\boot",m_PEToolPath.GetBuffer(),m_PEArch.GetBuffer());
		
		DestFilePath =  m_PEDirctory + L"\\media\\boot";
	
		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Creating the path for EFI files from AIK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files\\Windows AIK\\Tools\\PETools\\" + m_OSD_PE.m_PEArch;
		SrcFilePath = m_PEToolPath + L"PETools\\";
		SrcFilePath = SrcFilePath + m_PEArch;
		SrcFilePath = SrcFilePath + L"\\efi";

		//SrcFilePath.Format(L"%sPETools\\%s\\efi",m_PEToolPath.GetBuffer(),m_PEArch.GetBuffer());
		
		DestFilePath =  m_PEDirctory + L"\\media\\EFI";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Creating the path for wim file from ADK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files\\Windows AIK\\Tools\\PETools\\" + m_OSD_PE.m_PEArch;
		SrcFilePath = m_PEToolPath + L"PETools\\";
		SrcFilePath = SrcFilePath + m_PEArch;
		SrcFilePath = SrcFilePath + L"\\winpe.wim";

		//SrcFilePath.Format(L"%sPETools\\%s\\winpe.wim",m_PEToolPath.GetBuffer(),m_PEArch.GetBuffer());
		
		DestFilePath =  m_PEDirctory + L"\\media\\sources\\boot.wim";
		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Creating the path for bootmgr file from AIK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files\\Windows AIK\\Tools\\PETools\\" + m_OSD_PE.m_PEArch;
		SrcFilePath = m_PEToolPath + L"PETools\\";
		SrcFilePath = SrcFilePath + m_PEArch;
		SrcFilePath = SrcFilePath + L"\\bootmgr";

		//SrcFilePath.Format(L"%sPETools\\%s\\bootmgr",m_PEToolPath.GetBuffer(),m_PEArch.GetBuffer());
		
		DestFilePath =  m_PEDirctory + L"\\media\\bootmgr";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}
		
	//Creating the path for etfsboot file from ADK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files\\Windows AIK\\Tools\\PETools\\" + m_OSD_PE.m_PEArch;
		SrcFilePath = m_PEToolPath + L"PETools\\";
		SrcFilePath = SrcFilePath + m_PEArch;
		SrcFilePath = SrcFilePath + L"\\boot\\etfsboot.com";

		//SrcFilePath.Format(L"%sPETools\\%s\\boot\\etfsboot.com",m_PEToolPath.GetBuffer(),m_PEArch.GetBuffer());
		
		DestFilePath =  m_PEDirctory + L"\\etfsboot.com";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		return TRUE;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>CreateWinPEDirAIK::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>CreateWinPEDirAIK::Def Exception caught",gpCFileName,__LINE__);
	}
    return FALSE;
}

BOOL OSDPECommonUtils :: CreateWinPEDirADK() 
{
	try
	{
		BOOL bflag;
		CString SrcFilePath, DestFilePath ;
	//Create Mount Directory
		CString MountDir = m_PEDirctory + L"\\mount";
		bflag = CreateDirectory(m_PEDirctory, NULL);
		if(!bflag)
		{
			if(GetFileAttributes(m_PEDirctory) == INVALID_FILE_ATTRIBUTES)
			{
				return FALSE;
			}
		}
		bflag = CreateDirectory( MountDir ,NULL);
		if(!bflag)
		{
			if(GetFileAttributes(MountDir) == INVALID_FILE_ATTRIBUTES)
			{
				return FALSE;
			}
		}

	//Creating the path for Media files from ADK Directory and copy to WinPE folder
		 // = L"C:\\Program Files (x86)\\Windows Kits\\" + m_OSD_PE.m_WinVer;
		SrcFilePath = m_PEToolPath + L"Assessment and Deployment Kit\\Windows Preinstallation Environment\\";
		SrcFilePath = SrcFilePath + m_PEArch + L"\\Media";

		//SrcFilePath.Format(L"%sAssessment and Deployment Kit\\Windows Preinstallation Environment\\%s\\Media",
		//						m_PEToolPath.GetBuffer(), m_PEArch.GetBuffer() );

		DestFilePath = m_PEDirctory + L"\\media";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Creating the path for wim file from ADK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files (x86)\\Windows Kits\\" + m_OSD_PE.m_WinVer;
		SrcFilePath = m_PEToolPath + L"Assessment and Deployment Kit\\Windows Preinstallation Environment\\";
		SrcFilePath = SrcFilePath + m_PEArch + L"\\en-us\\winpe.wim";

		//SrcFilePath.Format(L"%sAssessment and Deployment Kit\\Windows Preinstallation Environment\\%s\\en-us\\winpe.wim",
		//						m_PEToolPath.GetBuffer(), m_PEArch.GetBuffer() );

		DestFilePath = DestFilePath + L"\\sources\\boot.wim";
		
		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Creating the path for etfsboot file from ADK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files (x86)\\Windows Kits\\" + m_OSD_PE.m_WinVer;
		SrcFilePath = m_PEToolPath + L"Assessment and Deployment Kit\\Deployment Tools\\";
		SrcFilePath = SrcFilePath + m_PEArch + L"\\Oscdimg\\etfsboot.com";

		//SrcFilePath.Format(L"%sAssessment and Deployment Kit\\Windows Preinstallation Environment\\%s\\Oscdimg\\etfsboot.com",
		//						m_PEToolPath.GetBuffer(), m_PEArch.GetBuffer() );

		DestFilePath = m_PEDirctory + L"\\fwfiles\\etfsboot.com";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Creating the path for efisys file from ADK Directory and copy to WinPE folder
		//SrcFilePath = L"C:\\Program Files (x86)\\Windows Kits\\" + m_OSD_PE.m_WinVer;
		SrcFilePath = m_PEToolPath + L"Assessment and Deployment Kit\\Deployment Tools\\";
		SrcFilePath = SrcFilePath + m_PEArch + L"\\Oscdimg\\efisys.bin";

		//SrcFilePath.Format(L"%sAssessment and Deployment Kit\\Windows Preinstallation Environment\\%s\\Oscdimg\\efisys.bin",
		//						m_PEToolPath.GetBuffer(), m_PEArch.GetBuffer() );

		DestFilePath = m_PEDirctory + L"\\fwfiles\\efisys.bin";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}
		
		return TRUE;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>CreateWinPEDirADK::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>CreateWinPEDirADK::Def Exception caught",gpCFileName,__LINE__);
	}
    return FALSE;
}

BOOL OSDPECommonUtils :: CopyPXEfilesAfterUnmount()
{
	try
	{
		BOOL bflag;
		DWORD dwError;
		CString SrcFilePath, DestFilePath, DestDir;

	//Copy EFI files
		SrcFilePath  = m_PEDirctory + L"\\media\\EFI";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\EFI";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy boot.sdi file 
		SrcFilePath  = m_PEDirctory + L"\\media\\boot\\boot.sdi";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\boot\\boot.sdi";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy for boot.wim file 
		SrcFilePath  = m_PEDirctory + L"\\media\\sources\\boot.wim";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\boot\\boot.wim";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy BCD file 
		SrcFilePath  = m_PEDirctory + L"\\media\\boot\\BCD";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\boot\\BCD";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		
		dwError = PXECreateBCD();
		if(dwError != ERROR_SUCCESS)
		{
			return FALSE;
		}

		return TRUE;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>CopyPXEfilesAfterUnmount::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>CopyPXEfilesAfterUnmount::Def Exception caught",gpCFileName,__LINE__);
	}
    return FALSE;
}

BOOL OSDPECommonUtils :: CopyPXEfilesBeforeUnmount()
{
	try
	{
		BOOL bflag;
		DWORD dwError;
		CString DestDir, SrcFilePath, DestFilePath;

	//Create PXE_BOOT Directory
		DestDir = m_PEDirctory + L"\\PXE_BOOT";
		bflag = CreateDirectory(DestDir ,NULL);

		DestDir = m_PEDirctory + L"\\PXE_BOOT\\EFI";
		bflag = CreateDirectory(DestDir ,NULL);

		DestDir = m_PEDirctory + L"\\PXE_BOOT\\boot";
		bflag = CreateDirectory(DestDir ,NULL);

		DestDir = m_PEDirctory + L"\\PXE_BOOT\\boot\\Fonts";
		bflag = CreateDirectory(DestDir ,NULL);
		if(!bflag)
		{
			dwError = ::GetLastError();
			if(GetFileAttributes(DestDir) == INVALID_FILE_ATTRIBUTES)
			{
				return FALSE;
			}
		}

	//Copy PXE_BOOT Files
		SrcFilePath  = m_PEDirctory + L"\\mount\\Windows\\boot\\PXE";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\boot";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy PXE_BOOT EFI Files
		SrcFilePath  = m_PEDirctory + L"\\mount\\Windows\\boot\\EFI";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\EFI";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy PXE_BOOT Font Files
		SrcFilePath  = m_PEDirctory + L"\\media\\boot\\Fonts";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\boot\\Fonts";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

	//Copy PXE_BOOT Font Files
		SrcFilePath  = m_PEDirctory + L"\\mount\\Windows\\boot\\Fonts";
		DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\boot\\Fonts";

		bflag = CopyMediaDir(SrcFilePath,DestFilePath);
		if(!bflag)
		{
			return FALSE;
		}

		return TRUE;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>CopyPXEfilesBeforeUnmount::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>CopyPXEfilesBeforeUnmount::Def Exception caught",gpCFileName,__LINE__);
	}
    return FALSE;
}

DWORD OSDPECommonUtils :: AddDriverDISM(CAtlString Driver)
{
	try
	{
		DWORD dwError;
		TCHAR TempBuffer[MAX_PATH] = {0};

		CAtlString commands = L"dism.exe /Add-Driver /Image:\"";
		wcscpy_s(TempBuffer,  CAtlString(m_PEDirctory).GetBuffer());
		commands.Append(TempBuffer);
		commands.Append(L"\\mount\" /Driver:\"");
		wcscpy_s(TempBuffer,  CString(Driver).GetBuffer());
		commands.Append(TempBuffer);
		commands.Append(L"\" /ForceUnsigned");

		CAtlString cmdExePath;

		if(m_dwWinPEver > 3)
		{
			cmdExePath = m_PEToolPath + L"Assessment and Deployment Kit\\Deployment Tools\\";

			if(OS_Manager::IsWow64())
			{
				cmdExePath = cmdExePath	+ m_PEArch + L"\\DISM\\dism.exe";
			}
			else
			{
				cmdExePath = cmdExePath	+ L"x86\\DISM\\dism.exe";
			}
		}
		else
		{
			cmdExePath = m_PEToolPath + L"Servicing\\dism.exe";
		}

		//cmdExePath.Format(L"%sAssessment and Deployment Kit\\Deployment Tools\\%s\\DISM\\dism.exe",
		//						m_PEToolPath.GetBuffer(), m_PEArch.GetBuffer() );

		//commands.Format(L"dism.exe /Add-Driver /Image:\"%s\\mount\" /Driver:\"%s\" /ForceUnsigned",
		//	m_PEDirctory.GetBuffer(),Driver.GetBuffer());

		dwError = CreatecmdProcess( cmdExePath, commands );

		return dwError;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>AddDriverDISM::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>AddDriverDISM::Def Exception caught",gpCFileName,__LINE__);
	}
	return ERROR_BAD_ENVIRONMENT;
}

DWORD OSDPECommonUtils :: AddDriverPEIMG(CString Driver)
{
	DWORD dwError;
	try
	{
		TCHAR TempBuffer[MAX_PATH] = {0};
		CAtlString commands = L"peimg.exe /inf=\"";
		wcscpy_s(TempBuffer,  CString(Driver).GetBuffer());
		commands.Append(TempBuffer);
		commands.Append(L"\" \"");
		wcscpy_s(TempBuffer,  CString(m_PEDirctory).GetBuffer());
		commands.Append(TempBuffer);
		commands.Append((L"\\mount\\Windows\""));

		CAtlString cmdExePath = m_PEToolPath + L"PETools\\peimg.exe";

		dwError = CreatecmdProcess( cmdExePath, commands );

		return dwError;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>AddDriverPEIMG::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>AddDriverPEIMG::Def Exception caught",gpCFileName,__LINE__);
	}
	return ERROR_BAD_ENVIRONMENT;
}

DWORD OSDPECommonUtils :: PXECreateBCD() 
{
	DWORD dwError;
	BOOL  bRedirect = 0;
	try
	{

		CAtlString cmdExePath = OSD_PE_BCD_BUILDER;

		CAtlString DestFilePath = m_PEDirctory + L"\\PXE_BOOT\\boot"; 

		CAtlString cmdArgs;
        cmdArgs.Append(cmdExePath);
		cmdArgs.Append(_T(" \""));
		cmdArgs.Append(DestFilePath.GetBuffer());
		cmdArgs.Append(_T("\" "));
		switch(m_dwWinPEver)
		{
		case 5:
			cmdArgs.Append(_T("5"));
			break;
		case 4:
			cmdArgs.Append(_T("4"));
			break;
		case 3:
			cmdArgs.Append(_T("3"));
			break;
		case 2:
			cmdArgs.Append(_T("2"));
			break;

		case 10:
		default:
			cmdArgs.Append(_T("10"));

		}

		if(OS_Manager::IsWow64())
		{
			bRedirect = Wow64EnableWow64FsRedirection(FALSE);
		}

		dwError = CreatecmdProcess( cmdExePath, cmdArgs );

		if(OS_Manager::IsWow64())
		{
			bRedirect ? Wow64EnableWow64FsRedirection(TRUE) : 0;
		}

		return dwError;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::Def Exception caught",gpCFileName,__LINE__);
	}
	return ERROR_BAD_ENVIRONMENT;
}

DWORD OSDPECommonUtils :: FormatDisk(CAtlString i_DestDirctory,BOOL IsFormatNTFS) 
{
	DWORD dwError;
	try
	{
		CHAR TempBuffer[MAX_PATH] = {0};
		CAtlStringA commands = "select volume ";
		if( !i_DestDirctory.IsEmpty()) 
		{
			strcpy_s (TempBuffer,  CStringA(i_DestDirctory).GetString());
			commands.Append(TempBuffer);	
		}

		commands.Append(("\n"));

		if(IsFormatNTFS)
			commands.Append(("format quick fs=ntfs label=\"WinPE\" override"));
		else
			commands.Append(("format quick fs=fat32 label=\"WinPE\" override"));
		
		commands.Append(("\n"));
		commands.Append(("exit"));

		ofstream ofile("testcmd.txt", ios::out);	
		ofile.write(commands.GetBuffer(),commands.GetLength());
		ofile.close();

		CAtlString cmdArgs;
		cmdArgs.Append(_T("diskpart.exe"));
		cmdArgs.Append(_T(" "));
		cmdArgs.Append(_T("/s"));
		cmdArgs.Append(_T(" "));
		cmdArgs.Append(_T("testcmd.txt"));
		
		TCHAR WinDir[MAX_PATH] = {0};
		if(!GetWindowsDirectory(WinDir,MAX_PATH))
		{
			dwError = GetLastError();
			return dwError;
		}

		CAtlString cmdExePath;
		cmdExePath.Format(_T("%s\\system32\\diskpart.exe"),WinDir);

		dwError = CreatecmdProcess( cmdExePath, cmdArgs );

		return dwError;
		
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::Def Exception caught",gpCFileName,__LINE__);
	}
	return ERROR_BAD_ENVIRONMENT;
}

DWORD OSDPECommonUtils :: MarkDriveActive(CAtlString i_DestDirctory) 
{
	DWORD dwError;
	try
	{
		CHAR TempBuffer[MAX_PATH] = {0};
		CAtlStringA commands = "select volume ";
		if( !i_DestDirctory.IsEmpty()) 
		{
			strcpy_s (TempBuffer,  CStringA(i_DestDirctory).GetString());
			commands.Append(TempBuffer);	
		}
		commands.Append(("\n"));
		commands.Append(("active"));
		commands.Append(("\n"));
		commands.Append(("exit"));

		ofstream ofile("testcmd.txt", ios::out);	
		ofile.write(commands.GetBuffer(),commands.GetLength());
		ofile.close();

		CAtlString cmdArgs;
		cmdArgs.Append(_T("diskpart.exe"));
		cmdArgs.Append(_T(" "));
		cmdArgs.Append(_T("/s"));
		cmdArgs.Append(_T(" "));
		cmdArgs.Append(_T("testcmd.txt"));
		
		TCHAR WinDir[MAX_PATH] = {0};
		if(!GetWindowsDirectory(WinDir,MAX_PATH))
		{
			dwError = GetLastError();
			return dwError;
		}

		CAtlString cmdExePath;
		cmdExePath.Format(_T("%s\\system32\\diskpart.exe"),WinDir);

		dwError = CreatecmdProcess( cmdExePath,cmdArgs );
					
		return dwError;

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::Def Exception caught",gpCFileName,__LINE__);
	}
	return ERROR_BAD_ENVIRONMENT;
}

DWORD OSDPECommonUtils :: SetBootCode(CAtlString i_DestDirctory) 
{
	DWORD dwError;
	try
	{
		TCHAR WinDir[MAX_PATH] = {0};
		if(!GetWindowsDirectory(WinDir,MAX_PATH))
		{
			dwError = GetLastError();
			return dwError;
		}

		CAtlString cmdExePath, cmdArgs;

		cmdExePath.Format(_T("%s\\system32\\bootsect.exe"),WinDir);
		cmdArgs.Format(_T("bootsect.exe /nt60 \"%s\" /force /mbr"),i_DestDirctory.Left(2).GetBuffer());

		dwError = CreatecmdProcess( cmdExePath, cmdArgs);

		return dwError;
	
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>PXECreateBCD::Def Exception caught",gpCFileName,__LINE__);
	}
	return ERROR_BAD_ENVIRONMENT;
}

DWORD OSDPECommonUtils :: CreatecmdProcess(CAtlString cmdExePath, CAtlString cmdArgs)
{
	try
	{
		DWORD dwError = ERROR_SUCCESS; 

		HANDLE hLog = GetNativeCmdLog(OSDNATIVE_CMD_LOG_PATH);

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si,sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		si.dwFlags |= STARTF_USESTDHANDLES;
		si.hStdInput = NULL;
		si.hStdError = hLog;
		si.hStdOutput = hLog;

		ZeroMemory(&pi,sizeof(pi));

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
			dwError = GetLastError();
			CloseHandle(hLog);
			return dwError;
		}
		
		//Wait for the diskpart complete
		WaitForSingleObject( pi.hProcess, INFINITE );

		DWORD retval;
		GetExitCodeProcess(pi.hProcess,LPDWORD(&retval));
		if(retval != ERROR_SUCCESS)
		{
			CloseHandle(hLog);
			return retval ;
		}

		if(NULL != pi.hProcess){
			CloseHandle( pi.hProcess );
			pi.hProcess = NULL;
		}
		if(NULL != pi.hThread){
			CloseHandle( pi.hThread );
			pi.hThread = NULL;
		}
		if(hLog != NULL)
			CloseHandle(hLog);
		
		return ERROR_SUCCESS;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[%S %d]>CreatecmdProcess::std Exception caught (%S)",gpCFileName,__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%S %d]>CreatecmdProcess::Def Exception caught",gpCFileName,__LINE__);
	}
    return ERROR_BAD_ENVIRONMENT;
}