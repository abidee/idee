#include "stdafx.h"
#include "MessageBlock.h"

namespace idee
{

MessageBlock::MessageBlock(size_t len, size_t head)
{
	this->init(len, head);
}

MessageBlock::MessageBlock(char* buf, size_t len)
	: base_(buf), size_(len), rd_ptr_(0), wr_ptr_(len)
{
	this->msg_type_ = MSG_NOT_FREE;
}

MessageBlock::~MessageBlock()
{
	release();
}

XQ_INLINE void MessageBlock::swap(MessageBlock& rhs)
{
	std::swap(base_, rhs.base_);
	std::swap(rd_ptr_, rhs.rd_ptr_);
	std::swap(wr_ptr_, rhs.wr_ptr_);
	std::swap(size_, rhs.size_);
	std::swap(msg_type_, rhs.msg_type_);
}

XQ_INLINE void MessageBlock::init(size_t len, size_t head)
{
	this->base_ = (char*)malloc(len);

	this->size_ = len;
	this->rd_ptr_ = head;
	this->wr_ptr_ = head;
}

XQ_INLINE void MessageBlock::release()
{
	if(msg_type_ != MSG_NOT_FREE)
		free(this->base_);
}
	
XQ_INLINE size_t MessageBlock::capacity()
{
	return this->size_;
}

XQ_INLINE size_t MessageBlock::space()
{
	return this->size_ - this->wr_ptr_;
}

XQ_INLINE size_t MessageBlock::data_len()
{
	size_t len = this->wr_ptr_ - this->rd_ptr_;
	return len >= 0 ? len : -1;
}

XQ_INLINE char *MessageBlock::rd_ptr()
{
	return this->base_ + this->rd_ptr_;
}

XQ_INLINE char *MessageBlock::wr_ptr()
{
	return this->base_ + this->wr_ptr_;
}

XQ_INLINE void MessageBlock::rd_ptr(int len)
{
	this->rd_ptr_ += len;  //TODO 小于0 或者 超过size 怎么办?
}

XQ_INLINE void MessageBlock::wr_ptr(int len)
{
	this->wr_ptr_ += len;
}

int  MessageBlock::append(const char* buf, size_t len)
{
	if(len <= this->space())  //正常可以放在后面的space
	{
		memcpy(this->wr_ptr(), buf, len);
		wr_ptr_ += len;
		return 0;
	}
	else if(len <= this->rd_ptr_) //前面可以放的下
	{
		size_t old_len = this->data_len();
		void *tmp = malloc(old_len);
		memcpy(tmp, this->rd_ptr(), old_len);

		memcpy(this->base_, tmp, old_len);
		memcpy((this->base_+old_len), buf, len);
		this->rd_ptr_ = 0;
		this->wr_ptr_ = old_len + len;

		return 0;
	}
	else  //前后都放不下, 需要扩充内存大小
	{
		this->release();
		this->base_ = (char*)malloc(len);
		memcpy(this->base_, buf, len);
		this->rd_ptr_ = 0;
		this->wr_ptr_ = len;
		return 0;
	}
}

}