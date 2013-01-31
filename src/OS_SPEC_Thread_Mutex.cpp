#include "OS_SPEC_Thread_Mutex.h"

//namespace ACE_OS_SPEC
//{
	

int thread_mutex_lock(ACE_thread_mutex_t *m)
{
  ::EnterCriticalSection (m);
  	return 0;
}


int thread_mutex_unlock (ACE_thread_mutex_t *m)
{
  ::LeaveCriticalSection (m);
  return 0;
}


int thread_mutex_trylock (ACE_thread_mutex_t *m)
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


int thread_mutex_init (ACE_thread_mutex_t *m, int lock_type, const char *name, void *arg)  //ACE_mutexattr_t
{  
    ::InitializeCriticalSection (m);   
  	return 0;
}


int thread_mutex_destroy (ACE_thread_mutex_t *m)
{
	::DeleteCriticalSection (m);
    return 0;
}

#if defined(ACE_WIN_PLATFORM)

#elif defined(ACE_LINUX_PLATFORM) 

ACE_INLINE int thread_mutex_lock(ACE_thread_mutex_t *m)
{
  pthread_mutex_lock(m);
}

#endif

//}
