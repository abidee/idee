xqlib is a temperary name of this library

xqlib is portable C++ library suport windows 64 and linux 64, it include base classes which multi-thread application
needed, and  network framework. 

this library will use std c++ and boost as base facility.

supported compiler: VC10 , MinGW on windows platform, GCC on linux . can be used as shared library(DLL) or source code.

Development enviroment is Visual Studi 2010 and Eclipse.

classes list by category:

Thread Syncrinize object: 
	ThreadMutex
	ThreadRWMutex
	ThreaSemaphore
	ThreadEvent 
	Guard
Process and Thread management:
	Process
	Thread
	ThreadPool
Message Queue:
	MessageQueue
Memoru Pool:
	MemoryPool
Network framework:
	Reactor

IO Poll strategy:
	Poll
	Epoll
	IOCP
Basic socket wrapper:
	Socket
Time object and TimeQueue:
	TimeValue
	TimerQueue
Other utlity:
	ConfigFile
	
	
local repo add one line