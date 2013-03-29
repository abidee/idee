// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

//project config head, must include this file first

#define XQLIB_EXPORT  //for exprot dll 

#include "config.h"

#if defined(XQ_WIN_PLATFORM)

	// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。
	// 如果要为以前的 Windows 平台生成应用程序，请包括 WinSDKVer.h，并将
	// WIN32_WINNT 宏设置为要支持的平台，然后再包括 SDKDDKVer.h。
	#include <SDKDDKVer.h>

	#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的信息

	// Windows 头文件:
	#include <windows.h>
	#include <WinBase.h>

	// TODO: 在此处引用程序需要的其他头文件
	#include <process.h>

	//std headers
	#include <list>
	#include <vector>
	#include <deque>
	
	//C runtime headers
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>

#elif defined(XQ_LINUX_PLATFORM)
	//os specific headers
	#include <pthread.h>
	#include <signal.h>
	#include <sched.h>
	#include <unistd.h>
	#include <semaphore.h>

	//std headers
	#include <list>
	#include <vector>
	#include <deque>

	//C runtime headers
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>

#endif

