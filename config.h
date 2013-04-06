/**
*  modify this head file to support different platforms
*  windows : 
*	#define XQ_WIN_PLATFORM
*	#include "config_win32.h"
*
*  MinGW on windows: 
*  #define XQ_LINUX_PLATFORM
*  #inlcude "config_win32_mingw.h"
*
*  Linux: 
*  #define XQ_LINUX_PLATFORM
*  #include "config_linux.h"
*/

#ifndef XQ_CONFIG_H
#define XQ_CONFIG_H

//for windows 
#define XQ_WIN_PLATFORM
#include "config_win32.h"

//for linux
//#define XQ_LINUX_PLATFORM
//#include "config_linux.h"

//for mingw on windows
//#define XQ_LINUX_PLATFORM
//#include "config_win32_mingw.h"

#endif
