// testlib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Mutex.h"
#include "Guard.h"
#include "MemoryPool.h"
#include "Semaphore.h"
#include "Thread.h"

#include <iostream>

int thr_func(void *);

using namespace idee;
using namespace idee::os_func;

using namespace std;

Semaphore sema;
int g_num;

int _tmain(int argc, _TCHAR* argv[])
{
	Thread t(thr_func, NULL);
	
	int ret = sema.init();
	int i = 0;
	while(true)
	{
		scanf("%d", &g_num);
		sema.post();
		if(g_num == 200)
			t.suspend();
		if(g_num == 300)
			t.resume();
		if(g_num == 100)
			break;
	}

	DWORD flag;
	t.join(&flag);

	return 0;
};

int thr_func(void *)
{
	while(true)
	{
		Sleep(1000);
		int i = sema.wait();
		if(i == 0)
			cout << "thread get signal" << endl;
		if(g_num == 100) break;

	}
	return 0;
}

