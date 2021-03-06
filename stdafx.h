// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#include <map>

#ifdef _WIN32
#include <Windows.h>
#include <WinIoCtl.h>
#include <ctime>
#include <string>
#include <atlstr.h>
#include <atlbase.h>
#include <tchar.h>
#include <assert.h>
#elif LINUX
#endif

#include "..\..\OSManagerUtils\include\Common.h"
#include "..\..\OSManagerUtils\include\DataTypes.h"
#include "..\..\OSManagerUtils\include\ErrorCode.h"
#include "..\..\OSManagerUtils\include\utils.h"


#define INFOLOG _tprintf
#define ERRORLOG _tprintf
#define ONELINELOG _tprintf


// TODO: reference additional headers your program requires here
