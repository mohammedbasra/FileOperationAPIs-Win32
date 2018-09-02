/*
 * Local File Access.h 
 * 
 * Copyright(C) 
 */

#ifndef __LOCAL_FILE_ACCESS__
#define __LOCAL_FILE_ACCESS__

namespace OS_Manager
{
/////////////////////////////////////////
//File Access default values
////////////////////////////////////////
#define CHECKLIST_EVENT_EVENT_AVAILABLE _T("CHECK_LIST_EVENT_AVAILABLE")

#define DEFAULT_RETRY_TIMEOUT_MINUTES	((1000 * 60) * 5)

#define DEFAULT_RETRY_TIMEOUT		(5*1000)
#define DEFAULT_RETRY_COUNT			( DEFAULT_RETRY_TIMEOUT_MINUTES / DEFAULT_RETRY_TIMEOUT )

/////////////////////////////////////////
// Local File Access Enumerations
////////////////////////////////////////

/////////////////////////////////////////
// Local File Access Structure
////////////////////////////////////////


#pragma pack(push)
#pragma pack(1)

#pragma pack(pop)

/////////////////////////////////
//Local FileAccess class declaration
////////////////////////////////

class CLocal_FileAccess : public IFileAccess
{
public:
	CLocal_FileAccess(FILE_DATA_SOURCE_ENUM i_DataSource);
	~CLocal_FileAccess();
	
private:

	HANDLE m_hWaitEvent;
	DWORD dwClusterSize;
	DWORD dwSectorSize;
	DWORD dwSectorPerCluster;
	INT64 m_RetryCount;
	INT64 m_RetryTimeout;
	LONGLONG bytesWritten = LONG_MAX;
	DWORD fragmentNumber = 0;
	LONGLONG tolerance = 1024;
	BOOL ReadCRC(HANDLE hFile,DWORD BytesToRead,DWORD dwClusterSize,DWORD &BytesReadDone,LPVOID i_lpBuffer,DWORD &LastError);
	
public:
	OSD_RESULT Init(DWORD i_PollTimeInSec, DWORD i_TimeoutInSec);
	OSD_RESULT Kill();

	VOID SetClusterSize(DWORD i_ClusterSize);
	VOID SetSectorSize(DWORD i_SectorSize);
	VOID SetSectorPerCluster(DWORD i_SectorPerCluster);
	VOID SetRetryCount(INT64 i_RetryCount);
	VOID SetRetryTimeout(INT64 i_RetryTimeout);
	DWORD GetClusterSize();
	DWORD GetSectorSize();
	DWORD GetSectorPerCluster();
	INT64 GetRetryCount();
	INT64 GetRetryTimeout();

	BOOL SetWaitEvent();
	BOOL ResetWaitEvent();
	
	HANDLE Create(OS_MANAGER_FILE_FLAG_ENUM i_Flag, 
		          FILE_ACCESS_TYPE_ENUM i_Type, 
				  INT i_Ref,
				  POSD_RESULT o_Result);

	BOOL ReCreate(FILE_ACCESS_TYPE_ENUM i_Type, 
								 INT i_Ref,
								 LARGE_INTEGER i_curPostion,
								 HANDLE &o_newHandle,
								 POSD_RESULT o_Result);

	HANDLE CreateEx(OS_MANAGER_FILE_FLAG_ENUM i_Flag,
		                    FILE_ACCESS_TYPE_ENUM i_Type, 
							INT i_Ref,
							POSD_RESULT o_Result);

	BOOL Read_OLD( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToRead,
               LPDWORD o_lpNumberOfBytesRead,
               LPOVERLAPPED io_lpOverlapped,
			   POSD_RESULT o_Result);

	BOOL Read( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToRead,
               LPDWORD o_lpNumberOfBytesRead,
               LPOVERLAPPED io_lpOverlapped,
			   POSD_RESULT o_Result);

