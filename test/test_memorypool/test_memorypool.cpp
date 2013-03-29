// testlib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Mutex.h"
#include "Guard.h"
#include "MemoryPool.h"
#include "Semaphore.h"
#include "Thread.h"

#include <iostream>


using namespace idee;
using namespace idee::os_func;

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	idee::MemoryPool pool;
	pool.init(128, 8);

	char *p = pool.pool_alloc(100);
	char *p1 = pool.pool_alloc(100);
	char *p2 = pool.pool_alloc(100);

	if(p) pool.pool_free(p);
	if(p1) pool.pool_free(p1);

	pool.release();

	return 0;
};


