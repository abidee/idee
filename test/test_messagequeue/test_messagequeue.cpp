// test_bind.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Mutex.h"
#include "MessageQueue.h"

using namespace idee;

int main(int argc, char** argv)
{
	MessageQueue *queue = new MessageQueue();
	MessageBlock *block = new MessageBlock(1024,8);
	
	block->rd_ptr(512);
	block->wr_ptr(1000);
	char *p = new char[256];
	memset(p,1,256);
	block->append(p, 256);
	queue->push_front(block);
	queue->push_back(block);

	size_t len = queue->msg_count();
	size_t len1 = queue->total_size();
	return 0;
}

