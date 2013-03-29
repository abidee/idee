/**
* @file MessageBlock.h
* @brief 提供对一小块连续内存的操作, 有读写指针, 容量可以比实际数据量大, 可以放入MessageQueue中
* @author X_Q
*/

#ifndef XQ_MESSAGEBLOCK_H
#define XQ_MESSAGEBLOCK_H

namespace idee
{

class XQ_EXPORT MessageBlock
{

enum 
{
	MSG_DATA	= 0x01,
	MSG_NOT_FREE = 0x80		//if MSG_NOT_DELETE, release() will not free data_
};

public:
	MessageBlock(size_t len);

	///ctor with pre-alloc data with length
	MessageBlock(char* buf, size_t len);
	~MessageBlock();

	void init(size_t len);
	void release();

	///get capacity 
	size_t capacity();

	///get real data length
	size_t length();
private:
	char* data_;		//data ptr
	
	size_t capacity_;	//data length
	size_t rd_ptr_;		//reader ptr
	size_t wr_ptr_;		//write ptr

	unsigned int msg_type_;	

};

}

#endif