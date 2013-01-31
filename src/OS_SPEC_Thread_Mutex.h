/**
*	@file 
* @brief 对不同操作系统的底层函数的封装
* @author xq
*/

#ifndef OS_SPEC_THREAD_IMPL_H
#define OS_SPEC_THREAD_IMPL_H

#include "stdafx.h"

//namespace ACE_OS_SPEC
//{



#if defined(ACE_WIN_PLATFORM)

typedef CRITICAL_SECTION ACE_thread_mutex_t;

#elif defined(ACE_LINUX_PLATFORM) 

typedef pthread_mutex_t ACE_thread_mutex_t;

#endif


int thread_mutex_lock(ACE_thread_mutex_t *m);
int thread_mutex_unlock (ACE_thread_mutex_t *m);
int thread_mutex_trylock (ACE_thread_mutex_t *m);
int thread_mutex_destroy (ACE_thread_mutex_t *m);
int thread_mutex_init (ACE_thread_mutex_t *m, int lock_type, const char *name, void *arg);


//}

#endif