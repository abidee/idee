/**
linux include files 
linux platform macro define
*/
#ifndef XQ_CONFIG_MINGW_H
#define XQ_CONFIG_MINGW_H


#ifndef IDEE_EXPORT
	#define XQ_EXPORT __declspec(dllimport)
	#define XQ_EXPORT_TEMPLATE
#else
	#define XQ_EXPORT __declspec(dllexport)
	#define XQ_EXPORT_TEMPLATE __declspec(dllexport)
#endif

#define XQ_INLINE inline

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
