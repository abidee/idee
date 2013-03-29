/**
 * @class Semaphore
 *
 * @brief Semaphore object crosss platform
 *
 * It maps to <HANDLE> on windows 
 * It maps to <sem_t> on linux
 */

#ifndef XQ_SEMAPHORE_H
#define XQ_SEMAPHORE_H

#if defined(XQ_WIN_PLATFORM)
	#include "win/Win_Sync_Func.h"
#elif defined(XQ_LINUX_PLATFORM) 
	#include "posix/Posix_Sync_Func.h"
#endif

namespace idee
{
using namespace os_func;

class XQ_EXPORT Semaphore
{
public:
	Semaphore(const char *name = 0);
	~Semaphore();

	///initialize sema
	int init();
	
	///wait sema decrese count by 1
	int wait();

	///post sema increse count by 1
	int post();

	///destroy semaphore object 
	int destroy();

	const XQ_sema_t &native_handle();
private:
	XQ_sema_t sema_;	//semaphore handle
};

} //end idee

#endif 