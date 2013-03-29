#ifndef __BUDDY_MEMORY_ALLOCATION_H__
#define __BUDDY_MEMORY_ALLOCATION_H__

//namespace idee
//{
//namespace os_func
//{

struct buddy;

extern "C" 
{
//����һ���ڴ��������size��ʾ�ж��ٸ���������2��n�η���
struct buddy * buddy_new(int size);
//ɾ���ڴ������
void buddy_delete(struct buddy *);
//���ڴ�������з��䣬size��ʾ���ٱ�����С���䵥λ�����ط�������
int buddy_alloc(struct buddy *, int size);
//�ͷ��ڴ棬offset��ʾalloc���ص����
void buddy_free(struct buddy *, int offset);
//һ����Ŵ���ʾ�����ݴ�С
int buddy_size(struct buddy *, int offset);
//�ж��ڴ�������Ƿ�δʹ��
unsigned int buddy_empty(struct buddy *);

}

//}
//}

#endif
