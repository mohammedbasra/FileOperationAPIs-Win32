////////////////////////////////////////////////////////////////////////////////
//@file:TimeUtils.cpp
//@date: 
//@author: Balamurugan
//@version: 1.0
//@history:
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TimeUtils.h"

/////////////////////////////////////////
// default values
////////////////////////////////////////
static const LPTSTR gpCFileName = L"TimeUtils.cpp";

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
OSDTime::OSDTime()
{
	m_Days         = 0;
	m_Hours        = 0;
	m_Minutes      = 0;
	m_Secs         = 0;
	m_MilliSecs    = 0;

	m_TimeFormat.Empty();
}

OSDTime::~OSDTime()
{

}

VOID OSDTime::ConvertToTimeString( )
{
	try
	{
		std::wstring format;
		TCHAR TempString[MAX_PATH] = {0};
	
		memset(TempString,0,MAX_PATH);
		if(this->m_Days > 0)
		{
			_itow_s(this->m_Days,TempString,10);
			format.append(TempString);
			format.append(_T(" "));

			if(this->m_Hours > 1)
			{
				format.append(_T("Days"));
			}
			else
			{
				format.append(_T("Day"));
			}
			format.append(_T(" "));
		}

		memset(TempString,0,MAX_PATH);
		if(this->m_Hours > 0 || this->m_Days > 0 )
		{
			_itow_s(this->m_Hours,TempString,10);
			format.append(TempString);
			format.append(_T(" "));

			if(this->m_Hours > 1)
			{
				format.append(_T("Hrs"));
			}
			else
			{
				format.append(_T("Hr"));
			}

			format.append(_T(" "));
		}

		memset(TempString,0,MAX_PATH);
		if( this->m_Minutes > 0 || this->m_MilliSecs > 0 || this->m_Secs > 0 )
		{
			_itow_s(this->m_Minutes,TempString,10);
			format.append(TempString);
			format.append(_T(" "));

			format.append(_T("Min"));
			format.append(_T(" "));
		}

		memset(TempString,0,MAX_PATH);
		if(this->m_Secs > 0 || this->m_MilliSecs > 0)
		{
			_itow_s(this->m_Secs,TempString,10);
			format.append(TempString);
			format.append(_T(" "));

			format.append(_T("Sec"));
			format.append(_T(" "));
		}

		memset(TempString,0,MAX_PATH);
		if( this->m_MilliSecs > 0)
		{
			_itow_s(this->m_MilliSecs,TempString,10);
			format.append(TempString);
			format.append(_T(" "));
			format.append(_T("ms"));
		}

		m_TimeFormat.Format(_T("%s"),format.c_str());

	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}
}

CAtlString OSDTime::GetTimeString( )
{
	try
	{
		return m_TimeFormat;	
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}
	return CString();
}

VOID OSDTime::ConvertToTimeFormat(ULONGLONG i_Seconds)
{
	try
	{
		this->m_Days      = UINT((i_Seconds/OSD_SECONDS_PER_HOUR) / OSD_HOURS_PER_DAY);
		this->m_Hours     = (i_Seconds/OSD_SECONDS_PER_HOUR) % OSD_HOURS_PER_DAY;

		i_Seconds         = ULONGLONG(i_Seconds % OSD_SECONDS_PER_HOUR);
		this->m_Minutes   = UINT(i_Seconds / OSD_MINUTES_PER_HOUR);

		i_Seconds         = i_Seconds % OSD_MINUTES_PER_HOUR;
		this->m_Secs      = UINT(i_Seconds);

		this->m_MilliSecs = 0;

		this->ConvertToTimeString();
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}
}

std::string getcurrenttimedate ()
{
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time (&rawtime);
	localtime_s(&timeinfo,&rawtime);

	strftime(buffer,80,"%d-%m-%Y %I:%M:%S",&timeinfo);
	std::string str(buffer);
  
	return str;
}

//Minimum supported client - Windows 2000 Professional [desktop apps only]
//Minimum supported server - Windows 2000 Server [desktop apps only]
//NOTE: GetTickCount function 
//Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms724408(v=vs.85).aspx
//ULONGLONG OSD_GetTickCount64()
//{
//	try
//	{
//		return ::GetTickCount64();
//	}
//	catch(std::exception &exc)
//	{
//	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
//	}
//	catch(...)
//	{
//		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
//	}
//	return FALSE;
//}

