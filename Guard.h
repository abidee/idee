#ifndef XQ_GUARD_H
#define XQ_GUARD_H

/**
 * @class Guard
 *
 * @brief It performs automatic acquisition and release of
 * a parameterized synchronization object T.
 *
 *  A successfully constructed Guard does NOT mean that the lock was acquired!  
 * Code like this is dangerous:
 *   {
 *     Guard<Lock> g(lock);
 *     ... perform critical operation requiring lock to be held ...
 *   }
 * Instead, one must do something like this:
 *   {
 *     Guard<Lock> g(lock);
 *     if (! g.locked())
 *       {
 *         ... handle error ...
 *       }
 *     else
 *       {
 *         ... perform critical operation requiring lock to be held ...
 *       }
 *   }
 */


namespace idee
{

template <class T>
class XQ_EXPORT_TEMPLATE Guard
{
public:
 
  Guard (T &lock);

  /// Implicitly and automatically acquire (or try to acquire) the lock.  
  /// If @a block is non-0 then acquire() the T, else tryacquire() it.
  Guard (T &lock, bool block);
  
  ///unlock in dtor 
  ~Guard (void);
    
  /// Explicitly acquire the lock.
  int lock (void);

  /// Conditionally acquire the lock (i.e., won't block).
  int try_lock (void);

  /// Explicitly release the lock, but only if it is held!
  int unlock (void);

   /// Explicitly remove the lock.
  int remove (void);

  /// Relinquish ownership of the lock so that it is not released
  /// implicitly in the destructor.
  void disown (void);

  /// true if locked, false if couldn't acquire the lock
  /// (errno will contain the reason for this).
  bool locked (void) const;
 
  /// Dump the state of an object.
  //void dump (void) const;

protected:
  /// Pointer to the T we're guarding.
  T *locker_;

  /// Keeps track of whether we acquired the lock or failed.
  //0: acquire lock -1: error
  int owner_;

private:
  //prevent assignment and initialization.
  void operator= (const Guard<T> &);
  Guard (const Guard<T> &);
};


template <class T> XQ_INLINE
Guard<T>::Guard (T &lock)
  : locker_ (&lock),
    owner_ (0)
{
	this->lock();
}

template <class T> XQ_INLINE
Guard<T>::Guard (T &lock, bool block)
  : locker_ (&lock),
    owner_ (0)
{
	if (block)
		this->lock();
	else
		this->try_lock();
}

//template <class T> XQ_INLINE
//Guard<T>::Guard (T &l, bool /* block */, int become_owner)
//  : locker_ (&l),
//    owner_ (become_owner == 0 ? -1 : 0)
//{
//}

template <class T> XQ_INLINE
Guard<T>::~Guard (void)
{
	this->unlock();
}

template <class T> XQ_INLINE int
Guard<T>::lock(void)
{
	return this->owner_ = this->locker_->lock();
}

template <class T> XQ_INLINE int
Guard<T>::try_lock(void)
{
	return this->owner_ = this->locker_->try_lock();
}

template <class T> XQ_INLINE int
Guard<T>::unlock (void)
{
	if(this->owner_ == -1)
		return -1;
	else
	{
		this->owner_ = -1;
		return this->locker_->unlock();
    }
}

template <class T> XQ_INLINE bool
Guard<T>::locked (void) const
{
	return this->owner_ != -1;
}

template <class T> XQ_INLINE int
Guard<T>::remove (void)
{
	return this->locker_->remove ();
}

template <class T> XQ_INLINE void
Guard<T>::disown (void)
{
	this->owner_ = -1;
}


}	//end idee

#endif