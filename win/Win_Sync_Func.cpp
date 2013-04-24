#include "..\stdafx.h"

#include "Win_Sync_Func.h"

namespace idee
{
namespace os_func
{

#if defined(XQ_WIN_PLATFORM)
/***********************************************************************************/
/* windows mutex CRITICAL_SECTION functions */

int mutex_init(XQ_mutex_t *m)  //, int lock_type, const char *name, void *arg)  //ACE_mutexattr_t
{  
	::InitializeCriticalSection (m);   
	return 0;
}

int mutex_destroy(XQ_mutex_t *m)
{
	::DeleteCriticalSection (m);
    return 0;
}

int mutex_lock(XQ_mutex_t *m)
{
	::EnterCriticalSection (m);
  	return 0;
}

int mutex_unlock(XQ_mutex_t *m)
{
	::LeaveCriticalSection (m);
	return 0;
}

int mutex_trylock(XQ_mutex_t *m)
{
	BOOL result = ::TryEnterCriticalSection (m);
	if (result == TRUE)
		return 0;
	else
	{
      //errno = EBUSY;
      return -1;
    }
}



/***********************************************************************************/
/* windows slim SRWLOCK functions */
int  rwmutex_init(XQ_rwmutex_t *m)
{
#if _WIN32_WINNT >= 0x0700 
	InitializeSRWLock(m);
#endif
	return 0;

}

int  rwmutex_destroy(XQ_rwmutex_t *m)
{
	return 0;
}

int  rwmutex_rdlock(XQ_rwmutex_t *m)
{
#if _WIN32_WINNT >= 0x0700 
	AcquireSRWLockShared(m);
#endif
	return 0;
}

int  rwmutex_rdunlock(XQ_rwmutex_t *m)
{
#if _WIN32_WINNT >= 0x0700 
	ReleaseSRWLockShared(m);
#endif
	return 0;
}

int  rwmutex_wrlock(XQ_rwmutex_t *m)
{
#if _WIN32_WINNT >= 0x0700 
	AcquireSRWLockExclusive(m);
#endif
	return 0;
}

int  rwmutex_wrunlock(XQ_rwmutex_t *m)
{
#if _WIN32_WINNT >= 0x0700 
	ReleaseSRWLockExclusive(m);
#endif
	return 0;
}

/***********************************************************************************/
//Semaphore functions
int sema_init(XQ_sema_t *m)
{
	*m = ::CreateSemaphore(NULL, 0, 100, NULL);
	if(*m == NULL)
		return -1;
	else 
		return 0;
}

int sema_wait(XQ_sema_t *m)
{
	int ret = -1;
	DWORD wait = WaitForSingleObject(*m, 0L);
	switch(wait)
	{
	case WAIT_OBJECT_0:	//sema was signaled
		ret = 0;
		break;
	case WAIT_TIMEOUT:
		ret = -1;
		break;
	default:
		break;
	}
	return ret;
}

int sema_post(XQ_sema_t *m)
{
	LONG prev;
	BOOL flag = ::ReleaseSemaphore(*m, 1, &prev); //increse count by 1
	if(flag)
		return 0;
	else
		return -1;
}

int sema_destroy(XQ_sema_t *m)
{
	::CloseHandle(*m);
	return 0;
}


#elif defined(XQ_LINUX_PLATFORM) 

#endif

}	//end os_func

}	//end idees
