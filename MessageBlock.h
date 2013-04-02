/**
* @file MessageBlock.h
* @brief �����ڴ滺�����Ĳ�����, ���ݴ����
* @author X_Q
* 
* ��ע: �ṩ��һ�������ڴ滺�����Ĳ���, �ж�дָ��
* ������Ϊһ�����ݿ�ʹ��, ����MessageQueue��
* Ҳ������Ϊһ��socket���ӵ� input buffer��output buffer��
* Ϊ���������, ���󲻳�������ʱֻ���Ӳ����� 
* 
* buffer memory structure:
*  +-------------------+-------------------------------+------------------------------+
*  + headable bytes    | readable bytes                |  writable bytes              |
*  +                   | ##### here is real data ##### |                              |
*  +-------------------+-------------------------------+------------------------------+
*  0     <=         rd_ptr_         <=            wr_ptr_          <=                size
*
*/

#ifndef XQ_MESSAGEBLOCK_H
#define XQ_MESSAGEBLOCK_H

namespace idee
{

class XQ_EXPORT MessageBlock
{
public:
	enum 
	{
		MSG_DATA	= 0x01,
		MSG_NOT_FREE = 0x80		//if MSG_NOT_DELETE, release() will not free data_
	};

	static const size_t max_buffer_size_ = 1024 * 1024;		//maxium buffer size 

public:
	//ctor with buffer size and head 
	MessageBlock(size_t len=1024, size_t head=8);

	//ctor with pre-alloc data with length
	MessageBlock(char *buf, size_t len);
	~MessageBlock();

	void swap(MessageBlock& rhs);

	void init(size_t len, size_t head);
	void release();

	///get capacity 
	size_t capacity();

	///get writable space
	size_t space();

	///get real data length
	size_t data_len();

	///append data into buffer
	int append(const char *buf, size_t len);

	//get read or write ptr
	char *rd_ptr();
	char *wr_ptr();

	//set read or write ptr
	void rd_ptr(size_t len);
	void wr_ptr(size_t len);
private:
	char *base_;		//base data ptr
	
	size_t size_;		//total bufer size 
	size_t rd_ptr_;		//read ptr
	size_t wr_ptr_;		//write ptr

	unsigned int msg_type_;	

};

}

#endif