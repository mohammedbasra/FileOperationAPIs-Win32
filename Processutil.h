
#define BUFSIZE MAX_PATH

namespace OSD_Util
{

class CProcess
{
	HANDLE m_hProcess;
	DWORD m_ProcessId;
	DWORD gv_ShutdownInProgress;
public:
	CProcess(void);
	~CProcess(void);
	HANDLE getHandle();
	DWORD getProcessID();
	DWORD executeFile(CAtlString path, CAtlString executeFileName, CAtlString &strRemarks,int timeout, BOOL showUI,DWORD dwnewprocessGroup=0);
	DWORD execute(CAtlString path, CAtlString executeFileName, CAtlString &remarks,int timeout, BOOL showUI,DWORD dwnewprocessGroup=0);
	
	HANDLE GetProcessHandle(DWORD i_ProcessId);
	BOOL IsProcessRunning(CString i_ExeName);
};

} //namespace
