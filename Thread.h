
/**
 *  @file    Thread.h
 *
 *  @brief Thread class 
 *
 *  @author X_Q
 */

#ifndef XQ_THREAD_H
#define XQ_THREAD_H

#if defined(XQ_WIN_PLATFORM)
	#include "win/Win_Thread_Func.h"
#elif defined(XQ_LINUX_PLATFORM) 
	#include "posix/Posix_Thread_Func.h"
#else
	#include "posix/Posix_Thread_Func.h"
#endif

namespace idee
{

using namespace os_func;

/**
 * @class Thread
 *
 * @brief Provides a wrapper for Threads.
 *
 * This class provides a common interface that is mapped onto POSIX PThreads and Win32 Threads
 * It maps to <HANDLE> on windows
 * It maps to <pthread_t> on linux
 */

class XQ_EXPORT Thread
{
public:
	Thread();
	Thread(int (*func)(void *), void *arg);   //constructor with external Thread_func
	virtual ~Thread();

public:
	///create thread 
	int spawn(int (*func)(void *), void *arg); 

	///get platform specified native handle
	XQ_thread_t& native_handle();

	/// join by a Thread other than this one 
	int join(XQ_THR_FUNC_RETURN *status);
	
	///this func no throws exception
	bool joinable();

	///resume Thread running
	int resume();

	///suspend Thread running
	int suspend();

	///yield CPU to another Thread 
	void yield();

	///this Thread sleep milliseconds
	void sleep(int ms);

	//TODO add get_id and Thread::id later ...
	//int get_id();

	//platform sepcific Thread function(windows, posix)
#if defined(XQ_WIN_PLATFORM)
    static unsigned WINAPI thread_func(void * arg);
#elif defined(XQ_LINUX_PLATFORM)
    static void * thread_func(void * arg);
#endif
	
private:
	XQ_thread_t handle_;		//platform sepcific Thread handle
	unsigned int thread_id_;  //Thread id only for windows 
	//Thread::id id_;			//id for linux
private:
	Thread(const Thread& rhs);
	Thread& operator = (const Thread& rhs);
};

/**
* @class ThreadStartInfo
* @brief thread entry func, arguments, thread_ptr data 
*/
struct ThreadStartInfo 
{
  int (*func_)(void *);		// Pointer to the function to be executed.
  void * arg_;				// Function argument for the Thread function.
  Thread * thread_ptr_;     // Pointer to the Thread object.
};

}	//end idee

#endif 
