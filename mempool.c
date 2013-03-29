
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "config.h"
//#include "ngx_config.h"
#include "buddy.h"
#include "Mutex.h"

#define DEFAULT_POOL_COUNT (1024) //默认的内存池的个数

#pragma pack(1)

struct poolunit{
	struct buddy		*alloc;//内存管理器
	char				*buffer;//缓冲区
};

#pragma pack()

class MemoryPool
{
private:
	unsigned short	block;						//最小块
	unsigned short	level;						//级数
	unsigned int	size;						//块数(块数=2的级数次方)
	struct poolunit	pools[DEFAULT_POOL_COUNT];	//内存池单元
	unsigned short	used;						//已经开辟的内存池数量
	
	Mutex locker_;		
public:
	MemoryPool()
	{
		init();
	};

	~MemoryPool()
	{
		release();
	};

/**************************************************************************
* 名称: create_pool
* 功能: 创建一个新的内存池
* 输入: block---单块内存最小大小，即是内存池可以分配的最小单位块
*		maxsize---内存池block数量，表示内存池可以管理的内存的大小
*
* 输出: N/A
* 返回: pool
* 备注: maxsize会进行2的n次方对齐：例如传入1000则会变成1024，传入1038则会变为2048
**************************************************************************/
void init(unsigned short block, unsigned int maxsize)
{
	//struct mempool* pool=malloc(sizeof(struct mempool));
	//memset(pool,0,sizeof(struct mempool));
	
	//unsigned int index=0;
	
	//计算内存池的级数(buddy的二叉树的级数)
	double mantissa=0.0;	
	mantissa=frexp((double)maxsize, &this->level);
	if(0.5==mantissa || (!maxsize&(maxsize-1)))
	{
		this->level--;
	}

	this->level=0;
	this->block = block;
	this->size  = maxsize;
	this->used  = 0;

	this->pools[this->used].buffer=(char*)malloc((1<<level) * block);
	if(!this->pools[this->used].buffer)
		return;
	//ngx_memzero(pool->pools[pool->used].buffer,(1<<level)*block); //#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
	memset(this->pools[this->used].buffer, 0, (1<<level) * block);

	//创建一个buddy用于管理内存池，输入参数为管理的块数
	this->pools[this->used].alloc=buddy_new(1<<level);
	this->used++;

	return ;
}

/**************************************************************************
* 名称: delete_pool
* 功能: 销毁内存池
* 输入: pool---内存池地址
* 输出: N/A
* 返回: N/A
* 备注: N/A
***************************************************************************/
void release()
{
	unsigned int index=0;
	//释放所有创建的buddy和开辟的内存
	for(;index<this->used;index++)
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
	}	
}

/**************************************************************************
* 名称: pool_alloc
* 功能: 从内存池分配一块内存
* 输入: len---数据长度(这个长度不能比maxsize更大)
* 输出: N/A
* 返回: nt_char* 分配出来的内存地址
* 备注: 最小分配长度为此内存池的一个最小单位（block的大小）
***************************************************************************/
char *pool_alloc(unsigned int len){
	int offset=(-1);
	unsigned int index=0;
	char *_buffer = NULL;
	if(!len || len > ((1<<this->level) * this->block))
		return NULL;
	//需要分配的长度进行2的n次方对齐
	if(len % this->block)
	{
		//len=ngx_align(len,pool->block);  //#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
		len = (((len) + (this->block - 1)) & ~(this->block - 1)) ;
	}
	//_lock_pool(pool);
	locker_.lock();

	//循环尝试从已经创建的内存池单元中分配内存
	for(;index<this->used;index++)
	{
		offset = buddy_alloc(this->pools[index].alloc, len/this->block);
		if(offset >= 0)
		{
			_buffer = this->pools[index].buffer + offset * this->block;
			//_unlock_pool(pool);
			locker_.unlock();
			return _buffer;
		}
	}
	//如果从已有的内存池单元中没有申请到内存，则新开辟一个内存池单元然后再分配
	this->pools[this->used].buffer=(char*)malloc((1<<this->level) * this->block);
	if(!this->pools[this->used].buffer){
		//_unlock_pool(pool);
		locker_.unlock();
		return NULL;
	}
	memset(this->pools[this->used].buffer,0,(1<<this->level) * this->block);
	this->pools[this->used].alloc = buddy_new(1<<this->level);
	this->used++;
	offset = buddy_alloc(this->pools[this->used - 1].alloc, len/this->block);
	if(offset>=0){
		_buffer=this->pools[this->used-1].buffer + offset * this->block;
		//_unlock_pool(pool);
		locker_.unlock();
		return _buffer;
	}

	//_unlock_pool(pool);
	locker_.unlock();
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
void pool_free(char* buf)
{
	int offset = -1,index = -1,idx = 0;
	//_lock_pool(pool);
	locker_.lock();
	
	//查找需要释放的内存输入某个内存池单元
	for(idx = 0;idx < this->used; idx++){
		if(buf >= this->pools[idx].buffer && buf < (this->pools[idx].buffer+(1<<this->level) * this->block)){
			index = idx;
			break;
		}
	}
	if(index<0 || index>=DEFAULT_POOL_COUNT){
		//_unlock_pool(pool);
		locker_.unlock();
		return;
	}
	//根据内存地址计算出此内存在内存池单元中的偏移
	offset = ((int)(buf - this->pools[index].buffer)) / this->block;
	//从buddy中释放此偏移的内存
	if(offset>=0 && offset <= 1<<this->level){
		buddy_free(this->pools[index].alloc, offset);
	}
	//如果整个内存池单元都已经空闲而且此内存池单元不是第一个被创建的内存池单元，则释放所有资源
	if(index && buddy_empty(this->pools[index].alloc))
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
		if(index!=this->used-1){
			//如果释放掉的内存池单元在内存池管理数组中的位置不是最后一个，则将内存池单元数组的最后一个交换到当前释放掉的地方
			this->pools[index].alloc = this->pools[this->used-1].alloc;
			this->pools[index].buffer = this->pools[this->used-1].buffer;
		}
		this->used--;
	}
	//_unlock_pool(pool);
	locker_.unlock();
}


};