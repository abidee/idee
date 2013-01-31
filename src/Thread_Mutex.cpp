#include "stdafx.h"


#include "Thread_Mutex.h"

ACE_Thread_Mutex::ACE_Thread_Mutex (const char *name, ACE_mutexattr_t *arg)
  : removed_ (false)
{
  if (thread_mutex_init (&this->lock_,
                                 			0,
                                 			name,
                                 			arg) != 0)
  {
	  int i=1;
  }
    //ACE_ERROR ((LM_ERROR,
    //            ACE_TEXT ("%p\n"),
    //            ACE_TEXT ("ACE_Thread_Mutex::ACE_Thread_Mutex")));
}


ACE_Thread_Mutex::~ACE_Thread_Mutex (void)
{
  this->remove ();
}


ACE_INLINE int
ACE_Thread_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return thread_mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::tryacquire (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return thread_mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::acquire_read (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return thread_mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::tryacquire_read (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return thread_mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::acquire_write (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return thread_mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::tryacquire_write (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return thread_mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::release (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return thread_mutex_unlock (&this->lock_);
}


ACE_INLINE int
ACE_Thread_Mutex::remove (void)
{
  int result = 0;
  if (!this->removed_)
    {
      this->removed_ = true;
      result = thread_mutex_destroy (&this->lock_);
    }
  return result;
}

ACE_INLINE const ACE_thread_mutex_t &
ACE_Thread_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_Thread_Mutex::lock");
  return this->lock_;
}

ACE_INLINE ACE_thread_mutex_t &
ACE_Thread_Mutex::lock (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::lock");
  return this->lock_;
}