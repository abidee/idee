/**
*	@file 
* @brief 对不同操作系统的底层函数的封装
* @author xq
*/

#ifndef POSIX_MUTEX_FUNC_H
#define POSIX_MUTEX_FUNC_H


namespace idee
{
namespace os_func
{


#if defined(XQ_WIN_PLATFORM)

typedef CRITICAL_SECTION XQ_mutex_t;  //windows critical_section
typedef SRWLOCK XQ_rwmutex_t;		  //windows slim read/write lock
typedef HANDLE XQ_sema_t;			  //windows semaphore is HANDLE	

#elif defined(XQ_LINUX_PLATFORM) 

typedef pthread_mutex_t XQ_mutex_t;		//linux mutex
typedef pthread_rwlock_t XQ_rwmutex_t;	//linux rw mutex
typedef sem_t XQ_sema_t;				//linux semaphore


//os_func Mutex functions
int mutex_lock(XQ_mutex_t *m);
int mutex_unlock (XQ_mutex_t *m);
int mutex_trylock (XQ_mutex_t *m);
int mutex_destroy (XQ_mutex_t *m);
int mutex_init (XQ_mutex_t *m);  //, int lock_type, const char *name, void *arg);

//os_func RWMutex functions
int rwmutex_init(XQ_rwmutex_t *m);
int rwmutex_destroy(XQ_rwmutex_t *m);
int rwmutex_rdlock(XQ_rwmutex_t *m);
int rwmutex_rdunlock(XQ_rwmutex_t *m);
int rwmutex_wrlock(XQ_rwmutex_t *m);
int rwmutex_wrunlock(XQ_rwmutex_t *m);

//Semaphore funcitons
int sema_init(XQ_sema_t *m);
int sema_wait(XQ_sema_t *m);
int sema_post(XQ_sema_t *m);
int sema_destroy(XQ_sema_t *m);

#endif

}	//end os_func

}	//end idee

#endif
