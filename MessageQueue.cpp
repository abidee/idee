#include "config_idee.h"

#include "Mutex.h"
#include "MessageQueue.h"

namespace idee
{

//template <class T>
MessageQueue::MessageQueue(size_t high_limit, size_t low_limit)
	:total_size_(0), high_limit_(high_limit), low_limit_(low_limit)
{
}

//template <class T>
MessageQueue::~MessageQueue()
{
}

void MessageQueue::init()
{
	return;
}

void MessageQueue::release()
{
	this->locker_.lock();

	std::deque<MessageBlock>::iterator it;
	for(it = this->deque_.begin(); it != this->deque_.end(); it++)
	{
		it->release();
	}
	this->deque_.clear();
	this->total_size_ = 0;
	this->locker_.unlock();
}

XQ_INLINE size_t MessageQueue::msg_count()
{
	return this->deque_.size();
}

XQ_INLINE size_t MessageQueue::total_size()
{
	return this->total_size_;
}

void MessageQueue::peek_front(MessageBlock* mb)
{
	this->locker_.lock();

	*mb = this->deque_.front();	

	this->locker_.unlock();
}

void MessageQueue::peek_back(MessageBlock* mb)
{
	this->locker_.lock();

	*mb = this->deque_.back();	

	this->locker_.unlock();
}

int MessageQueue::push_front(MessageBlock* mb)
{
	this->locker_.lock();

	if(this->total_size_ + mb->data_len() > this->high_limit_) 
	{
		//大于high_limit_, 把尾部的一个释放
		MessageBlock tmp = this->deque_.back();
		this->deque_.pop_back();
		this->total_size_ -= tmp.data_len();
		tmp.release();		
	}

	this->deque_.push_front(*mb);
	this->total_size_ += mb->data_len();

	this->locker_.unlock();

	return 0;
}

int MessageQueue::push_back(MessageBlock* mb)
{
	this->locker_.lock();

	if(this->total_size_ + mb->data_len() > this->high_limit_) 
	{
		//大于high_limit_, 把头部的一个释放
		MessageBlock tmp = this->deque_.front();
		this->deque_.pop_front();			
		this->total_size_ -= tmp.data_len();
		tmp.release();
	}

	this->deque_.push_back(*mb);
	this->total_size_ += mb->data_len();

	this->locker_.unlock();
	return 0;
}

int MessageQueue::pop_front(MessageBlock* mb)
{
	this->locker_.lock();

	*mb = this->deque_.front();
	this->deque_.pop_front();
	this->total_size_ -= mb->data_len();

	this->locker_.unlock();

	return 0;
}

int MessageQueue::pop_back(MessageBlock* mb)
{
	this->locker_.lock();

	*mb = this->deque_.back();
	this->deque_.pop_back();
	this->total_size_ -= mb->data_len();

	this->locker_.unlock();

	return 0;
}



}	//end idee