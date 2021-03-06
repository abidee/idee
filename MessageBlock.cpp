#include "config_idee.h"

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
	this->base_ = (char*)malloc(len + head);

	this->size_ = len + head;
	this->rd_ptr_ = head;
	this->wr_ptr_ = head;
}

XQ_INLINE void MessageBlock::release()
{
	if(msg_type_ != MSG_NOT_FREE)
	{
		free(this->base_);
		this->base_ = NULL;
	}
	this->size_	  = 0;
	this->rd_ptr_ = 0;
	this->wr_ptr_ = 0;
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

XQ_INLINE char *MessageBlock::rd_ptr() const
{
	return this->base_ + this->rd_ptr_;
}

XQ_INLINE char *MessageBlock::wr_ptr() const
{
	return this->base_ + this->wr_ptr_;
}

XQ_INLINE void MessageBlock::rd_ptr(int len)
{	
	if((this->rd_ptr_ + len) > this->size_)
		this->rd_ptr_ = this->size_;
	else
		this->rd_ptr_ += len;  //TODO 小于0怎么办?
}

XQ_INLINE void MessageBlock::wr_ptr(int len)
{
	if((this->wr_ptr_ + len) > this->size_)
		this->wr_ptr_ = this->size_;
	else
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
	else if(len <= this->rd_ptr_) //前面可以放的下,后面的前移,新的拼接起来
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
	else  //前后都放不下, 需要重新分配内存大小
	{	
		char *old_base = this->base_;
		char *old_data = this->rd_ptr();
		int old_len = this->data_len();

		this->init(old_len + len);
		this->append(old_data, old_len);
		this->append(buf, len);

		free(old_base);
		return 0;
	}
}

}