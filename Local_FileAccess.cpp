/*
* Local File Access.cpp
*
* Copyright(C)
*/

#include "stdafx.h"
#include "../include/FileAccess.h"
#include "../include/Local_FileAccess.h"
//#include "../OSManagerSplitter/SplitFactory.h"
//#include "DiskIO.h"
//#include "DataHandler.h"
#include "Windows.h"

#define FILE_SHARE_VALID_FLAGS (0x00000007)
using namespace std;
namespace OS_Manager
{
	/////////////////////////////////
	//Global Variables
	/////////////////////////////////
	static const CString gpCFileName = L"Local_FileAccess.cpp";

	static LARGE_INTEGER ReadBytes,
		WriteBytes,
		TotalSrcBytes;

	/////////////////////////////////
	//Local FileAccess class implementation
	////////////////////////////////
	CLocal_FileAccess::CLocal_FileAccess(FILE_DATA_SOURCE_ENUM i_DataSource)
		:IFileAccess(i_DataSource)
	{
		m_RetryCount = DEFAULT_RETRY_COUNT;
		m_RetryTimeout = DEFAULT_RETRY_TIMEOUT;

		m_hWaitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (ISNULL(m_hWaitEvent))
		{
			INFOLOG(L"[%s %d]>CLocal_FileAccess::Failed to Create Wait Event Err(%d)", gpCFileName, __LINE__, GetLastError());
		}
	}

	CLocal_FileAccess::~CLocal_FileAccess()
	{
		OSD_RESULT Result = Kill();

		if (OSD_ERROR_NONE != Result.RetCode)
		{
			INFOLOG(L"[%s %d]>CLocal_FileAccess::Kill failed \n", gpCFileName, __LINE__);
		}
	}

	BOOL CLocal_FileAccess::SetWaitEvent()
	{
		return SetEvent(this->m_hWaitEvent);
	}

	BOOL CLocal_FileAccess::ResetWaitEvent()
	{
		return ResetEvent(this->m_hWaitEvent);
	}

	INT64 CLocal_FileAccess::GetRetryCount()
	{
		return this->m_RetryCount;
	}

	INT64 CLocal_FileAccess::GetRetryTimeout()
	{
		return this->m_RetryTimeout;
	}

	VOID CLocal_FileAccess::SetClusterSize(DWORD i_ClusterSize)
	{
		this->dwClusterSize = i_ClusterSize;
	}
	VOID CLocal_FileAccess::SetSectorSize(DWORD i_SectorSize)
	{
		this->dwSectorSize = i_SectorSize;
	}
	VOID CLocal_FileAccess::SetSectorPerCluster(DWORD i_SectorPerCluster)
	{
		this->dwSectorPerCluster = i_SectorPerCluster;
	}

	DWORD CLocal_FileAccess::GetClusterSize()
	{
		return this->dwClusterSize;
	}
	DWORD CLocal_FileAccess::GetSectorSize()
	{
		return this->dwSectorSize;
	}
	DWORD CLocal_FileAccess::GetSectorPerCluster()
	{
		return this->dwSectorPerCluster;
	}

	OSD_RESULT CLocal_FileAccess::Init(DWORD i_PollTimeInSec, DWORD i_TimeoutInSec)
	{
		OSD_RESULT Result;
		try
		{
			//return SetTimeOut(i_PollTimeInSec,i_TimeoutInSec);
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_FILEACCESS, OSD_ERROR_NONE);

		return Result;
	}

	OSD_RESULT CLocal_FileAccess::Kill()
	{
		OSD_RESULT Result;
		try
		{
			OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_FILEACCESS, OSD_ERROR_NONE);

			return Result;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}

		OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_FILEACCESS, OSD_ERROR_NONE);

		return Result;
	}


	HANDLE CLocal_FileAccess::Create(OS_MANAGER_FILE_FLAG_ENUM i_Flag,
		FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		POSD_RESULT o_Result)
	{
		try {
			CDiskIO *poDiskIO = CDiskIO::Instance();

			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);

				return INVALID_HANDLE_VALUE;
			}
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			DWORD dwDesiredAccess,
				dwShareMode,
				dwCreationDisposition,
				dwFlagsAndAttributes;
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
			HANDLE hTemplateFile = INVALID_HANDLE_VALUE,
				hresFile;

			if (FILE_ACCESS_SRC == i_Type || DISKIO_FILE_READ_ONLY == i_Flag)
			{
				//12Mar2018, Read access is enough to source files which doesn't require write access. If enable write attribute 
				//file creation may fail for physicaldisk
				dwDesiredAccess = GENERIC_READ/* | GENERIC_WRITE*/;
				dwShareMode = FILE_SHARE_VALID_FLAGS;
				dwCreationDisposition = OPEN_EXISTING;
				dwFlagsAndAttributes = /*FILE_ATTRIBUTE_NORMAL*/FILE_ATTRIBUTE_READONLY;
			}
			else
			{
				//During the "Device is Not ready" or "Network error" writeFile get success without any failure. 
				//After analysis if GENERIC_READ commented means it works file and throwing errors 
				dwDesiredAccess = (i_Flag == DISKIO_OPEN_EXISTING) ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_WRITE;
				dwShareMode = (i_Flag == DISKIO_OPEN_EXISTING) ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : FILE_SHARE_READ;
				dwCreationDisposition = (i_Flag == DISKIO_OPEN_EXISTING) ? OPEN_EXISTING : CREATE_ALWAYS;
				dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL; // FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
			}

			hresFile = CreateFile(csFileName,
				dwDesiredAccess,
				dwShareMode,
				lpSecurityAttributes,
				dwCreationDisposition,
				dwFlagsAndAttributes,
				hTemplateFile);

			if (INVALID_HANDLE_VALUE != hresFile) {
				INFOLOG(L"[%s %d]>CLocal_FileAccess::Create() Success File(%s) FileAccess(%d) Ref(%d) \n"
					, gpCFileName,
					__LINE__,
					csFileName,
					i_Type,
					i_Ref);
				
				poDiskIO->SetHandle(i_Type, i_Ref, hresFile);

				if (i_Type == FILE_ACCESS_SRC)
				{
					TotalSrcBytes.LowPart = ::GetFileSize(hresFile, LPDWORD(&TotalSrcBytes.HighPart));
				}
			}
			else
			{

				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);

				POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, ::GetLastError());

				INFOLOG(L"[%s %d]>CLocal_FileAccess::Create()> Unable to create file(%s) ExtError(0x%x)IntError(%d)\n",
					gpCFileName,
					__LINE__,
					csFileName,
					o_Result->SystemRetCode,
					o_Result->RetCode);

				o_Result->ResString = csFileName;
				o_Result->ResString.Append(L" ");
				if (FILE_ACCESS_SRC == i_Type || DISKIO_FILE_READ_ONLY == i_Flag)
				{
					o_Result->ResString.Append(_T(" Read"));
				}
				else
				{
					o_Result->ResString.Append(_T(" Write"));
				}
			}

			return hresFile;
		}
		catch (std::exception &exc)
		{

			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{

			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}

		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);

		return INVALID_HANDLE_VALUE;
	}

	BOOL CLocal_FileAccess::ReCreate(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LARGE_INTEGER i_curPostion,
		HANDLE &o_newHandle,
		POSD_RESULT o_Result)
	{
		BOOL Status = FALSE;
		try
		{
			INFOLOG(L"[%s %d]>CLocal_FileAccess::ReCreate()>Enter PrevHandle(0x%x) PrevPos(%lld)\n",
				gpCFileName,
				__LINE__,
				o_newHandle,
				i_curPostion.QuadPart);

			Status = this->Close(i_Type, i_Ref, o_Result);
			if (!Status)
			{
				INFOLOG(L"[%s %d]>CLocal_FileAccess::ReCreate()>Close failed ExtError(0x%x)IntError(%d) PrevHandle(0x%x) Pos(%lld)\n",
					gpCFileName,
					__LINE__,
					o_Result->SystemRetCode,
					o_Result->RetCode,
					o_newHandle,
					i_curPostion.QuadPart);
			}
			o_newHandle = INVALID_HANDLE_VALUE;
			o_newHandle = this->Create(DISKIO_OPEN_EXISTING, i_Type, i_Ref, o_Result);
			if (o_newHandle == INVALID_HANDLE_VALUE)
			{
				INFOLOG(L"[%s %d]>CLocal_FileAccess::ReCreate()>create failed ExtError(0x%x)IntError(%d)\n",
					gpCFileName,
					__LINE__,
					o_Result->SystemRetCode,
					o_Result->RetCode);
				Status = FALSE;
			}
			else
			{
				this->SetFilePointer(i_Type, i_Ref, i_curPostion.LowPart, &i_curPostion.HighPart, OSD_FILE_BEGIN, o_Result);
				Status = TRUE;

				OSD_RESULT newResult;
				LARGE_INTEGER newPos = { 0 };
				newPos.LowPart = this->GetFilePointer(i_Type, i_Ref, newPos.HighPart, &newResult);

				INFOLOG(L"[%s %d]>CLocal_FileAccess::ReCreate()>Success ExtError(0x%x) IntError(%d) NewHandle(0x%x) PrevPos(%lld) NewPos(%lld)\n",
					gpCFileName,
					__LINE__,
					o_Result->SystemRetCode,
					o_Result->RetCode,
					o_newHandle,
					i_curPostion.QuadPart,
					newPos.QuadPart);
			}

			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}

		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);

		return FALSE;
	}

	HANDLE CLocal_FileAccess::CreateEx(OS_MANAGER_FILE_FLAG_ENUM i_Flag,
		FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		POSD_RESULT o_Result)
	{
		try {
			CDiskIO *poDiskIO = CDiskIO::Instance();

			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);

				return INVALID_HANDLE_VALUE;
			}
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			DWORD dwDesiredAccess,
				dwShareMode,
				dwCreationDisposition,
				dwFlagsAndAttributes;
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
			HANDLE hTemplateFile = INVALID_HANDLE_VALUE,
				hresFile;

			if (FILE_ACCESS_SRC == i_Type || DISKIO_FILE_READ_ONLY == i_Flag)
			{
				dwDesiredAccess = GENERIC_READ;
				dwShareMode = FILE_SHARE_READ;
				dwCreationDisposition = OPEN_EXISTING;
				dwFlagsAndAttributes = FILE_ATTRIBUTE_READONLY | FILE_FLAG_OVERLAPPED;
			}
			else
			{
				dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
				dwShareMode = FILE_SHARE_READ;
				dwCreationDisposition = (i_Flag == DISKIO_OPEN_EXISTING) ? OPEN_EXISTING : CREATE_ALWAYS;
				dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
			}
			hresFile = ::CreateFile(csFileName,
				dwDesiredAccess,
				dwShareMode,
				lpSecurityAttributes,
				dwCreationDisposition,
				dwFlagsAndAttributes,
				hTemplateFile);

			if (INVALID_HANDLE_VALUE != hresFile) {
				poDiskIO->SetHandle(i_Type, i_Ref, hresFile);
			}
			else
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
				POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, ::GetLastError());
			}

			return hresFile;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);

		return INVALID_HANDLE_VALUE;
	}

	BOOL CLocal_FileAccess::ReadBuffer(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LPVOID i_lpBuffer,
		DWORD  i_nNumberOfBytesToRead,
		LPDWORD o_lpNumberOfBytesRead,
		LPOVERLAPPED io_lpOverlapped,
		POSD_RESULT o_Result)
	{
		DWORD LastError = ERROR_SUCCESS;
		BOOL Status = FALSE;
		try
		{
#if 0
			//Added for FILE_ACCESS_DATA_BUFFER - Multicast handling
			if (ISNULL(this->GetDataHandler()))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_INVALID_PARAMETER);

				return FALSE;
			}
			DWORD redSize = 0;
			CDataHandler *poDataHandler = (CDataHandler*)this->GetDataHandler();

			poDataHandler->GetData(PVOID(i_lpBuffer), i_nNumberOfBytesToRead, redSize);
			*o_lpNumberOfBytesRead = redSize;

			if (i_nNumberOfBytesToRead != redSize)
			{
				LastError = ::GetLastError();
				INFOLOG(L"[%s %d]>>%S() DataHandler Read error in(%lu) out(%lu) ourout(%lu) LError(%x)\n"
					, gpCFileName
					, __LINE__
					, __FUNCTION__,
					i_nNumberOfBytesToRead,
					*o_lpNumberOfBytesRead,
					redSize,
					LastError);

				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_READ_FAILED);

				return FALSE;
			}
			POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
