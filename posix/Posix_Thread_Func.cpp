#include "..\stdafx.h"

#include "Posix_Thread_Func.h"


namespace idee
{
namespace os_func
{

#if defined(XQ_WIN_PLATFORM)


#elif defined(XQ_LINUX_PLATFORM) 
//linux thread functions

int thread_join(XQ_thread_t *t, XQ_THR_FUNC_RETURN *status)
{
	pthread_join(*t, status);
	return 0;
}

void thread_yield(XQ_thread_t *t)
{
	return;
}

#endif

}
}