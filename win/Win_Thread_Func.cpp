#include "..\stdafx.h"

#include "Win_Thread_Func.h"

namespace idee
{
namespace os_func
{

#if defined(XQ_WIN_PLATFORM)

int thread_create(XQ_thread_t *t)
{
	return 0;
}

int thread_join(XQ_thread_t *t, XQ_THR_FUNC_RETURN *status)
{
	::WaitForSingleObject(*t, INFINITE);
	CloseHandle(*t);
	*status = 0;
	return 0;	
}

int thread_resume(XQ_thread_t *t)
{
	if(::ResumeThread(*t) != -1)
		return 0;
	else
		return -1;
}

int thread_suspend(XQ_thread_t *t)
{
	if(::SuspendThread(*t) != -1)
		return 0;
	else
		return -1;
}

void thread_yield(XQ_thread_t *t)
{
	return;
}

int thread_kill(XQ_thread_t *t)
{
	 ::CloseHandle(*t);
	 return 0;
}


#elif defined(XQ_LINUX_PLATFORM) 


#endif

} //end os_func
} //end idee
