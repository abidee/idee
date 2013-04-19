// test_bind.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Mutex.h"
#include "MessageQueue.h"

using namespace idee;

int main(int argc, char** argv)
{
	MessageQueue *queue = new MessageQueue();
	MessageBlock *block = new MessageBlock();
	block->wr_ptr(16);
	queue->push_front(block);
	queue->push_front(block);

	size_t len = queue->msg_count();	

	return 0;
}

