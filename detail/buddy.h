#ifndef __BUDDY_MEMORY_ALLOCATION_H__
#define __BUDDY_MEMORY_ALLOCATION_H__

//namespace idee
//{
//namespace os_func
//{

struct buddy;

extern "C" 
{
//创建一个内存分配器，size表示有多少个（必须是2的n次方）
struct buddy * buddy_new(int size);
//删除内存分配器
void buddy_delete(struct buddy *);
//从内存分配器中分配，size表示多少倍的最小分配单位，返回分配的序号
int buddy_alloc(struct buddy *, int size);
//释放内存，offset表示alloc返回的序号
void buddy_free(struct buddy *, int offset);
//一个序号处表示的数据大小
int buddy_size(struct buddy *, int offset);
//判断内存分配器是否未使用
unsigned int buddy_empty(struct buddy *);

}

//}
//}

#endif
