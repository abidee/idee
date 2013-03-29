/**
 * @class Mutex
 *
 * @brief mutex only valid for threads in the same
 * process).
 *
 * This implementation is optimized for locking threads that are
 * in the same process.  
 * It maps to <CRITICAL_SECTION>s on NT and maps <pthread_mutex_t> on Linux.
 * Mutex is recursive on windows platforms. However, on linux it is 
 * not recursive. you must use <pthread_mutex_attr>
 ****************************************
 */

#ifndef XQ_MUTEX_H
#define XQ_MUTEX_H

#if defined(XQ_WIN_PLATFORM)
	#include "win/Win_Sync_Func.h"
#elif defined(XQ_LINUX_PLATFORM) 
	#include "posix/Posix_Sync_Func.h"
#endif

namespace idee
{
using namespace os_func;

class XQ_EXPORT Mutex
{ 
public:
  // Constructor with name
  Mutex (const char *name = 0 /*, ACE_mutexattr_t *attributes = 0*/);
  
  ~Mutex (void);

  /**
   * Explicitly destroy the mutex.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int lock (void);

 
  //int acquire (ACE_Time_Value &tv);

  /**
   * If @a tv == 0 the call acquire() directly.  Otherwise, Block the
   * thread until we acquire the mutex or until @a tv times out, in
   * which case -1 is returned with @c errno == @c ETIME.  Note that
   * @a tv is assumed to be in "absolute" rather than "relative" time.
   * The value of @a tv is updated upon return to show the actual
   * (absolute) acquisition time.
   */
  //int acquire (ACE_Time_Value *tv);

  /**
   * Conditionally acquire lock (i.e., don't wait on queue).  Returns
   * -1 on failure.  If we "failed" because someone else already had
   * the lock, @c errno is set to @c EBUSY.
   */
  int try_lock (void);

  /// Release lock and unblock a thread at head of queue.
  int unlock (void);

  /**
   * This calls lock() and is only here
   * to make the Mutex interface consistent with the
   * other synchronization Class.
   */
  int lock_read (void);

  /**
   * This calls lock() and is only here
   * to make the Mutex interface consistent with the
   * other synchronization Class.
   */
  int lock_write (void);
  
  int try_lock_read (void);
  
  int try_lock_write (void);  

  // Return the underlying mutex.
  const XQ_mutex_t &native_handle (void);
  
protected:
  // Mutex type that supports single-process locking efficiently.
  XQ_mutex_t locker_;

  /// Keeps track of whether remove() has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  bool removed_;

private:
	//Prevent = assignment and copy initialization.
	Mutex & operator= (const Mutex &);
	Mutex (const Mutex &);
};

}	//end idee

#endif
