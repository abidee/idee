#include "stdafx.h"

#include "ReadWriteMutex.h"

namespace idee
{

ReadWriteMutex::ReadWriteMutex (const char *name /*, ACE_mutexattr_t *arg*/)
  : removed_ (false), rw_flag_(RW_NULL)
{
	rwmutex_init (&this->locker_); 
    //ACE_ERROR ((LM_ERROR,
    //            ACE_TEXT ("%p\n"),
    //            ACE_TEXT ("ReadWriteMutex::ReadWriteMutex")));
}


ReadWriteMutex::~ReadWriteMutex (void)
{
	this->remove ();
}


XQ_INLINE int ReadWriteMutex::lock (void)
{
	return 0;
}

XQ_INLINE int ReadWriteMutex::try_lock (void)
{
	return 0;
}

XQ_INLINE int ReadWriteMutex::lock_read (void)
{
	rw_flag_ = RW_READ;
	return rwmutex_rdlock (&this->locker_);
}

XQ_INLINE int ReadWriteMutex::try_lock_read (void)
{
	rw_flag_ = RW_READ;
	return rwmutex_rdlock (&this->locker_);
}

XQ_INLINE int ReadWriteMutex::lock_write (void)
{
	rw_flag_ = RW_WRITE;
	return rwmutex_wrlock (&this->locker_);
}

XQ_INLINE int ReadWriteMutex::try_lock_write (void)
{
	rw_flag_ = RW_WRITE;
	return rwmutex_wrlock (&this->locker_);
}

XQ_INLINE int ReadWriteMutex::unlock (void)
{
	if(rw_flag_ == RW_READ)
		return rwmutex_rdunlock (&this->locker_);
	else
		return rwmutex_wrunlock (&this->locker_);
}


XQ_INLINE int ReadWriteMutex::remove (void)
{
	int result = 0;
	if (!this->removed_)
    {
      this->removed_ = true;
      result = rwmutex_destroy (&this->locker_);
    }
	return result;
}

XQ_INLINE const XQ_rwmutex_t &
ReadWriteMutex::native_handle (void)
{
	return this->locker_;
}

}	//end idee