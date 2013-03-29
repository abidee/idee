// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

//project config head, must include this file first

#define XQLIB_EXPORT  //for exprot dll 

#include "config.h"

#if defined(XQ_WIN_PLATFORM)

	// ���� SDKDDKVer.h ��������õ���߰汾�� Windows ƽ̨��
	// ���ҪΪ��ǰ�� Windows ƽ̨����Ӧ�ó�������� WinSDKVer.h������
	// WIN32_WINNT ������ΪҪ֧�ֵ�ƽ̨��Ȼ���ٰ��� SDKDDKVer.h��
	#include <SDKDDKVer.h>

	#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

	// Windows ͷ�ļ�:
	#include <windows.h>
	#include <WinBase.h>

	// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
	#include <process.h>

	//std headers
	#include <list>
	#include <vector>
	#include <deque>
	
	//C runtime headers
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>

#elif defined(XQ_LINUX_PLATFORM)
	//os specific headers
	#include <pthread.h>
	#include <signal.h>
	#include <sched.h>
	#include <unistd.h>
	#include <semaphore.h>

	//std headers
	#include <list>
	#include <vector>
	#include <deque>

	//C runtime headers
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>

#endif