#endif

			return TRUE;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}

	BOOL CLocal_FileAccess::ReadCRC(HANDLE hFile, DWORD BytesToRead, DWORD dwClusterSize, DWORD &BytesReadDone, LPVOID i_lpBuffer, DWORD &LastError)
	{
		try
		{
			BOOL breakLoop = FALSE;
			DWORD ReadLimit = BytesToRead;
			LastError = 0;
			//DWORD dwBytesReadDone = 0;
			//DWORD dwClusterSize = this->GetClusterSize();
			while (TRUE)
			{
				//Info:checking for read limit
				ReadLimit = BytesToRead - BytesReadDone;

				DWORD dwBytesToRead = 0;

				if (ReadLimit > dwClusterSize)
				{
					dwBytesToRead = dwClusterSize;
				}
				else
				{
					dwBytesToRead = ReadLimit;
					breakLoop = TRUE;
				}

				DWORD dwo_lpNumberOfBytesRead = 0;

				BOOL Status = ::ReadFile(hFile,
					LPVOID(PBYTE(i_lpBuffer) + BytesReadDone),
					dwBytesToRead,
					&dwo_lpNumberOfBytesRead,
					NULL);

				if (Status)
				{
					BytesReadDone += dwo_lpNumberOfBytesRead;
				}
				else
				{
					DWORD dwLastError = ::GetLastError();

					INFOLOG(L"[%S %d]>>%S() Read Failed in(%lu) out(%lu) ourout(%lu) LError(0x%x)",
						gpCFileName, __LINE__, __FUNCTION__,
						dwBytesToRead,
						dwo_lpNumberOfBytesRead,
						BytesReadDone,
						dwLastError);

					if (ERROR_CRC == dwLastError)
					{
						memset((PBYTE(i_lpBuffer) + BytesReadDone), '\0', dwClusterSize);
						BytesReadDone += dwClusterSize;
						LARGE_INTEGER filePointer;

						filePointer.QuadPart = (ULONGLONG(dwClusterSize));

						DWORD Ret = ::SetFilePointer(hFile,
							filePointer.LowPart,
							&filePointer.HighPart,
							FILE_CURRENT);

						DWORD dwLastError = ::GetLastError();
						if (Ret == INVALID_SET_FILE_POINTER)
						{
							INFOLOG(L"[%S %d]>>%S() CRC ERROR : Failed to set pointer Error(%d)", gpCFileName, __LINE__, __FUNCTION__, dwLastError);
						}
						else
						{
							INFOLOG(L"[%S %d]>>%S() CRC ERROR : File pointer set : %d", gpCFileName, __LINE__, __FUNCTION__, Ret);
						}
					}
					else if (ERROR_CRC != dwLastError)
					{
						INFOLOG(L"[%S %d]>>%S() Failed with error : %d", gpCFileName, __LINE__, __FUNCTION__, dwLastError);
						LastError = dwLastError;
						return FALSE;
					}

				}

				if (breakLoop)
				{
					return TRUE;
				}
			}

			return TRUE;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		return FALSE;
	}

	CAtlString CLocal_FileAccess::GetNextFragment(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, CAtlString srcPath, DWORD currFragment, POSD_RESULT o_Result) {


		HANDLE hFile;
		CDiskIO *poDiskIO = CDiskIO::Instance();
		if (ISNULL(poDiskIO))
		{
			POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
			ERRORLOG(L"[%s %d]>>%S()Unable to Create DiskIO Instance... LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, ::GetLastError());
		}
		//21 -> .021 
		int temp = currFragment;
		CAtlString a;
		a.Format(_T("%d"), temp);
		int l = a.GetLength();
		if (l == 1)a.Format(_T("-00%d"),temp);
		else if (l == 2)a.Format(_T("-0%d"), temp);
		else if (l >= 3)a.Format(_T("-%d"), temp);
		CAtlString tempDest = srcPath;
		tempDest += a;
		poDiskIO->SetFileName(i_Type, i_Ref, tempDest);
		return tempDest;
	}


	BOOL CLocal_FileAccess::Read(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref,
		LPVOID i_lpBuffer,
		DWORD  i_nNumberOfBytesToRead,
		LPDWORD o_lpNumberOfBytesRead,
		LPOVERLAPPED io_lpOverlapped,
		POSD_RESULT o_Result)
	{
		DWORD LastError = ERROR_SUCCESS;
		LARGE_INTEGER CurPosition, readRemaining;
		BOOL SendOnce = TRUE,
			Status = FALSE,
			Header;
		try {


			if (FILE_ACCESS_DATA_BUFFER == this->m_DataSource)
			{
				return this->ReadBuffer(i_Type,
					i_Ref,
					i_lpBuffer,
					i_nNumberOfBytesToRead,
					o_lpNumberOfBytesRead,
					io_lpOverlapped,
					o_Result);
			}

			CDiskIO *poDiskIO = CDiskIO::Instance();
			if (ISNULL(poDiskIO))
			{
				*o_lpNumberOfBytesRead = 0;
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return FALSE;
			}
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			INT64 LocRetryCount = GetRetryCount();
			DWORD BytesReadDone = 0;
			DWORD BytesToRead = 0;
			LONGLONG totalBytesRead = 0;
			DWORD tempNumberOfBytesToRead = i_nNumberOfBytesToRead;
			DWORD tempNumberOfBytesRead = 0;// = o_lpNumberOfBytesWrite;
			LONGLONG bytesReadRemaining; // Stores the number of bytes yet to be read from the current fragment in case the image is split in fragments

			ResetEvent(m_hWaitEvent);

			if (poDiskIO->GetReadSplitFlag() == FALSE) {//normal read

				tempNumberOfBytesToRead = i_nNumberOfBytesToRead;

			}

			else {//fragmented read..
				if (FALSE == this->GetFragmentReadRemaining(i_Type, i_Ref, readRemaining, o_Result)) {

					ERRORLOG(L"[%s %d]>>%S() Call to Get Fragment Read Remaining Failed File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);
					POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_CALL_FAILURE);
					return FALSE;
				}

				tempNumberOfBytesToRead = readRemaining.QuadPart;
				
				bytesReadRemaining = readRemaining.QuadPart;
				
				if (bytesReadRemaining >= i_nNumberOfBytesToRead) {
					tempNumberOfBytesToRead = i_nNumberOfBytesToRead;
				}
				
				else {
					tempNumberOfBytesToRead = bytesReadRemaining;
				}


			}

			//outer while loop maintains overall bytes read as well as retry count
			while (totalBytesRead < i_nNumberOfBytesToRead && LocRetryCount > 0) {

				hFile = poDiskIO->GetHandle(i_Type, 0);

				//update BytesReadDone to Zero after every write if fragments are split
 				if (poDiskIO->isSplitEnabled() == TRUE)BytesReadDone = 0;

				while (GetPollState() && LocRetryCount > 0)
				{
					

					if (hFile != INVALID_HANDLE_VALUE) {
						Status = ::ReadFile(hFile,
							LPVOID(PBYTE(i_lpBuffer) + totalBytesRead),
							tempNumberOfBytesToRead,
							&tempNumberOfBytesRead,
							io_lpOverlapped);

						BytesReadDone += tempNumberOfBytesRead;
						totalBytesRead += tempNumberOfBytesRead;


						CurPosition.LowPart = this->GetFilePointer(i_Type, i_Ref, CurPosition.HighPart, o_Result);
						LastError = ::GetLastError();

						if (tempNumberOfBytesRead != tempNumberOfBytesToRead)
						{
							//LastError = ::GetLastError();
							INFOLOG(L"[%S %d]>>%S() Read Failed in(%lu) out(%lu) ourout(%lu) LError(0x%x)"
								, gpCFileName, __LINE__, __FUNCTION__,
								BytesToRead,
								*o_lpNumberOfBytesRead,
								BytesReadDone,
								LastError);

							if (ERROR_CRC == LastError)
							{
								Status = ReadCRC(hFile, BytesToRead, this->GetClusterSize(), BytesReadDone, i_lpBuffer, LastError);
								ONELINELOG(L"[%d] CRC Error Occured", __LINE__);
							}
						}

						if (Status == FALSE)
						{
							LastError = ::GetLastError();
							//BytesReadDone += *o_lpNumberOfBytesRead;
							--LocRetryCount;
							o_Result->ResString = csFileName;
							POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
							POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

							if (SendOnce) {
#if 0
								CResponseHandler *poResponseHandler = CResponseHandler::Instance();
								if (ISNULL(poResponseHandler))
								{
									ERRORLOG(L"[%s %d]>Read :: invalid Response handler", gpCFileName, __LINE__);
								}

								DWORD dwResult = poResponseHandler->SendResponse(APPLICATION_RESP_RETRYING, *o_Result).RetCode;
								if (OSD_ERROR_NONE != dwResult)
								{
									ERRORLOG(L"[%s %d]>Read::SendResponse Failed File(%s) Resp(%d) Ret(%d)\n",
										gpCFileName,
										__LINE__,
										csFileName,
										APPLICATION_RESP_RETRYING,
										dwResult);
								}
#endif
								SendOnce = FALSE;
							}
							INFOLOG(L"[%S %d]>>%S() Read Failed Start Polling... LError(0x%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);
						}
						else
						{

							if (poDiskIO->GetReadSplitFlag() == FALSE) {
								*o_lpNumberOfBytesRead = BytesReadDone;
							}

							else {
								*o_lpNumberOfBytesRead += BytesReadDone;
								poDiskIO->SetReadBytesRemaining(poDiskIO->GetReadBytesRemaining() - tempNumberOfBytesRead);

								if (totalBytesRead < i_nNumberOfBytesToRead) {

									if (FALSE == this->GetFragmentReadRemaining(i_Type, i_Ref, readRemaining, o_Result)) {

										ERRORLOG(L"[%s %d]>>%S() Call to Get Fragment Read Remaining Failed File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);
										POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_CALL_FAILURE);
										break;
									}
									tempNumberOfBytesToRead = readRemaining.QuadPart;
									
								}

								if (tempNumberOfBytesToRead >(i_nNumberOfBytesToRead - totalBytesRead)) {
									tempNumberOfBytesToRead = i_nNumberOfBytesToRead - totalBytesRead;
								}

							}

							POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);

							break;
						}
					}

					WaitForSingleObject(m_hWaitEvent, (DWORD)GetRetryTimeout());

					//Added for recreation
					OSD_RESULT recResult;
					ReCreate(i_Type, i_Ref, CurPosition, hFile, &recResult);
				}
			}

			if (i_Type == FILE_ACCESS_SRC)
			{
				ReadBytes.QuadPart = ReadBytes.QuadPart + *o_lpNumberOfBytesRead;
			}
			else if (i_Type == FILE_ACCESS_DEST)
			{
				//throw std::exception("Invalid read operation");
			}

			return Status;
		}

		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}




	BOOL CLocal_FileAccess::Read_OLD(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LPVOID i_lpBuffer,
		DWORD  i_nNumberOfBytesToRead,
		LPDWORD o_lpNumberOfBytesRead,
		LPOVERLAPPED io_lpOverlapped,
		POSD_RESULT o_Result)
	{
		DWORD LastError = ERROR_SUCCESS;
		BOOL TryOnce = FALSE,
			Status = FALSE;
		try {

			if (FILE_ACCESS_DATA_BUFFER == this->m_DataSource)
			{
				return this->ReadBuffer(i_Type,
					i_Ref,
					i_lpBuffer,
					i_nNumberOfBytesToRead,
					o_lpNumberOfBytesRead,
					io_lpOverlapped,
					o_Result);
			}

			CDiskIO *poDiskIO = CDiskIO::Instance();
			if (ISNULL(poDiskIO))
			{
				*o_lpNumberOfBytesRead = 0;
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return FALSE;
			}
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			DWORD BytesReadDone = 0;
			DWORD BytesToRead = 0;
			while (GetPollState())
			{
				BytesToRead = (i_nNumberOfBytesToRead - BytesReadDone);
				Status = ::ReadFile(hFile,
					LPVOID(PBYTE(i_lpBuffer) + BytesReadDone),
					BytesToRead,
					o_lpNumberOfBytesRead,
					io_lpOverlapped);

				BytesReadDone += *o_lpNumberOfBytesRead;

				if (BytesToRead != *o_lpNumberOfBytesRead)
				{
					LastError = ::GetLastError();
					INFOLOG(L"[%S %d]>>%S() Read Failed in(%lu) out(%lu) ourout(%lu) LError(0x%x)"
						, gpCFileName, __LINE__, __FUNCTION__,
						BytesToRead,
						*o_lpNumberOfBytesRead,
						BytesReadDone,
						LastError);

					if (ERROR_CRC == LastError)
					{
						Status = ReadCRC(hFile, BytesToRead, this->GetClusterSize(), BytesReadDone, i_lpBuffer, LastError);
					}
				}

				if (Status == FALSE)
				{
					LastError = ::GetLastError();
					BytesReadDone += *o_lpNumberOfBytesRead;

					//Break the polling for second time
					if (TryOnce) {

						INFOLOG(L"[%S %d]>>%S() Read Failed Poll Timed out LError(0x%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);

						*o_lpNumberOfBytesRead = BytesReadDone;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

						break;
					}
					INFOLOG(L"[%S %d]>>%S() Read Failed Start Polling... LError(0x%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);

					//One time polling until given timeout
					BOOL PollResult = PollFilePath(i_Type, i_Ref, LastError);
					if (FALSE == PollResult)
					{
						Status = FALSE;
						*o_lpNumberOfBytesRead = BytesReadDone;

						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

						break;
					}
					else
					{
						TryOnce = TRUE;
					}
				}
				else
				{
					*o_lpNumberOfBytesRead = BytesReadDone;

					POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);

					break;
				}
			}

			if (i_Type == FILE_ACCESS_SRC)
			{
				ReadBytes.QuadPart = ReadBytes.QuadPart + *o_lpNumberOfBytesRead;
			}
			else if (i_Type == FILE_ACCESS_DEST)
			{
				//throw std::exception("Invalid read operation");
			}

			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}

	BOOL CLocal_FileAccess::Read_A(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LPVOID i_lpBuffer,
		DWORD  i_nNumberOfBytesToRead,
		LPOVERLAPPED io_lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE i_lpCompletionRoutine,
		POSD_RESULT o_Result)
	{
		DWORD LastError = ERROR_SUCCESS;
		BOOL  Status = FALSE;
		try {
			CDiskIO *poDiskIO = CDiskIO::Instance();

			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return FALSE;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);

			Status = ::ReadFileEx(hFile,
				i_lpBuffer,
				i_nNumberOfBytesToRead,
				io_lpOverlapped,
				i_lpCompletionRoutine);

			if (FALSE == Status)
			{
				LastError = ::GetLastError();
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
				POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);
			}
			else
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
			}
			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}
	/*
	BOOL CLocal_FileAccess::MergeCheck(DWORD i_FragmentCount, CString i_fileName) {

	OSD_RESULT o_Result;
	CString fragmentName;
	BOOL fileExists, check = TRUE;

	for (INT i = 1; i < i_FragmentCount; i++) {

	fragmentName = getFragmentName(i_fileName, i);

	if (IsFilePathExists(fragmentName, &o_Result)) {

	INFOLOG(L"[%s] File Fragment (%d) is missing...\n", fragmentName, fragmentNumber);
	check = FALSE;

	}
	}

	return check;

	}
	*/

	BOOL CLocal_FileAccess::GetFragmentWriteRemaining(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, LARGE_INTEGER &i_BytesRemaining, POSD_RESULT o_Result) {

		//OSD_RESULT o_Result;
		try {
			HANDLE hFile;
			DWORD currFragment;
			//LARGE_INTEGER bytesRemaining;
			BOOL ValidFile = FALSE;
			DWORD RetryCount = GetRetryCount();
			DWORD headerWritten = 0;
			CDiskIO *poDiskIO = CDiskIO::Instance();
			FILE_HEADER fileHeader;
			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return FALSE;
			}
			i_BytesRemaining.QuadPart = poDiskIO->GetWriteBytesRemaining();
			currFragment = poDiskIO->GetCurrentFragment();
			if (poDiskIO->CurrentFragmentFullWrite()) {

				currFragment++;
				poDiskIO->SetCurrentFragment(currFragment);
				CAtlString str = GetNextFragment(i_Type, i_Ref, poDiskIO->GetFileName(FILE_ACCESS_SRC, 1), currFragment, o_Result);

				while (!ValidFile && RetryCount > 0) {

					hFile = this->Create(DISKIO_CREATE_ALWAYS, i_Type, i_Ref, o_Result);
					if (hFile == INVALID_HANDLE_VALUE) {
						ERRORLOG(L"[%s %d]>>%S() File Fragment Creation Failed. Retrying...(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, str, ::GetLastError());
						RetryCount--;
					}
					poDiskIO->setFragmentHandle(hFile);
					poDiskIO->SetHandle(i_Type, i_Ref, hFile);
					poDiskIO->SetWriteBytesRemaining(poDiskIO->GetFragmentSize());
					ValidFile = TRUE;
					//RetryCount = GetRetryCount(); Restoring RetryCount in case of successful read.
				}
			
			}
			else {
				hFile = poDiskIO->getFragmentHandle();
				if (hFile != INVALID_HANDLE_VALUE) {
					poDiskIO->SetHandle(i_Type, i_Ref, hFile);
					ValidFile = TRUE;
				}
			}


			if (ValidFile == TRUE) {

				i_BytesRemaining.QuadPart = poDiskIO->GetWriteBytesRemaining();
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SUCCESS);
				return TRUE;
			}


			POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
			POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, ::GetLastError());
			return FALSE;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}
	 
	BOOL CLocal_FileAccess::GetFragmentReadRemaining(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, LARGE_INTEGER &i_BytesRemaining, POSD_RESULT o_Result) {

		try{
		HANDLE hFile;
		DWORD currFragment;
		//LARGE_INTEGER bytesRemaining;
		LARGE_INTEGER fileSize;
		BOOL ValidFile = FALSE;
		DWORD RetryCount = GetRetryCount();
		CDiskIO *poDiskIO = CDiskIO::Instance();

		if (ISNULL(poDiskIO))
		{
			POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
			return FALSE;
		}
		currFragment = poDiskIO->GetCurrentFragment();
		i_BytesRemaining.QuadPart = poDiskIO->GetReadBytesRemaining();

		//If the current fragment has been read completely, increment fragment counter and start reading newe fragment
		if (poDiskIO->CurrentFragmentFullRead()) {

			currFragment++;
			poDiskIO->SetCurrentFragment(currFragment);

			CAtlString str = GetNextFragment(i_Type,
				i_Ref,
				poDiskIO->GetSourcePath(),
				currFragment,
				o_Result);

			while (!ValidFile && RetryCount > 0) {

				hFile = this->Create(DISKIO_OPEN_EXISTING, i_Type, i_Ref, o_Result);
				
				if (hFile == INVALID_HANDLE_VALUE) {
					ERRORLOG(L"[%s %d]>>%S() File Fragment Creation Failed. Retrying...(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, str, ::GetLastError());
					RetryCount--;
					continue;
				}

				poDiskIO->setFragmentHandle(hFile);

				poDiskIO->SetHandle(i_Type, i_Ref, hFile);

				fileSize.LowPart = this->GetFileSize(i_Type,
					i_Ref,
					(LPDWORD)&fileSize.HighPart,
					o_Result);

				ValidFile = TRUE;
				RetryCount = GetRetryCount();
				poDiskIO->SetReadBytesRemaining(fileSize.QuadPart);
			}
		}

		// Continue reading from last read point of current fragment
		else {
			hFile = poDiskIO->getFragmentHandle();
			if (hFile != INVALID_HANDLE_VALUE) {
				ValidFile = TRUE;
				poDiskIO->SetHandle(i_Type, i_Ref, hFile);
			}
		}

		//Error handling to check if fragment has been created for read properly
		if (ValidFile == TRUE) {
			i_BytesRemaining.QuadPart = poDiskIO->GetReadBytesRemaining();
			//o_PosHigh = bytesRemaining.HighPart;
			POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SUCCESS);
			return TRUE;
		}

		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
		POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, ::GetLastError());
	}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}

	BOOL CLocal_FileAccess::Write(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LPVOID i_lpBuffer,
		DWORD  i_nNumberOfBytesToWrite,
		LPDWORD o_lpNumberOfBytesWrite,
		LPOVERLAPPED io_lpOverlapped,
		POSD_RESULT o_Result)

	{

		try {
			*o_lpNumberOfBytesWrite = 0;
			CDiskIO *poDiskIO = CDiskIO::Instance();
			BOOL Status = FALSE, SendOnce = TRUE;
			LARGE_INTEGER CurPosition;
			DWORD BytesWriteDone = 0;
			DWORD tempNumberOfBytesToWrite = i_nNumberOfBytesToWrite;
			DWORD tempNumberOfBytesWrite;// = o_lpNumberOfBytesWrite;
			LONGLONG bytesWriteRemaining;
			LARGE_INTEGER writeRemaining; // Stores the number of bytes remaining to be written on the current fragment
			DWORD LastError = ERROR_SUCCESS;
			LONGLONG totalBytesWritten = 0;

			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return FALSE;
			}



			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			CAtlString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			INT64 LocRetryCount = GetRetryCount();
			o_Result->ResString.Append(csFileName);
			ResetEvent(m_hWaitEvent);

			if (poDiskIO->isSplitEnabled() == FALSE) {//normal write without fragmentation

				tempNumberOfBytesToWrite = i_nNumberOfBytesToWrite;

			}

			else { // Write with fragmentation

				if (FALSE == this->GetFragmentWriteRemaining(i_Type, i_Ref, writeRemaining, o_Result)) {

					ERRORLOG(L"[%s %d]>>%S() Call to Get Fragment Write Remaining Failed File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);
					//POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_CALL_FAILURE);
					return FALSE;
				}
				bytesWriteRemaining = writeRemaining.QuadPart;

				if (bytesWriteRemaining >= i_nNumberOfBytesToWrite) {

					tempNumberOfBytesToWrite = i_nNumberOfBytesToWrite;

				}
				else {

					tempNumberOfBytesToWrite = bytesWriteRemaining;

				}

			}
			
			//outer while loop checks for retry count and whether all bytes from the buffer have been written onto the destination fragments/files
			while (totalBytesWritten < i_nNumberOfBytesToWrite && LocRetryCount > 0) {
 
				hFile = poDiskIO->GetHandle(i_Type, i_Ref);

				// In case of fragmented write BytesWriteDone is updated to 0 after every write
				if (poDiskIO->isSplitEnabled() == TRUE)BytesWriteDone = 0;

				while (this->GetPollState() && LocRetryCount > 0)
				{
					
					
					if (hFile != INVALID_HANDLE_VALUE) {
						Status = ::WriteFile(hFile,
							LPVOID(PBYTE(i_lpBuffer) + totalBytesWritten),
							(tempNumberOfBytesToWrite - BytesWriteDone),
							&tempNumberOfBytesWrite,
							io_lpOverlapped);

						BytesWriteDone += tempNumberOfBytesWrite;
						totalBytesWritten += tempNumberOfBytesWrite;


						//::SetLastError(0);
						//::FlushFileBuffers(hFile);

						//LastError = ::GetLastError();

						/*if(Status)
						{
						DEBUGLOG(L"[%s %d]>>%S() Write Success LError(%x)\n",gpCFileName,__LINE__, __FUNCTION__,LastError);
						}
						else
						{
						DEBUGLOG(L"[%s %d]>>%S() Write Failed LError(%x)\n",gpCFileName,__LINE__, __FUNCTION__,LastError);
						}
						*/

						CurPosition.LowPart = this->GetFilePointer(i_Type, i_Ref, CurPosition.HighPart, o_Result);
						if (Status == FALSE)// || 0 != LastError)
						{
							LastError = ::GetLastError();
							--LocRetryCount;
							ERRORLOG(L"[%s %d]>>%S() Write Failed File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);

							o_Result->ResString = csFileName;
							POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
							POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

							if (SendOnce) {
#if 0
								CResponseHandler *poResponseHandler = CResponseHandler::Instance();
								if (ISNULL(poResponseHandler))
								{
									ERRORLOG(L"[%s %d]>Read :: invalid Response handler", gpCFileName, __LINE__);
								}

								DWORD dwResult = poResponseHandler->SendResponse(APPLICATION_RESP_RETRYING, *o_Result).RetCode;
								if (OSD_ERROR_NONE != dwResult)
								{
									ERRORLOG(L"[%s %d]>Read::SendResponse Failed File(%s) Resp(%d) Ret(%d)\n",
										gpCFileName,
										__LINE__,
										csFileName,
										APPLICATION_RESP_RETRYING,
										dwResult);
								}
#endif

								SendOnce = FALSE;
							}
							ERRORLOG(L"[%s %d]>>%S() Write Failed Start Polling... File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);
						}
						else
						{
							//normal write procedure..
							if (poDiskIO->isSplitEnabled() == FALSE) {

								*o_lpNumberOfBytesWrite = BytesWriteDone;

							}

							//fragmented write procdure...
							else {
								*o_lpNumberOfBytesWrite += BytesWriteDone;
								poDiskIO->SetWriteBytesRemaining(poDiskIO->GetWriteBytesRemaining() - tempNumberOfBytesWrite);
								
								if (totalBytesWritten < i_nNumberOfBytesToWrite) {

									if (FALSE == this->GetFragmentWriteRemaining(i_Type, i_Ref, writeRemaining, o_Result)) {

										ERRORLOG(L"[%s %d]>>%S() Call to Get Fragment Write Remaining Failed File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);
										POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_CALL_FAILURE);
										break;
									}
									tempNumberOfBytesToWrite = writeRemaining.QuadPart;
								}

								if (tempNumberOfBytesToWrite >(i_nNumberOfBytesToWrite - totalBytesWritten)) {
									tempNumberOfBytesToWrite = i_nNumberOfBytesToWrite - totalBytesWritten;
								}
							}
							POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SUCCESS);

							break;


						}

					}

					WaitForSingleObject(m_hWaitEvent, (DWORD)GetRetryTimeout());

					//Added for recreation
					OSD_RESULT recResult;
					ReCreate(i_Type, i_Ref, CurPosition, hFile, &recResult);

				}
			}


			if (i_Type == FILE_ACCESS_DEST)
			{
				WriteBytes.QuadPart = WriteBytes.QuadPart + *o_lpNumberOfBytesWrite;
			}
			else if (i_Type == FILE_ACCESS_SRC)
			{
				throw std::exception("Invalid read operation");
			}

			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}

	BOOL CLocal_FileAccess::Write_OLD(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LPVOID i_lpBuffer,
		DWORD  i_nNumberOfBytesToWrite,
		LPDWORD o_lpNumberOfBytesWrite,
		LPOVERLAPPED io_lpOverlapped,
		POSD_RESULT o_Result)
	{
		try {
			BOOL Status = FALSE,
				TryOnce = FALSE;
			CDiskIO *poDiskIO = CDiskIO::Instance();
			DWORD LastError = ERROR_SUCCESS;
			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);

				return FALSE;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			DWORD BytesWriteDone = 0;
			while (this->GetPollState())
			{
				Status = ::WriteFile(hFile,
					LPVOID(PBYTE(i_lpBuffer) + BytesWriteDone),
					(i_nNumberOfBytesToWrite - BytesWriteDone),
					o_lpNumberOfBytesWrite,
					io_lpOverlapped);

				BytesWriteDone += *o_lpNumberOfBytesWrite;

				if (Status == FALSE)
				{
					LastError = ::GetLastError();
					BytesWriteDone += *o_lpNumberOfBytesWrite;

					ERRORLOG(L"[%s %d]>>%S() Write Failed File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);

					//Break the polling for second time
					if (TryOnce) {

						ERRORLOG(L"[%s %d]>>%S() Write Failed Poll Timed out File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);

						*o_lpNumberOfBytesWrite = BytesWriteDone;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

						break;
					}
					ERRORLOG(L"[%s %d]>>%S() Write Failed Start Polling... File(%s) LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, csFileName, LastError);

					//One time polling until given timeout
					BOOL PollResult = PollFilePath(i_Type, i_Ref, LastError, (i_nNumberOfBytesToWrite - BytesWriteDone));
					if (FALSE == PollResult)
					{
						Status = FALSE;
						*o_lpNumberOfBytesWrite = BytesWriteDone;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);
						break;
					}
					else
					{
						TryOnce = TRUE;
					}
				}
				else
				{
					*o_lpNumberOfBytesWrite = BytesWriteDone;

					POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SUCCESS);

					break;
				}
			}

			if (i_Type == FILE_ACCESS_DEST)
			{
				WriteBytes.QuadPart = WriteBytes.QuadPart + *o_lpNumberOfBytesWrite;
			}
			else if (i_Type == FILE_ACCESS_SRC)
			{
				throw std::exception("Invalid read operation");
			}

			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}

	BOOL CLocal_FileAccess::Write_A(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LPVOID i_lpBuffer,
		DWORD  i_nNumberOfBytesToWrite,
		LPOVERLAPPED io_lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE i_lpCompletionRoutine,
		POSD_RESULT o_Result)
	{
		try
		{
			CDiskIO *poDiskIO = CDiskIO::Instance();
			DWORD LastError = ERROR_SUCCESS;
			BOOL Status = FALSE;
			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);

				return FALSE;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);

			Status = ::WriteFileEx(hFile,
				i_lpBuffer,
				i_nNumberOfBytesToWrite,
				io_lpOverlapped,
				i_lpCompletionRoutine);

			if (Status == FALSE)
			{
				LastError = ::GetLastError();
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
				POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);
			}
			else
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
			}
			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}

	//API for splitting an existing OS Image
	BOOL CLocal_FileAccess::SplitFile(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, LARGE_INTEGER i_FragmentSize, POSD_RESULT o_Result) {

		try {
			HANDLE hFile;
			LARGE_INTEGER file;
			LONGLONG fileSize;
			DWORD LastError = ERROR_SUCCESS;
			DWORD lowByte, highByte, num_of_bytes_read, num_of_bytes_write, BUFFER_SIZE = IO_BUFFER_SIZE_LOW;
			CDiskIO *poDiskIO = CDiskIO::Instance();
			BYTE *buffer;
			BOOL Status = true;

			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return FALSE;
			}

			//Set the fragment size before starting to split
			poDiskIO->SetFragmentSize(i_FragmentSize.QuadPart);

			//Open Image file for reading
			if (INVALID_HANDLE_VALUE != this->Create(DISKIO_OPEN_EXISTING, FILE_ACCESS_SRC, 0, o_Result)) {

				file.LowPart = GetFileSize(FILE_ACCESS_SRC, 0, (LPDWORD) &file.HighPart, o_Result);
				fileSize = file.QuadPart;

				
				 
				buffer = (BYTE*)malloc(BUFFER_SIZE * sizeof(BYTE));
				//buffer = (BYTE*) VirtualMemAlloc(BUFFER_SIZE);
				
				//Splitting procedure using existing read/write API's 
				num_of_bytes_write = 0;

				while (fileSize > 0) {

					if (fileSize > BUFFER_SIZE) {

						if (TRUE == this->Read(FILE_ACCESS_SRC, 0, buffer, BUFFER_SIZE, &num_of_bytes_read, NULL, o_Result)) {
							
							if (TRUE == this->Write(FILE_ACCESS_DEST, 1, buffer, num_of_bytes_read, &num_of_bytes_write, NULL, o_Result)) {
								
								fileSize -= num_of_bytes_read;
							
							}
							else {
								
								Status = false;
								break;
							}
						}

						else {
							Status = false;
							break;
						}

					}

					else {

						if (TRUE == this->Read(FILE_ACCESS_SRC, 0, buffer, fileSize, &num_of_bytes_read, NULL, o_Result)) {

							if (TRUE == this->Write(FILE_ACCESS_DEST, 0, buffer, num_of_bytes_read, &num_of_bytes_write, NULL, o_Result)) {

								fileSize -= num_of_bytes_read;
							}

							else {
								Status = false;
								break;
							}

						}

						else {
							Status = false;
							break;
						}

					}

				}

			}
			if (Status == FALSE)
			{
				LastError = ::GetLastError();
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
				POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);
			}
			else
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
			}
			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}


	DWORD CLocal_FileAccess::SetFilePointer(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LONG   i_lDistanceToMove,
		PLONG  i_lpDistanceToMoveHigh,
		DWORD  i_dwMoveMethod,
		POSD_RESULT o_Result)
	{
		try
		{
			//Added for multicast handling
			if (FILE_ACCESS_DATA_BUFFER == this->m_DataSource)
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);

				return !(INVALID_SET_FILE_POINTER);
			}

			DWORD Ret = INVALID_SET_FILE_POINTER;
			BOOL TryOnce = FALSE;
			CDiskIO *poDiskIO = CDiskIO::Instance();
			DWORD LastError = ERROR_SUCCESS;
			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return INVALID_SET_FILE_POINTER;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);
			while (this->GetPollState())
			{
				Ret = ::SetFilePointer(hFile,
					i_lDistanceToMove,
					i_lpDistanceToMoveHigh,
					i_dwMoveMethod);
				LastError = ::GetLastError();
				if (Ret == INVALID_SET_FILE_POINTER)
				{
					//Break the polling for second time
					if (TryOnce) {

						INFOLOG(L"[%s %d]>>%S() SetFilePointer Failed Poll Timed out LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);

						Ret = INVALID_SET_FILE_POINTER;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

						break;
					}
					INFOLOG(L"[%s %d]>>%S() SetFilePointer Failed Start Polling... LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);

					//One time polling until given timeout
					BOOL PollResult = PollFilePath(i_Type, i_Ref, LastError);
					if (FALSE == PollResult)
					{
						Ret = INVALID_SET_FILE_POINTER;

						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);
						break;
					}
					else
					{
						TryOnce = TRUE;
					}
				}
				else
				{
					POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
					break;
				}
			}
			return Ret;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return INVALID_SET_FILE_POINTER;
	}

	DWORD CLocal_FileAccess::GetFilePointer(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LONG &o_PosHigh,
		POSD_RESULT o_Result)
	{
		try
		{
			DWORD Ret = INVALID_SET_FILE_POINTER;
			CDiskIO *poDiskIO = CDiskIO::Instance();
			DWORD LastError = ERROR_SUCCESS;
			BOOL TryOnce = FALSE;

			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return INVALID_SET_FILE_POINTER;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);
			LARGE_INTEGER position = { 0 };
			while (this->GetPollState())
			{
				Ret = position.LowPart = ::SetFilePointer(hFile, 0, &position.HighPart, FILE_CURRENT);
				o_PosHigh = position.HighPart;

				LastError = ::GetLastError();
				if (Ret == INVALID_SET_FILE_POINTER)
				{
					//Break the polling for second time
					if (TryOnce) {

						INFOLOG(L"[%s %d]>>%S() GetFilePointer Failed Poll Timed out LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);

						o_Result->ResString = csFileName;
						Ret = INVALID_SET_FILE_POINTER;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

						break;
					}
					INFOLOG(L"[%s %d]>>%S() GetFilePointer Failed Start Polling... LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);

					//One time polling until given timeout
					BOOL PollResult = PollFilePath(i_Type, i_Ref, LastError);
					if (FALSE == PollResult)
					{
						o_Result->ResString = csFileName;
						Ret = INVALID_SET_FILE_POINTER;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);
						break;
					}
					else
					{
						TryOnce = TRUE;
					}
				}
				else
				{
					POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
					break;
				}
			}
			return Ret;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return INVALID_SET_FILE_POINTER;
	}

	DWORD CLocal_FileAccess::GetFileSize(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		LPDWORD o_lpFileSizeHigh,
		POSD_RESULT o_Result)
	{
		try
		{
			DWORD Ret = 0;
			CDiskIO *poDiskIO = CDiskIO::Instance();
			DWORD LastError = ERROR_SUCCESS;
			BOOL TryOnce = FALSE;
			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return 0;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			while (this->GetPollState())
			{
				Ret = ::GetFileSize(hFile, o_lpFileSizeHigh);
				LastError = ::GetLastError();

				if (Ret == INVALID_SET_FILE_POINTER)
				{
					if (TryOnce) {

						INFOLOG(L"[%s %d]>>%S() GetFileSize Failed Poll Timed out LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);
						o_Result->ResString = csFileName;
						Ret = 0;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

						break;
					}
					INFOLOG(L"[%s %d]>>%S() GetFileSize Failed Start Polling... LError(%x)\n", gpCFileName, __LINE__, __FUNCTION__, LastError);

					//One time polling until given timeout
					BOOL PollResult = PollFilePath(i_Type, i_Ref, LastError);
					if (FALSE == PollResult)
					{
						o_Result->ResString = csFileName;
						POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
						POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);

						Ret = 0;
						break;
					}
					else
					{
						TryOnce = TRUE;
					}
				}
				else
				{
					POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
					break;
				}
			}
			return Ret;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return 0;
	}

	DWORD CLocal_FileAccess::MoveTo(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		ULONGLONG i_NewSizeInBytes,
		POSD_RESULT o_Result)
	{
		try
		{
			CDiskIO *poDiskIO = CDiskIO::Instance();

			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);
				return FALSE;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			LARGE_INTEGER CurrentPosition;
			CurrentPosition.LowPart = this->GetFilePointer(i_Type, i_Ref, CurrentPosition.HighPart, o_Result);

			if (INVALID_SET_FILE_POINTER == CurrentPosition.LowPart)
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_CALL_FAILURE);
				o_Result->ResString = csFileName;
				return FALSE;
			}
			LARGE_INTEGER newPosition;
			newPosition.QuadPart = CurrentPosition.QuadPart + i_NewSizeInBytes;

			LARGE_INTEGER resPosition, expectPosition;
			resPosition.QuadPart = 0;
			expectPosition.QuadPart = 0;

			expectPosition.LowPart = this->SetFilePointer(i_Type, i_Ref,
				expectPosition.LowPart,
				&expectPosition.HighPart,
				FILE_CURRENT,
				o_Result);

			INFOLOG(L"[%s %d]>>%S() Current file position(%lld) expected(%lld) Move(%lld) oldcurrent(%lld)\n",
				gpCFileName,
				__LINE__,
				__FUNCTION__,
				expectPosition.QuadPart,
				expectPosition.QuadPart + i_NewSizeInBytes,
				i_NewSizeInBytes,
				CurrentPosition.QuadPart);

			resPosition.LowPart = this->SetFilePointer(i_Type, i_Ref,
				newPosition.LowPart,
				&newPosition.HighPart,
				FILE_BEGIN,
				o_Result);
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);

		return INVALID_SET_FILE_POINTER;
	}

	DWORD CLocal_FileAccess::MoveFromCurrent(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		ULONGLONG i_NewSizeInBytes,
		POSD_RESULT o_Result)
	{
		try
		{
			CDiskIO *poDiskIO = CDiskIO::Instance();
			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);

				return INVALID_SET_FILE_POINTER;
			}
			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			LARGE_INTEGER newPosition;
			newPosition.QuadPart = i_NewSizeInBytes;

			if (i_NewSizeInBytes <= 0)
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
				return 0;
			}

			return this->SetFilePointer(i_Type, i_Ref,
				newPosition.LowPart,
				&newPosition.HighPart,
				FILE_CURRENT,
				o_Result);
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return INVALID_SET_FILE_POINTER;
	}

	BOOL CLocal_FileAccess::Close(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		POSD_RESULT o_Result)
	{
		try
		{
			//Added for multicast handling
			if (FILE_ACCESS_DATA_BUFFER == this->m_DataSource)
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);

				return TRUE;
			}

			CDiskIO *poDiskIO = CDiskIO::Instance();
			if (ISNULL(poDiskIO))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_DISKIO_INVALID);

				return INVALID_SET_FILE_POINTER;
			}

			DWORD LastError = ERROR_SUCCESS;
			BOOL Status = FALSE;
			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);
			o_Result->ResString.Append(csFileName);

			HANDLE hFile = poDiskIO->GetHandle(i_Type, i_Ref);

			poDiskIO->SetHandle(i_Type, i_Ref, INVALID_HANDLE_VALUE);

			if (INVALID_HANDLE_VALUE == hFile)
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_INVALID_PARAMETER);

				return FALSE;
			}
			Status = ::CloseHandle(hFile);
			LastError = ::GetLastError();
			if (FALSE == Status)
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_SYSTEM_ERROR);
				POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, i_Type, LastError);
			}
			else
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_NONE);
			}

			INFOLOG(L"[%s %d]>CLocal_FileAccess::Close() File(%s) FileAccess(%d) Ref(%d) LError(0x%x)\n"
				, gpCFileName,
				__LINE__,
				csFileName,
				i_Type,
				i_Ref,
				LastError);

			return Status;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, i_Type, OSD_ERROR_EXCEPTION_CAUGHT);
		return FALSE;
	}

	BOOL CLocal_FileAccess::IsFilePathExists(CString i_fileName, POSD_RESULT o_Result)
	{
		try
		{
			if (PathFileExists(i_fileName))
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, FILE_ACCESS_DEFAULT, OSD_ERROR_NONE);
				return TRUE;
			}
			else
			{
				POSD_SET_INTERNAL_ERR(o_Result, OSD_MODULE_FILEACCESS, FILE_ACCESS_DEFAULT, OSD_ERROR_SYSTEM_ERROR);
				POSD_SET_EXTERNAL_ERR(o_Result, OSD_EXTMODULE_FILEACCESS, FILE_ACCESS_DEFAULT, GetLastError());

				return FALSE;
			}
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}

		throw std::exception("IsfilepathExists Exception");
	}

	BOOL CLocal_FileAccess::IsFilePathExists(FILE_ACCESS_TYPE_ENUM i_Type,
		INT i_Ref,
		POSD_RESULT o_Result)
	{
		try
		{
			CDiskIO *poDiskIO = CDiskIO::Instance();

			if (ISNULL(poDiskIO))
			{
				throw std::exception("invalid diskio obj");
			}

			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);

			return PathFileExists(csFileName);
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}

		throw std::exception("IsfilepathExists Exception");
	}

	//RETRY HANDLING: RETRY FAILED & RETRY SUCCESS
	BOOL CLocal_FileAccess::PollFilePath(FILE_ACCESS_TYPE_ENUM i_Type, INT i_Ref, DWORD i_LastError, ULONGLONG i_RequiredSize /*=0*/)
	{
		OSD_RESULT Result;
		try
		{
#if 0
			CResponseHandler *poResponseHandler = CResponseHandler::Instance();
			if (ISNULL(poResponseHandler))
			{
				throw std::exception("invalid Response handler");
			}

			CDiskIO *poDiskIO = CDiskIO::Instance();

			if (ISNULL(poDiskIO))
			{
				throw std::exception("invalid diskio obj");
			}

			CString csFileName = poDiskIO->GetFileName(i_Type, i_Ref);

			switch (this->m_DataSource)
			{
			case FILE_ACCESS_LOCAL:
			case FILE_ACCESS_NETWORK_DRIVE:
			{
				/*Always poll the path during the failures*/
				/*if((ERROR_BAD_NET_NAME == i_LastError) ||
				(ERROR_BAD_NETPATH == i_LastError)  ||
				(ERROR_ACCESS_DENIED == i_LastError))*/
				//ERROR_NOT_ENOUGH_MEMORY
				{
					OSD_FILE_CHECKLIST checkNode;

					checkNode.m_FileName = csFileName;
					checkNode.m_objRef = NULL;
					checkNode.m_Ref = i_Ref;
					checkNode.m_Type = i_Type;
					checkNode.m_SpaceNeeded = i_RequiredSize;
					checkNode.m_LastError = OSD_ERROR_SYSTEM_ERROR;
					checkNode.m_IsSuccess = FALSE;

					Result.ResString = csFileName;
					OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_FILEACCESS, OSD_ERROR_SYSTEM_ERROR);
					OSD_SET_EXTERNAL_ERR(Result, OSD_EXTMODULE_FILEACCESS, i_LastError);

					DEBUGLOG(L"[%s %d]>PollFilePath::Retry start File(%s) Resp(%d) Ret(%d)\n",
						gpCFileName,
						__LINE__,
						csFileName,
						APPLICATION_RESP_PAUSED,
						Result.RetCode);

					Result = poResponseHandler->SendResponse(APPLICATION_RESP_RETRYING, Result);
					if (OSD_ERROR_NONE != Result.RetCode)
					{
						ERRORLOG(L"[%s %d]>PollFilePath::SendResponse Failed File(%s) Resp(%d) Ret(%d)\n",
							gpCFileName,
							__LINE__,
							csFileName,
							APPLICATION_RESP_PAUSED,
							Result.RetCode);
					}

					OSD_RESULT TempResult = this->WaitForAvailablity(checkNode, Result);
					if (OSD_ERROR_NONE != TempResult.RetCode)
					{
						ERRORLOG(L"[%s %d]>PollFilePath::Retry Failed File(%s) Resp(%d) Ret(%d) IsSucc(%d)\n",
							gpCFileName,
							__LINE__,
							csFileName,
							APPLICATION_RESP_PAUSED,
							Result.RetCode,
							checkNode.m_IsSuccess);

						return FALSE;
					}
					DEBUGLOG(L"[%s %d]>PollFilePath::Retry success File(%s) Resp(%d) Ret(%d) IsSucc(%d)\n",
						gpCFileName,
						__LINE__,
						csFileName,
						APPLICATION_RESP_RESUMED,
						Result.RetCode,
						checkNode.m_IsSuccess);

					OSD_SET_INTERNAL_ERR(Result, OSD_MODULE_FILEACCESS, OSD_ERROR_NONE);
					/*Result = poResponseHandler->SendResponse(APPLICATION_RESP_RETRY_SUCCESS,Result);
					if(OSD_ERROR_NONE != Result.RetCode)
					{
					INFOLOG(L"[%s %d]>PollFilePath::SendResponse Failed File(%s) Resp(%d) Ret(%d)\n",
					gpCFileName,
					__LINE__,
					csFileName,
					APPLICATION_RESP_RESUMED,
					Result.RetCode);
					}*/

					return TRUE;
				}
			}break;
			default:
				break;
			}
#endif	
			//For safe
			return FALSE;
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
		return FALSE;
	}

	VOID CLocal_FileAccess::PrintStatistics()
	{
		try
		{
			INFOLOG(L"[%s %d]>File Operation Statistics SrcTotal[%lld] Read[%lld] Write[%lld]\n",
				gpCFileName,
				__LINE__,
				TotalSrcBytes.QuadPart,
				ReadBytes.QuadPart,
				WriteBytes.QuadPart);
		}
		catch (std::exception &exc)
		{
			INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n", gpCFileName, __LINE__, __FUNCTION__, exc.what());
		}
		catch (...)
		{
			INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n", gpCFileName, __LINE__, __FUNCTION__);
		}
	}

}/*OS_Manager*/
