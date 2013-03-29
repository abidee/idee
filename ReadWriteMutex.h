/**
 * @class ReadWriteMutex
 *
 * @brief ReadWriteMutex wrapper (only valid for threads in the same
 * process).
 *
 * This implementation is optimized for locking performance on multiple read one write scenarios
 * It maps to <SRWLOCK> on windows 
 * It maps to <pthread_rwlock_t> on Linux
 */

#ifndef READWRITE_MUTEX_H
#define READWRITE_MUTEX_H

#if defined(XQ_WIN_PLATFORM)
	#include "win/Win_Sync_Func.h"
#elif defined(XQ_LINUX_PLATFORM) 
	#include "posix/Posix_Sync_Func.h"
#endif


namespace idee
{

using namespace os_func;

class XQ_EXPORT ReadWriteMutex
{ 
	typedef enum
	{
		RW_NULL = 0,
		RW_READ	= 1,
		RW_WRITE= 2
	} ReadWriteFlag;

public:
  /// Constructor.
  ReadWriteMutex (const char *name = 0 /*, ACE_mutexattr_t *attributes = 0*/);

  /// Implicitly destroy the mutex.
  ~ReadWriteMutex (void);

  /**
   * Explicitly destroy the mutex.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int lock (void);

  /**
   * Block the thread until we acquire the mutex or until @a tv times
   * out, in which case -1 is returned with @c errno == @c ETIME.  Note
   * that @a tv is assumed to be in "absolute" rather than "relative"
   * time.  The value of @a tv is updated upon return to show the
   * actual (absolute) acquisition time.
   */
  //int acquire (ACE_Time_Value &tv);

  
  /**
   * Conditionally acquire lock (i.e., don't wait on queue).  Returns
   * -1 on failure.  If we "failed" because someone else already had
   * the lock, @c errno is set to @c EBUSY.
   */
  int try_lock (void);

  /// Release lock and unblock a thread at head of queue.
  int unlock (void);

  /**
   * Get a read lock
   */
  int lock_read (void);

  /**
   * Acquire write lock
   */
  int lock_write (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * tryacquire() and is only here to make the ACE_Thread_Mutex
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, @c errno is set to @c EBUSY.
   */
  int try_lock_read (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * tryacquire() and is only here to make the ACE_Thread_Mutex
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, @c errno is set to @c EBUSY.
   */
  int try_lock_write (void);

  /**
   * This is only here to make the ACE_Thread_Mutex interface
   * consistent with the other synchronization APIs.  Assumes the
   * caller has already acquired the mutex using one of the above
   * calls, and returns 0 (success) always.
   */
  //int tryacquire_write_upgrade (void);

  // Return the underlying mutex.
  const XQ_rwmutex_t & native_handle(void);  

  /// Dump the state of an object.
  //void dump (void) const;  

protected:
  // Mutex type that supports single-process locking efficiently.
  XQ_rwmutex_t locker_;

  /// Keeps track of whether remove() has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  bool removed_;

  //Read or Write Flag used by unlock() to call different release function
  ReadWriteFlag rw_flag_;  

private:
  // = Prevent assignment and initialization.
  void operator= (const ReadWriteMutex &);
  ReadWriteMutex (const ReadWriteMutex &);
};

}	//end idee
#endif