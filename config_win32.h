/**
window include files 
windows platform macro define
*/
#ifndef XQ_CONFIG_WIN32_H
#define XQ_CONFIG_WIN32_H


#ifndef IDEE_EXPORT
	#define XQ_EXPORT __declspec(dllimport)
	#define XQ_EXPORT_TEMPLATE
#else 
	#define XQ_EXPORT __declspec(dllexport)
	#define XQ_EXPORT_TEMPLATE __declspec(dllexport)
#endif

#define XQ_INLINE inline


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

#endif

