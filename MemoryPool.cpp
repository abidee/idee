
#include "stdafx.h"

#include "detail/buddy.h"
#include "Mutex.h"
#include "Guard.h"

#include "MemoryPool.h"

namespace idee
{

MemoryPool::MemoryPool()
{		
}

MemoryPool::~MemoryPool()
{
	release();
}

void MemoryPool::init(unsigned short block, unsigned int maxsize)
{	
	//�����ڴ�صļ���(buddy�Ķ������ļ���)
	int level = 0;
	double mantissa=0.0;	
	mantissa=frexp((double)maxsize, &level);
	if(0.5==mantissa || (!maxsize & (maxsize - 1)))
	{
		level--;
	}

	this->level = level;
	this->block = block;
	this->size  = maxsize;
	this->used  = 0;

	this->pools[this->used].buffer=(char*)malloc((1<<level) * block);
	if(!this->pools[this->used].buffer)
		return;	
	memset(this->pools[this->used].buffer, 0, (1<<level) * block);

	//����һ��buddy���ڹ����ڴ�أ��������Ϊ����Ŀ���
	this->pools[this->used].alloc = buddy_new(1<<level);
	this->used++;

	return;
}


void MemoryPool::release()
{
	unsigned int index=0;
	//�ͷ����д�����buddy�Ϳ��ٵ��ڴ�,�ͷ�ǰ��Ҫֹͣ����ʹ�õ��߳���
	for(;index<this->used;index++)
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
	}	
	this->used = 0;
}

/**************************************************************************
* ����: pool_alloc
* ����: ���ڴ�ط���һ���ڴ�
* ����: len---���ݳ���(������Ȳ��ܱ�maxsize����)
* ���: N/A
* ����: nt_char* ����������ڴ��ַ
* ��ע: ��С���䳤��Ϊ���ڴ�ص�һ����С��λ��block�Ĵ�С��
***************************************************************************/
char* MemoryPool::pool_alloc(unsigned int len)
{
	int offset = (-1);
	unsigned int index = 0;
	char *_buffer = NULL;
	if(!len || len > ((1<<this->level) * this->block))
		return NULL;
	//��Ҫ����ĳ���block��С����, ����Ҳռ��һ��block
	if(len % this->block)
	{		
		len = (((len) + (this->block - 1)) & ~(this->block - 1));
	}
	
	//������Guard, ����������unlock
	Guard<Mutex> guard(this->locker_);
	if(!guard.locked())
		return NULL;

	//ѭ�����Դ��Ѿ��������ڴ�ص�Ԫ�з����ڴ�
	for(;index<this->used;index++)
	{
		offset = buddy_alloc(this->pools[index].alloc, len / this->block);
		if(offset >= 0)
		{
			_buffer = this->pools[index].buffer + offset * this->block;				
			return _buffer;
		}
	}

	//��������е��ڴ�ص�Ԫ��û�����뵽�ڴ棬���¿���һ���ڴ�ص�ԪȻ���ٷ���
	if(this->used >= MAX_POOL_NUM) //����Poolunit����
	{
		//log(LM_ERROR, "Memory is full");
		return NULL;
	}
	this->pools[this->used].buffer=(char*)malloc((1<<this->level) * this->block);
	if(!this->pools[this->used].buffer)
		return NULL;
	
	memset(this->pools[this->used].buffer, 0, (1<<this->level) * this->block);
	this->pools[this->used].alloc = buddy_new(1<<this->level);
	this->used++;
	offset = buddy_alloc(this->pools[this->used - 1].alloc, len/this->block);
	if(offset>=0)
	{
		_buffer=this->pools[this->used - 1].buffer + offset * this->block;		
		return _buffer;
	}
	
	return NULL;
}

/**************************************************************************
* ����: pool_free
* ����: ���ڴ���ͷ�һ���ڴ�
* ����: buf---�ڴ��ַ
* ���: N/A
* ����: N/A
* ��ע: N/A
***************************************************************************/
void MemoryPool::pool_free(char* buf)
{
	int offset = -1, index = -1, idx = 0;

	//������Guard, ��������ʱunlock
	Guard<Mutex> guard(this->locker_);
	if(!guard.locked())
		return; //TODO ��һû��ס, ���buf��Զ���ͷ�����ô��?
	
	//������Ҫ�ͷŵ��ڴ�����ĳ���ڴ�ص�Ԫ
	for(idx = 0;idx < this->used; idx++)
	{
		if(buf >= this->pools[idx].buffer && buf < (this->pools[idx].buffer+(1<<this->level) * this->block))
		{
			index = idx;
			break;
		}
	}
	if(index<0 || index>=MAX_POOL_NUM)	
		return;
	
	//�����ڴ��ַ��������ڴ����ڴ�ص�Ԫ�е�ƫ��,��buddy���ͷŴ�ƫ�Ƶ�ռ�ñ��
	offset = ((int)(buf - this->pools[index].buffer)) / this->block;	
	if(offset>=0 && offset <= 1<<this->level)
	{
		buddy_free(this->pools[index].alloc, offset);
	}

	//TODO ������Ǹ��õ�����,�Ƿ���Կ��ǲ���ÿ���ж�ɾ������poolunit??

	//��������ڴ�ص�Ԫ���Ѿ����ж��Ҵ��ڴ�ص�Ԫ���ǵ�һ�����������ڴ�ص�Ԫ�����ͷ�������Դ
	if(index && buddy_empty(this->pools[index].alloc))
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
		if(index!=this->used-1)
		{
			//����ͷŵ����ڴ�ص�Ԫ���ڴ�ع��������е�λ�ò������һ�������ڴ�ص�Ԫ��������һ����������ǰ�ͷŵ��ĵط�
			this->pools[index].alloc = this->pools[this->used-1].alloc;
			this->pools[index].buffer = this->pools[this->used-1].buffer;
		}
		this->used--;
	}
	
}

}	//end idee