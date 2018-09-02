/*
 * Utils
 * 
 * Copyright(C) 
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "Registry.h"
#include <string.h>
#include "TimeUtils.h"

using namespace std;
//////////////////////////////////////
// Utility macros, defines
//////////////////////////////////////

#define USERMSG		printf
#define ERRMSG		printf
#define DEBUGMSG	printf




#define  REG_HKLM					_T("HKEY_LOCAL_MACHINE")
#define	 REG_AGENT					_T("SOFTWARE\\Zoho\\osd\\agent")
#define	 REG_AGENT_INSTALLDIR		_T("InstallDir")
#define	 RED_AGENT_VERSION			_T("Version")
#define	 RED_AGENT_INSTALL_STATUS	_T("AgentInstallStatus")


#define EXEPATH _T("bin")
#define SEPARATOR _T("\\")

#define OLL_LOG_FOLDER		_T("Logs")
#define ONE_MEGA_BYTE		1*1024*1024

#define OSDNATIVE_CMD_LOG_PATH			 _T("..\\logs\\osdnativecmdlogs.txt")
#define OSD_PE_BCD_BUILDER				 L"BCDCreationPXE.cmd"
 
//////////////////////////////////////
// Utility function prototypes
//////////////////////////////////////

//BOOL ConvertANCItoWideChar(LPTSTR in_Src, WCHAR *out_Dst, INT in_MaxLen);

BYTE* VirtualReMemAlloc(BYTE *i_PrevBuffer, DWORD i_NewSize);
BYTE* VirtualMemAlloc(DWORD i_Size);
BOOL  VirtualMemFree(BYTE *i_Buffer);

void printToCoordinates(int x, int y, string text);
void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
void clear();
int RotateCursor();

CString WriteErrorMsgForID( DWORD msgID );
void printErrorMsgForID( DWORD msgID );

CAtlString GetSizeUnitString( ULONGLONG i_lluBytes ) ;

/*String conversion APIs*/
BOOL ConvertANCIToWideChar(CHAR *in_Src, WCHAR *out_Dst, INT in_MaxLen);
BOOL ConvertWideCharToANCI(WCHAR *in_Src, CHAR *out_Dst, INT in_MaxLen);

BOOL GetRootDirectoryFromPath(LPTSTR io_Root);
BOOL GetDiskFreeSpaceDetails(CAtlString i_Path,
	                         ULARGE_INTEGER &o_FreeBytesAvailable,
	                         ULARGE_INTEGER &o_TotalNumberOfBytes,
							 ULARGE_INTEGER &o_TotalNumberOfFreeBytes);

BOOL IsSpaceAvailable(ULONGLONG i_AvailableSize, ULONGLONG i_RequiredSize);

DWORD GetOSDAgent(CAtlString &strAgentDir);
CAtlStringA ChangeWideStrToUtf8Str(CAtlStringW inStr);
CAtlStringW ChangeUtf8ToWideStr(CAtlStringA inStr);
CAtlStringA ChangeToAnsiStr(CAtlStringW instr);
CAtlStringA ChangeAnsiToUTF8Str(CAtlStringA instr);
void GetLastErrorMessage();
BOOL ParseSharePath(CString i_Path, 
	                CString &o_HostName, 
					CString &o_FolderName, 
					CString &o_SubFolder, 
					CString &o_FileName,
					BOOL    i_IsFile);
void WriteEventLogEntry(CString srcname, PWSTR pszMessage, WORD wType);

INT64 OSD_atoi(CStringA i_String);
BOOL CreateDirectoryRecursive(CString i_Path);

VOID LogCurrentProcessMemroyUsage( );
DWORD GetCurrentProcessID( );
VOID PrintMemoryInfo( DWORD processID );
double GetPercentageValue(ULONGLONG i_TotalValue, double i_PercentNumber);
double GetNumberIntoPercent(ULONGLONG i_TotalValue, ULONGLONG i_currentVolue);

BOOL OSD_WString2Guid(CAtlString src, GUID & o_resGUId);

CString GetFolderName(CAtlString Directory);
HANDLE GetNativeCmdLog(CAtlString FileName);
DWORD DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories = true);

class OSDPECommonUtils
{

public:

	CAtlString	m_PEDirctory;
	CAtlString	m_PEToolPath;
	CAtlString	m_PEArch;
	DWORD		m_dwWinPEver;
	BOOL		m_Is32bit;

	OSDPECommonUtils()
	{
		m_PEDirctory.Empty();
		m_PEToolPath.Empty();
		m_PEArch.Empty();
		m_dwWinPEver = 0;
		m_Is32bit	 = FALSE; 
	}

	~OSDPECommonUtils()
	{}

	BOOL CopyMediaDir(CString SrcDir, CString DestDir);
	BOOL CreateWinPEDirAIK();
	BOOL CreateWinPEDirADK();

	BOOL CopyPXEfilesBeforeUnmount();
	BOOL CopyPXEfilesAfterUnmount();

	BOOL AddApplication();
	DWORD AddDriverDISM(CAtlString Driver);
	DWORD AddDriverPEIMG(CAtlString Driver);

	DWORD PXECreateBCD();
	DWORD SetBootCode(CAtlString i_DestDirctory);
	DWORD MarkDriveActive(CAtlString i_DestDirctory);
	DWORD FormatDisk(CAtlString i_DestDirctory,BOOL IsFormatNTFS);

	DWORD CreatecmdProcess(CAtlString cmdExePath, CAtlString cmdArgs);

};

#endif /*__UTILS_H__*/