
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "config.h"
//#include "ngx_config.h"
#include "buddy.h"
#include "Mutex.h"

#define DEFAULT_POOL_COUNT (1024) //Ĭ�ϵ��ڴ�صĸ���

#pragma pack(1)

struct poolunit{
	struct buddy		*alloc;//�ڴ������
	char				*buffer;//������
};

#pragma pack()

class MemoryPool
{
private:
	unsigned short	block;						//��С��
	unsigned short	level;						//����
	unsigned int	size;						//����(����=2�ļ����η�)
	struct poolunit	pools[DEFAULT_POOL_COUNT];	//�ڴ�ص�Ԫ
	unsigned short	used;						//�Ѿ����ٵ��ڴ������
	
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
* ����: create_pool
* ����: ����һ���µ��ڴ��
* ����: block---�����ڴ���С��С�������ڴ�ؿ��Է������С��λ��
*		maxsize---�ڴ��block��������ʾ�ڴ�ؿ��Թ�����ڴ�Ĵ�С
*
* ���: N/A
* ����: pool
* ��ע: maxsize�����2��n�η����룺���紫��1000�����1024������1038����Ϊ2048
**************************************************************************/
void init(unsigned short block, unsigned int maxsize)
{
	//struct mempool* pool=malloc(sizeof(struct mempool));
	//memset(pool,0,sizeof(struct mempool));
	
	//unsigned int index=0;
	
	//�����ڴ�صļ���(buddy�Ķ������ļ���)
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

	//����һ��buddy���ڹ����ڴ�أ��������Ϊ����Ŀ���
	this->pools[this->used].alloc=buddy_new(1<<level);
	this->used++;

	return ;
}

/**************************************************************************
* ����: delete_pool
* ����: �����ڴ��
* ����: pool---�ڴ�ص�ַ
* ���: N/A
* ����: N/A
* ��ע: N/A
***************************************************************************/
void release()
{
	unsigned int index=0;
	//�ͷ����д�����buddy�Ϳ��ٵ��ڴ�
	for(;index<this->used;index++)
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
	}	
}

/**************************************************************************
* ����: pool_alloc
* ����: ���ڴ�ط���һ���ڴ�
* ����: len---���ݳ���(������Ȳ��ܱ�maxsize����)
* ���: N/A
* ����: nt_char* ����������ڴ��ַ
* ��ע: ��С���䳤��Ϊ���ڴ�ص�һ����С��λ��block�Ĵ�С��
***************************************************************************/
char *pool_alloc(unsigned int len){
	int offset=(-1);
	unsigned int index=0;
	char *_buffer = NULL;
	if(!len || len > ((1<<this->level) * this->block))
		return NULL;
	//��Ҫ����ĳ��Ƚ���2��n�η�����
	if(len % this->block)
	{
		//len=ngx_align(len,pool->block);  //#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
		len = (((len) + (this->block - 1)) & ~(this->block - 1)) ;
	}
	//_lock_pool(pool);
	locker_.lock();

	//ѭ�����Դ��Ѿ��������ڴ�ص�Ԫ�з����ڴ�
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
	//��������е��ڴ�ص�Ԫ��û�����뵽�ڴ棬���¿���һ���ڴ�ص�ԪȻ���ٷ���
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
* ����: pool_free
* ����: ���ڴ���ͷ�һ���ڴ�
* ����: buf---�ڴ��ַ
* ���: N/A
* ����: N/A
* ��ע: N/A
***************************************************************************/
void pool_free(char* buf)
{
	int offset = -1,index = -1,idx = 0;
	//_lock_pool(pool);
	locker_.lock();
	
	//������Ҫ�ͷŵ��ڴ�����ĳ���ڴ�ص�Ԫ
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
	//�����ڴ��ַ��������ڴ����ڴ�ص�Ԫ�е�ƫ��
	offset = ((int)(buf - this->pools[index].buffer)) / this->block;
	//��buddy���ͷŴ�ƫ�Ƶ��ڴ�
	if(offset>=0 && offset <= 1<<this->level){
		buddy_free(this->pools[index].alloc, offset);
	}
	//��������ڴ�ص�Ԫ���Ѿ����ж��Ҵ��ڴ�ص�Ԫ���ǵ�һ�����������ڴ�ص�Ԫ�����ͷ�������Դ
	if(index && buddy_empty(this->pools[index].alloc))
	{
		buddy_delete(this->pools[index].alloc);
		free(this->pools[index].buffer);
		if(index!=this->used-1){
			//����ͷŵ����ڴ�ص�Ԫ���ڴ�ع��������е�λ�ò������һ�������ڴ�ص�Ԫ��������һ����������ǰ�ͷŵ��ĵط�
			this->pools[index].alloc = this->pools[this->used-1].alloc;
			this->pools[index].buffer = this->pools[this->used-1].buffer;
		}
		this->used--;
	}
	//_unlock_pool(pool);
	locker_.unlock();
}


};