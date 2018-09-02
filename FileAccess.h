/*
 * File Access.h 
 * 
 * Copyright(C) 
 */

#ifndef __FILE_ACCESS__
#define __FILE_ACCESS__

namespace OS_Manager
{
/////////////////////////////////////////
//File Access default values
////////////////////////////////////////

typedef enum tagFileDataSources
{
	/*0*/ FILE_ACCESS_LOCAL,
	/*1*/ FILE_ACCESS_NETWORK_DRIVE,
	/*2*/ FILE_ACCESS_DATA_BUFFER

}FILE_DATA_SOURCE_ENUM;


/////////////////////////////////////////
// File Access Structure
////////////////////////////////////////

/////////////////////////////////
//FileAccess class declaration
////////////////////////////////

class CDiskIO
{
private:
	CDiskIO()
	{
	}

	CString m_srcPath;
	CString m_destPath;
	CString m_tempSrc;
	HANDLE m_hsrc;
	HANDLE m_hdest;
	HANDLE m_frag;
	LONGLONG m_fragmentSize = 0; // Configure split write( >0 ) / normal write( 0 )
	BOOL m_headerFlag = FALSE; // Configure header / no header
	BOOL m_readSplit = FALSE; //Configure split read (TRUE) / normal read(FALSE)
	DWORD m_currentFragment = 0; //for split read
	LONGLONG m_currentBytesRemaining = 0; //for split read
	LONGLONG m_bytesWriteRemaining = 0;
	LONGLONG m_bytesReadRemaining = 0;
public:
	static CDiskIO	*m_oDiskIO;

	static CDiskIO *Instance()
	{
		/*If already not created then create the object.*/
		if(ISNULL(CDiskIO::m_oDiskIO))
		{
		    CDiskIO::m_oDiskIO = new CDiskIO();

			if(ISNULL(CDiskIO::m_oDiskIO))
			{
			    INFOLOG(L"[%s %d]>CDiskIO::Unable to create instance\n",__FILE__,__LINE__);
				
				return NULL;
			}
		}
		return CDiskIO::m_oDiskIO;
	}

	HANDLE GetHandle(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref)
	{
		switch(i_Type)
		{
			case FILE_ACCESS_SRC:
			{
				return m_hsrc;
			}
			case FILE_ACCESS_DEST:
			{
				return m_hdest; break;
			}
			default:
			{
				INFOLOG(L"[%s %d]>GetFileAccess:Invalid File Access type(%d)\n",__FILE__,__LINE__,i_Type);

			}break;
		}
		return INVALID_HANDLE_VALUE;
	}
	VOID SetHandle(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, HANDLE i_hFile)
	{
		switch(i_Type)
		{
			case FILE_ACCESS_SRC:
			{
				m_hsrc = i_hFile; break;
			}
			case FILE_ACCESS_DEST:
			{
				m_hdest = i_hFile; break;
			}
			
			default:
			{
				INFOLOG(L"[%s %d]>GetFileAccess:Invalid File Access type(%d)\n",__FILE__,__LINE__,i_Type);

			}break;
		}
	}
	CString GetFileName(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref)
	{
		switch(i_Type)
		{
			case FILE_ACCESS_SRC:
			{
				return this->m_srcPath; break;
			}
			case FILE_ACCESS_DEST:
			{
				return this->m_destPath; break;
			}
			
			default:
			{
				INFOLOG(L"[%s %d]>GetFileAccess:Invalid File Access type(%d)\n",__FILE__,__LINE__,i_Type);

			}break;
		}
		return CString();
	}
	VOID SetFileName(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, CAtlString i_FilePath)
	{
		switch(i_Type)
		{
			case FILE_ACCESS_SRC:
			{
				this->m_srcPath = i_FilePath; break;
			}
			case FILE_ACCESS_DEST:
			{
				m_destPath = i_FilePath; break;
			}
			
			default:
			{
				INFOLOG(L"[%s %d]>GetFileAccess:Invalid File Access type(%d)\n",__FILE__,__LINE__,i_Type);

			}break;
		}
	}