	BOOL Read_A( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToRead,
               LPOVERLAPPED io_lpOverlapped,
			   LPOVERLAPPED_COMPLETION_ROUTINE i_lpCompletionRoutine,
			   POSD_RESULT o_Result);

	BOOL ReadBuffer(FILE_ACCESS_TYPE_ENUM i_Type, 
					INT i_Ref,
					LPVOID i_lpBuffer,
					DWORD  i_nNumberOfBytesToRead,
					LPDWORD o_lpNumberOfBytesRead,
					LPOVERLAPPED io_lpOverlapped,
					POSD_RESULT o_Result);

	BOOL Write( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToWrite,
               LPDWORD o_lpNumberOfBytesWrite,
               LPOVERLAPPED io_lpOverlapped,
			   POSD_RESULT o_Result);

	BOOL Write_OLD( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToWrite,
               LPDWORD o_lpNumberOfBytesWrite,
               LPOVERLAPPED io_lpOverlapped,
			   POSD_RESULT o_Result);

	BOOL Write_A( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToWrite,
               LPOVERLAPPED io_lpOverlapped,
			   LPOVERLAPPED_COMPLETION_ROUTINE i_lpCompletionRoutine,
			   POSD_RESULT o_Result);

	DWORD SetFilePointer( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
		                  LONG   i_lDistanceToMove,
						  PLONG  i_lpDistanceToMoveHigh,
						  DWORD  i_dwMoveMethod,
						  POSD_RESULT o_Result);

	DWORD GetFilePointer( FILE_ACCESS_TYPE_ENUM i_Type, 
		                    INT i_Ref,
							LONG &o_PosHigh,
		                    POSD_RESULT o_Result);

	DWORD MoveTo(FILE_ACCESS_TYPE_ENUM i_Type, 
					INT i_Ref, 
					ULONGLONG i_NewSizeInBytes,
					POSD_RESULT o_Result);

	DWORD MoveFromCurrent(FILE_ACCESS_TYPE_ENUM i_Type, 
							INT i_Ref, 
							ULONGLONG i_NewSizeInBytes,
							POSD_RESULT o_Result);

	DWORD GetFileSize( FILE_ACCESS_TYPE_ENUM i_Type, 
						INT i_Ref, 
						LPDWORD o_lpFileSizeHigh,
						POSD_RESULT o_Result);

	BOOL Close(FILE_ACCESS_TYPE_ENUM i_Type, 
		        INT i_Ref,
				POSD_RESULT o_Result);

	BOOL IsFilePathExists(FILE_ACCESS_TYPE_ENUM i_Type,
		                  INT i_Ref,
						  POSD_RESULT o_Result);

	BOOL IsFilePathExists(CString i_fileName,
		                  POSD_RESULT o_Result);

	//utility function for managing write fragments
	BOOL GetFragmentWriteRemaining(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, LARGE_INTEGER &i_BytesRemaining, POSD_RESULT o_Result);
	
	//utility function for managing read fragments
	BOOL GetFragmentReadRemaining(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, LARGE_INTEGER &i_BytesRemaining, POSD_RESULT o_Result);

	//function to split a given file into multiple smaller sized fragments
	BOOL SplitFile(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, LARGE_INTEGER i_FragmentSize,POSD_RESULT o_Result);

	//A helper function to get the fragment paths Ex: (file1.osdi) -> (file1-001.osdi) 
	CAtlString GetNextFragment(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, CAtlString srcPath,
							DWORD currFragment, POSD_RESULT o_Result);

	

	static CLocal_FileAccess* CreateInstance(FILE_DATA_SOURCE_ENUM i_DataSource)
	{
	    return new CLocal_FileAccess(i_DataSource);
	}
	BOOL PollFilePath(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, DWORD i_LastError, ULONGLONG i_RequiredSize=0);
	VOID PrintStatistics();
};
}
#endif /*__LOCAL_FILE_ACCESS__*/

class Local_FileAccess
{
public:
	Local_FileAccess();
	~Local_FileAccess();
};
