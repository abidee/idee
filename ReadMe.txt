idee is a temperary name of this library

idee is portable C++ library suport windows X64 and linux X86_64, it include base classes which multi-thread application
needed, and network framework. 

this library will use std c++ and boost as base facility. conformance to C11 thread specification and extend some usable functions.

supported compiler: VC10 , MinGW on windows platform, GCC4.x on linux . can be used as shared library(DLL,so) or source code.

Development enviroment is Visual Studi 2010 and Eclipse Juno, the 2 IDE can work in one direcory.

classes list by category:

stage1: 
Thread Syncrinize object: 
	Mutex
	ReadWriteMutex
	Semaphore	
	Guard
Process and Thread management:
	Process
	Thread
	ThreadPool
Message Queue:
	MessageBlock
	MessageQueue
Memoru Pool:
	MemoryPool
Other utlity:
	ConfigFile
	
	
stage2:
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
