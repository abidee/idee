#include "stdafx.h"

#include "Semaphore.h"

namespace idee
{

Semaphore::Semaphore(const char *name)
{
	init();
}

Semaphore::~Semaphore()
{
	destroy();
}

///initialize sema
XQ_INLINE int Semaphore::init()
{
	return os_func::sema_init(&sema_);
}
	

XQ_INLINE int Semaphore::wait()
{
	return os_func::sema_wait(&sema_);
}


XQ_INLINE int Semaphore::post()
{
	return os_func::sema_post(&sema_);
}


XQ_INLINE int Semaphore::destroy()
{
	return os_func::sema_destroy(&sema_);
}

XQ_INLINE const XQ_sema_t& Semaphore::native_handle()
{
	return sema_;
}

}	//end idee