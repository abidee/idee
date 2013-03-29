/**
* @file Win_Thread_Func.h
* @brief windows thread functions wrapper
* @author xq
*/

#ifndef WIN_THREAD_FUNC_H
#define WIN_THREAD_FUNC_H

namespace idee
{
namespace os_func
{

#if defined(XQ_WIN_PLATFORM)

typedef HANDLE XQ_thread_t;			//windows thread handle
typedef DWORD XQ_THR_FUNC_RETURN;  //windows thread function return value

#elif defined(XQ_LINUX_PLATFORM) 

typedef pthread_t XQ_thread_t;		//linux thread handle 
typedef void* XQ_THR_FUNC_RETURN;	//linux thread function return value

#endif

//wondows thread functions 
int thread_create(XQ_thread_t *t);
int thread_join(XQ_thread_t *t, XQ_THR_FUNC_RETURN *status);
int thread_resume(XQ_thread_t *t);
int thread_suspend(XQ_thread_t *t);
void thread_yield(XQ_thread_t *t);
int thread_kill(XQ_thread_t *t); 

}
}
//typedef XQ_THR_FUNC_RETURN (*XQ_THR_FUNC )(void *);   //线程函数指针定义


#endif