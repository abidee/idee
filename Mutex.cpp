#include "config_idee.h"

#include "Mutex.h"

namespace idee
{

Mutex::Mutex(const char *name /*, XQ_mutexattr_t *arg*/)
  : removed_ (false)
{
  if(mutex_init(&this->locker_))
  {
	  //int i=1;
  }
  return;
}


Mutex::~Mutex(void)
{
  this->remove();
}

XQ_INLINE int Mutex::lock(void)
{
  return mutex_lock (&this->locker_);
}

XQ_INLINE int Mutex::try_lock (void)
{
  return mutex_trylock (&this->locker_);
}

XQ_INLINE int Mutex::lock_read (void)
{
  return mutex_lock (&this->locker_);
}

XQ_INLINE int Mutex::try_lock_read (void)
{
  return mutex_trylock (&this->locker_);
}

XQ_INLINE int Mutex::lock_write (void)
{
  return mutex_lock (&this->locker_);
}

XQ_INLINE int Mutex::try_lock_write (void)
{
  return mutex_trylock (&this->locker_);
}

XQ_INLINE int Mutex::unlock (void)
{
  return mutex_unlock (&this->locker_);
}


XQ_INLINE int Mutex::remove (void)
{
  int result = 0;
  if (!this->removed_)
    {
      this->removed_ = true;
      //result = mutex_destroy(&this->locker_);
    }
  return result;
}

XQ_INLINE const XQ_mutex_t& Mutex::native_handle(void)
{
  return this->locker_;
}

}	//end idee

