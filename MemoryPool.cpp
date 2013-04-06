
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
	//计算内存池的级数(buddy的二叉树的级数)
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

	//创建一个buddy用于管理内存池，输入参数为管理的块数
	this->pools[this->used].alloc = buddy_new(1<<level);
	this->used++;

	return;
}


void MemoryPool::release()
{
	unsigned int index=0;
	//释放所有创建的buddy和开辟的内存,释放前需要停止所有使用的线程序
	for(;index<this->used;index++)
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
	}	
	this->used = 0;
}

/**************************************************************************
* 名称: pool_alloc
* 功能: 从内存池分配一块内存
* 输入: len---数据长度(这个长度不能比maxsize更大)
* 输出: N/A
* 返回: nt_char* 分配出来的内存地址
* 备注: 最小分配长度为此内存池的一个最小单位（block的大小）
***************************************************************************/
char* MemoryPool::pool_alloc(unsigned int len)
{
	int offset = (-1);
	unsigned int index = 0;
	char *_buffer = NULL;
	if(!len || len > ((1<<this->level) * this->block))
		return NULL;
	//需要分配的长度block大小对齐, 不满也占用一个block
	if(len % this->block)
	{		
		len = (((len) + (this->block - 1)) & ~(this->block - 1));
	}
	
	//设置锁Guard, 函数返回锁unlock
	Guard<Mutex> guard(this->locker_);
	if(!guard.locked())
		return NULL;

	//循环尝试从已经创建的内存池单元中分配内存
	for(;index<this->used;index++)
	{
		offset = buddy_alloc(this->pools[index].alloc, len / this->block);
		if(offset >= 0)
		{
			_buffer = this->pools[index].buffer + offset * this->block;				
			return _buffer;
		}
	}

	//如果从已有的内存池单元中没有申请到内存，则新开辟一个内存池单元然后再分配
	if(this->used >= MAX_POOL_NUM) //所有Poolunit已满
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
* 名称: pool_free
* 功能: 从内存池释放一块内存
* 输入: buf---内存地址
* 输出: N/A
* 返回: N/A
* 备注: N/A
***************************************************************************/
void MemoryPool::pool_free(char* buf)
{
	int offset = -1, index = -1, idx = 0;

	//设置锁Guard, 函数返回时unlock
	Guard<Mutex> guard(this->locker_);
	if(!guard.locked())
		return; //TODO 万一没锁住, 这个buf永远不释放了怎么办?
	
	//查找需要释放的内存输入某个内存池单元
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
	
	//根据内存地址计算出此内存在内存池单元中的偏移,从buddy中释放此偏移的占用标记
	offset = ((int)(buf - this->pools[index].buffer)) / this->block;	
	if(offset>=0 && offset <= 1<<this->level)
	{
		buddy_free(this->pools[index].alloc, offset);
	}

	//TODO 如果考虑更好的性能,是否可以考虑不用每次判断删除整个poolunit??

	//如果整个内存池单元都已经空闲而且此内存池单元不是第一个被创建的内存池单元，则释放所有资源
	if(index && buddy_empty(this->pools[index].alloc))
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
		if(index!=this->used-1)
		{
			//如果释放掉的内存池单元在内存池管理数组中的位置不是最后一个，则将内存池单元数组的最后一个交换到当前释放掉的地方
			this->pools[index].alloc = this->pools[this->used-1].alloc;
			this->pools[index].buffer = this->pools[this->used-1].buffer;
		}
		this->used--;
	}
	
}

}	//end idee