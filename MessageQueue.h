/**
* @file MessageQueue.h
* @brief Simple message queue in multi-thread programming, this class has a lock 
* @author X_Q
*
* ±¸×¢:
* 
*
*
*/

#ifndef XQ_MESSAGE_QUEUE_H
#define XQ_MESSAGE_QUEUE_H

#include "MessageBlock.h"

namespace idee
{

//template <class T>
class XQ_EXPORT MessageQueue
{
public:
	//typedef T value_type;
public:
	MessageQueue();
	~MessageQueue(void);

protected:
	void init();
	void release();

	void peek_front(MessageBlock*);
	void peek_back(MessageBlock*);

	int push_front(MessageBlock*);
	int push_back(MessageBlock*);

	int pop_front(MessageBlock*);
	int pop_back(MessageBlock*);
	
	//message count
	size_t msg_count();
private:		
	size_t total_size_;		//total bytes of the queue

	size_t high_limit_;		//high limit size
	size_t low_limit_;		//low limit size

private:
	Mutex locker_;			//locker 
	std::deque<MessageBlock> deque_;	//Message ovbject store in dueqe
	

};

}  //end idee


#endif