	VOID SetFragmentSize(LONGLONG fragmentSize) {

		m_fragmentSize = fragmentSize;

	}

	BOOL GetHeaderFlag() {

		return m_headerFlag;

	}

	VOID SetHeaderFlag(BOOL b) {

		m_headerFlag = b;

	}

	LONGLONG GetFragmentSize() {

		return m_fragmentSize;

	}

	VOID SetCurrentFragment(DWORD currFragment) {

		m_currentFragment = currFragment;

	}

	DWORD GetCurrentFragment() {

		return m_currentFragment;

	}

	VOID SetCurrentBytesRemaining(LONGLONG currBytesRead) {

		m_currentBytesRemaining = currBytesRead;
	}

	LONGLONG GetCurrentBytesRemaining() {

		return m_currentBytesRemaining;

	}

	VOID SetReadSplitFlag(BOOL b) {

		m_readSplit = b;

	}

	BOOL GetReadSplitFlag() {

		return m_readSplit;

	}

	VOID setFragmentHandle(HANDLE hFrag) {

		m_frag = hFrag;

	}

	HANDLE getFragmentHandle() {

		return m_frag;

	}

	LONGLONG GetWriteBytesRemaining() {
		return m_bytesWriteRemaining;
	}

	VOID SetWriteBytesRemaining(LONGLONG bytesWriteRemaining) {
		m_bytesWriteRemaining = bytesWriteRemaining;
	}

	LONGLONG GetReadBytesRemaining() {
		return m_bytesReadRemaining;
	}

	VOID SetReadBytesRemaining(LONGLONG bytesReadRemaining) {
		m_bytesReadRemaining = bytesReadRemaining;
	}

	VOID SetSourcePath(CString srcPath) {
		m_tempSrc = srcPath;
	}

	CString GetSourcePath() {
		return m_tempSrc;
	}

	BOOL isSplitEnabled() {

		if (m_fragmentSize <= 0) {
			return FALSE;
		}

		else {
			return TRUE;
		}
	}

	BOOL CurrentFragmentFullRead() {

		if (m_bytesReadRemaining <= 0) {
			return TRUE;
		}

		else {
			return FALSE;
		}

	}
	BOOL CurrentFragmentFullWrite() {

		if (m_bytesWriteRemaining <= 0) {
			return TRUE;
		}

		else {
			return FALSE;
		}

	}

};

class IFileAccess
{
public:
	virtual ~IFileAccess();
	IFileAccess(FILE_DATA_SOURCE_ENUM i_DataSource);

	FILE_DATA_SOURCE_ENUM m_DataSource;

	// for FILE_ACCESS_DATA_BUFFER
	PVOID m_ExtDataHandler;

private:
	BOOL	m_PollState;
	
public:
	static IFileAccess* CreateInstance(FILE_DATA_SOURCE_ENUM i_DataSource);

	virtual VOID SetClusterSize(DWORD i_ClusterSize) = 0;
	virtual VOID SetSectorSize(DWORD i_SectorSize) = 0;
	virtual VOID SetSectorPerCluster(DWORD i_SectorPerCluster) = 0;

	virtual DWORD GetClusterSize() = 0;
	virtual DWORD GetSectorSize() = 0;
	virtual DWORD GetSectorPerCluster() = 0;

	virtual BOOL SetWaitEvent() = 0;
	virtual BOOL ResetWaitEvent() = 0;

	virtual HANDLE Create(OS_MANAGER_FILE_FLAG_ENUM i_Flag, 
		                  FILE_ACCESS_TYPE_ENUM i_Type, 
						  INT i_Ref,
						  POSD_RESULT o_Result)=0;

	virtual BOOL ReCreate(FILE_ACCESS_TYPE_ENUM i_Type, 
								 INT i_Ref,
								 LARGE_INTEGER i_curPostion,
								 HANDLE &o_newHandle,
								 POSD_RESULT o_Result) = 0;

