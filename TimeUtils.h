////////////////////////////////////////////////////////////////////////////////
//@file:TimeUtils.h
//@date: 
//@author: Balamurugan
//@version: 1.0
//@history:
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

/////////////////////////////////////////
// default values
////////////////////////////////////////

#define OSD_SECONDS_PER_HOUR       3600
#define OSD_HOURS_PER_DAY          24
#define OSD_MINUTES_PER_HOUR       60

#define OSD_CLOCK_PER_SECOND      CLOCKS_PER_SEC
//////////////////////////////////////
// Structure structure
//////////////////////////////////////
class OSDTime
{

public:
	OSDTime();
	~OSDTime();

	UINT   m_Days;
	UINT   m_Hours;
	UINT   m_Minutes;
	UINT   m_Secs;
	UINT   m_MilliSecs;
	
	CAtlString GetTimeString( );
	VOID ConvertToTimeFormat(ULONGLONG i_Seconds);
private:
	CAtlString m_TimeFormat;
	VOID ConvertToTimeString();
};

/////////////////////////////////////////
// Time functions declarations
////////////////////////////////////////

std::string getcurrenttimedate();
ULONGLONG   OSD_GetTickCount64();
ULONGLONG   OSD_CurrentClock();
VOID        OSD_CurrentClock(ULONGLONG &o_CurrentTime);
ULONGLONG   OSD_ClockDifferenceMilliSecs(ULONGLONG i_EndTime, ULONGLONG i_StartTime);
ULONGLONG   OSD_ClockDifferenceSecs(ULONGLONG i_EndTime, ULONGLONG i_StartTime);
ULONGLONG OSD_GetSpeed(ULONGLONG i_DataSize, ULONGLONG i_TimeInSec);
ULONGLONG OSD_GetEstimatedTimeInSec(ULONGLONG i_TotalDataSize, ULONGLONG i_DataRatePerSec);
ULONGLONG OSD_GetSysTimeMs64();


/////////////////////////////////////////////////////////////////////////