////////////////////////////////////////////////////////////////////////////////
//@file:SafeQueue.h
//@date: 
//@author: Bala
//@version: 1.0
//@history:
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __SAFE_QUEUE__
#define __SAFE_QUEUE__

#include<queue>

using namespace std;

namespace OS_Manager
{

/////////////////////////////////////////
//typdef declaration
////////////////////////////////////////
#define CSAFE_QUEUE_TEMPLATE			template <class GenericData>
#define CSAFE_QUEUE_TEMPLATE_DEFENITION	CSafeQueue<GenericData>		

#define OSD_SAFE_QUEUE_WAIT_TIMEOUT    (5 * 1000) //INFINITE 
/////////////////////////////////////////
//CSafeQueue declaration
////////////////////////////////////////

CSAFE_QUEUE_TEMPLATE
class CSafeQueue
{
private:
	typedef std::queue<GenericData> SafeQueue_t;
	SafeQueue_t        m_oSafeQueue;
	
	CRITICAL_SECTION   m_CriticalSection;
	HANDLE             m_hEvent;
	BOOL               IsActive;
		
public:
	CSafeQueue(VOID);
	~CSafeQueue(VOID);

	BOOL Init();
	VOID Kill();
	VOID Enqueue(GenericData i_Data);
	GenericData GetQueue(VOID);
	VOID ReleaseAllUsers(VOID);
	VOID WatiForEvent(VOID);
	SafeQueue_t& GetSafeQueObj(){return m_oSafeQueue;}
};

/////////////////////////////////////////
//CSafeQueue definition
////////////////////////////////////////
CSAFE_QUEUE_TEMPLATE
CSAFE_QUEUE_TEMPLATE_DEFENITION::CSafeQueue(VOID)
    : m_oSafeQueue()
  {
     BOOL Status = Init();

	 if(FALSE == Status)
	 {
	     throw std::exception("SafteQueue init failed");
	 }
	 IsActive = TRUE;
  }

CSAFE_QUEUE_TEMPLATE
CSAFE_QUEUE_TEMPLATE_DEFENITION::~CSafeQueue(VOID)
{
	Kill();
}

CSAFE_QUEUE_TEMPLATE
BOOL CSAFE_QUEUE_TEMPLATE_DEFENITION::Init()
{
	try
	{
		InitializeCriticalSection(&m_CriticalSection);

		m_hEvent =  CreateEvent(NULL,TRUE,FALSE,NULL); 
		if(ISNULL(m_hEvent)) 
		{ 
			 INFOLOG(_T("[Safe Queue: Event creation failed Lerror(0x%x)\n"),
					  GetLastError());
		} 
		  
		return TRUE;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CSafeQueue::Init() %d]>std Exception caught (%S)\n",__LINE__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CSafeQueue::Init() %d]>Def Exception caught\n",__LINE__);
	}

	return FALSE;
}

CSAFE_QUEUE_TEMPLATE
VOID CSAFE_QUEUE_TEMPLATE_DEFENITION::Kill()
{
	try
	{
		if(ISNOTNULL(m_hEvent))
		{
			CloseHandle(m_hEvent);
			m_hEvent = NULL;
		}
		DeleteCriticalSection(&m_CriticalSection);
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CSafeQueue::Kill() %d]>std Exception caught (%S)\n",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CSafeQueue::Kill() %d]>Def Exception caught\n",__LINE__);
	}
}

CSAFE_QUEUE_TEMPLATE
VOID CSAFE_QUEUE_TEMPLATE_DEFENITION::Enqueue(GenericData i_Data)
{
	try
	{
		EnterCriticalSection(&m_CriticalSection);

		m_oSafeQueue.push(i_Data);
		  
		LeaveCriticalSection(&m_CriticalSection);

		SetEvent(m_hEvent);

	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>std Exception caught (%S)\n",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>Def Exception caught\n",__LINE__);
	}
}

CSAFE_QUEUE_TEMPLATE
GenericData CSAFE_QUEUE_TEMPLATE_DEFENITION::GetQueue(VOID)
{
	try
	{
		if(!IsActive)
		{
		    return NULL;
		}
		EnterCriticalSection(&m_CriticalSection);
		
		if(m_oSafeQueue.empty())
		{
			ResetEvent(m_hEvent);

			//INFOLOG(L"[CSafeQueue::Enqueue() %d]>Reset event (%s)\n",__LINE__);

			LeaveCriticalSection(&m_CriticalSection);
			return NULL;
		}

		GenericData content = m_oSafeQueue.front();
		m_oSafeQueue.pop();
		LeaveCriticalSection(&m_CriticalSection);
		  
		return content;
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>std Exception caught (%S)\n",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>Def Exception caught\n",__LINE__);
	}
	return GenericData(0);
}

CSAFE_QUEUE_TEMPLATE
VOID CSAFE_QUEUE_TEMPLATE_DEFENITION::ReleaseAllUsers(VOID)
{
	try
	{
		IsActive  = FALSE;
		SetEvent(m_hEvent);
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>std Exception caught (%S)\n",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>Def Exception caught\n",__LINE__);
	}
}

CSAFE_QUEUE_TEMPLATE
VOID CSAFE_QUEUE_TEMPLATE_DEFENITION::WatiForEvent(VOID)
{
	try
	{
		WaitForSingleObject(m_hEvent,OSD_SAFE_QUEUE_WAIT_TIMEOUT);  
	}
	catch(std::exception &exc)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>std Exception caught (%S)\n",__LINE__, exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[CSafeQueue::Enqueue() %d]>Def Exception caught\n",__LINE__);
	}
}
}
#endif /*__SAFE_QUEUE__*/