	virtual HANDLE CreateEx(OS_MANAGER_FILE_FLAG_ENUM i_Flag,
		                    FILE_ACCESS_TYPE_ENUM i_Type, 
							INT i_Ref,
							POSD_RESULT o_Result)=0;

	virtual BOOL Read( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToRead,
               LPDWORD o_lpNumberOfBytesRead,
               LPOVERLAPPED io_lpOverlapped,
			   POSD_RESULT o_Result)=0;

	virtual BOOL Read_A( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToRead,
               LPOVERLAPPED io_lpOverlapped,
			   LPOVERLAPPED_COMPLETION_ROUTINE i_lpCompletionRoutine,
			   POSD_RESULT o_Result)=0;

	virtual BOOL Write( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToWrite,
               LPDWORD o_lpNumberOfBytesWrite,
               LPOVERLAPPED io_lpOverlapped,
			   POSD_RESULT o_Result)=0;

	virtual BOOL Write_A( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
               LPVOID i_lpBuffer,
               DWORD  i_nNumberOfBytesToWrite,
               LPOVERLAPPED io_lpOverlapped,
			   LPOVERLAPPED_COMPLETION_ROUTINE i_lpCompletionRoutine,
			   POSD_RESULT o_Result)=0;

	virtual DWORD SetFilePointer( FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
		                  LONG   i_lDistanceToMove,
						  PLONG  i_lpDistanceToMoveHigh,
						  DWORD  i_dwMoveMethod,
						  POSD_RESULT o_Result)=0;

	virtual DWORD GetFilePointer( FILE_ACCESS_TYPE_ENUM i_Type, 
		                          INT i_Ref,
								  LONG &o_PosHigh,
		                          POSD_RESULT o_Result)=0;

	virtual DWORD MoveTo(FILE_ACCESS_TYPE_ENUM i_Type, 
		                 INT i_Ref, 
						 ULONGLONG i_NewSizeInBytes,
						 POSD_RESULT o_Result)=0;

	virtual DWORD MoveFromCurrent(FILE_ACCESS_TYPE_ENUM i_Type, 
		                          INT i_Ref, 
								  ULONGLONG i_NewSizeInBytes,
								  POSD_RESULT o_Result)=0;

	virtual DWORD GetFileSize( FILE_ACCESS_TYPE_ENUM i_Type, 
		                       INT i_Ref, 
		                       LPDWORD o_lpFileSizeHigh,
							   POSD_RESULT o_Result)=0;

	virtual BOOL Close(FILE_ACCESS_TYPE_ENUM i_Type, 
		               INT i_Ref,
					   POSD_RESULT o_Result)=0;

	BOOL CalculateSize(DWORD i_Offset, LARGE_INTEGER &o_Size, DWORD i_SectorSize);

	virtual OSD_RESULT Init(DWORD i_PollTimeInSec, DWORD i_TimeoutInSec)=0;
	virtual BOOL IsFilePathExists(FILE_ACCESS_TYPE_ENUM i_Type, 
		                          INT i_Ref,
								  POSD_RESULT o_Result)=0;
	virtual BOOL IsFilePathExists(CString i_fileName,
		                          POSD_RESULT o_Result)=0;

	virtual BOOL SplitFile(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LARGE_INTEGER i_FragmentSize,
		POSD_RESULT o_Result) = 0;

	

	virtual OSD_RESULT StopPendingFileAccess();
	virtual VOID PrintStatistics()=0;

	BOOL GetPollState(){
		return this->m_PollState;
	}
	BOOL SetPollState(BOOL i_State){
		this->m_PollState = i_State;
	}

	PVOID GetDataHandler(){
		return this->m_ExtDataHandler;
	}
	BOOL SetDataHandler(PVOID i_ExtDataHandler){
		this->m_ExtDataHandler = i_ExtDataHandler;
		return TRUE;
	}
};

}
#endif /*__FILE_ACCESS__*/
/*
class FileAccess
{
public:
	FileAccess();
	~FileAccess();
};*/
