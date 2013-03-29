#include "stdafx.h"
#include "MessageBlock.h"

namespace idee
{

MessageBlock::MessageBlock(size_t len)
{
	this->init(len);
}

MessageBlock::MessageBlock(char* buf, size_t len)
	: data_(buf), capacity_(len), rd_ptr_(0), wr_ptr_(len)
{
	this->msg_type_ = MSG_NOT_FREE;
}

MessageBlock::~MessageBlock()
{
	release();
}

XQ_INLINE void MessageBlock::init(size_t len)
{
	this->data_ = (char*)malloc(len);

	this->capacity_ = len;
	this->rd_ptr_ = 0;
	this->wr_ptr_ = 0;
}

XQ_INLINE void MessageBlock::release()
{
	if(msg_type_ != MSG_NOT_FREE)
		free(this->data_);
}
	
XQ_INLINE size_t MessageBlock::capacity()
{
	return this->capacity_;
}


XQ_INLINE size_t MessageBlock::length()
{
	size_t len = this->wr_ptr_ - this->rd_ptr_;
	return len >= 0 ? len : -1;
}


}