ULONGLONG OSD_CurrentClock()
{
	try
	{
		/*	The elapsed wall-clock time since the start of the process (elapsed time in seconds times CLOCKS_PER_SEC). 
		If the amount of elapsed time is unavailable, the function returns –1, cast as a clock_t.*/

		//do not rely on this value for total elapsed time in processes that run for more than 214,748 seconds, 
		//or about 59 hours.

		return ::clock();
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

VOID OSD_CurrentClock(ULONGLONG &o_CurrentTime)
{
	try
	{
		/*	The elapsed wall-clock time since the start of the process (elapsed time in seconds times CLOCKS_PER_SEC). 
		If the amount of elapsed time is unavailable, the function returns –1, cast as a clock_t.*/

		//do not rely on this value for total elapsed time in processes that run for more than 214,748 seconds, 
		//or about 59 hours.

		o_CurrentTime = 0;

		o_CurrentTime = ::clock();
	}
	catch(std::exception &exc)
	{
	    INFOLOG(L"[%s %d]>>%S()-std Exception caught (%S)\n",gpCFileName,__LINE__, __FUNCTION__,exc.what());
	}
	catch(...)
	{
		INFOLOG(L"[%s %d]>>%S()-Def Exception caught\n",gpCFileName,__LINE__, __FUNCTION__);
	}
}

ULONGLONG OSD_ClockDifferenceMilliSecs(ULONGLONG i_EndTime, ULONGLONG i_StartTime)
{
	try
	{
		ULONGLONG diff = 0;

		if(i_EndTime >= i_StartTime)
		{
			//Normal case
			diff = ULONGLONG(ULONGLONG(i_EndTime) - ULONGLONG(i_StartTime));
		}
		else
		{
			//Clock wrap around case
			diff = ULONGLONG(ULONGLONG(i_StartTime) - ULONGLONG(i_EndTime));
		}

		if( 0 < diff )
		{
			return diff;
		}
		else
		{
			return 0;
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
	return FALSE;
}

ULONGLONG OSD_ClockDifferenceSecs(ULONGLONG i_EndTime, ULONGLONG i_StartTime)
{
	try
	{
		ULONGLONG diff = 0;
		
		if(i_EndTime >= i_StartTime)
		{
			//Normal case
			diff = ULONGLONG(ULONGLONG(i_EndTime) - ULONGLONG(i_StartTime));
		}
		else
		{
			//Clock wrap around case
			diff = ULONGLONG(ULONGLONG(i_StartTime) - ULONGLONG(i_EndTime));
		}

		if( 0 < diff )
		{
			return ULONGLONG(double(diff)/double(OSD_CLOCK_PER_SECOND));
		}
		else
		{
			return 0;
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
	return FALSE;
}

ULONGLONG OSD_GetSpeed(ULONGLONG i_DataSize, ULONGLONG i_TimeInSec)
{
	try
	{
		if( 0 < i_TimeInSec )
		{
			return ULONGLONG(ULONGLONG(i_DataSize)/ULONGLONG(i_TimeInSec));
		}
		else
		{
			return i_DataSize;
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
	return FALSE;
}

ULONGLONG OSD_GetEstimatedTimeInSec(ULONGLONG i_TotalDataSize, ULONGLONG i_DataRatePerSec)
{
	try
	{
		if( 0 < i_DataRatePerSec )
		{
			return ULONGLONG(double(i_TotalDataSize)/double(i_DataRatePerSec));
		}
		else
		{
			return i_TotalDataSize;
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
	return FALSE;
}

ULONGLONG OSD_GetSysTimeMs64()
{
 /* Windows */
 FILETIME ft;
 LARGE_INTEGER li;

 /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
  * to a LARGE_INTEGER structure. */
 GetSystemTimeAsFileTime(&ft);
 li.LowPart = ft.dwLowDateTime;
 li.HighPart = ft.dwHighDateTime;

 ULONGLONG ret = li.QuadPart;
 ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
 ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

 return ret;
}
