#include "..\stdafx.h"

#include "Posix_Sync_Func.h"


namespace idee
{
namespace os_func
{

#if defined(XQ_WIN_PLATFORM)

#elif defined(XQ_LINUX_PLATFORM) 
/***********************************************************************************/
/* linux pthread_mutex_t functions */

int  mutex_init(XQ_mutex_t *m)
{
  if(pthread_mutex_init(m, NULL) == 0)
	  return 0;	//success
  else
	  return -1;
}

int  mutex_destory(XQ_mutex_t *m)
{
  return pthread_mutex_destroy(m);
}

int  mutex_lock(XQ_mutex_t *m)
{
  return pthread_mutex_lock(m);
}

int mutex_trylock (XQ_mutex_t *m)
{
	return pthread_mutex_lock(m);
}

int  mutex_unlock(XQ_mutex_t *m)
{
  return pthread_mutex_unlock(m);
}


/***********************************************************************************/
/* linux pthread_rwlock_t functions */

int rwmutex_init(XQ_rwmutex_t *m)
{
	return ::pthread_rwlock_init(m, NULL);
}

int rwmutex_destroy(XQ_rwmutex_t *m)
{
	return ::pthread_rwlock_destroy(m);
}

int rwmutex_rdlock(XQ_rwmutex_t *m)
{
	return ::pthread_rwlock_rdlock(m);
}

int rwmutex_wrlock(XQ_rwmutex_t *m)
{
	return ::pthread_rwlock_wrlock(m);
}

int rwmutex_rdunlock(XQ_rwmutex_t *m)
{
	return ::pthread_rwlock_unlock(m);
}

int rwmutex_wrunlock(XQ_rwmutex_t *m)
{
	return ::pthread_rwlock_unlock(m);
}

/***********************************************************************************/
/* semaphore functions */
int sema_init(XQ_sema_t *m)
{
	return sem_init(m, 0, 1);	
}

int sema_wait(XQ_sema_t *m)
{
	return sem_wait(m);
}

int sema_post(XQ_sema_t *m)
{
	return sem_post(m);
}

int sema_destroy(XQ_sema_t *m)
{
	return sem_destroy(m);
}

#endif

}	//end os_func
}	//end idee
