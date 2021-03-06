
#include "config_idee.h"


#include "Thread.h"


namespace idee
{

Thread::Thread()
{
	
}

Thread::Thread(int (*func)(void *), void *arg)
{
	spawn(func, arg);
}

Thread::~Thread()
{	
}

int Thread::spawn(int (*func)(void *), void *arg)
{
	ThreadStartInfo * tsi = new ThreadStartInfo();
	tsi->func_ = func;
	tsi->arg_ = arg;
	tsi->thread_ptr_ = this;  

  // Create the thread
#if defined(XQ_WIN_PLATFORM)
	handle_ = (HANDLE) _beginthreadex(0, 0, thread_func, (void *) tsi, 0, &thread_id_);
#elif defined(XQ_LINUX_PLATFORM)
	if(pthread_create(&handle_, NULL, thread_func, (void *) tsi) != 0)
		handle_ = 0;
#endif

// Did we fail to create the thread?
	if(!handle_)
	{    
		delete tsi;
		return -1;
	}
	return 0;
}


#if defined(XQ_WIN_PLATFORM)
    unsigned WINAPI Thread::thread_func(void *arg)
#else
    void * Thread::thread_func(void *arg)
#endif
	{
	  // Get thread startup information
	  ThreadStartInfo *tsi = (ThreadStartInfo *) arg;
	  try
	  {
		  if(tsi->func_ != NULL)
		  {
			// Call the actual externel function
			tsi->func_(tsi->arg_);
		  }
		  else
		  {
			//you can call this Thread member function Task 
			Thread *this_ptr = (Thread*) tsi->thread_ptr_ ;
			this_ptr->task(NULL);
		  }
	  }
	  catch(...)
	  {
		// Uncaught exceptions will terminate the application (default behavior
		// according to the C++0x draft)
		//std::terminate();
	  }	  

	  // The thread is responsible for freeing the startup information
	  delete tsi;
	  return 0;
	}


XQ_INLINE int Thread::resume ()
{  
  return os_func::thread_resume (&this->handle_);
}

XQ_INLINE int Thread::suspend ()
{ 
  return os_func::thread_suspend (&this->handle_);
}

XQ_INLINE void Thread::yield()
{
	os_func::thread_yield(&this->handle_);
}

XQ_INLINE int Thread::join(XQ_THR_FUNC_RETURN *status)
{
	return os_func::thread_join(&this->handle_, status);
}

XQ_INLINE XQ_thread_t& Thread::native_handle()
{
	return handle_;
}

}	//end idee