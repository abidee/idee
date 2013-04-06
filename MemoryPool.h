/**
* @file MemoryPool.h
* @brief 提供预先分配内存池, 避免内村碎片, 提高分配速度
* @author nzl
*
* 备注: MemoryPool的使用, 要在每个poolunit的大小和poolunit数量之间平衡才有更好的性能
* poolunit数量控制在8以下可能性能更好, 如系统平常状态占用内存为5M, 每个块大小可以为4M
*
*/
#ifndef XQ_MEMORYPOOL_H
#define XQ_MEMORYPOOL_H

namespace idee
{

#define MAX_POOL_NUM (128) //默认的内存池的个数

//#pragma pack(1)
//struct poolunit{
//struct buddy		*alloc;//内存管理器
//char				*buffer;//缓冲区
//};
//#pragma pack()


class XQ_EXPORT MemoryPool
{

struct poolunit
{
	struct buddy		*alloc;//内存管理器
	char				*buffer;//缓冲区
};

private:
	unsigned short	block;					//最小块
	unsigned short	level;					//级数
	unsigned int	size;					//块数(块数=2的级数次方)
	struct poolunit	pools[MAX_POOL_NUM];	//内存池单元
	unsigned short	used;					//已经开辟的内存池数量
	
	Mutex locker_;							//锁
public:
	MemoryPool();
	

	~MemoryPool();
	

/**
* @brief init
* 创建一个新的内存池
* @param block 单块内存最小大小，即是内存池可以分配的最小单位块
* @param maxsize 内存池block数量，表示内存池可以管理的内存的大小
* @return 
*
* maxsize会进行2的n次方对齐：例如传入1000则会变成1024，传入1038则会变为2048
*/
void init(unsigned short block, unsigned int maxsize);

/**
* @brief release
* 功能: 销毁内存池
* @param pool 内存池地址
* @return: N/A
*
* 备注: N/A
*/
void release();

/**
* @brief pool_alloc
* 功能: 从内存池分配一块内存
* @praam len 数据长度(这个长度不能比maxsize更大)
* @return char* 分配出来的内存地址
*
* 备注: 最小分配长度为此内存池的一个最小单位（block的大小）
*/
char* pool_alloc(unsigned int len);

/**
* @brief pool_free
* 功能: 从内存池释放一块内存
* @param buf 内存地址
* @return 
*
* 备注: N/A
*/
void pool_free(char* buf);

};

} //end idee
#endif