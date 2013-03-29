/**
linux include files 
linux platform macro define
*/
#ifndef XQ_CONFIG_LINUX_H
#define XQ_CONFIG_LINUX_H


//#define XQLIB_EXPORT

#ifndef XQLIB_EXPORT
	#define XQ_EXPORT __declspec(dllimport)
	#define XQ_EXPORT_TEMPLATE
#else
	#define XQ_EXPORT __declspec(dllexport)
	#define XQ_EXPORT_TEMPLATE __declspec(dllexport)
#endif

#define XQ_INLINE inline


#endif
